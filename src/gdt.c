#include <stdint.h>

// Structure for a GDT entry (8 bytes)
struct __attribute__((packed)) gdt_entry
{
    uint16_t limit_low;     // Lower 16 bits of limit
    uint16_t base_low;      // Lower 16 bits of base
    uint8_t  base_middle;   // Next 8 bits of base
    uint8_t  access;        // Access flags
    uint8_t  granularity;   // Granularity & upper 4 bits of limit
    uint8_t  base_high;     // Last 8 bits of base
};

// Structure for the GDT pointer
struct __attribute__((packed)) gdt_ptr
{
    uint16_t limit;
    uint32_t base;
};

extern void gdt_flush(uint32_t);

struct gdt_entry gdt[3];
struct gdt_ptr gp;

// Helper to set an entry in the GDT
void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access      = access;
}

void init_gdt()
{
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);                // Null segment
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    gdt_flush((uint32_t)&gp);
}
