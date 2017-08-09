
#include "x86Platform.h"
#include "../platform/ibm/phys_virt.h"
#include "Kernel.h"

x86Platform::x86Platform(MultibootInfo *pInfo) { this->get_memory_map(pInfo); }

TerminalDriver *x86Platform::defaultTerminal() {
    return static_cast<TerminalDriver *>(&term_);
}

void x86Platform::initialize() {
    this->term_.clearScreen();

    init_page_allocator();

    init_gdt();
    init_idt();

    asm volatile("int $0x4");

    kputs("Platform initialized\n");
}

void x86Platform::init_page_allocator() {
    kputs("Init Page Allocator\n");
    pageAllocator_.initialize();
}

void x86Platform::init_gdt() {
    kputs("Init GDT\n");
    gdt_.create_gdt();
}

void x86Platform::init_idt() {
    kputs("Init IDT\n");
    idt_.initialize();
}

void x86Platform::get_memory_map(MultibootInfo *mbInfo) {
    /*MultibootMemMap *mmap = (MultibootMemMap *)(mbInfo->mmap_addr +
    VIRT_BASE);
    kputs("Memory map:\n");

    while(mmap < (MultibootMemMap *)(mbInfo->mmap_addr + mbInfo->mmap_length +
    VIRT_BASE)) {
        kputs("    Base address: 0x");
        print_hex(mmap->base_addr >> 32 & 0xFFFFFFFF);
        print_hex(mmap->base_addr & 0xFFFFFFFF);
        kputs("\n");

        kputs("    Length: 0x");
        print_hex(mmap->length >> 32 & 0xFFFFFFFF);
        print_hex(mmap->length & 0xFFFFFFFF);
        kputs("\n");

        if(mmap->type == 1) {
            kputs("    Usable\n");
        } else {
            kputs("    Not Usable\n");
        }

        kputs("\n");

        mmap = (MultibootMemMap*) ( (uint32_t)mmap + mmap->size +
    sizeof(mmap->size) );
    }*/
}
