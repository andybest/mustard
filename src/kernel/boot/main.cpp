#include "Platform.h"
#include "Kernel.h"

extern "C" void kernel_main(void) {
    // Platform dependant initialization
    Platform *p = platform_init();

    Kernel *kernel = Kernel::shared();
    kernel->init(p);
}