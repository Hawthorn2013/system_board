#define __INIT_C_
#include "includes.h"


/*-----------------------------------------------------------------------*/
/* 设置单片机的模式和时钟                                                */
/*-----------------------------------------------------------------------*/
void init_modes_and_clock(void) 
{
    ME.MER.R = 0x0000001D;	/* Enable DRUN, RUN0, SAFE, RESET modes */
	/* 设置sysclk */
    //CGM.FMPLL_CR.R = 0x02400100;	/* 8 MHz xtal: Set PLL0 to 64 MHz */
    CGM.FMPLL_CR.R = 0x01280000;	/* 8 MHz xtal: Set PLL0 to 80 MHz */
    //CGM.FMPLL_CR.R = 0x013C0000;	/* 8 MHz xtal: Set PLL0 to 120 MHz */ 
    ME.RUN[0].R = 0x001F0064;	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclk选择锁相环时钟 */
    ME.RUNPC[0].R = 0x00000010;	/* Peri. Cfg. 1 settings: only run in RUN0 mode      选择RUN0模式 */
  
	/* PCTL[?] 选择需要时钟模块(默认即可，不用设置) */
	//ME.PCTL[32].R = 0x00;	/* MPC56xxB/P/S ADC 0: select ME.RUNPC[0] */
    ME.PCTL[32].B.DBG_F = 0;
	/* Mode Transition to enter RUN0 mode */
    ME.MCTL.R = 0x40005AF0;	/* Enter RUN0 Mode & Key */
    ME.MCTL.R = 0x4000A50F;	/* Enter RUN0 Mode & Inverted Key */
    
    while (ME.GS.B.S_MTRANS) {}	/* Wait for mode transition to complete 等待模式转换完成 */
    while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode 等待选择RUN0模式 */
  
	/* 开peri0、1、2 */
	/* 外设时钟总线 可用于分频 */
	CGM.SC_DC[0].R = 0x84;	/* LIN */
	CGM.SC_DC[1].R = 0x80;	/* FLEXCAN,DSPI */
    CGM.SC_DC[2].R = 0x80;	/* eMIOS,CTU,ADC */
}


/*-----------------------------------------------------------------------*/
/* 禁用看门狗                                                            */
/* 据徐博说应该首先运行                                                  */
/*-----------------------------------------------------------------------*/
void disable_watchdog(void)
{
	SWT.SR.R = 0x0000c520;	/* rite keys to clear soft lock bit */
	SWT.SR.R = 0x0000d928;
	SWT.CR.R = 0x8000010A;	/* Clear watchdog enable (WEN) */
}


/*-----------------------------------------------------------------------*/
/* 初始化板载LED                                                         */
/*-----------------------------------------------------------------------*/
void init_led(void)
{
 	SIU.PCR[12].R = 0x0203;	/* PA12 */
  	SIU.PCR[13].R = 0x0203;
 	SIU.PCR[14].R = 0x0203; 
	SIU.PCR[15].R = 0x0203;	/* PA15 */
 	
	SIU.GPDO[12].R = 1;	/* 1=熄灭 */
	SIU.GPDO[13].R = 1;
	SIU.GPDO[14].R = 1;
	SIU.GPDO[15].R = 1;
}


/*-----------------------------------------------------------------------*/
/* 初始化PIT中断                                                         */
/* 10ms                                                                  */
/*-----------------------------------------------------------------------*/
void init_pit(void)
{
	/* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
	PIT.PITMCR.R = 0x00000001;	/* Enable PIT and configure timers to stop in debug modem */
	PIT.CH[1].LDVAL.R = 800000;	/* 800000==10ms */
	PIT.CH[1].TCTRL.R = 0x00000003;	/* Enable PIT1 interrupt and make PIT active to count */
	INTC_InstallINTCInterruptHandler(PitISR,60,1);	/* PIT 1 interrupt vector with priority 1 */
}


