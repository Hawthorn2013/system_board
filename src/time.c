#include "includes.h"


void init_I2C(void)
{
	I2C.IBFD.R = 0x1f;	//设置时钟
	
	I2C.IBAD.R = 0xa0;	//从机地址
	
	I2C.IBCR.B.MDIS = 0;	//使能I2C
	
	//这两句话不能加，否则没时钟。
	//I2C.IBCR.B.MS = 1;
	//I2C.IBCR.B.TX = 1;
	
	//I2C.IBCR.B.IBIE = 1;	//使能中断
	
	//I2C.IBIC.B.BIIE = 1;
	
	//I2C.IBCR.B.NOACK = 1;
	//I2C.IBSR.B.IBAL = 1;
	//I2C.IBCR.B.RSTA = 1;
	
	
	SIU.PCR[10].R = 0x0b10;	//SDA PA10
	SIU.PCR[11].R = 0x0b10;	//SCL PA11
}


void I2C_send_byte(BYTE data)
{
	I2C.IBCR.B.TX = 1;
	I2C.IBDR.R = data;
	while(!I2C.IBSR.B.TCF) {}
}


BYTE I2C_read_byte(void)
{
	BYTE data;
	
	I2C.IBCR.B.TX = 0;
	data = I2C.IBDR.R;
	
	return data;
}


BYTE read_time_module(BYTE address)
{
	BYTE data;
	
	I2C_send_byte(0xa2);
	I2C_send_byte(address);
	I2C_send_byte(0xa3);
	data = I2C_read_byte();
	
	return data;
}


void write_time_module(BYTE address, BYTE data)
{
	I2C_send_byte(0xa2);
	I2C_send_byte(address);
	I2C_send_byte(data);
}


void I2C_master_send_to_slave(BYTE address, BYTE cmd, BYTE data)
{
	while (I2C.IBSR.B.IBB) {}
	D0 = 0;
	I2C.IBCR.R = 0x30;	//主机模式
	//serial_port_0_TX((test = I2C.IBCR.R));
	I2C.IBDR.R = address;
	while (I2C.IBSR.B.IBB) {}
	D1 = 0;
	
	I2C.IBSR.B.IBIF = 1;
	I2C.IBDR.R = cmd;
	while (!I2C.IBSR.B.IBIF) {}
	//serial_port_0_TX((test = I2C.IBCR.R));
	D2 = 0;
	
	I2C.IBSR.B.IBIF = 1;
	I2C.IBDR.R = data;
	while (!I2C.IBSR.B.IBIF) {}
	//serial_port_0_TX((test = I2C.IBCR.R));
	D3 = 0;
	//I2C.IBSR.B.IBIF = 1;
	//I2C.IBCR.R = 0x00;	//从机模式
	
	I2C.IBCR.B.MS = 0;
}


void I2C_read_byte_from_time_module(BYTE address, BYTE *pData)
{
	//START
	while (I2C.IBSR.B.IBB) {}
	I2C.IBCR.R = 0x38;
	
	//写芯片地址 /写
	I2C.IBDR.R = 0xa2;
	while (I2C.IBSR.B.IBB) {}
	
	//写寄存器地址
	I2C.IBSR.B.IBIF = 1;
	I2C.IBDR.R = address;
	while (!I2C.IBSR.B.IBIF) {}
	
	//重启动
	I2C.IBCR.B.MS = 0;
	while (I2C.IBSR.B.IBB) {}
	I2C.IBCR.R = 0x38;
	
	//写芯片地址 /读
	I2C.IBDR.R = 0xa3;
	while (I2C.IBSR.B.IBB) {}
	
	//读取数据
	//I2C.IBCR.B.NOACK = 1;
	I2C.IBCR.B.TX = 0;
	I2C.IBSR.B.IBIF = 1;
	while (I2C.IBSR.B.TCF) {}
	while (!I2C.IBSR.B.IBIF) {}
	*pData = I2C.IBDR.R;
	
	//STOP
	I2C.IBCR.B.MS = 0;
}


