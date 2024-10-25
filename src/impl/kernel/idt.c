#include "idt.h"
#include "print.h"

// Declare the IDT and IDT pointer
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idt_pointer;

// Global flags
volatile int idt_initialized = 0;

// Default handler (no-op)
void default_handler() {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("Exception: Unknowen occurred!\n");
    while (1) {

    }
}

// Division by zero handler
void div_by_zero_handler() {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("Exception: Division by zero (0/0) occurred!\n");
    while (1) {

    }
}

// Set an IDT entry
void set_idt_entry(int index, void (*handler)(), uint16_t selector, uint8_t type_attr) {
    idt[index].offset_low = (uint16_t)((uintptr_t)handler & 0xFFFF);
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = type_attr;
    idt[index].offset_mid = (uint16_t)(((uintptr_t)handler >> 16) & 0xFFFF);
    idt[index].offset_high = (uint32_t)(((uintptr_t)handler >> 32) & 0xFFFFFFFF);
    idt[index].reserved = 0; // Ensure reserved bits are clear
}

// Initialize the IDT
void init_idt() {
    // Clear the IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_entry(i, default_handler, 0x08, 0x8E);
    }

    set_idt_entry(0, div_by_zero_handler, 0x08, 0x8E);

    // Load the IDT
    idt_pointer.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_pointer.base = (uint64_t)&idt;

    print_str("Loading IDT...\n");
    __asm__ volatile ("lidt %0" : : "m"(idt_pointer));
    print_str("IDT Loaded. Enabling Interrupts...\n");

    __asm__ volatile("cli; sti;"); //CLI to clear pending interrupts and sti to start interupts
}