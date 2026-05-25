#include "Shell.h"
#include "VGA.h"
#include "Keyboard.h"

#define INPUT_MAX 256

static void shell_process(const char *cmd) {
    if (cmd[0] == 'c' && cmd[1] == 'l' && cmd[2] == 'e'
     && cmd[3] == 'a' && cmd[4] == 'r' && cmd[5] == '\0') {
        vga_clear();
    } else if (cmd[0] != '\0') {
        vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        vga_puts("Unknown command: ");
        vga_puts(cmd);
        vga_putchar('\n');
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    }
}

void shell_run(void) {
    char buf[INPUT_MAX];
    int  pos = 0;

    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_puts("CheX-OS v0.1\n");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    while (1) {
        vga_set_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
        vga_puts("chex> ");
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        pos = 0;

        while (1) {
            char c = keyboard_getchar();
            if (c == '\n') {
                vga_putchar('\n');
                buf[pos] = '\0';
                break;
            } else if (c == '\b') {
                if (pos > 0) { pos--; vga_putchar('\b'); }
            } else if (pos < INPUT_MAX - 1) {
                buf[pos++] = c;
                vga_putchar(c);
            }
        }
        shell_process(buf);
    }
}