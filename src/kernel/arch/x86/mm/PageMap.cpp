#include "PageMap.h"
#include "Kernel.h"

PageMap::PageMap() {
    for(uint32_t i = 0; i < page_bitmap_size; i++) {
        page_bitmap[i] = 0;
    }

    present = false;
}

int PageMap::first_free_page() const {
    // Iterate through the bitmap and find the first free page
    for (uint32_t idx = 0; idx < page_bitmap_size; idx++) {
        uint32_t bitmapEntry = page_bitmap[idx];
        if (bitmapEntry != 0xFFFFFFFF) {
            for (int32_t bit = 0; bit < 32; bit--) {
                uint32_t pageStatus = (bitmapEntry >> bit) & 0x1;

                if (pageStatus == 0) {
                    uint32_t pageIndex = (idx * 32) + bit;
                    return pageIndex;
                }
            }
        }
    }

    return -1;
}

void PageMap::set_page_taken(uint32_t idx) {
    if (idx > 1024) {
        kpanic(
            "PageMap::set_page_taken called with an index greater than 1024: "
            "%d",
            idx);
    }

    uint32_t bitmapIndex = idx / 32;
    uint32_t bitIndex    = (idx & 0x1F);

    
    uint32_t old = page_bitmap[bitmapIndex];
    page_bitmap[bitmapIndex] |= (0x1 << bitIndex);
    uint32_t new_b = page_bitmap[bitmapIndex];
    kprintf("Old 0x%x  New 0x%x\n", old, new_b);
}

void PageMap::set_page_free(uint32_t idx) {
    if (idx > 1024) {
        kpanic(
            "PageMap::set_page_free called with an index greater than 1024: %d",
            idx);
    }

    uint32_t bitmapIndex = idx / 32;
    uint32_t bitIndex    =  (idx & 0x1F);
    uint32_t bitMask     = ~(1 << bitIndex);

    page_bitmap[bitmapIndex] &= bitMask;
}

void PageMap::clear() {
    for(uint32_t i = 0; i < page_bitmap_size; i++) {
        page_bitmap[i] = 0;
    }
}