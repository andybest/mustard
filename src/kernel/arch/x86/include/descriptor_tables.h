#pragma once

#include <stdint.h>

enum GDTAccessFlags {
    kGDTAccessFlagPresent = 1 << 7
};

enum GDTGranularityFlags {
    kGDTGranularityFlagGranularity  = 1 << 7,
    kGDTGranularityFlagOperandSize  = 1 << 6,
    kGDTGranularityFlagZero1        = 1 << 5,
    kGDTGranularityFlagZero2        = 1 << 4
};

struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct GDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute((packed));