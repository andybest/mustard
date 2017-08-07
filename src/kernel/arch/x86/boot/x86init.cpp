#include "Platform.h"
#include "x86Platform.h"
#include "Kernel.h"
#include "multiboot.h"
#include "../platform/ibm/phys_virt.h"
#include "../mm/mm.h"

extern const uint32_t KERNEL_MEM_END;

Platform *platform_init() {
    static x86Platform platform;
    return &platform;
}

void print_hex(uint32_t hex) {
    char buf[9];
    buf[8] = '\0';
    
    char hexMap[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };
    
    for(int i = 7; i >= 0; i--) {
        uint32_t idx = (hex >> (i * 4)) & 0xF;
        buf[7 - i] = hexMap[idx];
    }
    
    kputs(buf);
}

void get_memory_map(MultibootInfo *mbInfo) {
    MultibootMemMap *mmap = (MultibootMemMap *)(mbInfo->mmap_addr + VIRT_BASE);
    kputs("Memory map:\n");
    
    while(mmap < (MultibootMemMap *)(mbInfo->mmap_addr + mbInfo->mmap_length + VIRT_BASE)) {
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
        
        mmap = (MultibootMemMap*) ( (uint32_t)mmap + mmap->size + sizeof(mmap->size) );
    }
}

extern "C" void kernel_main(MultibootInfo *mbInfoPhys) {
    MultibootInfo *mbInfo = (MultibootInfo *)(((uint32_t)mbInfoPhys) + VIRT_BASE);
    
    Platform *p = platform_init();
    Kernel::shared()->init(p);
    
    get_memory_map(mbInfo);

    kputs("Kernel physical end: 0x");
    print_hex(KERNEL_MEM_END);
    kputs("\n");

    // Number of pages to allocate for kernel

    uint32_t pages = KERNEL_MEM_END >> 12;

    // Round up if it's not on a page boundary
    if( (KERNEL_MEM_END & 0xFFF) > 0){
        pages += 1;
    }

    kputs("Kernel pages: ");
    print_hex(pages);

    kputs("Enabling memory manager\n");
    PageAllocator pa(KERNEL_MEM_END, VIRT_BASE);
    kputs("Done!\n");
}