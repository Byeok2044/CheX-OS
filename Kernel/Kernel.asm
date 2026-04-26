global kernel_main

section .data
    msg db 'CheX-OS v0.1', 0

section .text
kernel_main:
    ; VGA text mode - write directly to 0xB8000
    mov edi, 0xB8000
    mov esi, msg
    mov ah, 0x0F        ; White on black

.print_loop:
    lodsb
    test al, al
    jz .done
    stosw               ; Write char + attribute
    jmp .print_loop

.done:
    cli
    hlt