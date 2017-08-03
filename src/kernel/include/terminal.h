#pragma once

class TerminalDriver {
public:
    virtual void putChar(const char c) = 0;
    
    void putString(const char *str) {
        const char *p = str;
        while(*p++ != '0') {
            putChar(*p);
        }
    }
};

void kputc(char);
void kputs(const char *);
void kprintf(const char *format, ...);
