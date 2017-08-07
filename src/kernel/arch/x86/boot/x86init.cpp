#include "Platform.h"
#include "x86Platform.h"
#include "Kernel.h"

Platform *platform_init(MultibootInfo *pInfo) {
    static x86Platform platform(pInfo);
    return &platform;
}

void print_hex(const uint32_t hex) {
    char buf[9];

    for (char &i : buf) {
        i = 0;
    }

    buf[8] = '\0';
    
    char hexMap[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };
    
    for(int i = 7; i >= 0; i--) {
        uint32_t idx = (hex >> (i * 4)) & 0xF;
        buf[7 - i] = hexMap[idx];
    }
    
    kputs(buf);
}


extern "C" void kernel_main(MultibootInfo *mbInfoPhys) {
    auto *mbInfo = (MultibootInfo *)(((uint32_t)mbInfoPhys) + VIRT_BASE);
    
    Platform *p = platform_init(mbInfo);
    Kernel::shared()->init(p);
}
