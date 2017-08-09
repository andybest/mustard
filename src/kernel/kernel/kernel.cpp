#include "Kernel.h"

Kernel::Kernel() {}

void Kernel::init(Platform *platform) {
    platform_ = platform;
    platform->initialize();

    kputs("Kernel Initialized\n");

    const char *foo = "Hello, world";
    kprintf("Hex: %x  String: %s  Int: %d", 0xCAFEBABE, foo, 12345);
}

Kernel *Kernel::shared() {
    static Kernel kernel;
    return &kernel;
}

TerminalDriver *Kernel::terminal() { return platform_->defaultTerminal(); }
