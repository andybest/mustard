
#include "PCTerminalDriver.h"

PCTerminalDriver::PCTerminalDriver() {
    cursorX_ = 0;
    cursorY_ = 0;
}

void PCTerminalDriver::putChar(const char c) {
    if (c == '\n') {
        newLine();
    } else if(c == '\t') {
        putChar(' ');
        putChar(' ');
        putChar(' ');
        putChar(' ');
    } else {
        uint16_t *currentChar =
            kVideoMemoryBase + (cursorY_ * kTerminalWidth) + cursorX_;
        *currentChar = static_cast<uint16_t>(c | 0x0700);

        cursorX_++;

        if (cursorX_ >= kTerminalWidth) {
            newLine();
        }
    }
}

void PCTerminalDriver::clearScreen() {
    for (uint32_t i = 0; i < kVideoMemorySize; i++) {
        kVideoMemoryBase[i] = 0x0000;
    }
}

void PCTerminalDriver::newLine() {
    cursorX_ = 0;
    cursorY_ += 1;

    if (cursorY_ >= kTerminalHeight) {
         scrollBuffer();
        cursorY_ = static_cast<uint8_t>(kTerminalHeight - 1);
    }
}

void PCTerminalDriver::scrollBuffer() {
    // Scroll all lines up by 1
    for (uint32_t line = 1; line < kTerminalHeight; line++) {
        uint16_t *lineStart = kVideoMemoryBase + (line * kTerminalWidth);
        uint16_t *prevLineStart =
            kVideoMemoryBase + ((line - 1) * kTerminalWidth);

        for (uint32_t x = 0; x < kTerminalWidth; x++) {
            prevLineStart[x] = lineStart[x];
        }
    }

    uint16_t *lastLineStart =
        kVideoMemoryBase + ((kTerminalHeight - 1) * kTerminalWidth);

    for (uint32_t x = 0; x < kTerminalWidth; x++) {
        lastLineStart[x] = 0x0000;
    }
}
