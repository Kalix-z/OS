#ifndef DISKATA_H
#define DISKATA_H

#include <stdint.h>
#include <io.h>
#include <screen.h>

void write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, uint32_t* bytes);

void read_sectors_ATA_PIO(uint32_t target_address, uint32_t LBA, uint8_t sector_count);

#endif