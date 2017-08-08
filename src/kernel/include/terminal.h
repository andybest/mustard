#pragma once

class TerminalDriver {
   public:
    virtual void putChar(const char c) = 0;

    void putString(const char *str) {
        unsigned int i = 0;

        while (str[i] != '\0') {
            putChar(str[i]);
            i++;
        }
    }
};

void kputc(char);
void kputs(const char *);
void kprintf(const char *format, ...);
