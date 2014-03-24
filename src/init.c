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
	CGM.SC_DC[0].R = 0x80;//LIN
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
  //eMIOS0��ʼ��80MHz��Ϊ4MHz
	EMIOS_0.MCR.B.GPRE= 19;   //GPRE+1=��Ƶϵ����/* Divide 80 MHz sysclk by 19+1 = 20 for 1MHz eMIOS clk*/
	EMIOS_0.MCR.B.GPREN = 1;	/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
	
  /**********���PWM 20kHZ E5����ת��E6�ڵ�ת0-50*************/ 
    //eMIOS0Dͨ��16����/* EMIOS 0 CH 16: Modulus Up Counter */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;	    /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[16].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[16].CADR.R =200;/********��������50us 20KHZ******** */
	EMIOS_0.CH[16].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;	/* Use internal counter */
    /* EMIOS 0 CH 21ǰ�����: OPWMB */
	EMIOS_0.CH[21].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[21].CCR.B.MODE = 0x60;/* Mode is OPWM Buffered */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[21].CADR.R = 1;///* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[21].CBDR.R =1;/* Trailing edge when channel counter bus=500*/
	SIU.PCR[69].R = 0x0600;    //[11:10]ѡ��AFx �˴�AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* EMIOS 0 CH 22�������: OPWMB */
	EMIOS_0.CH[22].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[22].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */
    EMIOS_0.CH[22].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[22].CADR.R = 1;///* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[22].CBDR.R = 1; /* Trailing edge when channel counter bus=500*/
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
}


void enable_irq(void)
{
  INTC.CPR.B.PRI = 0;          //Single Core: Lower INTC's current priority
  asm(" wrteei 1");	    	   //Enable external interrupts 
}


/*************************����0***********************/
void init_serial_port_0(void)
{
	LINFLEX_0.LINCR1.B.INIT=1;  //�����ʼ��ģʽ
	LINFLEX_0.LINCR1.R=0x00000015; 
	LINFLEX_0.LINIER.B.DRIE=1; //��������ж�
	/*�������㷨baud=Fperiph_clk/(16*LFDIV)
	DIV_M=LFDIV��������
	DIV_F=LFDIVС������*16  */ 	
	LINFLEX_0.LINIBRR.B.DIV_M= 43;  	//����������38400:80M-130+3 57600:80M-86+13 115200:80M-43+6  9600:80M-520+83
    LINFLEX_0.LINFBRR.B.DIV_F = 6;		//38400:64M-104+3
    LINFLEX_0.UARTCR.B.UART=1;
	LINFLEX_0.UARTCR.R=0x00000033;//8-bit data��UART mode
	LINFLEX_0.LINCR1.B.INIT=0; //�˳���ʼ��ģʽ
	
	SIU.PCR[18].R = 0x0400;    /* MPC56xxB: Configure port B2 as LIN0TX */
    SIU.PCR[19].R = 0x0103;    /* MPC56xxB: Configure port B3 as LIN0RX */
  	INTC_InstallINTCInterruptHandler(intc_serial_port_0_RX,79,02); 
}


void serial_port_0_TX(BYTE data)
{
	LINFLEX_0.BDRL.B.DATA0 = data;       //�������
	while(!LINFLEX_0.UARTSR.B.DTF){}
	LINFLEX_0.UARTSR.B.DTF=1;
}


void intc_serial_port_0_RX(void)
{
	BYTE rev_ch;
	
	while(!LINFLEX_0.UARTSR.B.DRF){}
	rev_ch = (BYTE)LINFLEX_0.BDRM.B.DATA4;
	LINFLEX_0.UARTSR.B.DRF=1;      //��ձ�־λ
}


/*************************����1***********************/
void init_serial_port_1(void) 
{
	LINFLEX_1.LINCR1.B.INIT=1;	//�����ʼ��ģʽ
	LINFLEX_1.LINCR1.R=0x00000011;
#if 1	//80M
	LINFLEX_1.LINIBRR.B.DIV_M= 520;	//����������38400:80M-130+3  115200:80M-43+6
    LINFLEX_1.LINFBRR.B.DIV_F =83;	//57600:80M-86+81
#endif

#if 0	//16M
	LINFLEX_1.LINIBRR.B.DIV_M= 8;	//����������115200:16M-8+11
    LINFLEX_1.LINFBRR.B.DIV_F =11;
#endif

    LINFLEX_1.UARTCR.B.UART=1;
	LINFLEX_1.UARTCR.R=0x00000033;
	
	LINFLEX_1.LINIER.B.DRIE=1;
	
	LINFLEX_1.LINCR1.B.INIT=0;	//�˳���ʼ��ģʽ
	
	SIU.PCR[38].R = 0x0400;	/* MPC56xxB: Configure port C6 as LIN1TX */
    SIU.PCR[39].R = 0x0103;	/* MPC56xxB: Configure port C7 as LIN1RX */
    
    INTC_InstallINTCInterruptHandler(intc_serial_port_1_RX, 99, 2);
}


