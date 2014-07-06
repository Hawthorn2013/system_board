#define __SDCARD_C_
#include "includes.h"


static void SD_SPI_to_4M(void);
static void DSPI_send_8_clocks(void);
static BYTE DSPI_read_write_byte(BYTE byte_write);
static BYTE SD_reset(void);
static BYTE SD_send_cmd(BYTE cmd, DWORD var);


void init_DSPI_1(void)
{
	DSPI_1.MCR.R = 0x803f0001;     /* Configure DSPI_0 as master */
	DSPI_1.CTAR[0].R = 0x3E0A7729;	//TF�� ���ڷ���8bits ��������Ϊ1����λΪ1������������Ϊ����31.35kbit/s
	DSPI_1.CTAR[1].R = 0x38087726;  //OLED SPI ����Ϊ0����λΪ0��baud rate=625Kbit/s
	DSPI_1.CTAR[2].R = 0x3E0A7729;  //L3G4200D SPI ����Ϊ1����λΪ1��baud rate=31.35kbit/s
	DSPI_1.MCR.B.HALT = 0x0;	     /* Exit HALT mode: go from STOPPED to RUNNING state*/
	SIU.PCR[34].R = 0x0604;	//PC2 SCK_1
	//SIU.PSMI[7].R = 0;	//SCK_1 PCR[34]
	SIU.PCR[35].R = 0x0503;	//PC3 CS0_1
	//SIU.PSMI[9].R = 0;	//CS0_1 PCR[35]
	SIU.PCR[36].R = 0x0104;	//PC4 SIN_1
	//SIU.PSMI[8].R = 0;	//SIN_1 PCR[36]
	SIU.PCR[62].R = 0x0604;	//PD14 CS1_1
	SIU.PCR[63].R = 0x0604;	//PD15 CS2_1
	SIU.PCR[67].R = 0x0A04;	//PE3 SOUT_1
	SIU.PCR[74].R = 0x0A04;	//PE10 CS3_1
	SIU.PCR[75].R = 0x0A04;	//PE11 CS4_1
	DSPI_1.RSER.B.TCFRE = 0;	//�رմ�������ж�
}


static void SD_SPI_to_4M(void)
{
	DSPI_1.CTAR[1].R = 0x3E0A7721;	//���ڷ���8bits ��������Ϊ1����λΪ1������������Ϊ4M
}


static BYTE DSPI_read_write_byte(BYTE byte_write)
{
	DWORD tmp_tx = 0x00000000;
	WORD tmp_rx;
	
	tmp_tx |= 0x98010000;
	tmp_tx |= (DWORD)byte_write;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (WORD)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return (BYTE)tmp_rx;
}


//----------------by-JJ----------------//
//----------------�˺����뷢��0xff��ͬ��Ƭѡ����----------------//
static void DSPI_send_8_clocks(void)
{
	DWORD tmp_tx = 0x980000FF;
	WORD tmp_rx;
	
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (WORD)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
}


/*-----------------------------------------------------------------------*/
/* ��ʼ��TF��                                                            */
/* ��������0                                                             */
/*-----------------------------------------------------------------------*/
int SD_init(void)
{
	int i = 0;
	
	for (i=0; i<20; i++)
	{
		if (!SD_reset())
		{
			SD_SPI_to_4M();
			return 0;
		}
	}
	
	return 1;
}


static BYTE SD_send_cmd(BYTE cmd, DWORD var)
{
	BYTE rev, retry;
	
	DSPI_send_8_clocks();
	DSPI_read_write_byte(cmd | 0x40);	//�ֱ�д������;��1��2λ=01
	DSPI_read_write_byte((BYTE)(var>>24));	//���ֽڵ�ַд�뵽cmd�ֽ�����
	DSPI_read_write_byte((BYTE)(var>>16));
	DSPI_read_write_byte((BYTE)(var>>8));
	DSPI_read_write_byte((BYTE)(var));
	DSPI_read_write_byte(0x95);
	retry = 0;
	while((rev = DSPI_read_write_byte(0xFF)) == 0xFF)	//�ȴ���Ӧ
	{
		if(retry++>100)
		{
			break;	//��ʱ�˳�
		}
	}
	
	return rev;
}


static BYTE SD_reset(void)
{
	BYTE i;
	BYTE retry;
	BYTE rev = 0x00;
	
	for(i=0; i<10; i++)
	{
		DSPI_send_8_clocks();	//����74��ʱ�ӣ������!!!
	}
	//SD����λ
	//����CMD0������������ʾ�ɹ�����idle״̬
	for(retry=0; rev!=0x01; retry++)
	{
		rev = SD_send_cmd(0, 0);	//��idle����
		if(retry>100)
		{
			return 1;
		}
	}
	//SD����ʼ��(CMD1/CMD55+ACMD41���ַ���)
	for(retry=0; rev!=0x00; retry++)
	{
		rev = SD_send_cmd(1, 0);	//��idle����
		if(retry>100)
		{
			return 2;
		}
	}
	rev = SD_send_cmd(59, 0);	//��crc
	rev = SD_send_cmd(16, 512);	//��������С512
	
	return 0;	//��������
}


