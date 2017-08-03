#pragma once
#include "terminal.h"

/* Functions that platforms should implement */

void platform_init();

class Platform {
public:
    virtual TerminalDriver *defaultTerminal() = 0;
};
