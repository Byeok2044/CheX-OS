; io.asm — low-level port I/O helpers
; Exported as C-callable functions (cdecl, 32-bit)

global outb
global inb

section .text

; void outb(uint16_t port, uint8_t val)
; Stack: [esp+4] = port, [esp+8] = val
outb:
    mov dx, [esp+4]
    mov al, [esp+8]
    out dx, al
    ret

; uint8_t inb(uint16_t port)
; Stack: [esp+4] = port
; Returns: al
inb:
    mov dx, [esp+4]
    in  al, dx
    ret