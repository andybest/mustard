
.global gdt_flush_asm
.type gdt_flush_asm, @function
gdt_flush_asm:
    movl 4(%esp), %eax  // Get GDT pointer
    lgdt (%eax)         // Load new GDT

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x80, $flush
flush:
    ret
