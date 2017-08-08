#pragma once

#include <stdint.h>

struct IDTDescriptor {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attribute;
    uint16_t offset_2;
}