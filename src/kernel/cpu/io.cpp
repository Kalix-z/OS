#include "io.h"

/* outputs val on port */
void outb(uint16_t port, uint8_t val) { 
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}



/* reads one byte from port port */
unsigned char inb(uint16_t port){ 
    uint8_t returnVal;
    asm volatile ("inb %1, %0"
    : "=a"(returnVal)
    : "Nd"(port));
    return returnVal;
}

/* reads 2 bytes from port port */
uint16_t inw (uint16_t port) {
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}


/* outputs val on port */
void outw (unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

/* outputs val on port */
void outl(uint32_t port, uint32_t value) {
    __asm__ __volatile__("outl %%eax,%%dx"::"d" (port), "a" (value));
};

/* reads 4 bytes from port port */
uint32_t inl(uint32_t port) {
    uint32_t result;
    __asm__ __volatile__("inl %%dx,%%eax":"=a" (result):"d"(port));
    return result;
}