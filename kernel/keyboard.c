#include "keyboard.h"
#include "vga.h"

#define KB_DATA_PORT   0x60
#define KB_STATUS_PORT 0x64

extern uint8_t inb(uint16_t port);

static const char sc_to_ascii[128] = {
    0,   0,  '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,   'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/',
    0,   '*', 0, ' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0
};

static const char sc_to_ascii_shift[128] = {
    0,   0,  '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,   'A','S','D','F','G','H','J','K','L',':','"','~',
    0,  '|','Z','X','C','V','B','N','M','<','>','?',
    0,   '*', 0, ' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0
};

#define SC_LSHIFT   0x2A
#define SC_RSHIFT   0x36
#define SC_LSHIFT_R 0xAA
#define SC_RSHIFT_R 0xB6

static int shift_held = 0;

void keyboard_init(void) { shift_held = 0; }

char keyboard_getchar(void) {
    while (1) {
        while (!(inb(KB_STATUS_PORT) & 0x01));
        uint8_t sc = inb(KB_DATA_PORT);
        if (sc == SC_LSHIFT || sc == SC_RSHIFT)    { shift_held = 1; continue; }
        if (sc == SC_LSHIFT_R || sc == SC_RSHIFT_R){ shift_held = 0; continue; }
        if (sc & 0x80) continue;
        char c = shift_held ? sc_to_ascii_shift[sc] : sc_to_ascii[sc];
        if (c) return c;
    }
}
