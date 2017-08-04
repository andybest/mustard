#include "Kernel.h"

Kernel::Kernel()
{

}

void Kernel::init(Platform *platform)
{
    platform_ = platform;

    this->terminal()->putString("Kernel Initialization Complete\n");
}

Kernel *Kernel::shared()
{
    static Kernel kernel;
    return &kernel;
}

TerminalDriver *Kernel::terminal()
{
    return platform_->defaultTerminal();
}





