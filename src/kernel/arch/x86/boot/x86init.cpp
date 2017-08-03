#include "platform.h"
#include "../drivers/PCTerminalDriver.h"

void platform_init() {
    PCTerminalDriver terminalDriver;
    terminalDriver.clearScreen();
    terminalDriver.putChar('H');
}
