#include "includes.h"


uint8_t sd_buffer[SD_BUFFER_SECTOR_MAX][SD_SECTOR_SIZE];

//***********����****************
void init_DSPI_2(void) {
	DSPI_2.MCR.R = 0x80010001;     /* Configure DSPI_0 as master */
	DSPI_2.CTAR[0].R = 0x780A7727; /* Configure CTAR0  */	//���ڷ���16bits
	DSPI_2.CTAR[1].R = 0x3E0A7729;	//���ڷ���8bits ��������Ϊ1����λΪ1������������Ϊ����31.35kbit/s
	DSPI_2.MCR.B.HALT = 0x0;	     /* Exit HALT mode: go from STOPPED to RUNNING state*/
	SIU.PCR[45].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 SOUT output */	//C13
	SIU.PCR[44].R = 0x0103;        /* MPC56xxB: Config pad as DSPI_0 SIN input */	//C12
	SIU.PCR[46].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 SCK output */	//C14
	SIU.PCR[47].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 PCS0 output */	//C15
	DSPI_2.RSER.B.TCFRE = 0;	//�رմ�������ж�
	INTC_InstallINTCInterruptHandler(INTC_DSPI_2_TC, 117, 3);
}


//*************�ӻ�********************
void init_DSPI_1(void) {
	DSPI_1.MCR.R = 0x00010001;     /* Configure DSPI_1 as slave */
	DSPI_1.CTAR[0].R = 0x380A7727; /* Configure CTAR0  */	//���ڽ���8bits��0x780A7727���ڽ���16bits
	DSPI_1.MCR.B.HALT = 0x0;	     /* Exit HALT mode: go from STOPPED to RUNNING state*/
	SIU.PCR[68].R = 0x0903;        /* MPC56xxB: Config pad as DSPI_1 SCK input */	//E4
	SIU.PSMI[7].R = 1;             /* MPC56xxB: Select PCR 68 for DSPI_1 SCK input */
	SIU.PCR[36].R = 0x0103;        /* MPC56xxB: Config pad as DSPI_1 SIN input */	//C4
	SIU.PSMI[8].R = 0;             /* MPC56xxB: Select PCR 8 for DSPI_1 SIN input */
	SIU.PCR[37].R = 0x0604;        /* MPC56xxB: Config pad as DSPI_1 SOUT output*/	//C5
	SIU.PCR[69].R = 0x0903;        /* MPC56xxB: Config pad as DSPI_1 PCS0/SS input */	//E5
	SIU.PSMI[9].R = 2;             /* MPC56xxB: Selec PCR 15 for DSPI_1 SS input */
	DSPI_1.RSER.B.TCFRE = 0;
	INTC_InstallINTCInterruptHandler(INTC_DSPI_1_TC, 97, 3);
}

void ReadDataDSPI_1(void) {
	while (DSPI_1.SR.B.RFDF != 1){}  /* Wait for Receive FIFO Drain Flag = 1 */
	//RecDataSlave = DSPI_1.POPR.R;    /* Read data received by slave SPI */
	DSPI_1.SR.R = 0x80020000;        /* Clear TCF, RDRF flags by writing 1 to them */
}

void ReadDataDSPI_0(void) {
	while (DSPI_0.SR.B.RFDF != 1){}  /* Wait for Receive FIFO Drain Flag = 1 */
	//RecDataMaster = DSPI_0.POPR.R;   /* Read data received by master SPI */
	DSPI_0.SR.R = 0x90020000;        /* Clear TCF, RDRF, EOQ flags by writing 1 */
}


void INTC_DSPI_2_TC(void)
{
	uint16_t tmp_rev;
	
	DSPI_2.SR.R = 0x90020000;        /* Clear TCF, RDRF, EOQ flags by writing 1 */
	tmp_rev = (uint16_t)DSPI_2.POPR.B.RXDATA;
}


