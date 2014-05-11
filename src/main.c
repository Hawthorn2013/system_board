#include "includes.h"


int main(void)
{
	FATFS fatfs;
	FIL fil;
	TCHAR *path = "0:";
	TCHAR *tchar = "test.txt";
	BYTE input[] = "I am Jian Jiao.";
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
	init_DSPI_1();
	//init_I2C();
	enable_irq();

	SD_init();
	initLCD();
	
	/* Loop forever */
	for (;;)
	{
		if (f_mount(&fatfs, path, 1) == FR_OK)
		{
			D0 = ~D0;
		}
		f_open(&fil, tchar, FA_CREATE_ALWAYS);
		f_close(&fil);
		f_open(&fil, tchar, FA_WRITE);
		f_write(&fil, (const void *)input, 16, &bw);
		f_sync(&fil);
		f_mount((void *)0, path, 1);
		LCD_DISPLAY();
	}
}



