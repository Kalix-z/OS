OUTPUT_FORMAT(elf32-i386)
ENTRY (_start)

INPUT 
(   
    bin/kmain.o
    bin/multiboot_header.o
    bin/_start.o
    bin/io.o
    bin/screen.o
    bin/keyboard.o
    bin/gdt.o
    bin/gdtasm.o
    bin/idt.o
    bin/interupt.o
    bin/idtasm.o
    bin/functions.o
    bin/isr.o
    bin/filesystem.o
    bin/fat16.o
    bin/memory.o
    bin/diskATA.o
)

OUTPUT 
(
    bin/kernel.bin
)

SECTIONS
{

    . = 1M;

    .boot :
    {
        *(.multiboot_header)
    }

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text)
    }
    
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

}