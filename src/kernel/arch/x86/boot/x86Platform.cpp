
#include "x86Platform.h"


TerminalDriver *x86Platform::defaultTerminal()
{
    return reinterpret_cast<TerminalDriver *>(&term_);
}

x86Platform::x86Platform()
{
    this->term_.clearScreen();
    this->term_.putString("Platform initialized\n");
}
