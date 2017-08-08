#pragma once

#include <stdint.h>

struct IDTEntry {
    uint16_t base_low;
    uint16_t segment_selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} __attribute((packed));

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute((packed));

struct InterruptFrame {
    uint32_t data_segment_selector;
    uint32_t edi, esi, sbp, esp, ebx, edx, ecx, eax;
    uint32_t interrupt_number;
    uint32_t error_code;
    uint32_t epi, cs, eflags, useresp, ss;
} __attribute((packed));

class Idt {
   public:
    Idt();

    void initialize();

   private:
    void idt_flush();

    IDTPointer idt_ptr_;
};
