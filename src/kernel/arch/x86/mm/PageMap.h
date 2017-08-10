#pragma once

#include <stdint.h>

struct PageMap {
    static const uint32_t page_bitmap_size = 1024 / 32;
    uint32_t              page_bitmap[1024/32];
    uint32_t*             page_table_virtual_address;
    bool                  present;

    PageMap();

    int  first_free_page() const;
    void set_page_taken(uint32_t idx);
    void set_page_free(uint32_t idx);
    void clear();
};