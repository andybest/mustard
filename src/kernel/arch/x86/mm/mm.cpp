#include "mm.h"
#include <stdint.h>
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
    // This is at the 4k aligned address of the end of the kernel
    uint32_t initialPagetablePhysicalAddress = kernel_physical_end_4k_aligned();

    uint32_t pageTablePhysicalAddress = initialPagetablePhysicalAddress;
    uint32_t pageTableVirtualAddress  = pageTablePhysicalAddress + VIRT_BASE;
    uint32_t pageCount                = kernel_4k_page_count();
    uint32_t numPageTables            = num_page_tables_for_pages(pageCount);
    numPageTables = num_page_tables_for_pages(pageCount + numPageTables);
    pageCount += numPageTables + 1;

    for (uint32_t pt = 0; pt < numPageTables; pt++) {
        // Mark the page table's physical address as taken
        set_physical_page_taken(pageTablePhysicalAddress >> 12);

        uint32_t numPagesInTable = pageCount - (pt *  1024);
        if (numPagesInTable > 1024) {
            numPagesInTable = 1024;
        }

        // Pointer to the current page table virtual address
        uint32_t *ptPtr = (uint32_t *)pageTableVirtualAddress;

        kernel_page_map_[pt].present                    = true;
        kernel_page_map_[pt].page_table_virtual_address = ptPtr;

        for (uint32_t frameNum = 0; frameNum < numPagesInTable; frameNum++) {
            uint32_t pageFramePhysicalAddress =
                (pt * 1024 * 4096) + (frameNum * 4096);

            PageFrame frame;
            frame.physical_address = pageFramePhysicalAddress >> 12;
            frame.present          = 1;
            frame.read_write       = 1;

            ptPtr[frameNum] = frame.to_uint32();

            // Set the page as taken in the page map
            kernel_page_map_[pt].set_page_taken(frameNum);
            set_physical_page_taken((pt * 1024) + frameNum);

            temp_page_table_entry_address_ = (uint32_t)&ptPtr[frameNum];
            temp_page_table_virtual_address_ = pageFramePhysicalAddress + VIRT_BASE;
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

void PageAllocator::set_physical_page_taken(uint32_t index) {
    if (index > kernel_physical_memory_bitmap_count_) {
        kpanic(
            "PageAllocator::set_physical_page_taken called with an index "
            "greater than max physical memory: %d",
            index);
    }

    uint32_t bitmapIndex = index >> 5;
    uint32_t bitIndex    = 31 - (index & 0x1F);

    physical_memory_bitmap_[bitmapIndex] |= (0x1 << bitIndex);
}

void PageAllocator::set_physical_page_free(uint32_t index) {
    if (index > kernel_physical_memory_bitmap_count_) {
        kpanic(
            "PageAllocator::set_physical_page_free called with an index "
            "greater than max physical memory: %d",
            index);
    }

    uint32_t bitmapIndex = index >> 5;
    uint32_t bitIndex    = 31 - (index & 0x1F);
    uint32_t bitMask     = ~(1 << bitIndex);

    physical_memory_bitmap_[bitmapIndex] &= bitMask;
}

int PageAllocator::next_free_physical_page() {
    // Iterate through the bitmap and find the first free page
    for (uint32_t idx = 0; idx < kernel_physical_memory_bitmap_count_; idx++) {
        uint32_t bitmapEntry = physical_memory_bitmap_[idx];
        if (bitmapEntry != 0xFFFFFFFF) {
            for (int32_t bit = 31; bit >= 0; bit--) {
                uint32_t pageStatus = (bitmapEntry >> bit) & 0x1;

                if (pageStatus == 0) {
                    uint32_t pageIndex = (idx * 32) + (31 - bit);
                    return pageIndex;
                }
            }
        }
    }

    return -1;
}

void PageAllocator::flush_tlb(uint32_t address) {
    asm volatile("invlpg (%0)" ::"r"(address) : "memory");
}

int PageAllocator::next_free_kernel_page_directory_entry() {
    for(uint32_t i = 0; i < kernel_process_space_table_count_; i++) {
        if(kernel_page_map_[i].present == false) {
            return i;
        }
    }

    return -1;
}

int PageAllocator::next_free_page() {
    for(uint32_t i = 0; i < kernel_process_space_table_count_; i++) {
        if(kernel_page_map_[i].present) {
            int firstFreePage = kernel_page_map_[i].first_free_page();
            if(firstFreePage >= 0) {
                return firstFreePage + (i * 1024);
            }
        }
    }

    return -1;
}

uint32_t* PageAllocator::map_new_page() {
    int nextFreePage = next_free_page();
    
    if(nextFreePage == -1) {
        kprintf("No free pages, mapping new table\n");
        map_new_page_table();
        nextFreePage = next_free_page();
        kprintf("Mapped!\n");
    }

    if(nextFreePage < 0) {
        kpanic("Unable to allocate page!");
    }

    int nextFreePhysicalPage = next_free_physical_page();
    if(nextFreePhysicalPage < 0) {
        kpanic("Unable to allocate page- out of memory!");
    }

    PageFrame frame;
    frame.physical_address = nextFreePhysicalPage;
    frame.present = 1;
    frame.read_write = 1;

    uint32_t pageMapIdx = ((uint32_t)nextFreePage) >> 10;
    uint32_t pageIdx = nextFreePage & 0x3FF;

    uint32_t *pageTable = kernel_page_map_[pageMapIdx].page_table_virtual_address;

    pageTable[pageIdx] = frame.to_uint32();

    kernel_page_map_[pageMapIdx].set_page_taken(pageIdx);
    set_physical_page_taken(nextFreePhysicalPage);

    uint32_t pageAddress = (nextFreePage << 12) + VIRT_BASE;

    for(int i = 0; i < 1024; i++) {
        ((uint32_t *)pageAddress)[i] = 0xCAFEBABE;
    }

    return (uint32_t *)pageAddress;
}

bool PageAllocator::map_new_page_table() {
    int nextPhysicalPageIdx = next_free_physical_page();
    if (nextPhysicalPageIdx < 0) {
        return false;
    }

    uint32_t pageTablePhysicalAddress = nextPhysicalPageIdx << 12;

    // Temporarily map this page table into the spare page frame
    PageFrame tempFrame;
    tempFrame.physical_address = nextPhysicalPageIdx;
    tempFrame.present          = true;
    tempFrame.read_write       = true;

    uint32_t *ptPtr = (uint32_t *)temp_page_table_entry_address_;
    *ptPtr          = tempFrame.to_uint32();

    // Flush the TLB for this entry with its virtual address
    flush_tlb(temp_page_table_entry_address_ + VIRT_BASE);

    // Construct a pointer to the new page table
    uint32_t *newPageTable =
        (uint32_t *)temp_page_table_virtual_address_;

    // Identity map the new page table to itself
    PageFrame ptFrame;
    ptFrame.physical_address = nextPhysicalPageIdx;
    ptFrame.present          = true;
    ptFrame.read_write       = true;

    newPageTable[0] = ptFrame.to_uint32();

    // Create a page directory entry for the new page
    int nextFreePageDirectoryEntry = next_free_kernel_page_directory_entry();

    if(nextFreePageDirectoryEntry < 0) {
        kpanic("No more free pages!\n");
    }

    PageDirectoryEntry entry;
    entry.page_table_physical_address = pageTablePhysicalAddress >> 12;
    entry.present = 1;
    entry.read_write = 1;
    
    uint32_t pageDirectoryIndex = nextFreePageDirectoryEntry + (VIRT_BASE >> 22);
    page_directory[pageDirectoryIndex] = entry.to_uint32();

    newPageTable = (uint32_t *)(pageDirectoryIndex << 22);
    // Mark the page table's address as taken
    kernel_page_map_[nextFreePageDirectoryEntry].clear();
    kernel_page_map_[nextFreePageDirectoryEntry].set_page_taken(0);
    kernel_page_map_[nextFreePageDirectoryEntry].page_table_virtual_address = newPageTable;
    kernel_page_map_[nextFreePageDirectoryEntry].present = true;

    // Mark the physical page as taken
    set_physical_page_taken(nextPhysicalPageIdx);

    // Unmap the temp frame
    *ptPtr = 0;

    flush_tlb(temp_page_table_entry_address_ + VIRT_BASE);

    return true;
}