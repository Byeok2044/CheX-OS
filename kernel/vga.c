#include "VGA.h"

#define VGA_WIDTH    80
#define VGA_HEIGHT   25
#define VGA_MEMORY   ((uint16_t *)0xB8000)

/* Hardware cursor I/O ports */
#define VGA_CTRL_REG  0x3D4
#define VGA_DATA_REG  0x3D5

/* Provided by IO.asm */
extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);

static size_t    vga_row;
static size_t    vga_col;
static uint8_t   vga_attr;
static uint16_t *vga_buf;

static inline uint8_t make_attr(vga_color_t fg, vga_color_t bg) {
    return (uint8_t)((bg << 4) | (fg & 0x0F));
}

static inline uint16_t make_entry(char c, uint8_t attr) {
    return (uint16_t)c | ((uint16_t)attr << 8);
}

static void hw_cursor_update(void) {
    uint16_t pos = (uint16_t)(vga_row * VGA_WIDTH + vga_col);
    outb(VGA_CTRL_REG, 14);
    outb(VGA_DATA_REG, (uint8_t)(pos >> 8));
    outb(VGA_CTRL_REG, 15);
    outb(VGA_DATA_REG, (uint8_t)(pos & 0xFF));
}

static void scroll(void) {
    /* Move all rows up by one */
    for (size_t r = 1; r < VGA_HEIGHT; r++)
        for (size_t c = 0; c < VGA_WIDTH; c++)
            vga_buf[(r - 1) * VGA_WIDTH + c] = vga_buf[r * VGA_WIDTH + c];

    /* Blank the last row */
    for (size_t c = 0; c < VGA_WIDTH; c++)
        vga_buf[(VGA_HEIGHT - 1) * VGA_WIDTH + c] =
            make_entry(' ', vga_attr);

    vga_row = VGA_HEIGHT - 1;
}

void vga_init(void) {
    vga_buf  = VGA_MEMORY;
    vga_row  = 0;
    vga_col  = 0;
    vga_attr = make_attr(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_clear();
}

void vga_set_color(vga_color_t fg, vga_color_t bg) {
    vga_attr = make_attr(fg, bg);
}

void vga_clear(void) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga_buf[i] = make_entry(' ', vga_attr);
    vga_row = 0;
    vga_col = 0;
    hw_cursor_update();
}

void vga_set_cursor(size_t row, size_t col) {
    vga_row = row;
    vga_col = col;
    hw_cursor_update();
}

void vga_putchar(char c) {
    if (c == '\n') {
        vga_col = 0;
        if (++vga_row == VGA_HEIGHT)
            scroll();
    } else if (c == '\r') {
        vga_col = 0;
    } else if (c == '\b') {
        if (vga_col > 0) {
            vga_col--;
            vga_buf[vga_row * VGA_WIDTH + vga_col] =
                make_entry(' ', vga_attr);
        }
    } else {
        vga_buf[vga_row * VGA_WIDTH + vga_col] = make_entry(c, vga_attr);
        if (++vga_col == VGA_WIDTH) {
            vga_col = 0;
            if (++vga_row == VGA_HEIGHT)
                scroll();
        }
    }
    hw_cursor_update();
}

void vga_puts(const char *str) {
    while (*str)
        vga_putchar(*str++);
}