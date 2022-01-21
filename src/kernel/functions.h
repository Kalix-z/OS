#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* str);

void memcpy(void *dest, const void *src, uint32_t len);

void memset(uint8_t *dest, uint8_t val, uint32_t len);

void itoa (char* buf, int num, int base);

void itoa (char* buf, uint32_t num, int base);

int abs (int val);

#endif