/* the interface for dealing with file I/O */
#include "filesystem.h"


void initFS() {
    /* finds the correct file system, then initializes it */
    switch (getFS()) {
    case FAT16:
        InitFat16();
        break;
    }
}