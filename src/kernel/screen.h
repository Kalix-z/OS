#ifndef SCREEN_H
#define SCREEN_H
/* all types for integers (uint16_t, uint32_t, etc) */
#include <stdint.h>
/* for converting ints to strings */
#include "functions.h"
/* lets you access the io ports in the proccessor */
#include <io.h>

/* where the GPU reads pixel data (mapped to 0xb8000) */
#define VIDEO_MEMORY 0xb8000
/* screen width, in text units */
#define SCREEN_WIDTH 80
/* screen height, in text units */
#define SCREEN_HEIGHT 25
/* how large one pixel is in memory, in bytes */
#define PIXEL_WIDTH 2
/* printing to the screen */
void kprintf(const char* str);

void setCursorPos(uint16_t position);

enum decimalTypes { decimal, hex };

class ostream {
public:
    decimalTypes dt = decimal;

    void print(const char* val) {
        kprintf (val);
    }

    void printp(void* p) {
      // temporarily switch to hex to print the address of the pointer
      decimalTypes oldDt = dt;
      dt = hex;
      printiu((uint32_t)p);
      dt = oldDt;
    }

    void printi(int val) {
        if (dt == hex) {
            char buf[12];
            itoa(buf, val, 16);
            kprintf (buf);
        } else {
            /* the stream is set to decimal */
            char buf[13];
            itoa(buf, val, 10);
            kprintf (buf);
        }
    }
    
    void printiu(uint32_t val) {
        if (dt == hex) {
            char buf[12];
            itoa(buf, val, 16);
            kprintf (buf);
        } else {
            /* the stream is set to decimal */
            char buf[13];
            itoa(buf, val, 10);
            kprintf (buf);
        }
    }
};

void clearScreen();

static ostream &operator<<(ostream &out, int val) {
  out.printi(val);
  return out;
}

static ostream &operator<<(ostream &out, uint32_t val) {
  out.printiu(val);
  return out;
}

static ostream &operator<<(ostream &out, char val) {
  char str[2];
  str[0] = val;
  str[1] = '\0';
  out.print(str);
  return out;
}

static ostream &operator<<(ostream &out, const char* val) {
  out.print(val);
  return out;
}

static ostream &operator<<(ostream &out, decimalTypes dt) {
  out.dt = dt;
  return out;  
}

static ostream &operator<<(ostream &out, void* p) {
  out.printp(p);
  return out;
}

static ostream &operator<<(ostream &out, bool val) {
  if (val == true) out.print("true");
  else out.print("false");
  return out;
}

extern ostream kout;

#endif