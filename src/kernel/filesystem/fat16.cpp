#include "fat16.h"

fat_data fdata;


void initFat16() {
    char bootSec[512];
    readBootSec(bootSec);
    fat_BS* fbs = (fat_BS*)bootSec;

    initFatData(fbs);

    FHANDLE hFile = fatopen("texdoc.txt");
    kout << hFile;
}

FHANDLE fatopen(std::string path) {
    fat_file_handle* internalHandle = new fat_file_handle;
    
    char buf[SEC_PER_CLUSTER * SECTOR_SIZE];

    fat_file_t* fp = (fat_file_t*)readCluster(fdata.firstRootDirSec + START_SECTOR, buf);
    
    // used to check we dont go out of bounds later
    char* orig = (char*)fp;
    if (fp->name[0] == DOT_ENTRY)
        fp++;

    while (isValidFile(fp) && (char*)fp < orig + SEC_PER_CLUSTER * SECTOR_SIZE) {
        bool dir = isDir(fp);
        std::string fname = fatToCStr(fp->name, dir);
        // case doesnt matter in fat
        os::upper(path);

        kout << fname << '\n';

        if (fname == path) {
            internalHandle->clusterNum = fp->cluster_num;
            return internalHandle;
        }
        fp++;       
    } 
    return nullptr;
}

// checks whether or not the given pointer to fat file struct is a valid file (by checking attribute byte)
bool isValidFile(fat_file_t* fp) {
    if (fp->name[0] != 0 && fp->name[0] != 0xE5)
        return true;
    return false;
}

// whether the file is a directory
bool isDir(fat_file_t* fp) {
    if (fp->attr == TYPE_DIRECTORY)
        return true;
    return false;
}

// converts from the way fat stores names, into a c str
std::string fatToCStr(const char* fatName, bool dir) {
    std::string cname;

    // calculating the size of the non extention part of the file
    int sz = 8;
    for (int i = 7; i >= 0; i--) {
        if (fatName[i] != ' ')
            break;
        sz --;
    }

    // adds the filename
    for (int i = 0; i < sz; i++) {
        cname += fatName[i];
    }
    
    if (!dir) {
        // dot before extention
        cname += '.';

        // last three letters for extention
        cname += fatName[8];
        cname += fatName[9];
        cname += fatName[10];

    }

    return cname;
}   

void initFatData(fat_BS* fbs) {
    fdata.fatSize = fbs->table_size_16;

    fdata.rootDirSecCount = ((fbs->root_entry_count * 32) + (fbs->bytes_per_sector - 1)) / fbs->bytes_per_sector;
    fdata.firstDataSec = fbs->reserved_sector_count + (fbs->table_count * fdata.fatSize) + fdata.rootDirSecCount;
    fdata.secPerCluster = fbs->sectors_per_cluster;
    fdata.firstRootDirSec = fdata.firstDataSec - fdata.rootDirSecCount;
}

uint32_t clusterToSector(uint16_t clusterNum) {
    return ((clusterNum - 2) * fdata.secPerCluster) + fdata.firstDataSec;
}

char* readAllFile(fat_file_t* fp) {
    char* buf = new char[fdata.secPerCluster];
    uint32_t beginSectors = clusterToSector(fp->cluster_num);
    kout << "sectors: " << beginSectors << '\n';
    read_sectors_ATA_PIO((uint32_t)buf, beginSectors + START_SECTOR, fdata.secPerCluster);
    return buf;
}


void* readCluster(uint32_t sectorNum, char* buf) {
    char* orig = buf;
    for (int i = 0; i < SEC_PER_CLUSTER; i++) {
        read_sectors_ATA_PIO((uint32_t)buf, sectorNum, 1);
        orig += SECTOR_SIZE;
    }
    return buf;
}
