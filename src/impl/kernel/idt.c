#include "idt.h"
#include "print.h" // Ensure this file has print function definitions

// Declare the IDT and IDT pointer
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idt_pointer;

// Global flags
volatile int idt_initialized = 0;
volatile int interrupt_triggered = 0;

// Example interrupt handler
void example_interrupt_handler() {
    interrupt_triggered = 1; // Set the flag to indicate the interrupt was handled
    print_str("Interrupt handler executed!\n");
}

// Set an IDT entry
void set_idt_entry(int index, void (*handler)(), uint16_t selector, uint8_t type_attr) {
    idt[index].offset_low = (uint64_t)handler & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = type_attr;
    idt[index].offset_mid = ((uint64_t)handler >> 16) & 0xFFFF;
    idt[index].offset_high = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
    idt[index].reserved = 0;
}

// Initialize the IDT
void init_idt() {
    idt_pointer.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_pointer.base = (uint64_t)&idt;

    // Set the example interrupt handler for interrupt vector 32 (IRQ 0)
    set_idt_entry(32, example_interrupt_handler, 0x08, 0x8E); // Example: IRQ 0 (Timer)

    // Load the IDT
    __asm__ volatile ("lidt %0" : : "m"(idt_pointer));

    // Check IDT initialization
    if (idt_pointer.base != 0 && idt_pointer.limit == (sizeof(struct idt_entry) * IDT_SIZE) - 1) {
        idt_initialized = 1; // Set the flag to indicate successful initialization
        print_str("IDT initialized successfully!\n");
    } else {
        print_str("IDT initialization failed!\n");
    }
}
