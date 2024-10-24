#include "print.h"
#include "idt.h"
#include "gdt.h"

void kernel_main() {
    // Clear Bios Text
    print_clear();

    // Notify User
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("64 Bit long mode started!\n");

    // Init
    init_idt();
    setup_gdt();

    // Notify Success
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("Kernel Loaded!\n");

    test();
    return 0;
}

void test() {
    double test = 0/0;
    return 0;
}