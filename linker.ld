/* linker.ld */
ENTRY(_start)

SECTIONS
{
    . = 0x100000;        /* Load kernel at 1MB (standard for multiboot) */

    .multiboot :
    {
        *(.multiboot)    /* Multiboot header must be first */
    }

    .text :
    {
        *(.text)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}