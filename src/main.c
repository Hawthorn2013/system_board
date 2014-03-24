#include "includes.h"


int main(void)
{
	int m;
	
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
	
	while(SD_reset()){}
	SD_SPI_to_4M();
	
	//clear_sd_buffer(sd_buffer);//Çå¿Õ½ÓÊÕ»º³åÆ÷
	for (m=0; m<512; m++)
	{
		sd_buffer[m] = 'A';
	}
	if(!SD_write_block(512, sd_buffer))
	{
		D0 = 0;
	}
	else
	{
		D0 = 1;
	}
	
	if(!SD_read_block(512, sd_buffer))
	{
		D1 = 0;
	}
	
	for (m=0; m<512; m++)
	{
		if (sd_buffer[m] == 'A')
		{
			D2 = 0;
		}
		else
		{
			D2 = 1;
		}
	}
	
	/* Loop forever */
	for (;;)
	{
		/*
		if (f_pit)
		{
			uint8_t tmp_rev;
			
			f_pit = 0;
			D3 = ~D3;\
			//DSPI_2_TX(0x8801, 0x5678);\
		}
		*/
		
		
	}
}



