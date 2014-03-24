#include "includes.h"


uint8_t sd_buffer[SD_SECTOR_SIZE] = "";

//***********主机****************
void init_DSPI_2(void) {
	DSPI_2.MCR.R = 0x80010001;     /* Configure DSPI_0 as master */
	DSPI_2.CTAR[0].R = 0x780A7727; /* Configure CTAR0  */	//用于发送16bits
	DSPI_2.CTAR[1].R = 0x3E0A7729;	//用于发送8bits 调整极性为1，相位为1，调整波特率为低速31.35kbit/s
	DSPI_2.MCR.B.HALT = 0x0;	     /* Exit HALT mode: go from STOPPED to RUNNING state*/
	SIU.PCR[45].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 SOUT output */	//C13
	SIU.PCR[44].R = 0x0103;        /* MPC56xxB: Config pad as DSPI_0 SIN input */	//C12
	SIU.PCR[46].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 SCK output */	//C14
	SIU.PCR[47].R = 0x0A04;        /* MPC56xxB: Config pad as DSPI_0 PCS0 output */	//C15
	DSPI_2.RSER.B.TCFRE = 0;	//关闭传输完成中断
	INTC_InstallINTCInterruptHandler(INTC_DSPI_2_TC, 117, 3);
}


//*************从机********************
void init_DSPI_1(void) {
	DSPI_1.MCR.R = 0x00010001;     /* Configure DSPI_1 as slave */
	DSPI_1.CTAR[0].R = 0x380A7727; /* Configure CTAR0  */	//用于接收8bits，0x780A7727用于接收16bits
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
	//while(!DSPI_2.SR.B.TCF){}	//不能加，否则导致未知问题，pit中断进不去
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


void DSPI_send_8_clocks(void)
{
	uint32_t tmp_tx = 0x980100FF;
	uint16_t tmp_rx;
	
	DSPI_2.PUSHR.R = tmp_tx;
	while(!DSPI_2.SR.B.TCF){}
	tmp_rx = (uint16_t)DSPI_2.POPR.B.RXDATA;
	DSPI_2.SR.B.TCF = 1;
}


uint8_t SD_send_cmd(uint8_t cmd, uint32_t var)
{
	uint8_t rev, retry;
	
	//DSPI_read_write_byte(0xFF);	//产生8个时钟信号
	DSPI_send_8_clocks();
	//SD_select();
	DSPI_read_write_byte(cmd | 0x40);	//分别写入命令;第1、2位=01
	DSPI_read_write_byte((uint8_t)(var>>24));	//将字节地址写入到cmd字节序列
	DSPI_read_write_byte((uint8_t)(var>>16));
	DSPI_read_write_byte((uint8_t)(var>>8));
	DSPI_read_write_byte((uint8_t)(var));
	DSPI_read_write_byte(0x95);
	
	retry = 0;
	while((rev = DSPI_read_write_byte(0xFF)) == 0xFF)	//等待响应
	{
		if(retry++>100)
		{
			break;	//超时退出
		}
	}
	//SD_deselect();
	
	return rev;
}


uint8_t SD_reset(void)
{
	uint8_t i;
	uint8_t retry;
	uint8_t rev = 0x00;
	
	//SD_deselect();	//关闭片选
	for(i=0; i<10; i++)
	{
		//DSPI_read_write_byte(0xff);	//至少74个时钟，必须的!!!
		DSPI_send_8_clocks();
	}
	//SD卡复位
	//发送CMD0，正常跳出表示成功进入idle状态
	for(retry=0; rev!=0x01; retry++)
	{
		rev = SD_send_cmd(0, 0);	//发idle命令
		if(retry>100)
		{
			return 1;
		}
	}
	//SD卡初始化(CMD1/CMD55+ACMD41两种方法)
	for(retry=0; rev!=0x00; retry++)
	{
		rev = SD_send_cmd(1, 0);	//发idle命令
		if(retry>100)
		{
			return 2;
		}
	}
	rev = SD_send_cmd(59, 0);	//关crc
	rev = SD_send_cmd(16, 512);	//设扇区大小512
	
	return 0;	//正常返回
}


uint8_t SD_read_block(uint32_t sector, uint8_t *buffer)	//sector=address,buffer=数据缓冲区
{
	uint8_t rev;          
	uint16_t i;
	
	rev = SD_send_cmd(17, sector<<9);  //读命令,发送CMD17,收到0x00表示成功 	
	if(rev != 0x00)
	{
		return rev;
	}
	
	//等数据的开始
	while(DSPI_read_write_byte(0xFF) != 0xFE){}//连续读直到读到开始字节0xfe
	for(i=0; i<SD_SECTOR_SIZE; i++)	//读512个数据，写入数据缓冲区
	{
		*buffer++ = DSPI_read_write_byte(0xFF);
	}
	DSPI_read_write_byte(0xFF);	//读两个CRC字节
	DSPI_read_write_byte(0xFF);  
	DSPI_send_8_clocks();	//按时序补8个时钟
	
	return 0;	//读取操作成功
}


uint8_t SD_write_block(uint32_t sector, uint8_t *buffer)	//sector=address,buffer=数据缓存区
{
	uint8_t rev;
	uint16_t i;
	
	if(sector<1)
	{
		return 0xff;	//为了保护SD卡引导区，跳过该区
	}
	rev = SD_send_cmd(24, sector<<9);	//写命令,sector<<9:将地址左移9位,address=address*512,将块地址（扇区地址）转为字节地址
	if(rev != 0x00)
	{
		return rev;	//收到0x00表示成功
	}
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xfe);	//发开始符

	for(i=0; i<SD_SECTOR_SIZE; i++)	//送512字节数据
	{
		DSPI_read_write_byte(*buffer++);
	}
	
	DSPI_read_write_byte(0xFF);	//写入2个字节的CRC校验码，Don't care
	DSPI_read_write_byte(0xFF);

	rev = DSPI_read_write_byte(0xFF);	//读取返回值
	if((rev&0x1f) != 0x05)	//若返回值为XXX00101,说明数据已经被SD卡接受
	{
		return rev;	//写入失败
	}
	//等待操作完
	while(!DSPI_read_write_byte(0xFF)){}	//等待SD卡不忙
	DSPI_send_8_clocks();	//按SD卡操作时序补8个时钟
	
	return 0;	//说明写扇区操作成功
}


void SD_SPI_to_4M(void)
{
	DSPI_2.CTAR[1].R = 0x3E0A7721;	//用于发送8bits 调整极性为1，相位为1，调整波特率为4M
}


/*************************************************************/
/*                        清空缓冲区                         */
/*************************************************************/
void clear_sd_buffer(uint8_t buffer[])
{
    int i;     
    for(i=0;i<512;i++)	
		*(buffer+i)=0;
} 