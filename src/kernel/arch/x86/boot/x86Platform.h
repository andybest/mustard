#pragma once

#include "../descriptorTables/Interrupts.h"
#include "../descriptorTables/gdt.h"
#include "../drivers/PCTerminalDriver.h"
#include "../mm/mm.h"
#include "../platform/ibm/phys_virt.h"
#include "Platform.h"
#include "multiboot.h"

extern const uint32_t KERNEL_MEM_END;

class x86Platform : public Platform {
   public:
    x86Platform(MultibootInfo *pInfo);

    TerminalDriver *defaultTerminal();

    void initialize();

   private:
    void init_gdt();
    void init_idt(); 

    PCTerminalDriver term_;
    Gdt              gdt_;
    Idt              idt_;
    PageAllocator    pageAllocator_ = PageAllocator(KERNEL_MEM_END, VIRT_BASE);

    void get_memory_map(MultibootInfo *pInfo);

    void init_page_allocator();
};