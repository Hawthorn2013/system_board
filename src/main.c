#include "includes.h"


int main(void)
{
	int i, j;
	
	disable_watchdog();
	init_modes_and_clock();
	//initEMIOS_0MotorAndSteer();
	//init_pit();
	init_led();
	//init_serial_port_0();
	//init_serial_port_1();
	//init_serial_port_2();
	//init_supersonic_receive_0();
	//init_supersonic_trigger_0();
	//init_optical_encoder();
	init_DSPI_2();
	//init_DSPI_1();
	enable_irq();
	
	SD_init();
	D3 = 0;

	clear_sd_buffer(sd_buffer);
	if (!SD_read_multiple_block(512, SD_BUFFER_SECTOR_MAX, sd_buffer))
	{
		D1 = 0;
	}
	D2 = 0;
	for (i=0; i<SD_BUFFER_SECTOR_MAX; i++)
	{
		for (j=0; j<SD_SECTOR_SIZE; j++)
		{
			if (sd_buffer[i][j] != (BYTE)i+'A')
			{
				D2 = 1;
			}
		}
	}
	
	for (i=0; i<SD_BUFFER_SECTOR_MAX; i++)
	{
		for (j=0; j<SD_SECTOR_SIZE; j++)
		{
			sd_buffer[i][j] = (BYTE)i+'A';
		}
	}
	if (!SD_write_multiple_block(512, SD_BUFFER_SECTOR_MAX, sd_buffer))
	{
		D0 = 0;
	}
	
	D0 = 1;
	D1 = 1;
	D2 = 1;
	
	clear_sd_buffer(sd_buffer);
	if (!SD_read_multiple_block(512, SD_BUFFER_SECTOR_MAX, sd_buffer))
	{
		D1 = 0;
	}
	D2 = 0;
	for (i=0; i<SD_BUFFER_SECTOR_MAX; i++)
	{
		//SD_read_block(512+i, sd_buffer[i]);
		for (j=0; j<SD_SECTOR_SIZE; j++)
		{
			if (sd_buffer[i][j] != (BYTE)i+'A')
			{
				D2 = 1;
			}
		}
	}
	
	for (i=0; i<SD_BUFFER_SECTOR_MAX; i++)
	{
		for (j=0; j<SD_SECTOR_SIZE; j++)
		{
			sd_buffer[i][j] = (BYTE)i+'A';
		}
	}
	if (!SD_write_multiple_block(512, SD_BUFFER_SECTOR_MAX, sd_buffer))
	{
		D0 = 0;
	}
	
	/* Loop forever */
	for (;;)
	{
		/*
		if (f_pit)
		{
			BYTE tmp_rev;
			
			f_pit = 0;
			D3 = ~D3;
			//DSPI_2_TX(0x8801, 0x5678);
		}
		*/
		
		
	}
}



