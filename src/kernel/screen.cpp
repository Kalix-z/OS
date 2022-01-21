#include "screen.h"

uint16_t cursorPos = 0;

/* creates ostream object that other functions can access */
ostream kout;

void setCursorPos(uint16_t position) { 
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint16_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint16_t)((position >> 8) & 0xFF));
    cursorPos = position;
}

void clearScreen() {
    uint16_t* ptr = (uint16_t*)VIDEO_MEMORY;
    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        *ptr = 0;
        ptr++;
    }
    
}

/* draws str to the cursor position on the screen */
void kprintf(const char* str) {
    uint8_t* charPtr = (uint8_t*) str;
    uint16_t index = cursorPos;
    while (*charPtr != 0) {
        switch (*charPtr)
        {
        case '\n':
            index += SCREEN_WIDTH;
            index -= index % SCREEN_WIDTH;
            charPtr++; 
            break;
        
        default:
            *((char*)VIDEO_MEMORY + index * PIXEL_WIDTH) = *charPtr;
            index++;
            charPtr++; 
            break;
        }
        
    }
    setCursorPos(index);
}