#include "includes.h"


int main(void)
{
	int i = 0;
	
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
#endif
	
	/* 设置初始速度 */
	set_speed_target(15);
	
	/* Loop forever */
	for (;;)
	{
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
	}
}