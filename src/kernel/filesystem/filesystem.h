#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "fat16.h"

// general file handle
#ifndef FHANDLE
#define FHANDLE void*
#endif

enum FileSys {
    FAT16
};

// returns a handle to the file that can later be used to do I/O with the file
extern FHANDLE(*fileOpen)(std::string path);

// frees all allocated data and should be called after all file IO is done to a file, after this, the file handle becomes invalid and must not be used again
extern void(*fileClose)(FHANDLE file);

// reads from the file handle file, into the buffer buf, with a size of sz, starting from the offset offset
extern void(*fileRead)(FHANDLE file, char* buf, uint32_t size, uint32_t offset);

// returns size of the file
extern uint32_t(*fileSize)(FHANDLE file);


// initalizes whichever file system the computer is running
void initFS();

FileSys getFS();


#endif