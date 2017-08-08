#pragma once
#include "../arch/x86/boot/multiboot.h"
#include "Terminal.h"

/* Functions that platforms should implement */

class Platform;

Platform *platform_init(MultibootInfo *pInfo);

class Platform {
   public:
    virtual TerminalDriver *defaultTerminal() = 0;
    virtual void            initialize()      = 0;
};
