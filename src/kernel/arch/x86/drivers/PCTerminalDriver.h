#pragma once

#include "Terminal.h"
#include <stdint.h>

class PCTerminalDriver: public TerminalDriver {
public:
    PCTerminalDriver();
    
    void putChar(const char c);

    void clearScreen();
    
private:
    uint16_t *kVideoMemoryBase = (uint16_t *)0xC00B8000;
    const uint32_t kVideoMemorySize = 80*25;
    const uint32_t kTerminalWidth = 80;
    const uint32_t kTerminalHeight = 25;

    uint8_t cursorX_;
    uint8_t cursorY_;

    void newLine();

    void scrollBuffer();
};
