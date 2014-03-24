#include "includes.h"


int main(void)
{
	FATFS fatfs;
	FIL fil;
	TCHAR *path = "0:";
	TCHAR *tchar = "test.txt";
	BYTE input[] = "I am Jian Jiao.";
	FRESULT res;
	UINT bw;
	
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
	
	SD_init();
	
	if (f_mount(&fatfs, path, 1) == FR_OK)
	{
		D0 = 0;
	}
	f_open(&fil, tchar, FA_CREATE_ALWAYS);
	f_close(&fil);
	f_open(&fil, tchar, FA_WRITE);
	res = f_write(&fil, (const void *)input, 16, &bw);
	serial_port_0_TX((BYTE)res);
	f_sync(&fil);
	
	
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



