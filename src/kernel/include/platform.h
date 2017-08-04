#pragma once
#include "Terminal.h"

/* Functions that platforms should implement */

class Platform;

Platform *platform_init();

class Platform {
public:
    virtual TerminalDriver *defaultTerminal() = 0;
};
