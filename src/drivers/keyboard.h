#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "io.h"

/* OS specific keycode type */
#define KEYCODE uint8_t
/* ps/2 scancode type */
#define SCANCODE KEYCODE
/* where to read from to get keyboard input */
#define KEYBOARD_PORT 0x60
/* value to convert a ps/2 scancode from a KeyDown case to a KeyUp case */
#define TO_KEYUP 128


typedef struct KeyState {
    bool isDown;
    bool firstTimeDown;

} KeyState_t;

extern "C" KeyState_t GetKeyState(KEYCODE vKey);

#endif