#include "includes.h"


int main(void)
{
	int i = 0;
	BYTE test[30];
	
	disable_watchdog();
	init_modes_and_clock();
	initEMIOS_0MotorAndSteer();
	init_pit();
	init_led();
	init_serial_port_0();
	init_serial_port_1();
	//init_serial_port_2();
	//init_serial_port_3();
	//init_supersonic_receive_0();
	//init_supersonic_receive_1();
	//init_supersonic_receive_2();
	//init_supersonic_receive_3();
	//init_supersonic_trigger_0();
	//init_supersonic_trigger_1();
	//init_supersonic_trigger_2();
	//init_supersonic_trigger_3();
	//init_optical_encoder();
	init_DSPI_1();
	//init_DSPI_2();
	//init_I2C();
	enable_irq();
	
	//SD_init();
	initLCD();
	LCD_DISPLAY();
	LCD_Fill(0x00);
	for (i=0; i<5; i++)
	{
		BYTE rev;
		
		ReadReg(WHO_AM_I, &rev);
		if (0xD3 == rev)
		{
			D0 = 0;
			break;
		}
		SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);
		SetMode(NORMAL);
	}
	/* Loop forever */
	for (;;)
	{
#if 1
		u8_t status;
		
		GetSatusReg(&status);
		if (status & 80)
		{
			AngRateRaw_t rev;
			GetAngRateRaw(&rev);
			//serial_port_1_TX_array((BYTE *)(&rev), sizeof(AngRateRaw_t));
			//delay_ms(300);
			LCD_PrintoutInt(0, 0, (rev.x));
			LCD_PrintoutInt(0, 2, (rev.y));
			LCD_PrintoutInt(0, 4, (rev.z));

			if (1 || g_remote_control_flags.send_gyro_data)
			{
				send_remote_frame(WIFI_CMD_GET_GYRO_DATA, (BYTE *)&rev, sizeof(rev));
			}

			//serial_port_0_TX_array((BYTE *)&rev, sizeof(rev));
			
		}
		//serial_port_0_TX(TestWhoAmI());
#endif

#if 0
		D2 = ~D2;
		for (i = 0; i < 50; i++)
		{
			serial_port_0_TX(0x00);
		}
		serial_port_0_TX_array(test, sizeof(test));
#endif
		
		delay_ms(100);
	}
}


