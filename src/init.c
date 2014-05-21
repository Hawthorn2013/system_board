#include "includes.h"




void init_modes_and_clock(void) 
{
    ME.MER.R = 0x0000001D;          //Enable DRUN, RUN0, SAFE, RESET modes
  //����sysclk
    //CGM.FMPLL_CR.R = 0x02400100;  //8 MHz xtal: Set PLL0 to 64 MHz  
    CGM.FMPLL_CR.R = 0x01280000;    //8 MHz xtal: Set PLL0 to 80 MHz 
    //CGM.FMPLL_CR.R = 0x013C0000;  //8 MHz xtal: Set PLL0 to 120 MHz    
    ME.RUN[0].R = 0x001F0064;       // RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclkѡ�����໷ʱ��
    ME.RUNPC[0].R = 0x00000010;     // Peri. Cfg. 1 settings: only run in RUN0 mode      ѡ��RUN0ģʽ
  
  //PCTL[?] ѡ����Ҫʱ��ģ��(Ĭ�ϼ��ɣ���������)
	//ME.PCTL[32].R = 0x00; 	    //MPC56xxB/P/S ADC 0: select ME.RUNPC[0] 
    ME.PCTL[32].B.DBG_F = 0;
  //Mode Transition to enter RUN0 mode                                
    ME.MCTL.R = 0x40005AF0;         //Enter RUN0 Mode & Key 
    ME.MCTL.R = 0x4000A50F;         //Enter RUN0 Mode & Inverted Key 
    
    while (ME.GS.B.S_MTRANS) {}     //Wait for mode transition to complete �ȴ�ģʽת�����                         
    while(ME.GS.B.S_CURRENTMODE != 4) {} //Verify RUN0 is the current mode �ȴ�ѡ��RUN0ģʽ
  
  //��peri0��1��2
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
 	
	SIU.GPDO[12].R = 1;	//0��������1��Ϩ��
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


void init_encoder(void)	//PD12ģ����������ڣ�������
{
	EMIOS_0.CH[24].CCR.B.MODE = 0x51; // Mode is MCB, 
	EMIOS_0.CH[24].CCR.B.BSL = 0x3; // Use internal counter
	EMIOS_0.CH[24].CCR.B.UCPRE=0; /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[24].CCR.B.UCPEN = 1; /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[24].CCR.B.FREN = 0;/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[24].CCR.B.EDPOL=1; //Edge Select rising edge
	EMIOS_0.CH[24].CADR.R=0xffff;

	SIU.PCR[60].R = 0x0100;  // Initialize pad for eMIOS channel Initialize pad for input
	
	SIU.PCR[59].R = 0x0102;  //PD11�������ת
}


void initEMIOS_0MotorAndSteer(void)
{
  //eMIOS0��ʼ��80MHz��Ϊ16MHz
	EMIOS_0.MCR.B.GPRE= 4;	//19   //GPRE+1=��Ƶϵ����/* Divide 80 MHz sysclk by 4+1 = 5 for 16MHz eMIOS clk*/
	EMIOS_0.MCR.B.GPREN = 1;	/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
	
  /**********���PWM 5kHZ E5����ת��E6�ڵ�ת0-50*************/ 
    //eMIOS0Dͨ��16����/* EMIOS 0 CH 16: Modulus Up Counter */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;	    /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[16].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[16].CADR.R = 3200;	//3200/********��������200us 5KHZ******** */
	EMIOS_0.CH[16].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;	/* Use internal counter */
    /* EMIOS 0 CH 21ǰ�����: OPWMB */
	EMIOS_0.CH[21].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[21].CCR.B.MODE = 0x60;/* Mode is OPWM Buffered */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[21].CADR.R = 0;///* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[21].CBDR.R = 0;/* Trailing edge when channel counter bus=500*/
	SIU.PCR[69].R = 0x0600;    //[11:10]ѡ��AFx �˴�AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* EMIOS 0 CH 22�������: OPWMB */
	EMIOS_0.CH[22].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[22].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */
    EMIOS_0.CH[22].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[22].CADR.R = 0;///* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[22].CBDR.R = 0; /* Trailing edge when channel counter bus=500*/
	SIU.PCR[70].R = 0x0600;    //[11:10]ѡ��AFx �˴�AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	
  /**********���PWM 50HZ A9�����0-20000**********/
    //eMIOS0Dͨ��8����/* EMIOS 0 CH 8: Modulus Up Counter */
    EMIOS_0.CH[8].CCR.B.UCPRE=3;	    /* Set channel prescaler to divide by 4 */
	EMIOS_0.CH[8].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 4 */
	EMIOS_0.CH[8].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[8].CADR.R = 20000;/********��������0.02s  50HZ******* */
	EMIOS_0.CH[8].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[8].CCR.B.BSL = 0x3;	/* Use internal counter */
    /* EMIOS 0 CH 9: Output Pulse Width Modulation */
	EMIOS_0.CH[9].CCR.B.BSL = 0x1;	/* Use counter bus C (default) */
	EMIOS_0.CH[9].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */  
    EMIOS_0.CH[9].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[9].CADR.R = 1;///* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[9].CBDR.R = STEER_HELM_CENTER;/* Trailing edge when channel counter bus=500*/
	SIU.PCR[9].R = 0x0600;    //[11:10]ѡ��AFx �˴�AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	
	//�źŶ����ʼ��
	EMIOS_0.CH[12].CCR.B.BSL = 0x1;
	EMIOS_0.CH[12].CCR.B.MODE = 0x60;  
    EMIOS_0.CH[12].CCR.B.EDPOL = 1;
	EMIOS_0.CH[12].CADR.R = 1;
	EMIOS_0.CH[12].CBDR.R = SINGLE_HELM_CENTER;
	SIU.PCR[44].R = 0x0600;
}


void enable_irq(void)
{
  INTC.CPR.B.PRI = 0;          //Single Core: Lower INTC's current priority
  asm(" wrteei 1");	    	   //Enable external interrupts 
}







//**********************�жϴ��С��***************************
//MPC5604��� ��λ�ڵ�
int is_big_endian()
{
	WORD a = 0x1234;
	BYTE b = *(BYTE *)&a;	//ͨ����intǿ������ת����char���ֽڣ�ͨ���ж���ʼ�洢λ�á������� ȡb����a�ĵ͵�ַ����  
    
    if(b == 0x12)
	{
    	return 1;
	}
    
	return 0;
}


//******************��������**********************
void init_optical_encoder(void)	//PD12ģ����������ڣ�������
{
	//*********��������*************
	EMIOS_0.CH[24].CCR.B.MODE = 0x51; // Mode is MCB, 
	EMIOS_0.CH[24].CCR.B.BSL = 0x3; // Use internal counter
	EMIOS_0.CH[24].CCR.B.UCPRE=0; /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[24].CCR.B.UCPEN = 1; /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[24].CCR.B.FREN = 0;/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[24].CCR.B.EDPOL=1; //Edge Select rising edge
	EMIOS_0.CH[24].CADR.R=0xffff;

	SIU.PCR[60].R = 0x0100;  // Initialize pad for eMIOS channel Initialize pad for input
	
	//(WORD)EMIOS_0.CH[24].CCNTR.R ���ݼĴ���
	
	//********���򲿷�**************
	SIU.PCR[28].R = 0x0100;	//PB12
	
	//SIU.GPDI[28].B.PDI ���ݼĴ���
}


//****************��ʱ******************
void delay_us(DWORD us)	//��������80M
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