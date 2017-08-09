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
    uint32_t gs, fs, es, ds;

    // same order as pusha
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

    // saved by our interrupt service routine
    uint32_t int_no;

    // cpu's interrupt setup
    uint32_t err_code;
    uint32_t eip, cs, eflags;

    // only if privilege change i.e. from ring 3 to 0:
    uint32_t useresp, ss;
};

class Idt {
   public:
    Idt();

    void initialize();

   private:
    void idt_set_entry(uint8_t idx, uint32_t base, uint16_t selector,
                       uint8_t flags);
    void idt_flush(uint32_t idt_ptr);
};

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();
