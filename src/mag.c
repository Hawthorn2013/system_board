#define __MAG_C_
#include "includes.h"


WORD mag_left=0;
WORD mag_right=0;
WORD mag_right_old=0;
WORD mag_left_old=0;
WORD ab_s=0;


/*-----------------------------------------------------------------------*/
/* ��ʼ��AD                                                              */
/*-----------------------------------------------------------------------*/
void init_ADC(void)
{
	ADC.MCR.R = 0x20000100;     //Conversion times for 80MHz ADClock  ����ģʽ
	ADC.NCMR[1].R = 0x000000FF; //Select ANS0 ANS1 ANS2 ANS3 inputs for conversion 
	ADC.CTR[1].R = 0x00008606;  //Conversion times for 32MHz ADClock���� 
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
/* ��Ŷ�ֵ����                                                          */
/*-----------------------------------------------------------------------*/
void mag_read(void)
{
	mag_right_old=mag_right;
	mag_right=(WORD)ADC.CDR[33].B.CDATA;//PB9
	mag_left_old=mag_left;
	mag_left=(WORD)ADC.CDR[32].B.CDATA;//PB8
}


/*-----------------------------------------------------------------------*/
/* �ж��Ƿ��ڴ�����                                                      */
/*-----------------------------------------------------------------------*/
int is_on_mag_line(void)
{
	mag_read();
	if (mag_left != 0 && mag_right != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/*-----------------------------------------------------------------------*/
/* ��ŷ��ͺ���                                                          */
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
/* �����������                                                          */
/*-----------------------------------------------------------------------*/
/*
#define data_steer_helm.center (3301)
#define STEER_HELM_TEST (1500)
#define STEER_HELM_LEFT (2247)
#define STEER_HELM_RIGHT (4292)
*/
void control_steer_helm(void)
{	
	int error = 0,error_count = 0;
	int kp=3,kd=0,ki=0;
	int pos=0;
	int steer_rate = 0;
	static int last_error=0;
	static int steer_pwm =0;	/* ��ȫ�ֱ�����Ϊ�ֲ���̬���� */
	
	pos = mag_left - mag_right;
	//LCD_PrintoutInt(0, 0,(pos));
	error = -pos;
	if(abs(error)>20)
	{
	error_count = (error-last_error);
	steer_rate = (kp*(error)+kd*error_count);
	//LCD_PrintoutInt(0, 0,(steer_rate));
	//LCD_PrintoutInt(0, 2,(mag_right));
	//LCD_PrintoutInt(0, 4,(mag_left));
	last_error = error;
	if(mag_left<=30)steer_rate=1400;
	if(mag_right<=30)steer_rate=-1400;
	steer_pwm = steer_rate;
	//LCD_PrintoutInt(0,2,(steer_pwm));
	set_steer_helm((WORD)(3293+steer_pwm));	/* �㾯�� */
	}
	else
	{
		set_steer_helm(3293);
	}
}