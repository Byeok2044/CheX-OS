; Kernel/Kernel.asm — entry point, calls C kernel_main
global kernel_main
extern shell_run

section .text
kernel_main:
    call shell_run   ; hand off to C shell
    cli
.hang:
    hlt
    jmp .hang