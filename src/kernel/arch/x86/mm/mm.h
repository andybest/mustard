#pragma once

#include <stdint.h>

// Page Directory Flags

enum PageDirectoryFlag {
    kPageDirectoryFlagPresent = 1,
    kPageDirectoryFlagReadWrite = 1 << 1,
    kPageDirectoryFlagUserSupervisor = 1 << 2,
    kPageDirectoryFlagWriteThrough = 1 << 3,
    kPageDirectoryFlagCacheDisabled = 1 << 4,
    kPageDirectoryFlagAccessed = 1 << 5,
    kPageDirectoryFlagZero = 1 << 6,
    kPageDirectoryFlagPageSize = 1 << 7,
    kPageDirectoryFlagIgnored = 1 << 8
};

enum PageTableFlag {
    kPageTableFlagPresent = 1,
    kPageTableFlagReadWrite = 1 << 1,
    kPageTableFlagUserSupervisor = 1 << 2,
    kPageTableFlagWriteThrough = 1 << 3,
    kPageTableFlagCacheDisabled = 1 << 4,
    kPageTableFlagAccessed = 1 << 5,
    kPageTableFlagDirty = 1 << 6,
    kPageTableFlagZero = 1 << 7,
    kPageTableFlagGlobal = 1 << 8
};

class PageAllocator {
public:
    PageAllocator(uint32_t kernel_physical_end, uint32_t kernel_location);

    void initialize();

private:
    uint32_t kernel_location_;
    uint32_t kernel_physical_end_;

    // End of kernel + initial page tables
    uint32_t kernel_pages_end_;
    uint32_t reserved_page_frame_idx_;
    uint32_t reserved_page_table_virtual_;

    void switch_page_directory(uint32_t page_directory_phys) const;

    void initialize_page_directory() const;

    uint32_t kernel_4k_page_count() const;

    void
    initialize_kernel_pagetables(uint32_t kernelPages, uint32_t pageTablesNeeded,
                                 uint32_t kernelEnd4kAligned);

    uint32_t page_tables_needed(const uint32_t kernelPages) const;
};