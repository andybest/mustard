#include <stdint.h>
#include "../platform/ibm/phys_virt.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));

// Keep a 4kb block free for use as a temporaty page table to allocate
// other page tables
uint32_t page_table_temp[1024] __attribute__((aligned(4096)));

void init_paging() {
    // Not present, supervisor only, read only
    uint32_t initialPageTableValue = 0;
    for(int i = 0; i < 1024; i++) {
        page_directory[i] = initialPageTableValue;
    }
}

uint32_t page_directory_offset(uint32_t addr) {
    return addr >> 12;
}

void new_page_table(uint32_t offset) {
    
}

void map_kernel() {
    uint32_t pageDirectoryOffset = page_directory_offset(VIRT_BASE);
    
    
    
    
}