

global gdt_flush
gdt_flush:
    mov eax, [esp + 4]  ; Get the GDT pointer from the parameter
    lgdt [eax]          ; Load the new GDT pointer
    

; Reload the segement registers into the CPU
global reload_segments
reload_segments:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax