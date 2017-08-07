#pragma once
#include "Terminal.h"
#include "../arch/x86/boot/multiboot.h"

/* Functions that platforms should implement */

class Platform;

Platform *platform_init(MultibootInfo *pInfo);

class Platform {
public:
    virtual TerminalDriver *defaultTerminal() = 0;
    virtual void initialize() = 0;
};
