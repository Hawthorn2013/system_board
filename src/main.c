#include "includes.h"


int main(void)
{
	int i = 0;
	
	/* TF�� */
	FATFS fatfs;
	//FIL fil;
	TCHAR *path = "0:";
	//TCHAR *tchar = "SteHel";
	//BYTE input[] = "I am Jian Jiao.";
	//UINT bw;
	//UINT br;
	
	disable_watchdog();
	init_modes_and_clock();
	initEMIOS_0MotorAndSteer();
	init_pit();
	init_led();
	init_serial_port_0();
	init_serial_port_1();
	init_serial_port_2();
	//init_serial_port_3();
	//init_supersonic_receive_0();
	//init_supersonic_receive_1();
	//init_supersonic_receive_2();
	//init_supersonic_receive_3();
	//init_supersonic_trigger_0();
	//init_supersonic_trigger_1();
	//init_supersonic_trigger_2();
	//init_supersonic_trigger_3();
	init_optical_encoder();
	init_DSPI_1();
	//init_DSPI_2();
	//init_I2C();
	enable_irq();
	
	SD_init();
	initLCD();
	LCD_DISPLAY();
	LCD_Fill(0x00);
	
	/* ��ʼ�������� */
	for (i=0; i<5; i++)
	{
		BYTE rev = 0x00;
		
		//ReadReg(WHO_AM_I, &rev);
		ReadReg(WHO_AM_I, &rev);
		if (I_AM_L3G4200D == rev)
		{
			
		}
		SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);
		SetMode(NORMAL);
	}
	
	set_speed_target(20);
	

	/* ��ȡ������� */
	f_mount(&fatfs, path, 1);
	read_steer_helm_data_from_TF();
	set_steer_helm(data_steer_helm.center);
	
	send_RFID_cmd(rfid_cmd_energetic_mode_enable);
	
	/* Loop forever */
	for (;;)
	{
#if 1
		u8_t status;
		
		/* ���������� */
		if (MEMS_SUCCESS == GetSatusReg(&status))
		{
			if (status & 80)
			{
				AngRateRaw_t rev;
				GetAngRateRaw(&rev);
				LCD_PrintoutInt(0, 0, (rev.x));
				LCD_PrintoutInt(0, 2, (rev.y));
				LCD_PrintoutInt(0, 4, (rev.z));
				if (g_remote_control_flags.send_gyro_data)
				{
					generate_remote_frame(WIFI_CMD_GET_GYRO_DATA, (BYTE *)&rev, sizeof(rev));
				}
			}
			//serial_port_0_TX(TestWhoAmI());
		}
#endif
		/* ִ��Զ������ */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
		
	
		delay_ms(100);
	}
}


