#include <vector>
#include <keyboard.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <filesystem.h>
#include "memory.h"
#include "screen.h"

extern "C" void syscall(registers_t regs);

char* buf;

extern "C" void kmain() {
    kout << "Starting OaS\n";

    heapInit();
    gdtInit();
    idtInit();
    initFS();

    register_interrupt_handler(48, syscall);

    FHANDLE f = fatOpen("C:/dir1/code/testPrm/program.bin");

    if (!f) {
        kout << "una problema";
        while (1) {}
    }
        

    uint32_t sz = fatFileSize(f);
    buf = new char[sz+1];

    fatFileRead(f, buf, sz, 0);

    int(*main)() = (int(*)())&buf[0x1010];

    main();

    
    while (true)
    {
        KeyState_t state = GetKeyState('a');

        if (state.isDown && state.firstTimeDown)
            kprintf("A down\n");
    }
    
}

extern "C" void syscall(registers_t regs) {
    kout << *(char*)((uint32_t)buf + 0x2000) << '\n';
    kout << "ebx: " << hex << (uint32_t)regs.ebx << "\nbuf: " << hex << (uint32_t)buf + 0x2000;// + 0x2000; //+ (uint32_t)buf);
    kout << "iasdnt";
    while(true){}
}
