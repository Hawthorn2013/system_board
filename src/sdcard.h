#ifndef __SDCARD_H__
#define __SDCARD_H__


#define SD_SECTOR_SIZE (512)
#define SD_BUFFER_SECTOR_MAX (20)

extern void init_DSPI_1(void);
extern int SD_init(void);
extern BYTE SD_read_block(DWORD sector, BYTE *buffer);
extern BYTE SD_write_block(DWORD sector, const BYTE *buffer);
extern BYTE SD_read_multiple_block(DWORD sector, DWORD n, BYTE buffer[][SD_SECTOR_SIZE]);
extern BYTE SD_write_multiple_block(DWORD sector, DWORD n, const BYTE buffer[][SD_SECTOR_SIZE]);
extern void clear_sd_buffer(BYTE buffer[][SD_SECTOR_SIZE]);


#endif