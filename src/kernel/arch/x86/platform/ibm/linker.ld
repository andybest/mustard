/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
  . = 0x00100000;
  /* You can use extern link_kernel_start in C to find where your kernel starts */
  link_kernel_start = .;

  /* Multiboot header must be in the first 4K of the kernel */
  .multiboot : {
    *(.multiboot)
  }

  .text : {
    *(.text)
  }

  .rodata ALIGN (0x1000) : {
    *(.rodata)
  }

  .data ALIGN (0x1000) : {
    *(.data)
  }

  .bss : {
    sbss = .;
    *(COMMON)
    *(.bss)
    ebss = .;
  }
  /* You can use extern link_kernel_end in C to find where your kernel ends */
  link_kernel_end = .;
}