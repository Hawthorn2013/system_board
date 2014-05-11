#include "includes.h"




void init_modes_and_clock(void) 
{
    ME.MER.R = 0x0000001D;          //Enable DRUN, RUN0, SAFE, RESET modes
  //设置sysclk
    //CGM.FMPLL_CR.R = 0x02400100;  //8 MHz xtal: Set PLL0 to 64 MHz  
    CGM.FMPLL_CR.R = 0x01280000;    //8 MHz xtal: Set PLL0 to 80 MHz 
    //CGM.FMPLL_CR.R = 0x013C0000;  //8 MHz xtal: Set PLL0 to 120 MHz    
    ME.RUN[0].R = 0x001F0064;       // RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclk选择锁相环时钟
    ME.RUNPC[0].R = 0x00000010;     // Peri. Cfg. 1 settings: only run in RUN0 mode      选择RUN0模式
  
  //PCTL[?] 选择需要时钟模块(默认即可，不用设置)
	//ME.PCTL[32].R = 0x00; 	    //MPC56xxB/P/S ADC 0: select ME.RUNPC[0] 
    ME.PCTL[32].B.DBG_F = 0;
  //Mode Transition to enter RUN0 mode                                
    ME.MCTL.R = 0x40005AF0;         //Enter RUN0 Mode & Key 
    ME.MCTL.R = 0x4000A50F;         //Enter RUN0 Mode & Inverted Key 
    
    while (ME.GS.B.S_MTRANS) {}     //Wait for mode transition to complete 等待模式转换完成                         
    while(ME.GS.B.S_CURRENTMODE != 4) {} //Verify RUN0 is the current mode 等待选择RUN0模式
  
  //开peri0、1、2
	CGM.SC_DC[0].R = 0x84;//LIN
	CGM.SC_DC[1].R = 0x80;//FLEXCAN,DSPI
    CGM.SC_DC[2].R = 0x80;//eMIOS,CTU,ADC
}


void disable_watchdog(void)
{
	SWT.SR.R = 0x0000c520; //Write keys to clear soft lock bit
	SWT.SR.R = 0x0000d928;
	SWT.CR.R = 0x8000010A; //Clear watchdog enable (WEN)
}


void init_led(void)
{
	SIU.PCR[15].R = 0x0203;	//PA15
 	SIU.PCR[14].R = 0x0203; 
  	SIU.PCR[13].R = 0x0203;
 	SIU.PCR[12].R = 0x0203;	//PA12
 	
	SIU.GPDO[12].R = 1;	//0：点亮，1：熄灭
	SIU.GPDO[13].R = 1;
	SIU.GPDO[14].R = 1;
	SIU.GPDO[15].R = 1;
}


void init_pit(void)	//10ms
{                              //NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
	PIT.PITMCR.R = 0x00000001;       //Enable PIT and configure timers to stop in debug mode 
	PIT.CH[1].LDVAL.R = 800000;       //1600000--20ms
	PIT.CH[1].TCTRL.R = 0x000000003; //Enable PIT1 interrupt and make PIT active to count
	//INTC.PSR[60].R = 0x02;           //PIT 1 interrupt vector with priority 1
	INTC_InstallINTCInterruptHandler(PitISR,60,1);
}


void init_encoder(void)	//PD12模数计数器入口，上升沿
{
	EMIOS_0.CH[24].CCR.B.MODE = 0x51; // Mode is MCB, 
	EMIOS_0.CH[24].CCR.B.BSL = 0x3; // Use internal counter
	EMIOS_0.CH[24].CCR.B.UCPRE=0; /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[24].CCR.B.UCPEN = 1; /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[24].CCR.B.FREN = 0;/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[24].CCR.B.EDPOL=1; //Edge Select rising edge
	EMIOS_0.CH[24].CADR.R=0xffff;

	SIU.PCR[60].R = 0x0100;  // Initialize pad for eMIOS channel Initialize pad for input
	
	SIU.PCR[59].R = 0x0102;  //PD11光编正反转
}


