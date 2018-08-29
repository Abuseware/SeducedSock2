OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
    .multiboot : ALIGN(8) {
        *(.multiboot)
    }
    . = 1M;
    .text BLOCK(4K) : ALIGN(4K) {
        *(.text)
    }
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }
    .bss BLOCK(4K) : ALIGN(4K) {
        *(.bss)
    }
    _heap = ALIGN(4);
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.eh_frame_hdr)
    }
}

