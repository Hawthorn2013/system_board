#ifndef __TIME_H__
#define __TIME_H__


extern void write_time_module(BYTE address, BYTE data);
extern BYTE read_time_module(BYTE address);
extern void I2C_read_byte_from_time_module(BYTE address, BYTE *pData);
extern int I2C_read_multiple_byte_from_time_module(BYTE address, BYTE *pData, BYTE cnt);
extern int I2C_write_byte_to_time_module(BYTE address, BYTE data);
extern int I2C_read_byte_from_time_module_2(BYTE address, BYTE *data);


#endif