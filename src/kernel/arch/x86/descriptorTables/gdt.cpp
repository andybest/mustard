#include "gdt.h"
#include "../platform/ibm/phys_virt.h"
#include <stdint.h>

extern "C" void gdt_flush_asm(uint32_t gdt_ptr);

void Gdt::create_gdt() {
    gdt_pointer_.limit = (sizeof(GDTEntry) * 5) - 1;
    gdt_pointer_.base = ((uint32_t) &gdt_entries_);

    // Null segment
    gdt_set_entry(0, 0, 0, 0, 0);

    // Code segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Data segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // User mode code segment
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // User mode data segment
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush();
}

void Gdt::gdt_set_entry(int32_t idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries_[idx].base_low = static_cast<uint16_t>(base & 0xFFFF);
    gdt_entries_[idx].base_middle = static_cast<uint8_t>((base >> 16) & 0xFF);
    gdt_entries_[idx].base_high = static_cast<uint8_t>((base >> 24) & 0xFF);

    gdt_entries_[idx].limit_low = static_cast<uint16_t>(limit & 0xFFFF);
    gdt_entries_[idx].granularity = static_cast<uint8_t>((limit >> 16) & 0x0F);

    gdt_entries_[idx].granularity |= granularity & 0xF0;
    gdt_entries_[idx].access = access;
}

void Gdt::gdt_flush() {
    gdt_flush_asm((uint32_t)&gdt_pointer_);
}
