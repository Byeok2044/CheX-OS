global kernel_main

section .data
    msg db 'CheX-OS v0.1', 0

section .text
kernel_main:
    mov edi, 0xB8000
    mov esi, msg

.print_loop:
    lodsb               ; Load next char into al
    test al, al
    jz .done
    mov ah, 0x0F        ; White on black (set AFTER lodsb, every iteration)
    stosw               ; Write ax (al=char, ah=attr) to VGA buffer
    jmp .print_loop

.done:
    cli
    hlt