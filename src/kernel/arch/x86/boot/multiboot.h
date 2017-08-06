#pragma once

#include <stdint.h>

struct MultibootMemMap {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
};

struct MultibootInfo {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t symbols[4];
    
    uint32_t mmap_length;
    uint32_t mmap_addr;
    
    uint32_t drives_length;
    uint32_t drives_addr;
    
    uint32_t config_table;
    
    uint32_t boot_loader_name;
    
    uint32_t apm_table;
    
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_mode;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;
};