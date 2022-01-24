#include "fat16.h"

fat_data fdata;


void initFat16() {

    

    char bootSec[512];
    readBootSec(bootSec);
    fat_BS* fbs = (fat_BS*)bootSec;
    
    initFatData(fbs);


    FHANDLE hFile = fatOpen("C:/dir1/file1.txt");

    if (!hFile) {
        kout << "Failed to open file";
        return;
    }

    uint32_t sz = fatFileSize(hFile);


    char* buf = new char[sz];

    fatFileRead(hFile, buf, sz);

    kout << buf;

    fatClose(hFile);
}

FHANDLE fatOpen(std::string path) {    
    char buf[CLUSTER_BYTES];

    // remove the C:/ (until there is support for multiple drives)
    path.erase(0, 3);

    std::vector<std::string> pathSplit = os::split(path, '/');
    

    fat_file_t* fp = nullptr;

    for (int i = 0; i < pathSplit.size(); i++) {
        // if its the first time going around, then start at the rootDirSector
        if (i == 0) {
            fp = (fat_file_t*)readCluster(fdata.firstRootDirSec + START_SECTOR, buf);
        } else if (fp->attr == TYPE_DIRECTORY) { 
            fp = (fat_file_t*)readCluster(clusterToSector(fp->cluster_num) + START_SECTOR, buf);
        }

        fp = findFileInDir(fp, pathSplit[i]);
        
        // if the path given was not valid
        if (!fp)
            return nullptr;        
    }

    // creating the pointer to the handle, to be returned to the caller
    fat_file_handle* internalHandle = new fat_file_handle;
    internalHandle->clusterNum = fp->cluster_num;
    internalHandle->size = fp->size;

    memcpy(internalHandle->name, fp->name, 8);
    memcpy(internalHandle->ext, (&fp->name+8), 3);
    return internalHandle;
}

uint32_t fatFileSize(FHANDLE file) {
    return ((fat_file_handle*)file)->size;
}



void fatClose(FHANDLE file) {
    // free the alloced fat_file_handle (apparently deleting a void* is undefined so cast to char*)
    delete (char*)file;
}

void fatFileRead(FHANDLE file, char* _buf, uint32_t sz, uint32_t off) {
    char* buf = _buf;
    fat_file_handle* handle = (fat_file_handle*)file;

    uint32_t numOfClusters = sizeToCluster(sz);

    uint32_t bytesRemaining = sz;

    uint16_t activeCluster = handle->clusterNum;

    char clusterBuf[CLUSTER_BYTES];

    for (int i = 0; i < numOfClusters; i++) {
        // make sure we are starting at the right offset
      //  if (clusterToSector(activeCluster) * SECTOR_SIZE)
        // if the remaining data is less than the size of a cluster
        if (bytesRemaining < CLUSTER_BYTES && bytesRemaining != 0) {
            // read into a buffer with enough space for the cluster in order to not run into buffer overflows
            readCluster(clusterToSector(activeCluster) + START_SECTOR, clusterBuf);
            memcpy(buf, clusterBuf, bytesRemaining);
            return;
        }
        readCluster(clusterToSector(activeCluster) + START_SECTOR, buf);
        //read_sectors_ATA_PIO((uint32_t)buf, clusterToSector(activeCluster) + START_SECTOR, 1);
        // gets the new cluster
        unsigned char FATTable[SECTOR_SIZE];
        unsigned int fatOffset = activeCluster * 2;
        unsigned int fatSector = fdata.firstFatSec + (fatOffset / SECTOR_SIZE);
        unsigned int entOffset = fatOffset % SECTOR_SIZE;
        read_sectors_ATA_PIO((uint32_t)FATTable, fatSector + START_SECTOR, 1);
        activeCluster = *(uint16_t*)&FATTable[entOffset];
        
        // there arent more sectors
        if (activeCluster >= FAT16_CLUSTER_LAST_MIN)
            return;

        bytesRemaining -= CLUSTER_BYTES;
        buf += CLUSTER_BYTES;
    }
}

fat_file_t* findFileInDir(fat_file_t* fp, std::string name) {
    
    char* orig = (char*)fp;

    // dont bother with the sped files
    if (fp->name[0] == DOT_ENTRY)
        fp++;
    
    while (isValidFile(fp) && (char*)fp < orig + CLUSTER_BYTES) {
        
        bool dir = isDir(fp);
        std::string fname = fatToCStr(fp->name, dir);
        // case doesnt matter in FAT
        os::upper(name);

        if (fname == name) {
            return fp;
        }
        fp++;       
    } 
    return 0;
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
    fdata.firstFatSec = fbs->reserved_sector_count;
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
        read_sectors_ATA_PIO((uint32_t)orig, sectorNum+i, 1);
        orig += SECTOR_SIZE;
    }
    return buf;
}
