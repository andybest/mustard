#include "Interrupts.h"

Idt::Idt() {

}

void Idt::initialize() {

}

void Idt::idt_flush() {
    __asm__ volatile(
        "lidt (%%eax)\n"          // Load new IDT
        ::"a" ((uint32_t) &idt_ptr_)
    );
}
