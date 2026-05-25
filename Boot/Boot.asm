extern kernel_main

MBALIGN  equ 1 << 0
MEMINFO  equ 1 << 1
FLAGS    equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
_start:
    cli                  ; Disable interrupts — no IDT set up yet
    mov esp, stack_top   ; Set up the stack
    call kernel_main     ; Jump into the kernel

.hang:
    hlt                  ; Halt the CPU
    jmp .hang            ; If an interrupt wakes it, halt again