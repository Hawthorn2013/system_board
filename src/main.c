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
	/* 漂移 */
	//delay_ms(1000);
	//drift_right1();
	delay_ms(1000);
	drift_right1();
	delay_ms(5000);
	drift_right5();
	delay_ms(5000);
	drift_right6();
	delay_ms(5000);
#endif	
	
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
	/* 读取舵机参数 */
	f_mount(&fatfs, path, 1);
	read_steer_helm_data_from_TF();
	set_steer_helm(data_steer_helm.center);
	
	send_RFID_cmd(rfid_cmd_energetic_mode_enable);	/* 开启RFID读卡器主动模式 */
#endif

	set_speed_target(20);
	
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
		
		delay_ms(100);
	}
}


