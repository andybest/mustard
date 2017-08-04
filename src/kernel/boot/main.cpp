#include "Platform.h"

extern "C" void kernel_main(void) {
    // Platform dependant initialization
    Platform *p = platform_init();

    p->defaultTerminal()->putString("Kernel init\n");
    
}