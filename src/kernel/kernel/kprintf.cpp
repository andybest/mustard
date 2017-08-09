
#include "Kernel.h"

void kputs(const char *str) { Kernel::shared()->terminal()->putString(str); }

void print_hex(const uint32_t hex) {
    char buf[9];

    for (char &i : buf) {
        i = 0;
    }

    buf[8] = '\0';

    char hexMap[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                       '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (int i = 7; i >= 0; i--) {
        uint32_t idx = (hex >> (i * 4)) & 0xF;
        buf[7 - i]   = hexMap[idx];
    }

    kputs(buf);
}