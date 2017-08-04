
#include "Kernel.h"

void kputs(const char *str) {
    Kernel::shared()->terminal()->putString(str);
}