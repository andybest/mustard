#pragma once

#include "Platform.h"
#include "Terminal.h"

class Kernel {
   public:
    Kernel();

    // Pointer to static instance
    static Kernel *shared();

    TerminalDriver *terminal();

    void init(Platform *platform);

   private:
    Platform *platform_;
};

void kputs(const char *str);
void print_hex(const uint32_t hex);
void kprintf(const char *fmt, ...);
void kpanic(const char *fmt, ...);