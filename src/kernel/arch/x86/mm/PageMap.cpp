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

void PageMap::set_page_taken(uint32_t idx) {
    if (idx > 1024) {
        kpanic(
            "PageMap::set_page_taken called with an index greater than 1024: "
            "%d",
            idx);
    }

    uint32_t bitmapIndex = idx >> 5;
    uint32_t bitIndex    = 31 - (idx & 0x1F);

    page_bitmap[bitmapIndex] |= (0x1 << bitIndex);
}

void PageMap::set_page_free(uint32_t idx) {
    if (idx > 1024) {
        kpanic(
            "PageMap::set_page_free called with an index greater than 1024: %d",
            idx);
    }

    uint32_t bitmapIndex = idx >> 5;
    uint32_t bitIndex    = 31 - (idx & 0x1F);
    uint32_t bitMask     = ~(1 << bitIndex);

    page_bitmap[bitmapIndex] &= bitMask;
}

void PageMap::clear() {
    for(int i = 0; i < page_bitmap_size; i++) {
        page_bitmap[i] = 0;
    }
}