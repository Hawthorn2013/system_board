#include "includes.h"


WORD mag_left=0;
WORD mag_right=0;
WORD mag_right_old=0;
WORD mag_left_old=0;
WORD ab_s=0;


/*-----------------------------------------------------------------------*/
/* 初始化AD                                                              */
/*-----------------------------------------------------------------------*/
void init_ADC(void)
{
	ADC.MCR.R = 0x20000100;     //Conversion times for 80MHz ADClock  连续模式
	ADC.NCMR[1].R = 0x000000FF; //Select ANS0 ANS1 ANS2 ANS3 inputs for conversion 
	ADC.CTR[1].R = 0x00008606;  //Conversion times for 32MHz ADClock？？ 
	ADC.MCR.B.NSTART=1;       //Trigger normal conversions for ADC0
	SIU.PCR[24].R = 0x2100;     //MPC56xxB: Initialize PB[8] as ANS0 CDR32
	SIU.PCR[25].R = 0x2100;     //MPC56xxB: Initialize PB[9] as ANS1 CDR33
	SIU.PCR[26].R = 0x2100;     //MPC56xxB: Initialize PB[10] as ANS2 34
	SIU.PCR[27].R = 0x2100;     //MPC56xxB: Initialize PB[11] as ANS3 35
	SIU.PCR[60].R = 0x2100;     //MPC56xxB: Initialize PD[12] as ANS4 36
	SIU.PCR[61].R = 0x2100;     //MPC56xxB: Initialize PD[13] as ANS5 37
	SIU.PCR[62].R = 0x2100;     //MPC56xxB: Initialize PD[14] as ANS6 38
	SIU.PCR[63].R = 0x2100;     //MPC56xxB: Initialize PD[15] as ANS7 39
}


/*-----------------------------------------------------------------------*/
/* 电磁读值函数                                                          */
/*-----------------------------------------------------------------------*/
void mag_read(void)
{
	mag_right_old=mag_right;
	mag_right=(WORD)ADC.CDR[33].B.CDATA;//PB9
	mag_left_old=mag_left;
	mag_left=(WORD)ADC.CDR[32].B.CDATA;//PB8
		
}


/*-----------------------------------------------------------------------*/
/* 电磁发送函数                                                          */
/*-----------------------------------------------------------------------*/
void mag_TX(void)
{
	BYTE a,b,m,n,p,q,s,t;
	WORD c = 0x0000;
	WORD d = 0x0000;
	WORD e = 0x0000;
	WORD f = 0x0000;
	
	c=0x03ff & mag_left_old;
	a=(BYTE)c;
	b=(BYTE)(c>>8);
	serial_port_1_TX(b);
	serial_port_1_TX(a);
	
	d=0x03ff & mag_left;
	m=(BYTE)d;
	n=(BYTE)(d>>8);
	serial_port_1_TX(n);
	serial_port_1_TX(m);
	
	e=0x03ff & mag_right_old;
	p=(BYTE)d;
	q=(BYTE)(d>>8);
	serial_port_1_TX(p);
	serial_port_1_TX(q);
	
	f=0x03ff & mag_right;
	s=(BYTE)d;
	t=(BYTE)(d>>8);
	serial_port_1_TX(s);
	serial_port_1_TX(t);
	
}


/*-----------------------------------------------------------------------*/
/* 舵机参数控制                                                          */
/*-----------------------------------------------------------------------*/
/*
#define STEER_HELM_CENTER (3301)
#define STEER_HELM_TEST (1500)
#define STEER_HELM_LEFT (2247)
#define STEER_HELM_RIGHT (4292)
*/


void control_steer_helm(void)
{
	
	if(mag_left-mag_left_old>5)
	set_steer_helm(STEER_HELM_CENTER-((mag_left-mag_right)*(STEER_HELM_CENTER-STEER_HELM_LEFT)/300));
	
	else if(mag_right-mag_right_old>15)
	set_steer_helm(STEER_HELM_CENTER+((mag_right-mag_left)*(STEER_HELM_CENTER-STEER_HELM_LEFT)/300));
	
	else if(mag_left-mag_right>20||mag_right-mag_left>20)
	{
		if(mag_left-mag_right>80)
		set_steer_helm(STEER_HELM_CENTER-((mag_left-mag_right)*(STEER_HELM_CENTER-STEER_HELM_LEFT)/500));
		else if(mag_left-mag_right>20)
		set_steer_helm(STEER_HELM_CENTER-((mag_left-mag_right)*(STEER_HELM_CENTER-STEER_HELM_LEFT)/800));
		else if(mag_right-mag_left>80)
		set_steer_helm(STEER_HELM_CENTER+((mag_right-mag_left)*(STEER_HELM_CENTER-STEER_HELM_LEFT)/500));
		else if(mag_right-mag_left>20)
		set_steer_helm(STEER_HELM_CENTER+((mag_right-mag_left)*(STEER_HELM_CENTER-STEER_HELM_LEFT)/800));
		
	}
	
	else
	set_steer_helm(STEER_HELM_CENTER);
	/*
	if(mag_left>mag_right)
	set_steer_helm(STEER_HELM_LEFT);
	if(mag_left<mag_right)
	set_steer_helm(STEER_HELM_RIGHT);
	*/

}
