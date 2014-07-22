#include "includes.h"


int main(void)
{
	int i = 0;
	
	/* TF�� */
	FATFS fatfs;
	TCHAR *path = "0:";
	
	disable_watchdog();
	init_modes_and_clock();
	initEMIOS_0MotorAndSteer();
	init_pit();
	init_led();
	init_serial_port_0();
	init_serial_port_1();
	init_serial_port_2();
	init_ADC();
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
	
	/* �رյ��ѭ�� */
	g_f_enable_mag_steer_control = 0;
	
	enable_irq();
	
	SD_init();
	initLCD();
	LCD_DISPLAY();
	LCD_Fill(0x00);	
	
#if 0	
	/* ��ʼ�������� */
	for (i=0; i<5; i++)
	{
		BYTE rev = 0x00;
		
		ReadReg(WHO_AM_I, &rev);
		if (I_AM_L3G4200D == rev)
		{
			
		}
		SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);
		SetMode(NORMAL);
	}
#endif
	
#if 1
	/* ����TF���ļ�ϵͳ */
	f_mount(&fatfs, path, 1);
	
	/* ��ȡ������� */
	read_steer_helm_data_from_TF();
	set_steer_helm(data_steer_helm.center);
	
	/* ��ȡ�豸�� */
	read_device_no_from_TF();
	
	/* ����RFID����������ģʽ */
	send_RFID_cmd(rfid_cmd_energetic_mode_enable);
	delay_ms(100);
	send_RFID_cmd(rfid_cmd_energetic_mode_enable_new);
	
#endif

	
	/* ���ó�ʼ�ٶ� */
	set_speed_target(0);
	
	/* Loop forever */
	for (;;)
	{
#if 0
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

#if 1
		/* ִ��Զ������ */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
#endif

#if 0
		/* ���������� */
		if (RFID_site_data.is_new_site)
		{
			RFID_site_data.is_new_site = 0;
			
			punctured_ballon(RFID_site_data.site);
		}
#endif

#if 1
		if (1 == RFID_site_data.is_new_site)
		{
			RFID_site_data.is_new_site = 0;
			
			if (0xDAA23548 == RFID_site_data.site)
			{
				set_speed_target(260);
				delay_ms(1500);
				set_speed_target(0);
				g_f_enable_mag_steer_control = 1;
				delay_ms(500);
				set_speed_target(20);
			}
		}
#endif
	}
}