void initEMIOS_0MotorAndSteer(void)
{
  //eMIOS0初始化80MHz分为4MHz
	EMIOS_0.MCR.B.GPRE= 19;   //GPRE+1=分频系数；/* Divide 80 MHz sysclk by 19+1 = 20 for 1MHz eMIOS clk*/
	EMIOS_0.MCR.B.GPREN = 1;	/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
	
  /**********电机PWM 20kHZ E5口正转、E6口倒转0-50*************/ 
    //eMIOS0D通道16设置/* EMIOS 0 CH 16: Modulus Up Counter */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;	    /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[16].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[16].CADR.R =200;/********设置周期50us 20KHZ******** */
	EMIOS_0.CH[16].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;	/* Use internal counter */
    /* EMIOS 0 CH 21前进输出: OPWMB */
	EMIOS_0.CH[21].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[21].CCR.B.MODE = 0x60;/* Mode is OPWM Buffered */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[21].CADR.R = 1;///* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[21].CBDR.R =1;/* Trailing edge when channel counter bus=500*/
	SIU.PCR[69].R = 0x0600;    //[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* EMIOS 0 CH 22后退输出: OPWMB */
	EMIOS_0.CH[22].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[22].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */
    EMIOS_0.CH[22].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[22].CADR.R = 1;///* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[22].CBDR.R = 1; /* Trailing edge when channel counter bus=500*/
	SIU.PCR[70].R = 0x0600;    //[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	
  /**********舵机PWM 50HZ A9口输出0-20000**********/
    //eMIOS0D通道8设置/* EMIOS 0 CH 8: Modulus Up Counter */
    EMIOS_0.CH[8].CCR.B.UCPRE=3;	    /* Set channel prescaler to divide by 4 */
	EMIOS_0.CH[8].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 4 */
	EMIOS_0.CH[8].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[8].CADR.R = 20000;/********设置周期0.02s  50HZ******* */
	EMIOS_0.CH[8].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[8].CCR.B.BSL = 0x3;	/* Use internal counter */
    /* EMIOS 0 CH 9: Output Pulse Width Modulation */
	EMIOS_0.CH[9].CCR.B.BSL = 0x1;	/* Use counter bus C (default) */
	EMIOS_0.CH[9].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */  
    EMIOS_0.CH[9].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[9].CADR.R = 1;///* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[9].CBDR.R = STEER_HELM_TEST;/* Trailing edge when channel counter bus=500*/
	SIU.PCR[9].R = 0x0600;    //[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
}


void enable_irq(void)
{
  INTC.CPR.B.PRI = 0;          //Single Core: Lower INTC's current priority
  asm(" wrteei 1");	    	   //Enable external interrupts 
}




//**********************超声0***************************
void init_supersonic_trigger_0(void)
{
	EMIOS_0.CH[3].CCR.B.BSL = 0x3;
	EMIOS_0.CH[3].CCR.B.UCPRE=0;
	EMIOS_0.CH[3].CCR.B.UCPEN = 1;
	EMIOS_0.CH[3].CCR.B.FREN = 0;
	EMIOS_0.CH[3].CCR.B.EDPOL=0;
	EMIOS_0.CH[3].CCR.B.EDSEL = 0;
	EMIOS_0.CH[3].CCR.B.FEN=0;
	
	EMIOS_0.CH[3].CADR.B;
	SIU.PCR[3].R =0x0603;	//trigger A3
	
	EMIOS_0.CH[3].CADR.B.CADR = 0x0000ff;
	
	EMIOS_0.CH[3].CCR.B.MODE = 0x03;
}


void init_supersonic_trigger_1(void)
{
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;
	EMIOS_0.CH[0].CCR.B.UCPRE=0;
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;
	EMIOS_0.CH[0].CCR.B.FREN = 0;
	EMIOS_0.CH[0].CCR.B.EDPOL=0;
	EMIOS_0.CH[0].CCR.B.EDSEL = 0;
	EMIOS_0.CH[0].CCR.B.FEN=0;
	
	EMIOS_0.CH[0].CADR.B;
	SIU.PCR[0].R =0x0603;	//trigger A0
	
	EMIOS_0.CH[0].CADR.B.CADR = 0x0000ff;
	
	EMIOS_0.CH[0].CCR.B.MODE = 0x03;
}


