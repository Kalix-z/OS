#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "fat16.h"

/* to reference any file kind (fat16, ext4, etc) a void pointer can be used and later converted to the correct file system */
#define __TEMPLATE_FILE_P void*

enum fileSysType {
    FAT16
};

void initFS();

fileSysType getFS();

class file {
private:
    __TEMPLATE_FILE_P internalFile;

public:
    void read(size_t offset, size_t size, char* buf);

    std::string readStr(size_t offset);

};

#endif