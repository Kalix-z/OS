#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include "screen.h"

struct block {
    size_t size; 
    int free;    
    struct block *next;  
};


void initialize();

void *kmalloc(size_t noOfBytes);

void kfree(void* ptr);

#endif