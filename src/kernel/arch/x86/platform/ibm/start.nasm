; Multiboot header
MODULEALIGN equ  1<<0             ; align loaded modules on page boundaries
MEMINFO     equ  1<<1             ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002     ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)  ; checksum required

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM


VIRT_BASE equ 0x80000000

%define PHYS(x) ((x) - VIRT_BASE)
%define VIRT(x) ((x) + VIRT_BASE)

section .bootstrap
global _start
_start:

    ; Physical address of boot page table
    mov edi, PHYS(boot_page_table)
    ; Create identity map
    ; Present, writable, 4kb
    mov eax, 0x0003
    ; Number of pages for the kernel address space
    mov ecx, VIRT_BASE >> 12
    
loop_identity_map:
    ; Set up page table entry 
    mov [edi], eax
    
    ; Address of next physical page (+ 4kb)
    add eax, 0x1000
    ; Address of next entry
    add edi, 0x4
    
    dec ecx
    jnz loop_identity_map
    
    ; Map physical memory into the kernel address space
    
    ; Present, writable
    lea edx, [0x0003]
    ; Number of pages in the kernel address space
    lea ecx, [((0xFFFFFFFF - VIRT_BASE + 1) >> 12)]
    
loop_kernel_map:
    ; Set up page table entry
    mov [edi], edx
    
    ; Address of next physical page (+ 4kb)
    add edx, 0x1000
    ; Address of next entry
    add edi, 0x4
    
    dec ecx
    jnz loop_kernel_map
    
    ; Create the page directory
    mov edi, PHYS(boot_page_directory)
    mov edx, PHYS(boot_page_table) + 0x003
    
    ; Number of entries in the directory
    mov ecx, 0x400
    
loop_directory_entry:
    ; Set up page directory entry
    mov [edi], edx
    
    ; Address of next page (+ 4kb)
    add edx, 0x1000
    ; Address of next page directory entry
    add edi, 0x4
    
    dec ecx
    jnz loop_directory_entry
    
    
    ; Activate paging
    
    ; Physical address of the page directory
    mov ecx, PHYS(boot_page_directory)
    mov cr3, ecx
    
    ; Enable paging and set the write-protect bit
    mov ecx, cr0
    or ecx, 0x80010000
    mov cr0, ecx
    
    ; The cpu is still running in the higher half. Jump to the higher half
    ; with an absolute jump
    mov ecx, _higher_half
    jmp ecx
    
twirl:
    add byte [0xb8140], 0x1
    jmp twirl

extern kernel_main
section .text
global _higher_half
_higher_half:
    
    ; Code is now running in the virtual address space
    
    ; Unmap the identity mapping
    
    ; Physical address of boot page directory
    lea edi, [PHYS(boot_page_directory)]
    ; Number of directory entries below kernel address space
    lea ecx, [VIRT_BASE >> 12 >> 10]
    
    jmp twirl
    
loop_unmap_identity:
    ; Directory entry not present
    mov dword [edi], 0
    
    ; Address of next entry in the page directory (4kb)
    add edi, 0x4
    
    dec ecx
    jnz loop_unmap_identity
    
    
    ; Set up stack pointer
    mov esp, stack_top
    
    call kernel_main
    
    cli
k_stop:
    hlt
    jmp k_stop
    
    
section .bss
align 16
stack_bottom:
resb 16384  ; Reserve 16KiB for the stack
stack_top:


section .bss
align 4096
boot_page_directory:
resb 4096
boot_page_table:
resb 4096 * 1024
    