#include "idt.h"
#include "print.h" // Ensure this file has print function definitions

// Declare the IDT and IDT pointer
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idt_pointer;

// Global flags
volatile int idt_initialized = 0;
volatile int interrupt_triggered = 0;

// Division by zero handler
void div_by_zero_handler() {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("Exception: Division by zero (0/0) occurred!\n");
    
    while (1) {
        __asm__ volatile ("hlt"); // Halt the CPU until next interrupt
    }
}

// General interrupt handler
void general_interrupt_handler() {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("General Interrupt Handler Executed!\n");

    while (1) {
        __asm__ volatile ("hlt"); // Halt the CPU until next interrupt
    }
}

// Set an IDT entry
void set_idt_entry(int index, void (*handler)(), uint16_t selector, uint8_t type_attr) {
    idt[index].offset_low = (uint64_t)handler & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = type_attr;
    idt[index].offset_mid = ((uint64_t)handler >> 16) & 0xFFFF;
    idt[index].offset_high = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
    idt[index].reserved = 0; // Ensure reserved bits are clear
}

// Initialize the IDT
void init_idt() {
    // Clear the IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        idt[i] = (struct idt_entry){0}; // Zero out each entry
    }

    idt_pointer.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_pointer.base = (uint64_t)&idt;

    // Set the IDT entries
    set_idt_entry(0, div_by_zero_handler, 0x08, 0x8E); // Divide by zero
    set_idt_entry(1, general_interrupt_handler, 0x08, 0x8E); // General interrupt

    // Load the IDT
    __asm__ volatile ("lidt %0" : : "m"(idt_pointer));

    // Check IDT initialization
    if (idt_pointer.base != 0 && idt_pointer.limit == (sizeof(struct idt_entry) * IDT_SIZE) - 1) {
        idt_initialized = 1; // Set the flag to indicate successful initialization
        print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
        print_str("IDT initialized successfully!\n");
    } else {
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("IDT initialization failed!\n");
    }
}
