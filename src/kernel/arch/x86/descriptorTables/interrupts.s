.text

.macro DEFISR_NOERR isr_num
    .global isr\isr_num
isr\isr_num:
    cli
    push $0
    push $\isr_num
    jmp isr_common
.endm

.macro DEFISR_ERR isr_num
    .global isr\isr_num
isr\isr_num:
    cli
    push $\isr_num
    jmp isr_common
.endm


isr_common:
    pusha

    push %ds
    push %es
    push %fs
    push %gs
    movl %cr2, %eax
    push %eax

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movl %esp, %eax
    push %eax

    call isr_handler
    
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

DEFISR_NOERR 0      // Divide By Zero
DEFISR_NOERR 1      // Debug Exception
DEFISR_NOERR 2      // Non Maskable Interrupt
DEFISR_NOERR 3      // Breakpoint
DEFISR_NOERR 4      // Into Detected Overflot
DEFISR_NOERR 5      // Out of Bounds
DEFISR_NOERR 6      // Invalid Opcode
DEFISR_NOERR 7      // No Coprocessor
DEFISR_ERR 8        // Double Fault
DEFISR_NOERR 9      // Coprocessor Segment Overrun
DEFISR_ERR 10       // Bad TSS
DEFISR_ERR 11       // Segment Not Present
DEFISR_ERR 12       // Stack Fault
DEFISR_ERR 13       // General Protection Fault
DEFISR_ERR 14       // Page Fault
DEFISR_NOERR 15     // Unknown Interrupt
DEFISR_NOERR 16     // Coprocessor fault
DEFISR_NOERR 17     // Alignment Check
DEFISR_NOERR 18     // Machine Check
DEFISR_NOERR 19
DEFISR_NOERR 20
DEFISR_NOERR 21
DEFISR_NOERR 22
DEFISR_NOERR 23
DEFISR_NOERR 24
DEFISR_NOERR 25
DEFISR_NOERR 26
DEFISR_NOERR 27
DEFISR_NOERR 28
DEFISR_NOERR 29
DEFISR_NOERR 30
DEFISR_NOERR 31