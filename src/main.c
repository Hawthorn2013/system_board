#include "includes.h"


int main(void)
{
	FATFS fatfs;
	FIL fil;
	TCHAR *path = "0:";
	TCHAR *tchar = "test_1__.txt";
	FRESULT res;
	
	disable_watchdog();
	init_modes_and_clock();
	//initEMIOS_0MotorAndSteer();
	//init_pit();
	init_led();
	init_serial_port_0();
	//init_serial_port_1();
	//init_serial_port_2();
	//init_supersonic_receive_0();
	//init_supersonic_trigger_0();
	//init_optical_encoder();
	init_DSPI_2();
	//init_DSPI_1();
	enable_irq();
	//
	SD_init();
	
	if ((res=f_mount(&fatfs, path, 1)) == FR_OK)
	{
		D0 = 0;
	}
	
	if ((res=f_open(&fil, tchar, FA_CREATE_ALWAYS)) == FR_OK)
	{
		D1 = 0;
	}
	serial_port_0_TX((BYTE)res);
	
	if (f_sync(&fil) == FR_OK)
	{
		D2 = 0;
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



