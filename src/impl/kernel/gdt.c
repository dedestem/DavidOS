#include "gdt.h"
#include "print.h"
#include <stdbool.h>

// GDT entries
gdt_entry_t gdt[GDT_ENTRIES];
gdt_ptr_t gdt_pointer;

// Function to set up a GDT entry
void setup_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].access = access;
    gdt[index].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
    gdt[index].base_high = (base >> 24) & 0xFF;
}

// Set up the GDT
void setup_gdt() {
    gdt_pointer.limit = sizeof(gdt_entry_t) * GDT_ENTRIES - 1;
    gdt_pointer.base = (uint64_t)&gdt;

    // Code segment (base 0, limit 4GB)
    setup_gdt_entry(0, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Present, ring 0, executable, code
    // Data segment (base 0, limit 4GB)
    setup_gdt_entry(1, 0, 0xFFFFFFFF, 0x92, 0xCF); // Present, ring 0, writable, data

    // Null segment (base 0, limit 0)
    setup_gdt_entry(2, 0, 0, 0, 0); // Null descriptor

    // Load the GDT
    asm volatile("lgdt (%0)" : : "r"(&gdt_pointer));

    // Check GDT
    check_gdt();
}

bool validate_gdt() {
    // Check Code Segment (GDT entry 0)
    if (gdt[0].access != 0x9A || gdt[0].granularity != 0xCF) {
        return false;  // Code segment is not set up correctly
    }

    // Check Data Segment (GDT entry 1)
    if (gdt[1].access != 0x92 || gdt[1].granularity != 0xCF) {
        return false;  // Data segment is not set up correctly
    }

    // Check Null Segment (GDT entry 2)
    if (gdt[2].limit_low != 0 || gdt[2].base_low != 0) {
        return false;  // Null segment is not set up correctly
    }

    return true;  // All checks passed
}

void check_gdt() {
    if (validate_gdt()) {
        print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
        print_str("GDT initialized successfully.\n");
    } else {
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("GDT initialization failed!\n");
    }
}