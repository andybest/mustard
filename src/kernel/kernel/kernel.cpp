#include "Kernel.h"

Kernel::Kernel() {}

void Kernel::init(Platform *platform) {
    platform_ = platform;
    platform->initialize();

    kputs("Kernel Initialized\n");
}

Kernel *Kernel::shared() {
    static Kernel kernel;
    return &kernel;
}

TerminalDriver *Kernel::terminal() { return platform_->defaultTerminal(); }
