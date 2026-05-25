#include "VGA.h"
#include "Keyboard.h"
#include "Shell.h"

void kernel_main(void) {
    vga_init();
    keyboard_init();
    shell_run();
}