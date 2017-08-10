#pragma once

#include <stdint.h>

// Page Directory Flags

enum PageDirectoryFlag {
    kPageDirectoryFlagPresent        = 1,
    kPageDirectoryFlagReadWrite      = 1 << 1,
    kPageDirectoryFlagUserSupervisor = 1 << 2,
    kPageDirectoryFlagWriteThrough   = 1 << 3,
    kPageDirectoryFlagCacheDisabled  = 1 << 4,
    kPageDirectoryFlagAccessed       = 1 << 5,
    kPageDirectoryFlagZero           = 1 << 6,
    kPageDirectoryFlagPageSize       = 1 << 7,
    kPageDirectoryFlagIgnored        = 1 << 8
};

enum PageTableFlag {
    kPageTableFlagPresent        = 1,
    kPageTableFlagReadWrite      = 1 << 1,
    kPageTableFlagUserSupervisor = 1 << 2,
    kPageTableFlagWriteThrough   = 1 << 3,
    kPageTableFlagCacheDisabled  = 1 << 4,
    kPageTableFlagAccessed       = 1 << 5,
    kPageTableFlagDirty          = 1 << 6,
    kPageTableFlagZero           = 1 << 7,
    kPageTableFlagGlobal         = 1 << 8
};

struct PageFrame {
    uint32_t present : 1;
    uint32_t read_write : 1;
    uint32_t user_supervisor : 1;
    uint32_t write_through : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t zero : 1;
    uint32_t global : 1;
    uint32_t available : 3;
    uint32_t physical_address : 20;

    uint32_t to_uint32() {
        uint32_t flags = 0;
        flags |= present;
        flags |= (read_write << 1);
        flags |= (user_supervisor << 2);
        flags |= (write_through << 3);
        flags |= (cache_disabled << 4);
        flags |= (accessed << 5);
        flags |= (dirty << 6);
        flags |= (global << 8);

        return (physical_address << 12) | flags;
    }

};

struct PageDirectoryEntry {
    uint32_t present : 1;
    uint32_t read_write : 1;
    uint32_t user_supervisor : 1;
    uint32_t write_through : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed : 1;
    uint32_t zero : 1;
    uint32_t page_size : 1;
    uint32_t ignored : 1;
    uint32_t available : 3;
    uint32_t page_table_physical_address : 20;

    uint32_t to_uint32() {
        uint32_t flags = 0;
        flags |= present;
        flags |= (read_write << 1);
        flags |= (user_supervisor << 2);
        flags |= (write_through << 3);
        flags |= (cache_disabled << 4);
        flags |= (accessed << 5);
        flags |= (page_size << 7);

        return (page_table_physical_address << 12) | flags;
    }

};

constexpr uint32_t kernel_page_table_array_size();

class PageAllocator {
   public:
    PageAllocator();

    void initialize();

   private:
    // End of kernel + initial page tables
    uint32_t kernel_pages_end_;
    uint32_t reserved_page_frame_idx_;
    uint32_t reserved_page_table_virtual_;

    uint32_t temp_page_table_physical_address_;

    void switch_page_directory(uint32_t page_directory_phys) const;

    void initialize_page_directory() const;

    void initialize_kernel_pagetables();
};