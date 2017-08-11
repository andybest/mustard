#pragma once

#include <stdint.h>
#include "../platform/ibm/phys_virt.h"
#include "IPageAllocator.h"
#include "Kernel.h"
#include "PageMap.h"

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

class PageAllocator : public IPageAllocator {
   public:
    PageAllocator();

    void initialize();

   private:
    static uint32_t constexpr kernel_process_space_table_count_ =
        ((((0xFFFFFFFF - VIRT_BASE) + 1) / 4096) / 1024);

    PageMap  kernel_page_map_[kernel_process_space_table_count_];
    uint32_t temp_page_table_entry_address_;
    uint32_t temp_page_table_virtual_address_;

    static uint32_t constexpr kernel_max_physical_memory_pages_ = 0x100000;
    static uint32_t constexpr kernel_physical_memory_bitmap_count_ =
        kernel_max_physical_memory_pages_ >> 5;

    // Bitmap to hold allocated pages in physical memory
    uint32_t physical_memory_bitmap_[kernel_physical_memory_bitmap_count_];

    void switch_page_directory(uint32_t page_directory_phys) const;
    void initialize_page_directory() const;
    void initialize_kernel_pagetables();

    void set_physical_page_taken(uint32_t index);
    void set_physical_page_free(uint32_t index);
    int next_free_physical_page();
    int next_free_page();

    void flush_tlb(uint32_t address);
    int   next_free_kernel_page_directory_entry();
    void* map_new_page();
    bool  map_new_page_table();
};