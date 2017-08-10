
#include <stdarg.h>
#include <stdlib.h>
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

void sprint_hex(const uint32_t hex, char *buffer, int *bufIdx) {
    char hexMap[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                       '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (int i = 7; i >= 0; i--) {
        uint32_t idx            = (hex >> (i * 4)) & 0xF;
        buffer[7 - i + (*bufIdx)] = hexMap[idx];
    }

    (*bufIdx) += 8;
}

void reverse(char *str, int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        char s = str[start];
        char e = str[end];

        str[start] = e;
        str[end] = s;
        //swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = "0123456789ABCDEF"[rem % base];//(rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);
 
    return str;
}

int kstrlen(const char *str) {
    int         len  = 0;
    const char *sptr = str;

    while (*sptr != '\0') {
        len += 1;
        sptr++;
    }

    return len;
}

char tempBuf[64];

int kvasprintf(char *buffer, const char *fmt, va_list args) {
    int len    = kstrlen(fmt);
    int bufIdx = 0;

    for (int i = 0; len > 0 && fmt[i] != '\0'; i++) {
        if (fmt[i] != '%') {
            // Format arg
            buffer[bufIdx++] = fmt[i];
            continue;
        }

        // Increment past the percent
        i++;

        char *   strval;
        uint32_t intval;

        switch (fmt[i]) {
            case 's':
                // String pointer
                strval = (char *)va_arg(args, char *);
                while (*strval) {
                    buffer[bufIdx++] = *strval++;
                }

                break;

            case 'x':
                // Hex
                intval = (uint32_t)va_arg(args, uint32_t);
                /*strval = itoa(intval, tempBuf, 16);

                while (*strval) {
                    buffer[bufIdx++] = *strval++;
                }*/
                sprint_hex(intval, buffer, &bufIdx);

                break;

            case 'd':
                // Decimal
                intval = (uint32_t)va_arg(args, uint32_t);
                strval = itoa(intval, tempBuf, 10);

                while (*strval) {
                    buffer[bufIdx++] = *strval++;
                }
                break;

            case '%':
                // Escaped %
                buffer[bufIdx++] = '%';
                break;

            default:
                buffer[bufIdx++] = fmt[i];
                break;
        }
    }

    buffer[bufIdx] = '\0';
    return bufIdx;
}

void kprintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    char buffer[1024];
    kvasprintf(buffer, fmt, ap);
    kputs(buffer);
    va_end(ap);
}

void kpanic(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    char buffer[1024];
    kvasprintf(buffer, fmt, ap);
    kputs("PANIC: ");
    kputs(buffer);
    va_end(ap);

    while(true) {}
}