#include "Interrupts.h"
#include "../platform/ibm/phys_virt.h"
#include "Kernel.h"
#include <string.h>

Idt::Idt() {}

IDTEntry   idt_entries_[256];
IDTPointer idt_pointer_;

const char *exception_name_[32] = {
        "Divide by Zero",
        "Debug",
        "Non-maskable Interrupt",
        "Breakpoint",
        "Overflow",
        "Bound Range Exceeded",
        "Invalid Opcode",
        "Device Not Availiable",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",
        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
    }; 

extern "C" void idt_flush();

extern "C" void isr_handler(InterruptFrame *frame) {
    if(frame->int_no < 32) {
        kputs("Interrupt 0x");
        print_hex(frame->int_no);
        kputs(" triggered.\n    ");
        kputs(exception_name_[frame->int_no]);
        kputs(" Exception!\n");
    } else {
        kputs("Interrupt 0x");
        print_hex(frame->int_no);
        kputs(" triggered.\n");
    }
}

void Idt::initialize() {
    idt_pointer_.base  = (uint32_t)&idt_entries_;
    idt_pointer_.limit = sizeof(IDTEntry) * 256;

    //memset(&idt_entries_, 0, sizeof(IDTEntry) * 256);
    for (int i = 0; i < 256; i++) {
        idt_entries_[i].base_low         = 0;
        idt_entries_[i].segment_selector = 0;
        idt_entries_[i].zero             = 0;
        idt_entries_[i].flags            = 0;
        idt_entries_[i].base_high        = 0;
    }

    idt_set_entry(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_entry(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_entry(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_entry(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_entry(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_entry(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_entry(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_entry(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_entry(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_entry(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_entry(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_entry(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_entry(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_entry(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_entry(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_entry(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_entry(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_entry(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_entry(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_entry(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_entry(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_entry(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_entry(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_entry(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_entry(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_entry(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_entry(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_entry(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_entry(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_entry(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_entry(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_entry(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_pointer_);
    // asm volatile("int $0x4");
}

void Idt::idt_set_entry(uint8_t idx, uint32_t base, uint16_t selector,
                        uint8_t flags) {
    kputs("Adding isr handler for 0x");
    print_hex(base);
    kputs("\n");

    idt_entries_[idx].base_low  = base & 0xFFFF;
    idt_entries_[idx].base_high = base >> 16;

    idt_entries_[idx].segment_selector = selector;
    idt_entries_[idx].zero             = 0;

    // | 0x60 - set priv level to 3 when in user-mode
    idt_entries_[idx].flags = flags;
}

void Idt::idt_flush(uint32_t idt_ptr) {
    __asm__ volatile("lidt (%0)" : : "r"(idt_ptr));
}
