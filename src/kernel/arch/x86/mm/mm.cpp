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

constexpr uint32_t _kernel_size() { return _kernel_end - _kernel_start; }

constexpr uint32_t kernel_physical_end_4k_aligned() {
    uint32_t physical_end = _kernel_end & ~0x3FF;
    if ((_kernel_end & 0x3FF) > 0) {
        physical_end += 0x1000;
    }
    return physical_end - VIRT_BASE;
}

constexpr uint32_t kernel_4k_page_count() {
    // Make sure to save an extra page after the kernel for a temporary
    // page for allocating new page tables
    return (kernel_physical_end_4k_aligned() / 4096) + 1;
}

constexpr uint32_t num_page_tables_for_pages(uint32_t num_pages) {
    uint32_t pageTables = (num_pages / 1024);
    if ((num_pages & 0x3FF) > 0) {
        pageTables += 1;
    }
    return pageTables;
}

constexpr uint32_t kernel_page_table_array_size() {
    return num_page_tables_for_pages(kernel_4k_page_count()) * 1024;
}

PageAllocator::PageAllocator() {}

void PageAllocator::initialize() {
    kprintf("Kernel Start: 0x%x  Kernel End: 0x%x\n", _kernel_start,
            _kernel_end);
    kprintf("Kernel Size: %d\n", _kernel_end - _kernel_start);

    // Calculate the number of 4k pages to allocate to the kernel
    const uint32_t kernelPages           = kernel_4k_page_count();
    uint32_t       kernel_physical_start = _kernel_start;

    kprintf("Kernel pages: %d\n", kernelPages);
    kprintf("Kernel physical start: 0x%x\n", kernel_physical_start);
    kprintf("Kernel physical end (4k): 0x%x\n",
            kernel_physical_end_4k_aligned());
    kprintf("Kernel page tables: %d\n", num_page_tables_for_pages(kernelPages));

    initialize_page_directory();
    initialize_kernel_pagetables();

    // Switch to the new page directory!
    switch_page_directory(((uint32_t)&page_directory) - VIRT_BASE);
}

void PageAllocator::initialize_kernel_pagetables() {
    // Initial physical address of the first page table.
    // This is at the 4k aligned address of the end of the kernel,
    // plus 1024 bytes to save space for our temporary page table.
    uint32_t initialPagetablePhysicalAddress =
        kernel_physical_end_4k_aligned() + 4096;

    temp_page_table_physical_address_ = kernel_physical_end_4k_aligned();

    uint32_t pageTablePhysicalAddress = initialPagetablePhysicalAddress;
    uint32_t pageTableVirtualAddress  = pageTablePhysicalAddress + VIRT_BASE;
    uint32_t pageCount                = kernel_4k_page_count();
    uint32_t numPageTables            = num_page_tables_for_pages(pageCount);

    for (uint32_t pt = 0; pt < numPageTables; pt++) {
        uint32_t numPagesInTable = pageCount - (pt * 1024);
        if (numPagesInTable > 1024) {
            numPagesInTable = 1024;
        }

        // Pointer to the current page table virtual address
        uint32_t *ptPtr = (uint32_t *)pageTableVirtualAddress;

        for (uint32_t frameNum = 0; frameNum < numPagesInTable; frameNum++) {
            uint32_t pageFramePhysicalAddress =
                (pt * 1024 * 4096) + (frameNum * 4096);

            PageFrame frame;
            frame.physical_address = pageFramePhysicalAddress >> 12;
            frame.present          = 1;
            frame.read_write       = 1;

            ptPtr[frameNum] = frame.to_uint32();
        }

        uint32_t directoryIndex = ((pt * 1024 * 4096) + VIRT_BASE) >> 22;

        PageDirectoryEntry entry;
        entry.page_table_physical_address = pageTablePhysicalAddress >> 12;
        entry.present                     = 1;
        entry.read_write                  = 1;

        page_directory[directoryIndex] = entry.to_uint32();

        // Move to next page table (4KiB)
        pageTablePhysicalAddress += 4096;
        pageTableVirtualAddress += 4096;
    }
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