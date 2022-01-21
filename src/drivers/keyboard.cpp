#include "keyboard.h"

extern const char ScanCodeTable[];

SCANCODE lastScanCode = 0;

extern "C" KeyState_t GetKeyState(KEYCODE vKey) {
    SCANCODE scanCode = inb(KEYBOARD_PORT);
    /* if the raw scancode converted to a Slav Dog OS keyCode is equal to the key specified */
    if (ScanCodeTable[scanCode] == vKey) {
        KeyState_t state;
        state.isDown = true;

        if (lastScanCode == scanCode + TO_KEYUP)
            state.firstTimeDown = true;
        else 
            state.firstTimeDown = false;
        lastScanCode = scanCode;
        return state;
    } else {
        KeyState_t state;
        state.isDown = false;
        lastScanCode = scanCode;
        return state;
    }
}

/* table for converting ps/2 scancodes to ascii */
const char ScanCodeTable[] {
    0, 1, '1', '2', '3',
    '4', '5', '6', '7', 
    '8', '9', '0', '-',
    '=', 2, 3, 'q', 'w',
    'e', 'r', 't', 'y', 
    'u', 'i' , 'o', 'p',
    '[', ']', 4, 5, 'a',
    's', 'd', 'f', 'g',
    'h', 'j', 'k', 'l',
    ';', '\'', '`', 6,
    '\\', 'z', 'x', 'c',
    'v', 'b', 'n', 'm',
    ',', '.', '/', 7, 
    '*', 8, ' ' 
};
