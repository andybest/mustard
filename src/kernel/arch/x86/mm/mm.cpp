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

// Keep a 4kb block free for use as a temporaty page table to allocate
// other page tables
uint32_t page_table_temp[1024] __attribute__((aligned(4096)));

extern void print_hex(uint32_t hex);

PageAllocator::PageAllocator(uint32_t kernel_physical_end,
                             uint32_t kernel_location) {
    kernel_location_     = kernel_location;
    kernel_physical_end_ = kernel_physical_end;
}

void PageAllocator::initialize() {
    // Calculate the number of 4k pages to allocate to the kernel
    const uint32_t kernelPages = kernel_4k_page_count();

    initialize_page_directory();

    // How many 4MB pages do we need for the kernel?
    uint32_t pages4mb = kernel_physical_end_ >> 22;
    if ((kernel_physical_end_ & PAGE_DIRECTORY_MASK) > 0) {
        pages4mb++;
    }

    // How many page tables do we need to allocate for the kernel?
    uint32_t pageTablesNeeded = page_tables_needed(kernelPages);

    // Check that the amount of free memory is enough to cover the kernel and
    // the initial page tables
    if (((pages4mb << 22) - (kernelPages << 12)) < (pageTablesNeeded << 12)) {
        pages4mb += 1;
    }

    // Set up the 4mb pages
    for (uint32_t i = 0; i < pages4mb; i++) {
        uint32_t physicalAddress = i << 22;
        uint32_t virtualAddress  = physicalAddress + VIRT_BASE;

        uint32_t pdOffset = virtualAddress >> 22;
        // Create a page directory entry- present, read/write, 4MB
        uint32_t pdEntry =
            (physicalAddress & 0xFFC00000) | kPageDirectoryFlagPresent |
            kPageDirectoryFlagReadWrite | kPageDirectoryFlagPageSize;
        page_directory[pdOffset] = pdEntry;
    }

    // Calculate the physical address of the page directory
    uint32_t page_directory_phys = ((uint32_t)page_directory) - VIRT_BASE;

    // Temporarily use the new page directory
    switch_page_directory(page_directory_phys);

    // Initialize the actual page tables
    uint32_t kernelEnd4kAligned = kernelPages * 4096;
    initialize_kernel_pagetables(kernelPages, pageTablesNeeded,
                                 kernelEnd4kAligned);
}

uint32_t PageAllocator::page_tables_needed(const uint32_t kernelPages) const {
    uint32_t pageTablesNeeded = (kernelPages >> 10);
    if ((kernelPages & 0x3FF) > 0) {
        pageTablesNeeded += 1;
    }
    return pageTablesNeeded;
}

void PageAllocator::initialize_kernel_pagetables(
    const uint32_t kernelPages, const uint32_t pageTablesNeeded,
    const uint32_t kernelEnd4kAligned) {
    // Number of pages to add- making sure to map in the extra pages needed for
    // the page tables
    // plus one reserved page frame for allocating new page tables
    uint32_t pagesToProcess = kernelPages + pageTablesNeeded + 1;
    uint32_t freePageFrames = 0;

    for (uint32_t i = 0; i < pageTablesNeeded; i++) {
        // Physical address of the new page table
        auto pageTable =
            (uint32_t *)(kernelEnd4kAligned + (i * 1024) + VIRT_BASE);

        kputs("Allocating page table: 0x");
        print_hex((uint32_t)pageTable);
        kputs("\n");

        uint32_t ptPhysicalAddress = i * 4096;
        uint32_t ptVirtualAddress  = ptPhysicalAddress + VIRT_BASE;

        for (uint32_t p = 0; p < 1024; p++) {
            uint32_t ptEntry = ((p + (i * 1024)) << 12) |
                               kPageTableFlagPresent | kPageTableFlagReadWrite;
            pageTable[p] = ptEntry;

            if (pagesToProcess == 0) {
                freePageFrames               = 1024 - p - 1;
                reserved_page_frame_idx_     = p;
                reserved_page_table_virtual_ = ptVirtualAddress;

                kernel_pages_end_ = (uint32_t)pageTable + 4096 - VIRT_BASE;
                break;
            }
            pagesToProcess--;
        }

        uint32_t pageDirectoryEntry = (ptPhysicalAddress & 0xFFC00000) |
                                      kPageDirectoryFlagPresent |
                                      kPageDirectoryFlagReadWrite;
        uint32_t pageDirectoryOffset        = ptVirtualAddress >> 22;
        page_directory[pageDirectoryOffset] = pageDirectoryEntry;
    }

    kputs("Kernel physical end: 0x");
    print_hex(kernel_physical_end_);
    kputs("\n");

    kputs("Kernel pages end: 0x");
    print_hex(kernel_pages_end_);
    kputs("\n");
}

uint32_t PageAllocator::kernel_4k_page_count() const {
    uint32_t kernelPages = kernel_physical_end_ >> 12;

    if ((kernel_physical_end_ & 0xFFF) > 0) {
        kernelPages++;
    }
    return kernelPages;
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