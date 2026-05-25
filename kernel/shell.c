#include "Shell.h"
#include "VGA.h"
#include "Keyboard.h"
#include <stddef.h>

#define INPUT_MAX 256
#define PROMPT    "CheX> "

/* ── string helpers (no libc in freestanding env) ── */

static size_t kstrlen(const char *s) {
    size_t n = 0;
    while (s[n]) n++;
    return n;
}

static int kstrcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

static int kstrncmp(const char *a, const char *b, size_t n) {
    while (n-- && *a && *a == *b) { a++; b++; }
    if (n == (size_t)-1) return 0;
    return (unsigned char)*a - (unsigned char)*b;
}

/* ── banner ── */

static void print_banner(void) {
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    vga_puts("  ____  _          __  __       ___  ____  \n");
    vga_puts(" / ___|| |__   ___ \\ \\/ /      / _ \\/ ___| \n");
    vga_puts("| |    | '_ \\ / _ \\ \\  /  ___ | | | \\___ \\ \n");
    vga_puts("| |___ | | | |  __/ /  \\ |___|| |_| |___) |\n");
    vga_puts(" \\____||_| |_|\\___/_/\\_\\        \\___/|____/ \n");
    vga_set_color(VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
    vga_puts("                                  v0.1 shell\n");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_puts("\n Type 'help' for available commands.\n\n");
}

/* ── individual commands ── */

static void cmd_help(void) {
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_puts("Commands:\n");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_puts("  help          - show this message\n");
    vga_puts("  clear         - clear the screen\n");
    vga_puts("  echo <text>   - print text to screen\n");
    vga_puts("  version       - show OS version\n");
    vga_puts("  reboot        - reboot the machine\n");
}

static void cmd_clear(void) {
    vga_clear();
}

static void cmd_echo(const char *args) {
    vga_puts(args);
    vga_putchar('\n');
}

static void cmd_version(void) {
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    vga_puts("CheX-OS v0.1 — x86 Assembly + C\n");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

/* Triple-fault reboot via keyboard controller */
static void cmd_reboot(void) {
    extern void outb(unsigned short port, unsigned char val);
    /* Pulse the reset line on the PS/2 controller */
    outb(0x64, 0xFE);
    /* Fallback: hang */
    while (1) __asm__ volatile ("hlt");
}

/* ── command dispatch ── */

static void dispatch(char *line) {
    /* Skip leading spaces */
    while (*line == ' ') line++;

    if (*line == '\0') return;   /* empty line */

    if (kstrcmp(line, "help") == 0) {
        cmd_help();
    } else if (kstrcmp(line, "clear") == 0) {
        cmd_clear();
    } else if (kstrcmp(line, "version") == 0) {
        cmd_version();
    } else if (kstrcmp(line, "reboot") == 0) {
        cmd_reboot();
    } else if (kstrncmp(line, "echo ", 5) == 0) {
        cmd_echo(line + 5);
    } else if (kstrcmp(line, "echo") == 0) {
        vga_putchar('\n');
    } else {
        vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        vga_puts("Unknown command: ");
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        vga_puts(line);
        vga_putchar('\n');
    }
}

/* ── prompt + readline ── */

static void print_prompt(void) {
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_puts(PROMPT);
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

static size_t readline(char *buf, size_t max) {
    size_t len = 0;
    while (1) {
        char c = keyboard_getchar();

        if (c == '\n') {
            vga_putchar('\n');
            break;
        }

        if (c == '\b') {
            if (len > 0) {
                len--;
                vga_putchar('\b');
            }
            continue;
        }

        if (len < max - 1) {
            buf[len++] = c;
            vga_putchar(c);
        }
    }
    buf[len] = '\0';
    return len;
}


void shell_run(void) {
    keyboard_init();
    vga_init();
    print_banner();

    static char input[INPUT_MAX];

    while (1) {
        print_prompt();
        readline(input, INPUT_MAX);
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        dispatch(input);
    }
}