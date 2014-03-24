#ifndef __SDCARD_H__
#define __SDCARD_H__


#define SD_SECTOR_SIZE (512)

extern uint8_t sd_buffer[];

extern void init_DSPI_2(void);
extern void init_DSPI_1(void);
extern void INTC_DSPI_2_TC(void);
extern void INTC_DSPI_1_TC(void);
void DSPI_2_TX(uint16_t cmd, uint16_t data);
extern void DSPI_1_TX(uint16_t data);

extern uint8_t DSPI_read_write_byte(uint8_t byte_write);
extern uint8_t SD_send_cmd(uint8_t cmd, uint32_t var);
extern uint8_t SD_reset(void);
extern uint8_t SD_read_block(uint32_t sector, uint8_t *buffer);
extern uint8_t SD_write_block(uint32_t sector, uint8_t *buffer);
extern void DSPI_send_8_clocks(void);
extern void SD_SPI_to_4M(void);
extern void clear_sd_buffer(uint8_t *buffer);
extern uint8_t SD_read_multiple_block(uint32_t sector, uint32_t n, uint8_t buffer[][SD_SECTOR_SIZE]);
extern uint8_t SD_write_multiple_block(uint32_t sector, uint32_t n, uint8_t buffer[][SD_SECTOR_SIZE]);

#endif