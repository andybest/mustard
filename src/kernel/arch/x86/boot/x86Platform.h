#pragma once

#include "../descriptorTables/Interrupts.h"
#include "../descriptorTables/gdt.h"
#include "../drivers/PCTerminalDriver.h"
#include "../mm/mm.h"
#include "../platform/ibm/phys_virt.h"
#include "Platform.h"
#include "multiboot.h"

class x86Platform : public Platform {
   public:
    x86Platform(MultibootInfo *pInfo);

    TerminalDriver *defaultTerminal();
    IPageAllocator *pageAllocator();

    void initialize();

   private:
    void init_gdt();
    void init_idt(); 

    PCTerminalDriver term_;
    Gdt              gdt_;
    Idt              idt_;
    PageAllocator    pageAllocator_;

    void get_memory_map(MultibootInfo *pInfo);

    void init_page_allocator();
};