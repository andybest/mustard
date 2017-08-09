#include "Kernel.h"
#include "Platform.h"
#include "x86Platform.h"

Platform *platform_init(MultibootInfo *pInfo) {
    static x86Platform platform(pInfo);
    return &platform;
}

extern "C" void kernel_main(MultibootInfo *mbInfoPhys) {
    auto *mbInfo = (MultibootInfo *)(((uint32_t)mbInfoPhys) + VIRT_BASE);

    Platform *p = platform_init(mbInfo);
    Kernel::shared()->init(p);
}
