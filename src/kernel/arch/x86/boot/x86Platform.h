#pragma once

#include "Platform.h"
#include "../drivers/PCTerminalDriver.h"

class x86Platform: public Platform {
public:
    x86Platform();

    x86Platform(const PCTerminalDriver &term_);

    TerminalDriver *defaultTerminal();

private:
    PCTerminalDriver term_;
};