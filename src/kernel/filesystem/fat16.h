#ifndef FAT16_H
#define FAT16_H



#include <vector>
#include <string>
#include <stdint.h>
#include <functions.h>
#include <diskATA.h>
#include <screen.h>

#define SEC_PER_CLUSTER              0x4
#define SECTOR_SIZE                  0x200
#define START_SECTOR                 0x5000
#define FAT16_CLUSTER_FREE           0x0000
#define FAT16_CLUSTER_RESERVED_MIN   0xfff0
#define FAT16_CLUSTER_RESERVED_MAX   0xfff6
#define FAT16_CLUSTER_BAD            0xfff7
#define FAT16_CLUSTER_LAST_MIN       0xfff8
#define FAT16_CLUSTER_LAST_MAX       0xffff
#define LFN_ATTR                     0x0F
#define CLUSTER_BYTES                (SECTOR_SIZE * SEC_PER_CLUSTER)

#define FAT_MAX_NAME                 8
#define FAT_MAX_EXT                  3
#define FAT_DIRENTRY_DELETED         0xE5
#define FAT_DIRENTRY_LFNLAST         (1 << 6)
#define FAT_DIRENTRY_LFNSEQMASK      ((1 << 6) - 1)

#define ROOT_DIR                     0
#define DOT_ENTRY                    0x2E
#define ENTRY_DELETED                0xE5
#define TYPE_DIRECTORY               0x10
#define NORMAL_FILE                  0x20

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

typedef PACK(struct fat_extBS_16
{
	//extended fat12 and fat16 stuff
	unsigned char		bios_drive_num;
	unsigned char		reserved1;
	unsigned char		boot_signature;
	unsigned int		volume_id;
	unsigned char		volume_label[11];
	unsigned char		fat_type_label[8];

})fat_extBS_16_t;

typedef struct PACK (fat_BS
{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 	    bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;

	//this will be cast to it's specific type once the driver actually knows what type of FAT this is.
	unsigned char		extended_section[54];

}) fat_BS_t;

typedef struct  fat_file {
	uint8_t 	lfn_order;
	uint16_t 	first_5[5];
	uint8_t 	lfn_attr;
	uint8_t     entry_type;
	uint8_t     checksum;
	uint16_t    next6[6];
	uint16_t    zero1;
	uint16_t    last2[2];
	char        name[11];
	uint8_t 	attr;
	uint8_t 	reserved;
	uint8_t    	creaton_time_tenths;
	uint16_t   	creation_time;
	uint16_t   	creation_date;
	uint16_t   	last_accesed;
	uint16_t 	zero2;
	uint16_t 	modification_time;
	uint16_t 	modification_date;
	uint16_t 	cluster_num;
	uint32_t 	size;
}__attribute__((__packed__)) fat_file_t;

struct fat_data {
	uint32_t    firstDataSec;
	uint8_t     secPerCluster;
	uint32_t    firstRootDirSec;
	uint32_t    firstFatSec;
	uint32_t    rootDirSecCount;
	uint32_t    fatSize;
};

struct fat_file_handle {
	uint16_t dirClusterNum;
	uint16_t clusterNum;
	uint32_t size;
	char name[8];
	char ext[3];

};



#ifndef FHANDLE
#define FHANDLE void*
#endif

inline static void readBootSec(char* ptr) {
    read_sectors_ATA_PIO((uint32_t)ptr, START_SECTOR, 1);
}

inline static uint32_t sizeToCluster(uint32_t sz) {
	const uint32_t clusterSz = SEC_PER_CLUSTER * SECTOR_SIZE;
	// rounds up value to the nearest cluster size
    sz += clusterSz;
    sz /= clusterSz;
	return sz;
}
// initializes the FAT-16 driver, must be called before any FAT file I/O
void initFat16();
// returns a handle to the file that can later be used to do io with the file
FHANDLE fatOpen(std::string path);
// frees all allocated data and should be called after all file IO is done to a file, after this, the file handle becomes invalid and must not be used again
void fatClose(FHANDLE file);
// reads from the file handle file, into the buffer buf, with a size of sz, starting from the offset off
void fatFileRead(FHANDLE file, char* buf, uint32_t sz, uint32_t off);
// writes to the file handle file, from the buffer buf, with a size of sz, starting from the offset off
void fatFileWrite(FHANDLE file, const char* buf, uint32_t sz, uint32_t off);
// returns the size of the file
uint32_t fatFileSize(FHANDLE file);
// creates a new file at the path with the name and ext given
FHANDLE fatCreate(const char* path, const char* name, const char* ext);


// Internal FAT-16 driver functions, do not call
bool fatFileDeleted(fat_file_t* fp);
void writeCluster(uint32_t sectorNum, char* buf);
void* readCluster(uint32_t sectorNum, char* buf);
bool isValidFile(fat_file_t* fp);
void reserveCluster(uint16_t* outCluster, uint32_t* outSector);
fat_file_t* findFileInDir(fat_file_t* fp, std::string name);
uint32_t clusterToSector(uint16_t clusterNum);
uint16_t sectorToCluster(uint32_t sector);
void initFatData(fat_BS* fbs);
std::string fatToCStr(const char* fatName, bool dir);
bool isDir(fat_file_t* fp);







#endif