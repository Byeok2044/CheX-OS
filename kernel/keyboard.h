#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);

/* Block until a printable key (or Enter/Backspace) is pressed.
   Returns the ASCII character. */
char keyboard_getchar(void);

#endif