ENTRY(_start)
KERNEL_HIGH_VMA = 0xC0000000;

SECTIONS {
    . = 1M;
    KERNEL_BOOT_VMA = .;
    
    .boot : ALIGN(4k) {
        *(.multiboot)
        *(.bootstrap)
        . = ALIGN(4k);
    }
    
    . += KERNEL_HIGH_VMA;
    _core_start = .;
    
    .text : AT(ADDR(.text) - KERNEL_HIGH_VMA) ALIGN(4k) {
        _core_code = .;
        *(.text.*)
        *(.text)
    }
    
	.rodata : AT(ADDR(.rodata) - KERNEL_HIGH_VMA) ALIGN(4k) {
		*(.rodata.*)
		*(.rodata)
	}
    
	/* Read-write data (initialized) */
	.data : AT(ADDR(.data) - KERNEL_HIGH_VMA) ALIGN(4K) {
        _core_data = .;
		*(.data.*)
		*(.data)
	}
    
	/* Read-write data (uninitialized) and stack */
	.bss : AT(ADDR(.bss) - KERNEL_HIGH_VMA) {
        _core_bss = .;
		*(.bss.*)
		*(.bss)
		*(COMMON)
        _core_ebss = .;
	}
    
	/* Sections to be discarded */
	/DISCARD/ : {
		*(.text.exit)
		*(.data.exit)
		*(.exitcall.exit)
	}
    
    _core_end = .;
}