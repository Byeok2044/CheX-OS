#include "vga.h"
#include "keyboard.h"
#include "shell.h"

void kernel_main(void) {
    vga_init();
    keyboard_init();
    shell_run();
}