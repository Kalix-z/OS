#include <vector>
#include <keyboard.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <diskATA.h>
#include <fat16.h>
#include "memory.h"
#include "screen.h"

char ae[100];

extern "C" void kmain() {
    kout << "Starting OS\n";

    heapInit();
    gdtInit();
    idtInit();
    initFat16();

    while (true)
    {
        KeyState_t state = GetKeyState('a');

        if (state.isDown && state.firstTimeDown)
            kprintf("A down\n");
    }
    
}
