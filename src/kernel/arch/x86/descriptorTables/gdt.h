#pragma once

#include "x86DescriptorTables.h"

class Gdt {
public:
    void create_gdt();
private:
    void gdt_set_entry(int32_t idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
    void gdt_flush();

    GDTEntry gdt_entries_[5];
    GDTPointer gdt_pointer_;
};