void serial_port_1_TX(unsigned char data)
{
 

	LINFLEX_1.BDRL.B.DATA0=data;	//�������
	while(!LINFLEX_1.UARTSR.B.DTF){}	//�ȴ����ݷ������
	LINFLEX_1.UARTSR.B.DTF=1;	//��ձ�־λ
}


void intc_serial_port_1_RX()
{
	BYTE rev_ch;
	
	while(!LINFLEX_1.UARTSR.B.DRF){}
	rev_ch = (BYTE)LINFLEX_1.BDRM.B.DATA4;
	LINFLEX_1.UARTSR.B.DRF = 1;
}


/*************************����2***********************/
void init_serial_port_2(void)
{
	LINFLEX_2.LINCR1.B.INIT=1;
	LINFLEX_2.LINCR1.R=0x00000011; 
	LINFLEX_2.LINIER.B.DRIE=1;
	LINFLEX_2.LINIBRR.B.DIV_M= 520;
    LINFLEX_2.LINFBRR.B.DIV_F = 83;
    LINFLEX_2.UARTCR.B.UART=1;
	LINFLEX_2.UARTCR.R=0x00000033;
	LINFLEX_2.LINCR1.B.INIT=0;
	
	SIU.PCR[40].R = 0x0400;	/* MPC56xxB: Configure port C8 as LIN0TX */
    SIU.PCR[41].R = 0x0103;	/* MPC56xxB: Configure port C9 as LIN0RX */
  	INTC_InstallINTCInterruptHandler(intc_serial_port_2_RX, 119, 2); 
}


void serial_port_2_TX(BYTE data)
{
	LINFLEX_2.BDRL.B.DATA0=data;
	while(!LINFLEX_2.UARTSR.B.DTF){}
	LINFLEX_2.UARTSR.B.DTF=1;
}


void intc_serial_port_2_RX(void)
{
	BYTE rev_ch;
	
	while(!LINFLEX_2.UARTSR.B.DRF){}
	rev_ch=(BYTE)LINFLEX_2.BDRM.B.DATA4;
	LINFLEX_2.UARTSR.B.DRF=1;
}


/*************************����3***********************/
void init_serial_port_3(void)
{
	LINFLEX_3.LINCR1.B.INIT=1;
	LINFLEX_3.LINCR1.R=0x00000011; 
	LINFLEX_3.LINIER.B.DRIE=1;
	LINFLEX_3.LINIBRR.B.DIV_M= 520;
    LINFLEX_3.LINFBRR.B.DIV_F = 83;
    LINFLEX_3.UARTCR.B.UART=1;
	LINFLEX_3.UARTCR.R=0x00000033;
	LINFLEX_3.LINCR1.B.INIT=0;
	
	SIU.PCR[74].R = 0x0400;	/* MPC56xxB: Configure port E10 as LIN0TX */
    //SIU.PCR[75].R = 0x0103;	/* MPC56xxB: Configure port E11 as LIN0RX */	//���� �漰�����Ÿ���
  	//INTC_InstallINTCInterruptHandler(intc_serial_port_3_RX, 122, 2); 
}


void serial_port_3_TX(BYTE data)
{
	LINFLEX_3.BDRL.B.DATA0 = data;       //�������
	while(!LINFLEX_3.UARTSR.B.DTF){}
	LINFLEX_3.UARTSR.B.DTF = 1;
}


void intc_serial_port_3_RX(void)
{
	BYTE rev_ch;
	
	while(!LINFLEX_3.UARTSR.B.DRF){}
	rev_ch = (BYTE)LINFLEX_3.BDRM.B.DATA4;
	LINFLEX_3.UARTSR.B.DRF = 1;
}


//**********************����0***************************
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
	
	tmp_time.R = 0x00000000;	//���δ��ʼ������
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
	
	EMIOS_0.CH[2].CSR.B.FLAG = 1;	//����жϱ�־λ
	
	//���ж�
	//EMIOS_0.CH[2].CCR.B.FEN = 0;
	//EMIOS_0.CH[4].CCR.B.FEN = 0;
}


//**********************�жϴ��С��***************************
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
void delayms(int ms)
{   
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<20000;jj++){}//1ms      
}


