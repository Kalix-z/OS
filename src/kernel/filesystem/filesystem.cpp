/* the interface for dealing with file I/O */
#include "filesystem.h"

// returns a handle to the file that can later be used to do I/O with the file
FHANDLE(*fileOpen)(std::string path);

// frees all allocated data and should be called after all file IO is done to a file, after this, the file handle becomes invalid and must not be used again
void(*fileClose)(FHANDLE file);

// reads from the file handle file, into the buffer buf, with a size of sz, starting from the offset offset
void(*fileRead)(FHANDLE file, char* buf, uint32_t size, uint32_t offset);

// returns size of the file
uint32_t(*fileSize)(FHANDLE file);

// creates a file
FHANDLE(*fileCreate)(const char* path, const char* name, const char* ext);

void initFS() {
    /* finds the correct file system, then initializes it */
    switch (getFS()) {
    case FAT16:
        initFat16();
        fileOpen = fatOpen;
        fileClose = fatClose;
        fileRead = fatFileRead;
        fileSize = fatFileSize;
        break;
    }
}

FileSys getFS() {
    return FAT16;
}