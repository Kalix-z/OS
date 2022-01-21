#include "memory.h"

#define HEAP_SIZE 0x100000
char memory[2];// = {1};
#define MB(x)   ((size_t) (x) << 20)
size_t alloced = 0;
void* allocPtr = (void*)MB(0x20);
struct block *freeList = (block*)memory;

void initialize() {
    freeList->size = HEAP_SIZE - sizeof(block);
    freeList->free = 1;
    freeList->next = NULL;
}

void split(struct block *fitting_slot, size_t size) {
    struct block *newBlock=(block*)((void*)fitting_slot+size+sizeof(struct block));
    newBlock->size=(fitting_slot->size)-size-sizeof(struct block);
    newBlock->free=1;
    newBlock->next=fitting_slot->next;
    fitting_slot->size=size;
    fitting_slot->free=0;
    fitting_slot->next=newBlock;
}


void *kmalloc(size_t noOfBytes) {
    void* p = allocPtr;
    *(size_t*) &allocPtr += noOfBytes;
    return p; 
    alloced += noOfBytes;
    block *curr,*prev;
    void *result;
    if(!freeList->size){
        initialize();
    }
    curr = freeList;
    while( ( (curr->size < noOfBytes ) || (curr->free == 0) ) && (curr->next != nullptr) ) {
        prev = curr;
        curr = curr->next;
    }
    if( (curr->size) == noOfBytes) {
        curr->free = 0;
        curr++;
        result = (void*)curr;
        return result;
    }
    else if(curr->size > ( noOfBytes + sizeof(block) ) ) {
        split(curr, noOfBytes);
        curr++;
        result = (void*)curr;
        return result;
    }
    else {
        result = nullptr;
        kout << "HEAP ERROR: Insuficent Memory\n";
        kout << "Bytes Allocated: " << hex << "0x" << (uint32_t)alloced << '\n';
        asm ("cli   \n hlt");
        return result;
    }
}

void merge(){
 struct block *curr,*prev;
 curr=freeList;
 while((curr->next)!=NULL){
  if((curr->free) && (curr->next->free)){
   curr->size+=(curr->next->size)+sizeof(struct block);
   curr->next=curr->next->next;
  }
  prev=curr;
  curr=curr->next;
 }
}

void kfree(void* ptr){
    return;
 if(((void*)memory<=ptr)&&(ptr<=(void*)(memory+HEAP_SIZE))){
  struct block* curr=(block*)ptr;
  --curr;
  curr->free=1;
  merge();
 }
 else {
    kout << "Invalid Pointer\n";
    asm ("cli \n hlt");
 }

}

void* operator new(size_t sz) {
    return kmalloc (sz);
    
}

void* operator new[](size_t sz) {
    return kmalloc (sz);
}

void operator delete (void* addr) {
    kfree(addr);
}

void operator delete (void* addr, size_t __unused) {
    kfree(addr);
}

void operator delete[] (void* addr) {
    kfree(addr);
}