void INTC_DSPI_1_TC(void)
{
	uint16_t tmp_rev;
	
	tmp_rev = (uint16_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.R = 0x80020000;        /* Clear TCF, RDRF flags by writing 1 to them */
}


void DSPI_2_TX(uint16_t cmd, uint16_t data)
{
	uint32_t tmp_tx = 0x00000000;
	
	tmp_tx |= (uint32_t)cmd<<16;
	tmp_tx |= (uint32_t)data;
	DSPI_2.PUSHR.R = tmp_tx;
	//while(!DSPI_2.SR.B.TCF){}	//���ܼӣ�������δ֪���⣬pit�жϽ���ȥ
	//DSPI_2.SR.B.TCF = 1;
}


void DSPI_1_TX(uint16_t data)
{
	DSPI_1.PUSHR.B.TXDATA = data;
}


uint8_t DSPI_read_write_byte(uint8_t byte_write)
{
	uint32_t tmp_tx = 0x00000000;
	uint16_t tmp_rx;
	
	tmp_tx |= 0x98010000;
	tmp_tx |= (uint32_t)byte_write;
	DSPI_2.PUSHR.R = tmp_tx;
	while(!DSPI_2.SR.B.TCF){}
	tmp_rx = (uint16_t)DSPI_2.POPR.B.RXDATA;
	DSPI_2.SR.B.TCF = 1;
	
	return (uint8_t)tmp_rx;
}


//----------------by-JJ----------------//
//----------------�˺����뷢��0xff��ͬ��Ƭѡ����----------------//
void DSPI_send_8_clocks(void)
{
	uint32_t tmp_tx = 0x980000FF;
	uint16_t tmp_rx;
	
	DSPI_2.PUSHR.R = tmp_tx;
	while(!DSPI_2.SR.B.TCF){}
	tmp_rx = (uint16_t)DSPI_2.POPR.B.RXDATA;
	DSPI_2.SR.B.TCF = 1;
}


uint8_t SD_send_cmd(uint8_t cmd, uint32_t var)
{
	uint8_t rev, retry;
	
	DSPI_send_8_clocks();
	DSPI_read_write_byte(cmd | 0x40);	//�ֱ�д������;��1��2λ=01
	DSPI_read_write_byte((uint8_t)(var>>24));	//���ֽڵ�ַд�뵽cmd�ֽ�����
	DSPI_read_write_byte((uint8_t)(var>>16));
	DSPI_read_write_byte((uint8_t)(var>>8));
	DSPI_read_write_byte((uint8_t)(var));
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


uint8_t SD_reset(void)
{
	uint8_t i;
	uint8_t retry;
	uint8_t rev = 0x00;
	
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
uint8_t SD_read_block(uint32_t sector, uint8_t *buffer)	//sector=address,buffer=���ݻ�����
{
	uint8_t rev;          
	uint16_t i;
	
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
uint8_t SD_read_multiple_block(uint32_t sector, uint32_t n, uint8_t buffer[][SD_SECTOR_SIZE])
{
	uint8_t rev;          
	uint16_t i;
	uint32_t j;
	
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
uint8_t SD_write_block(uint32_t sector, uint8_t *buffer)	//sector=address,buffer=���ݻ�����
{
	uint8_t rev;
	uint16_t i;
	
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
uint8_t SD_write_multiple_block(uint32_t sector, uint32_t n, uint8_t buffer[][SD_SECTOR_SIZE])
{
	uint8_t rev;
	uint16_t i;
	uint32_t j;
	
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


void SD_SPI_to_4M(void)
{
	DSPI_2.CTAR[1].R = 0x3E0A7721;	//���ڷ���8bits ��������Ϊ1����λΪ1������������Ϊ4M
}


//----------------������----------------//
void SD_SPI_to_20M(void)
{
	DSPI_2.CTAR[1].R = 0x3E087720;	//���ڷ���8bits ��������Ϊ1����λΪ1������������Ϊ20M
}


void SD_SPI_to_10M(void)
{
	DSPI_2.CTAR[1].R = 0x3E087721;	//���ڷ���8bits ��������Ϊ1����λΪ1������������Ϊ20M
}

/*************************************************************/
/*                        ��ջ�����                         */
/*************************************************************/
void clear_sd_buffer(uint8_t buffer[][SD_SECTOR_SIZE])
{
	int i, j;
	
	for (i=0; i<SD_BUFFER_SECTOR_MAX; i++)
	{
		for (j=0; j<SD_SECTOR_SIZE; j++)
		{
			sd_buffer[i][j] = 0;
		}
	}
} 