//----------------by-�첩----------------//
BYTE SD_read_block(DWORD sector, BYTE *buffer)	//sector=address,buffer=���ݻ�����
{
	BYTE rev;          
	WORD i;
	
	rev = SD_send_cmd(17, sector<<9);  //������,����CMD17,�յ�0x00��ʾ�ɹ� 	
	if(rev != 0x00)
	{
		return rev;
	}
	
	//�����ݵĿ�ʼ
	while(DSPI_read_write_byte(0xFF) != 0xFE){}//������ֱ��������ʼ�ֽ�0xfe
	for(i=0; i<SD_SECTOR_SIZE; i++)	//��512�����ݣ�д�����ݻ�����
	{
		*buffer++ = DSPI_read_write_byte(0xFF);
	}
	DSPI_read_write_byte(0xFF);	//������CRC�ֽ�
	DSPI_read_write_byte(0xFF);  
	DSPI_send_8_clocks();	//��ʱ��8��ʱ��
	
	return 0;	//��ȡ�����ɹ�
}


//----------------by-JJ----------------//
BYTE SD_read_multiple_block(DWORD sector, DWORD n, BYTE buffer[][SD_SECTOR_SIZE])
{
	BYTE rev;          
	WORD i;
	DWORD j;
	
	rev = SD_send_cmd(18, sector<<9);
	if(rev != 0x00)
	{
		return rev;
	}
	for (j=0; j<n; j++)
	{
		while(DSPI_read_write_byte(0xFF) != 0xFE){}
		for(i=0; i<SD_SECTOR_SIZE; i++)
		{
			buffer[j][i] = DSPI_read_write_byte(0xFF);
		}
		DSPI_read_write_byte(0xFF);
		DSPI_read_write_byte(0xFF);  
	}
	SD_send_cmd(12, 0);	//����STOP_TRANSMISSION
	DSPI_send_8_clocks();
	
	return 0;
}


//----------------by-�첩----------------//
BYTE SD_write_block(DWORD sector, const BYTE *buffer)	//sector=address,buffer=���ݻ�����
{
	BYTE rev;
	WORD i;
	
	rev = SD_send_cmd(24, sector<<9);	//д����,sector<<9:����ַ����9λ,address=address*512,�����ַ��������ַ��תΪ�ֽڵ�ַ
	if(rev != 0x00)
	{
		return rev;	//�յ�0x00��ʾ�ɹ�
	}
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xfe);	//����ʼ��

	for(i=0; i<SD_SECTOR_SIZE; i++)	//��512�ֽ�����
	{
		DSPI_read_write_byte(*buffer++);
	}
	
	DSPI_read_write_byte(0xFF);	//д��2���ֽڵ�CRCУ���룬Don't care
	DSPI_read_write_byte(0xFF);

	rev = DSPI_read_write_byte(0xFF);	//��ȡ����ֵ
	if((rev&0x1f) != 0x05)	//������ֵΪXXX00101,˵�������Ѿ���SD������
	{
		return rev;	//д��ʧ��
	}
	//�ȴ�������
	while(!DSPI_read_write_byte(0xFF)){}	//�ȴ�SD����æ
	DSPI_send_8_clocks();	//��SD������ʱ��8��ʱ��
	
	return 0;	//˵��д���������ɹ�
}


//----------------by-JJ----------------//
BYTE SD_write_multiple_block(DWORD sector, DWORD n, const BYTE buffer[][SD_SECTOR_SIZE])
{
	BYTE rev;
	WORD i;
	DWORD j;
	
	rev = SD_send_cmd(25, sector<<9);
	if(rev != 0x00)
	{
		return rev;
	}
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xff);
	for(i=0; i<n; i++)
	{
		DSPI_read_write_byte(0xfc);
		for(j=0; j<SD_SECTOR_SIZE; j++)
		{
			DSPI_read_write_byte(buffer[i][j]);
		}
		DSPI_read_write_byte(0xFF);
		DSPI_read_write_byte(0xFF);
		rev = DSPI_read_write_byte(0xFF);
		if((rev&0x1f) != 0x05)
		{
			return rev;
		}
		while(!DSPI_read_write_byte(0xFF)){}
	}
	DSPI_read_write_byte(0xfb);
	while(!DSPI_read_write_byte(0xFF)){}
	//SD_send_cmd(12, 0);	//��仰���º����Ķ�����������
	DSPI_send_8_clocks();
	
	return 0;
}

/*************************************************************/
/*                        ��ջ�����                         */
/*************************************************************/
void clear_sd_buffer(BYTE buffer[][SD_SECTOR_SIZE])
{
	int i, j;
	
	for (i=0; i<SD_BUFFER_SECTOR_MAX; i++)
	{
		for (j=0; j<SD_SECTOR_SIZE; j++)
		{
			buffer[i][j] = 0;
		}
	}
}


/*-----------------------------------------------------------------------*/
/* ��ȡ���������ݴ�TF��                                                */
/*-----------------------------------------------------------------------*/
int read_steer_helm_data_from_TF()
{
	FIL fil;
	TCHAR *tchar = "SteHel";
	UINT br;
	
	f_open(&fil, tchar, FA_READ);
	f_read(&fil, (void *)&data_steer_helm, sizeof(data_steer_helm), &br);
	f_close(&fil);
	
	return 0;
}


/*-----------------------------------------------------------------------*/
/* д�뷽�������ݵ�TF��                                                */
/*-----------------------------------------------------------------------*/
int write_steer_helm_data_to_TF()
{
	FIL fil;
	TCHAR *tchar = "SteHel";
	UINT wr;
	
	f_open(&fil, tchar, FA_CREATE_ALWAYS);
	f_close(&fil);
	f_open(&fil, tchar, FA_WRITE);
	f_write(&fil, (void *)&data_steer_helm, sizeof(data_steer_helm), &wr);
	f_close(&fil);
	
	return 0;
}