void init_supersonic_trigger_2(void)
{
	EMIOS_0.CH[5].CCR.B.BSL = 0x3;
	EMIOS_0.CH[5].CCR.B.UCPRE=0;
	EMIOS_0.CH[5].CCR.B.UCPEN = 1;
	EMIOS_0.CH[5].CCR.B.FREN = 0;
	EMIOS_0.CH[5].CCR.B.EDPOL=0;
	EMIOS_0.CH[5].CCR.B.EDSEL = 0;
	EMIOS_0.CH[5].CCR.B.FEN=0;
	
	EMIOS_0.CH[5].CADR.B;
	SIU.PCR[5].R =0x0603;	//trigger A5
	
	EMIOS_0.CH[5].CADR.B.CADR = 0x0000ff;
	
	EMIOS_0.CH[5].CCR.B.MODE = 0x03;
}


void init_supersonic_receive_0(void)
{
	EMIOS_0.CH[2].CCR.B.MODE = 0x04; // Mode is Input Pulse Width Measurement 
	EMIOS_0.CH[2].CCR.B.BSL = 0x3;   // Use internal counter
	EMIOS_0.CH[2].CCR.B.UCPRE=0; //Set channel prescaler to divide by 1
	EMIOS_0.CH[2].CCR.B.UCPEN = 1;	//Enable prescaler; uses default divide by 1
	EMIOS_0.CH[2].CCR.B.FREN = 0;	//Freeze channel counting when in debug mode
	EMIOS_0.CH[2].CCR.B.EDPOL=1; //Edge Select rising edge
	EMIOS_0.CH[2].CCR.B.FEN=1;  //interupt enbale

	SIU.PCR[2].R = 0x0100;  //E   A2
	INTC_InstallINTCInterruptHandler(intc_get_supersonic_time_0, 142, 4);
}


void trigger_supersonic_0(void)
{
	EMIOS_0.CH[3].CCR.B.MODE = 0x01;
	EMIOS_0.CH[3].CCR.B.MODE = 0x03;
}


void intc_get_supersonic_time_0(void)
{
	DWORD tmp_a, tmp_b;
	union {
		DWORD R;
		struct {
			BYTE byte_0;
			BYTE byte_1;
			BYTE byte_2;
			BYTE byte_3;
		} B;
	} tmp_time;
	
	tmp_time.R = 0x00000000;	//躲过未初始化警告
	tmp_a = EMIOS_0.CH[2].CADR.R;
	tmp_b = EMIOS_0.CH[2].CBDR.R;
	
	if(tmp_a >= tmp_b)
	{
		tmp_time.R = tmp_a - tmp_b;
	}
	else
	{
		tmp_time.R = 0x00ffffff - tmp_b + tmp_a;
	}
	
	EMIOS_0.CH[2].CSR.B.FLAG = 1;	//清除中断标志位
	
	//关中断
	//EMIOS_0.CH[2].CCR.B.FEN = 0;
	//EMIOS_0.CH[4].CCR.B.FEN = 0;
}


//**********************判断大端小端***************************
int is_big_endian()
{
	WORD a = 0x1234;
	BYTE b = *(BYTE *)&a;	//通过将int强制类型转换成char单字节，通过判断起始存储位置。即等于 取b等于a的低地址部分  
    
    if(b == 0x12)
	{
    	return 1;
	}
    
	return 0;
}


//******************光电编码器**********************
void init_optical_encoder(void)	//PD12模数计数器入口，上升沿
{
	//*********计数部分*************
	EMIOS_0.CH[24].CCR.B.MODE = 0x51; // Mode is MCB, 
	EMIOS_0.CH[24].CCR.B.BSL = 0x3; // Use internal counter
	EMIOS_0.CH[24].CCR.B.UCPRE=0; /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[24].CCR.B.UCPEN = 1; /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[24].CCR.B.FREN = 0;/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[24].CCR.B.EDPOL=1; //Edge Select rising edge
	EMIOS_0.CH[24].CADR.R=0xffff;

	SIU.PCR[60].R = 0x0100;  // Initialize pad for eMIOS channel Initialize pad for input
	
	//(WORD)EMIOS_0.CH[24].CCNTR.R 数据寄存器
	
	//********方向部分**************
	SIU.PCR[28].R = 0x0100;	//PB12
	
	//SIU.GPDI[28].B.PDI 数据寄存器
}


//****************延时******************
void delay_us(DWORD us)	//依赖总线80M
{
	volatile int i, j;
	
	for (i = 0; i < us; i++)
	{
		for (j = 0; j < 9; j++) {}
	}
}


void delay_ms(DWORD ms)
{
	int i;
	
	for (i = 0; i < ms; i++)
	{
		delay_us(1000);
	}
}