/*-----------------------------------------------------------------------*/
/* 初始化eMIOS0                                                          */
/* 初始化电机和舵机                                                      */
/*-----------------------------------------------------------------------*/
void initEMIOS_0MotorAndSteer(void)
{
	/* eMIOS0初始化80MHz分为10MHz */
	EMIOS_0.MCR.B.GPRE= 7;	/* GPRE+1=分频系数；/* Divide 80 MHz sysclk by 7+1 = 8 for 10MHz eMIOS clk */
	EMIOS_0.MCR.B.GPREN = 1;	/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
	
    /* Modulus Up Counter 5kHZ */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[16].CCR.B.FREN = 1;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[16].CADR.R = 2000;	/* 设置周期200us 5KHZ */
	EMIOS_0.CH[16].CCR.B.MODE = 0x50;	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;	/* Use internal counter */
    /* 前进输出 OPWMB PE5 输出0-2000 */
	EMIOS_0.CH[21].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[21].CCR.B.MODE = 0x60;	/* Mode is OPWM Buffered */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[21].CADR.R = 0;	/* Leading edge when channel counter bus= */
	EMIOS_0.CH[21].CBDR.R = 0;	/* Trailing edge when channel counter bus= */
	SIU.PCR[69].R = 0x0600;	/*[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* 前进输出 OPWMB PE6 输出0-2000 */
	EMIOS_0.CH[22].CCR.B.BSL = 0x1;
	EMIOS_0.CH[22].CCR.B.MODE = 0x60;
    EMIOS_0.CH[22].CCR.B.EDPOL = 1;
	EMIOS_0.CH[22].CADR.R = 0;
	EMIOS_0.CH[22].CBDR.R = 0;
	SIU.PCR[70].R = 0x0600;
	
    /* Modulus Up Counter 50HZ */
    EMIOS_0.CH[8].CCR.B.UCPRE=3;	/* Set channel prescaler to divide by 4 */
	EMIOS_0.CH[8].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 4 */
	EMIOS_0.CH[8].CCR.B.FREN = 1;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[8].CADR.R = 50000;	/* 设置周期0.02s  50HZ */
	EMIOS_0.CH[8].CCR.B.MODE = 0x50;	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[8].CCR.B.BSL = 0x3;	/* Use internal counter */
    /* 方向舵机 PWM PA9 输出0-50000 */
	EMIOS_0.CH[9].CCR.B.BSL = 0x1;	/* Use counter bus C (default) */
	EMIOS_0.CH[9].CCR.B.MODE = 0x60;	/* Mode is OPWM Buffered */  
    EMIOS_0.CH[9].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[9].CADR.R = 1;	/* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[9].CBDR.R = STEER_HELM_CENTER;	/* Trailing edge when channel counter bus=500*/
	SIU.PCR[9].R = 0x0600;	/* [11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* 信号舵机 PWM PA12 输出0-50000 */
	EMIOS_0.CH[12].CCR.B.BSL = 0x1;
	EMIOS_0.CH[12].CCR.B.MODE = 0x60;  
    EMIOS_0.CH[12].CCR.B.EDPOL = 1;
	EMIOS_0.CH[12].CADR.R = 1;
	EMIOS_0.CH[12].CBDR.R = SINGLE_HELM_CENTER;
	SIU.PCR[44].R = 0x0600;
}


/*-----------------------------------------------------------------------*/
/* 使能外部中断                                                          */
/* 总开关                                                                */
/*-----------------------------------------------------------------------*/
void enable_irq(void)
{
  INTC.CPR.B.PRI = 0;	/* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	/* Enable external interrupts */
}


/*-----------------------------------------------------------------------*/
/* 判断大端                                                              */
/* MPC5604大端 高位在低                                                  */
/*-----------------------------------------------------------------------*/
int is_big_endian()
{
	WORD a = 0x1234;
	BYTE b = *(BYTE *)&a;	/*通过将int强制类型转换成char单字节，通过判断起始存储位置。即等于 取b等于a的低地址部分 */
    
    if(b == 0x12)
	{
    	return 1;
	}
    
	return 0;
}


/*-----------------------------------------------------------------------*/
/* 初始化光电编码器                                                      */
/*-----------------------------------------------------------------------*/
void init_optical_encoder(void)	//PD12模数计数器入口，上升沿
{
	/* 计数部分 PD12 */
	EMIOS_0.CH[24].CCR.B.MODE = 0x51;	/* Mode is MCB */
	EMIOS_0.CH[24].CCR.B.BSL = 0x3;	/* Use internal counter */
	EMIOS_0.CH[24].CCR.B.UCPRE=0;	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[24].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[24].CCR.B.FREN = 0;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[24].CCR.B.EDPOL=1;	/* Edge Select rising edge */
	EMIOS_0.CH[24].CADR.R=0xffff;
	/* (WORD)EMIOS_0.CH[24].CCNTR.R 数据寄存器 */
	SIU.PCR[60].R = 0x0100;	/* Initialize pad for eMIOS channel Initialize pad for input */
	
	/* 方向部分 PB12 */
	SIU.PCR[28].R = 0x0100;
	/* SIU.GPDI[28].B.PDI 数据寄存器 */
}


/*-----------------------------------------------------------------------*/
/* 延时 xus                                                              */
/* 依赖总线80M                                                           */
/*-----------------------------------------------------------------------*/
void delay_us(DWORD us)
{
	volatile int i, j;
	
	for (i = 0; i < us; i++)
	{
		for (j = 0; j < 9; j++) {}
	}
}


/*-----------------------------------------------------------------------*/
/* 延时 xms                                                              */
/* 依赖delay_us()                                                        */
/*-----------------------------------------------------------------------*/
void delay_ms(DWORD ms)
{
	int i;
	
	for (i = 0; i < ms; i++)
	{
		delay_us(1000);
	}
}


