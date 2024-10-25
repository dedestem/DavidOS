#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_SIZE 256

// IDT Entry Structure
struct idt_entry {
    uint16_t offset_low;    // Lower 16 bits of the offset to the handler
    uint16_t selector;      // Code segment selector
    uint8_t  zero;          // Reserved
    uint8_t  type_attr;     // Type and attributes
    uint16_t offset_mid;    // Next 16 bits of the offset to the handler
    uint32_t offset_high;   // Upper 32 bits of the offset to the handler
    uint32_t reserved;      // Reserved
} __attribute__((packed));

// IDT Pointer Structure
struct idt_ptr {
    uint16_t limit;         // Size of the IDT - 1
    uint64_t base;          // Address of the first IDT entry
} __attribute__((packed));

// Function prototypes
void init_idt();
void set_idt_entry(int index, void (*handler)(), uint16_t selector, uint8_t type_attr);

// Global variables for IDT status
extern volatile int idt_initialized;

#endif // IDT_H