int I2C_read_multiple_byte_from_time_module(BYTE address, BYTE *pData, BYTE cnt)
{
	//START
	while (I2C.IBSR.B.IBB) {}
	I2C.IBCR.R = 0x38;
	
	//写芯片地址 /写
	I2C.IBDR.R = 0xa2;
	while (I2C.IBSR.B.IBB) {}
	if (I2C.IBSR.B.RXAK)
	{
		I2C.IBCR.B.MS = 0;
		return 1;
	}
	
	//写寄存器地址
	I2C.IBSR.B.IBIF = 1;
	I2C.IBDR.R = address;
	while (!I2C.IBSR.B.IBIF) {}
#if 0
	if ((int)I2C.IBSR.B.RXAK)
	{
		I2C.IBCR.B.MS = 0;
		return 2;
	}
#endif
	
	//重启动
	I2C.IBCR.B.MS = 0;
	while (I2C.IBSR.B.IBB) {}
	I2C.IBCR.R = 0x38;
	
	//写芯片地址 /读
	I2C.IBDR.R = 0xa3;
	while (I2C.IBSR.B.IBB) {}
#if 0
	if (I2C.IBSR.B.RXAK)
	{
		I2C.IBCR.B.MS = 0;
		return 3;
	}
#endif
	
	//读取数据
	I2C.IBCR.B.NOACK = 1;
	I2C.IBCR.B.TX = 0;
	I2C.IBSR.B.IBIF = 1;
	while (I2C.IBSR.B.TCF) {}
	while (!I2C.IBSR.B.IBIF) {}
	*pData = I2C.IBDR.R;
	
	//STOP
	I2C.IBCR.B.MS = 0;
}


int I2C_write_byte_to_time_module(BYTE address, BYTE data)
{
	//START
	while (I2C.IBSR.B.IBB) {}
	I2C.IBCR.R = 0x30;	//主机模式
	
	I2C.IBDR.R = 0xa2;
	while (!I2C.IBSR.B.IBIF) {}
	while (I2C.IBSR.B.RXAK) {}
	
	I2C.IBSR.B.IBIF = 1;
	I2C.IBDR.R = address;
	while (!I2C.IBSR.B.IBIF) {}
	while (I2C.IBSR.B.RXAK) {}
	
	I2C.IBSR.B.IBIF = 1;
	I2C.IBDR.R = data;
	while (!I2C.IBSR.B.IBIF) {}
	while (I2C.IBSR.B.RXAK) {}
	
	I2C.IBCR.B.MS = 0;
	
	return 0;
}


int I2C_read_byte_from_time_module_2(BYTE address, BYTE data[])
{
	//START
	while (I2C.IBSR.B.IBB) {}
	I2C.IBCR.B.TX = 1;
	I2C.IBCR.B.MS = 1;
	
	//写芯片地址 /写
	I2C.IBDR.R = 0xa2;
	while (!I2C.IBSR.B.IBIF) {}
	while (I2C.IBSR.B.RXAK) {}
	
	//写寄存器地址
	I2C.IBSR.B.IBIF = 1;
	I2C.IBDR.R = address;
	while (!I2C.IBSR.B.IBIF) {}
	while (I2C.IBSR.B.RXAK) {}
	
	//重启动
	I2C.IBCR.B.MS = 1;
	//I2C.IBCR.B.MS = 0;
	//while (I2C.IBSR.B.IBB) {}
	//I2C.IBCR.R = 0x38;
	
	//写芯片地址 /读
	I2C.IBDR.R = 0xa3;
	while (!I2C.IBSR.B.IBIF) {}
	while (I2C.IBSR.B.RXAK) {}
	
	//读取数据
	I2C.IBSR.B.IBIF = 1;
	I2C.IBCR.B.TX = 0;
	//I2C.IBCR.B.NOACK = 1;
	//while (I2C.IBSR.B.IBB) {}
	//data[0] = I2C.IBDR.R;
	I2C.IBDR.R = 0xff;
	while (!I2C.IBSR.B.IBIF) {}
	//while (!I2C.IBSR.B.TCF) {}
	while (I2C.IBSR.B.RXAK) {}
	
	I2C.IBCR.B.MS = 0;
	data[1] = I2C.IBDR.R;

	
#if 0
	I2C.IBSR.B.IBIF = 1;
	I2C.IBCR.B.TX = 0;
	I2C.IBCR.B.NOACK = 1;
	data[2] = I2C.IBDR.R;
	while (!I2C.IBSR.B.IBIF) {}
	data[3] = I2C.IBDR.R;
	
	//STOP
	I2C.IBCR.B.MS = 0;
#endif

	return 0;
}