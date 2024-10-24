#include "print.h"
#include "idt.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("64 Bit kernel loaded!\n");

    // Initialize the IDT
    init_idt();

    // Simulate an interrupt (this requires you to trigger it based on your environment)
    while (!interrupt_triggered) {
        // This loop is just for simulation purposes
    }

    // Check if IDT initialized successfully
    if (idt_initialized) {
        print_str("IDT is working correctly.\n");
    } else {
        print_str("IDT is not working correctly.\n");
    }
}
