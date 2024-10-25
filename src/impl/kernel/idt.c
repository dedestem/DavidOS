#include "idt.h"
#include "print.h"

// Declare the IDT and IDT pointer
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idt_pointer;

// Default handler (no-op)
void default_handler(uint64_t interrupt_number) {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("Exception: Unknown interrupt #");
    print_dec(interrupt_number); // Print the interrupt number
    print_str(" occurred!\n");
}

// Division by zero handler
void div_by_zero_handler() {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("Exception: Division by zero (0/0) occurred!\n");
    while (1); // Hang here indefinitely
}

// Invalid opcode handler
void invalid_opcode_handler() {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("Exception: Invalid opcode occurred!\n");
    while (1); // Hang here indefinitely
}

// Page fault handler
void page_fault_handler() {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("Exception: Page fault occurred!\n");
    while (1); // Hang here indefinitely
}

// Keyboard interrupt handler
void keyboard_interrupt_handler() {
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("Interrupt: Keyboard input received!\n");
    // Here you would typically read the keyboard input
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

// Exception handler that calls the default handler with the interrupt number
void handle_interrupt(uint64_t interrupt_number) {
    if (interrupt_number < IDT_SIZE && idt[interrupt_number].type_attr) {
        void (*handler)() = (void (*)())((uint64_t)idt[interrupt_number].offset_low |
                                         ((uint64_t)idt[interrupt_number].offset_mid << 16) |
                                         ((uint64_t)idt[interrupt_number].offset_high << 32));
        handler(); // Call the specific handler if it exists
    } else {
        default_handler(interrupt_number); // Call the default handler if unknown
    }
}

// Initialize the IDT
void init_idt() {
    // Clear the IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_entry(i, (void (*)())default_handler, 0x08, 0x8E);
    }

    // Set specific handlers
    set_idt_entry(0, (void (*)())div_by_zero_handler, 0x08, 0x8E);          // Division by zero
    set_idt_entry(6, (void (*)())invalid_opcode_handler, 0x08, 0x8E);      // Invalid opcode
    set_idt_entry(14, (void (*)())page_fault_handler, 0x08, 0x8E);         // Page fault
    set_idt_entry(33, (void (*)())keyboard_interrupt_handler, 0x08, 0x8E); // Keyboard interrupt

    // Load the IDT
    idt_pointer.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_pointer.base = (uint64_t)&idt;

    print_str("Loading IDT...\n");
    __asm__ volatile ("lidt %0" : : "m"(idt_pointer));
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("IDT Inited\n");
    __asm__ volatile("sti"); // Enable interrupts
}

// Add this function in your assembly code to call handle_interrupt
// Example for an interrupt that would call handle_interrupt:
// void isr_handler() {
//     uint64_t interrupt_number; // Get this from the interrupt context
//     handle_interrupt(interrupt_number);
// }
