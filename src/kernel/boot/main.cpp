#include "platform.h"

extern "C" void kernel_main(void) {
    // Platform dependant initialization
    platform_init();
    
    
}