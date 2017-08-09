#include "mm.h"
#include <stdint.h>
#include "../platform/ibm/phys_virt.h"
#include "Kernel.h"

#define PAGE_DIRECTORY_OFFSET(x) (x >> 22)
#define PAGE_DIRECTORY_OFFSET_TO_ADDR(x) (x << 22)
#define PAGE_TABLE_OFFSET(x) (x >> 12)
#define PAGE_TABLE_OFFSET_TO_ADDR(x) (x << 12)
#define PAGE_TABLE_MASK 0xFFFFF000
#define PAGE_DIRECTORY_MASK 0x3FFFFF
#define PAGE_TABLE_SIZE_MASK 0x3FF

uint32_t page_directory[1024] __attribute__((aligned(4096)));

extern const uint32_t _kernel_start;
extern const uint32_t _kernel_end;

constexpr uint32_t _kernel_size() {
    return _kernel_end - _kernel_start;
} 

constexpr uint32_t kernel_4k_page_count() {
    //const uint32_t kernelSize = _kernel_end - _kernel_start;
    uint32_t kernelPhysicalOffset = (_kernel_end - VIRT_BASE);
    uint32_t kernelPages = kernelPhysicalOffset >> 12;
    if ((kernelPhysicalOffset & 0xFFF) > 0) {
        kernelPages++;
    }

    // Make sure to save an extra page after the kernel for a temporary
    // page for allocating new page tables
    kernelPages++;
    return kernelPages;
}

constexpr uint32_t num_page_tables_for_pages(uint32_t num_pages) {
    uint32_t pages = num_pages >> 10;
    if ((num_pages & 0x3FF) > 0) {
        pages += 1;
    }
    return pages;
}

constexpr uint32_t kernel_page_table_array_size() {
    return num_page_tables_for_pages(kernel_4k_page_count()) * 1024;
}

PageAllocator::PageAllocator() {
}

void PageAllocator::initialize() {
    kprintf("Kernel Start: 0x%x  Kernel End: 0x%x\n", _kernel_start, _kernel_end);
    kprintf("Kernel Size: %d\n", _kernel_end - _kernel_start);

    // Calculate the number of 4k pages to allocate to the kernel
    const uint32_t kernelPages = kernel_4k_page_count();
    uint32_t kernel_physical_start = _kernel_start;

    kprintf("Kernel pages: %d\n", kernelPages);
    kprintf("Kernel physical start: 0x%x\n", kernel_physical_start);

    initialize_page_directory();
    initialize_kernel_pagetables();
}

void PageAllocator::initialize_kernel_pagetables() {
}

void PageAllocator::initialize_page_directory()
    const {  // Initialize the page directory
    uint32_t initialPageDirectoryValue = 0;
    for (unsigned int &i : page_directory) {
        i = initialPageDirectoryValue;
    }
}

void PageAllocator::switch_page_directory(
    uint32_t page_directory_phys) const {  // Enable paging
    asm volatile(
        // Enable 4MB pages
        "movl %%cr4, %%eax\n\t"
        "orl $0x00000010, %%eax\n\t"
        "movl %%eax, %%cr4\n\t"

        // Move page directory
        "movl %%ecx, %%eax\n\t"
        "movl %%eax, %%cr3\n\t" ::"c"(page_directory_phys)
        : "eax");
}