#include "shell.h"
#include "vga.h"
#include "keyboard.h"

#define INPUT_MAX 256

/* Simple string equality — no string.h in a freestanding kernel */
static int k_streq(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

static void shell_process(const char *cmd) {
    if (k_streq(cmd, "clear")) {
        vga_clear();
    } else if (k_streq(cmd, "help")) {
        vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
        vga_puts("Commands: clear, help\n");
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
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