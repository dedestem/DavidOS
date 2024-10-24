#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// Number of GDT entries
#define GDT_ENTRIES 3

// GDT entry structure
typedef struct {
    uint16_t limit_low;      // Lower 16 bits of limit
    uint16_t base_low;       // Lower 16 bits of base
    uint8_t  base_middle;     // Next 8 bits of base
    uint8_t  access;          // Access flags
    uint8_t  granularity;     // Granularity flags
    uint8_t  base_high;       // Last 8 bits of base
} __attribute__((packed)) gdt_entry_t;

// Pointer structure for the GDT
typedef struct {
    uint16_t limit;           // Size of the GDT
    uint64_t base;            // Address of the first GDT entry
} __attribute__((packed)) gdt_ptr_t;

// Function prototypes
void setup_gdt();

#endif // GDT_H