/*-----------------------------------------------------------------------*/
/* 初始化并自检                                                          */
/*-----------------------------------------------------------------------*/
void init_all_and_POST(void)
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
	//init_DSPI_2();
	//init_I2C();
	
	//enable_irq();
	
	/* 初始化SPI总线 */
	init_DSPI_1();
	
	/* 开启外部总中断 */
	enable_irq();
	
	/* 初始化显示屏 */
	initLCD();
	//LCD_DISPLAY();
	LCD_Fill(0xFF);	/* 亮屏 */
	delay_ms(500);
	LCD_Fill(0x00);	/* 黑屏 */
	delay_ms(500);
	
	/* 初始化TF卡 */
	LCD_P8x16Str(0,0, (BYTE*)"TF..");
	if (!SD_init())
	{
		/* 挂载TF卡文件系统 */
		if (FR_OK == f_mount(&fatfs, path, 1))
		{
			/* 文件读写测试 */
			FIL fil;
			TCHAR *tchar = "TEST";
			UINT br;
			UINT wr;
			DWORD test_write_to_TFCard = 0x0A1B2C3D;
			DWORD test_read_from_TFCard = 0x00000000;
			
			f_open(&fil, tchar, FA_CREATE_ALWAYS);
			f_close(&fil);
			f_open(&fil, tchar, FA_WRITE);
			f_write(&fil, (const void *)&test_write_to_TFCard, sizeof(test_write_to_TFCard), &wr);
			f_close(&fil);
			f_open(&fil, tchar, FA_READ);
			f_read(&fil, (void *)&test_read_from_TFCard, sizeof(test_read_from_TFCard), &br);
			f_close(&fil);
			if (test_write_to_TFCard == test_read_from_TFCard)
			{
				g_devices_init_status.TFCard_is_OK = 1;
			}
		}
	}
	if (g_devices_init_status.TFCard_is_OK)
	{
		LCD_P8x16Str(0,0, (BYTE*)"TF..OK");
	}
	else
	{
		LCD_P8x16Str(0,0, (BYTE*)"TF..NOK");
	}
		
	/* 初始化陀螺仪 */
	LCD_P8x16Str(0,2, (BYTE*)"L3G..");
	for (i=0; i<5; i++)
	{
		BYTE rev = 0x00;
		
		ReadReg(WHO_AM_I, &rev);
		if (I_AM_L3G4200D == rev)
		{
			g_devices_init_status.L3G4200D_is_OK = 1;
			SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);
			SetMode(NORMAL);
			break;
		}
		
	}
	if (g_devices_init_status.L3G4200D_is_OK)
	{
		LCD_P8x16Str(0,2, (BYTE*)"L3G..OK");
	}
	else
	{
		LCD_P8x16Str(0,2, (BYTE*)"L3G..NOK");
	}
		
	
	
#if 1
	/* 读取设备号 */
	read_device_no_from_TF();
	LCD_P8x16Str(0, 4, (BYTE*)"DeviceNo=");
	LCD_PrintoutInt(72, 4, g_device_NO);
	
	/* 开启RFID读卡器主动模式 */
	send_RFID_cmd(rfid_cmd_energetic_mode_enable);
	delay_ms(100);
	send_RFID_cmd(rfid_cmd_energetic_mode_enable_new);
	delay_ms(100);
	if (1 == g_devices_init_status.RFIDCard_energetic_mode_enable_is_OK)
	{
		LCD_P8x16Str(0, 6, (BYTE*)"RFID..OK");
	}
	else
	{
		LCD_P8x16Str(0, 6, (BYTE*)"RFID..NOK");
	}
	
	
	/* 换屏 */
	delay_ms(1500);
	LCD_Fill(0x00);
	
	/* 读取舵机参数 */
	read_steer_helm_data_from_TF();
	LCD_P8x16Str(0, 0, (BYTE*)"StH.L=");
	LCD_PrintoutInt(48, 0, data_steer_helm.left_limit);
	set_steer_helm(data_steer_helm.left_limit);
	delay_ms(500);
	LCD_P8x16Str(0, 2, (BYTE*)"StH.R=");
	LCD_PrintoutInt(48, 2, data_steer_helm.right_limit);
	set_steer_helm(data_steer_helm.right_limit);
	delay_ms(500);
	LCD_P8x16Str(0, 4, (BYTE*)"StH.C=");
	LCD_PrintoutInt(48, 4, data_steer_helm.center);
	set_steer_helm(data_steer_helm.center);
#endif

	/* 换屏 */
	delay_ms(1500);
	LCD_Fill(0x00);
	
	
	/* 速度闭环测试 */
	g_f_enable_speed_control = 1;	/* 启用速度控制 */
	LCD_P8x16Str(0, 0, (BYTE*)"S.T=10");
	set_speed_target(10);
	delay_ms(2000);
	LCD_P8x16Str(0, 2, (BYTE*)"S.T=-10");
	set_speed_target(-10);
	delay_ms(2000);
	LCD_P8x16Str(0, 4, (BYTE*)"S.T=0");
	set_speed_target(0);
	delay_ms(2000);
	
	/* 换屏 */
	delay_ms(1500);
	LCD_Fill(0x00);
	
	/* 测试电感 */
	LCD_P8x16Str(0, 0, (BYTE*)"I.L=");
	LCD_P8x16Str(0, 2, (BYTE*)"I.R=");
	for (i = 0; i < 5; i++)
	{
		mag_read();
		LCD_PrintoutInt(32, 0, mag_left);
		LCD_PrintoutInt(32, 2, mag_right);
		delay_ms(500);
	}
	
	/* 换屏 */
	delay_ms(1500);
	LCD_Fill(0x00);
}









