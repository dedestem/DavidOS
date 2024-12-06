#include "print.h"
#include "idt.h"
#include "gdt.h"

#include <stdint.h>

// Functions
void print_system_info() {
    print_str("System Information:\n");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);

    // CPU VENDOR
    char cpu_vendor[13];
    uint32_t eax, ebx, ecx, edx;

    eax = 0;
    __asm__ volatile (
        "cpuid":
        "=b"(ebx), "=d"(edx), "=c"(ecx):
        "a"(eax)
    );

    *((uint32_t *)&cpu_vendor[0]) = ebx;
    *((uint32_t *)&cpu_vendor[4]) = edx;
    *((uint32_t *)&cpu_vendor[8]) = ecx;
    cpu_vendor[12] = '\0'; // Null-terminate the string

    print_str("CPU Vendor: ");
    print_str(cpu_vendor);
    print_str("\n");

    // PRIVILEGE LEVEL
    uint64_t cs;
    __asm__ volatile ("movq %%cs, %0" : "=r"(cs)); // Move CS register into cs
    uint64_t privilege_level = cs & 0x3; // Lowest 2 bits indicate privilege level

    print_str("Ring: ");
    print_str((privilege_level == 0) ? "0 (Kernel)\n" : "3 (User)\n");
}

// MAIN
void kernel_main() {
    // Clear BIOS Text
    print_clear();

    // Notify User
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("64 Bit long mode started!\n");

    // Initialize IDT and GDT
    init_idt();
    //setup_gdt();

    // Notify Success
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("Kernel Loaded!\n");

    // System INFO
    print_set_color(PRINT_COLOR_LIGHT_CYAN, PRINT_COLOR_BLACK);
    print_str("\n");
    print_system_info();

    while (1);
}

