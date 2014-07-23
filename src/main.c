#include "includes.h"


int main(void)
{
	int i = 0;
	int flag=1;
	/* TF卡 */
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
	
	/* 打开电磁循迹 */
	g_f_enable_mag_steer_control = 1;
	
	enable_irq();
	
	SD_init();
	initLCD();
	LCD_DISPLAY();
	LCD_Fill(0x00);	
	
#if 0	
	/* 初始化陀螺仪 */
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
	/* 挂载TF卡文件系统 */
	f_mount(&fatfs, path, 1);
	
	/* 读取舵机参数 */
	read_steer_helm_data_from_TF();
	set_steer_helm(data_steer_helm.center);
	
	/* 读取设备号 */
	read_device_no_from_TF();
	
	/* 开启RFID读卡器主动模式 */
	send_RFID_cmd(rfid_cmd_energetic_mode_enable);
	delay_ms(100);
	send_RFID_cmd(rfid_cmd_energetic_mode_enable_new);
	
#endif

	
	/* 设置初始速度 */
	set_speed_target(0);
	
	/* Loop forever */
	for (;;)
	{
	
	if(RFID_site_data.site==RFID_CARD_ID_1&&flag==1)
	{
		g_f_enable_mag_steer_control=0;
		set_steer_helm(data_steer_helm.center);
	}
	if(RFID_site_data.site==RFID_CARD_ID_1&&flag==0)
	{
		g_f_enable_mag_steer_control=1;
		set_speed_target(15);
			
	}
	if(RFID_site_data.site==RFID_CARD_ID_2&&flag==1)
	{
	    flag=0;
	    set_speed_target(-10);
	}
	
#if 0
		u8_t status;
		
		/* 调试陀螺仪 */
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
		/* 执行远程命令 */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
#endif

#if 0
		/* 测试扎气球 */
		if (RFID_site_data.is_new_site)
		{
			RFID_site_data.is_new_site = 0;
			
			punctured_ballon(RFID_site_data.site);
		}
#endif

#if 0
		/* 测试扎气球2 */
		if (RFID_site_data.is_new_site)
		{
			RFID_site_data.is_new_site = 0;
			
			if (RFID_CARD_ID_PUNCTURED_BALLON_START == RFID_site_data.site)
			{
				set_speed_target(20);
			}
			else if (RFID_CARD_ID_PUNCTURED_BALLON_STOP  == RFID_site_data.site)
			{
				set_speed_target(0);
			}
			else if (3 == g_device_NO)
			{
				if (RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN1 == RFID_site_data.site)
				{
					g_f_enable_mag_steer_control = 0;
					set_steer_helm(data_steer_helm.right_limit);
					set_speed_target(20);
					delay_ms(500);
					set_steer_helm(data_steer_helm.left_limit);
					delay_ms(800);
					set_steer_helm(data_steer_helm.center);
					set_speed_target(10);
					g_f_enable_mag_steer_control = 1;
				}
				else if (RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN2 == RFID_site_data.site)
				{
					g_f_enable_mag_steer_control = 0;
					set_steer_helm(data_steer_helm.right_limit);
					set_speed_target(20);
					delay_ms(500);
					set_steer_helm(data_steer_helm.left_limit);
					delay_ms(800);
					set_steer_helm(data_steer_helm.center);
					set_speed_target(10);
					g_f_enable_mag_steer_control = 1;
				}
			}
			else if (1 == g_device_NO)
			{
				if (RFID_CARD_ID_PUNCTURED_BALLON_CAR4_TURN1 == RFID_site_data.site)
				{
					g_f_enable_mag_steer_control = 0;
					set_steer_helm(data_steer_helm.right_limit);
					set_speed_target(20);
					delay_ms(600);
					set_steer_helm(data_steer_helm.left_limit);
					delay_ms(950);
					set_steer_helm(data_steer_helm.center);
					set_speed_target(10);
					g_f_enable_mag_steer_control = 1;
				}
				else if (RFID_CARD_ID_PUNCTURED_BALLON_CAR4_TURN2 == RFID_site_data.site)
				{
					g_f_enable_mag_steer_control = 0;
					set_steer_helm(data_steer_helm.right_limit);
					set_speed_target(20);
					delay_ms(700);
					set_steer_helm(data_steer_helm.left_limit);
					delay_ms(900);
					set_steer_helm(data_steer_helm.center);
					set_speed_target(10);
					g_f_enable_mag_steer_control = 1;
				}
			}
			
		}
#endif
	}
}