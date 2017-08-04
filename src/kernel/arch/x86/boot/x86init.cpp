#include "Platform.h"
#include "x86Platform.h"

Platform *platform_init() {
    static x86Platform platform;
    return &platform;
}