#define __CONTROL_C_
#include "includes.h"


int g_f_pit = 0;
DWORD g_time_basis_PIT = 0x00000000;	/* ʱ���׼ */


/*-----------------------------------------------------------------------*/
/* PIT�жϴ�����                                                       */
/*-----------------------------------------------------------------------*/
void PitISR(void)
{
	g_f_pit = 1;
	
	g_time_basis_PIT++;	/* ��ʱ */
	
	/* start:encoder */
	data_encoder.is_forward = SIU.GPDI[28].B.PDI;
	data_encoder.cnt_old = data_encoder.cnt_new;
	data_encoder.cnt_new = (WORD)EMIOS_0.CH[24].CCNTR.R;
	if (data_encoder.cnt_new >= data_encoder.cnt_old)
	{
		data_encoder.speed_now = data_encoder.cnt_new - data_encoder.cnt_old;
	}
	else
	{
		data_encoder.speed_now = 0xffff - (data_encoder.cnt_old - data_encoder.cnt_new);
	}
	/* end:encoder */
	
	/* ��ʼִ���ٶȿ����㷨 */
	contorl_speed_encoder_pid();

#if 0
	/* ������ */
	punctured_ballon(rfid_site);
#endif

	/* ���ѭ�� */
	mag_read();
	control_steer_helm();
	
	PIT.CH[1].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1 
}


/*-----------------------------------------------------------------------*/
/* �����ٶ�PWM                                                           */
/*-----------------------------------------------------------------------*/
void set_speed_pwm(int16_t speed_pwm)	//speed_pwm��Ϊ��ǰ����Ϊ���
{
	if (speed_pwm>0)	//forward
	{
		if (speed_pwm>SPEED_PWM_MAX)
		{
			speed_pwm = SPEED_PWM_MAX;
		}
		EMIOS_0.CH[21].CBDR.R = speed_pwm;
		EMIOS_0.CH[22].CBDR.R = 1;
		
	}
	else if (speed_pwm<0)	//backward
	{
		speed_pwm = 0-speed_pwm;
		if (speed_pwm>SPEED_PWM_MAX)
		{
			speed_pwm = SPEED_PWM_MAX;
		}

		EMIOS_0.CH[21].CBDR.R = 1;
		EMIOS_0.CH[22].CBDR.R = speed_pwm;	
	}
	else
	{
		EMIOS_0.CH[21].CBDR.R = 1;
		EMIOS_0.CH[22].CBDR.R = 1;	
	}
}


/*-----------------------------------------------------------------------*/
/* BangBang�ٶȿ���                                                      */
/*-----------------------------------------------------------------------*/
void contorl_speed_encoder_bb(void)
{
	int32_t tmp_speed_now;
	
	
	if (data_encoder.is_forward)
	{
		tmp_speed_now = data_encoder.speed_now;
	}
	else
	{
		tmp_speed_now = 0 - data_encoder.speed_now;
	}
	
	if (tmp_speed_now > data_speed_settings.speed_target)
	{
		set_speed_pwm(0 - SPEED_PWM_MAX);
	}
	else if (tmp_speed_now < data_speed_settings.speed_target)
	{
		set_speed_pwm(SPEED_PWM_MAX);
	}
}


/*-----------------------------------------------------------------------*/
/* ����ٶ�ƫ��                                                          */
/* ��������Ҷ��                                                          */
/*-----------------------------------------------------------------------*/
static SWORD get_e0()
{
	SWORD tmp_speed_now;
	SWORD e0;
	if (data_encoder.is_forward)
	{
		tmp_speed_now =(SWORD) data_encoder.speed_now;
	}
	else
	{
		tmp_speed_now = 0 - (SWORD)data_encoder.speed_now;
	}
	e0=data_speed_settings.speed_target-tmp_speed_now;
	return e0;
	
}


/*-----------------------------------------------------------------------*/
/* PID�ٶȿ���                                                           */
/* ��������Ҷ��                                                          */                                                          
/*-----------------------------------------------------------------------*/
void contorl_speed_encoder_pid(void)
{
	float d_speed_pwm;
	SWORD e0=get_e0();
	static SWORD e1=0;
	static SWORD e2=0;
	static SWORD speed_pwm=SPEED_PWM_MIN;
	d_speed_pwm=(float)(data_speed_pid.p*(e0-e1));       //P����
	d_speed_pwm+=(float)(data_speed_pid.d*(e0+e2-2*e1));
	d_speed_pwm+=(float)(data_speed_pid.i*(e0));		
	if(d_speed_pwm>200)
	      d_speed_pwm=200;
	if(d_speed_pwm<-200)
	      d_speed_pwm=-200;   //����pwm�仯��
	speed_pwm+=d_speed_pwm;
	if(speed_pwm>SPEED_PWM_MAX)
			speed_pwm = SPEED_PWM_MAX;
	else if (speed_pwm<0-SPEED_PWM_MAX)
			speed_pwm =0- SPEED_PWM_MAX;    //��ֹ�������ɸ�����
	set_speed_pwm(speed_pwm);
	e2=e1;
	e1=e0;	
}


/*-----------------------------------------------------------------------*/
/* ����Ŀ���ٶ�                                                          */
/*-----------------------------------------------------------------------*/
void set_speed_target(SWORD speed_target)
{
	data_speed_settings.speed_target = speed_target;
}


/*-----------------------------------------------------------------------*/
/* �����ٶ�PID����Pֵ                                                    */
/*-----------------------------------------------------------------------*/
void set_speed_KP(WORD kp)
{
	data_speed_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* �����ٶ�PID����Iֵ                                                    */
/*-----------------------------------------------------------------------*/
void set_speed_KI(WORD ki)
{
	data_speed_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* �����ٶ�PID����Dֵ                                                    */
/*-----------------------------------------------------------------------*/
void set_speed_KD(WORD kd)
{
	data_speed_pid.d = kd;
}


/*-----------------------------------------------------------------------*/
/* ���÷�����λ��                                                      */
/* ��Զ���ᣬռ�ձ����󣬶����ʱ����ת�������ǵĳ�����档            */
/*-----------------------------------------------------------------------*/
void set_steer_helm(WORD helmData)
{
	/*
	if(helmData<=STEER_HELM_LEFT)
	{
		helmData=STEER_HELM_LEFT;
	}
	else if(helmData>=STEER_HELM_RIGHT)
	{
		helmData=STEER_HELM_RIGHT;
	}
	*/
	
	EMIOS_0.CH[9].CBDR.R = helmData;
	helm_data_record = helmData;
}


/*-----------------------------------------------------------------------*/
/* ��ȡ�������ڼ����Ĳ�ֵ�����ù�д�ɺ���                                */
/*-----------------------------------------------------------------------*/
DWORD diff_time_basis_PIT(DWORD new_time, DWORD old_time)
{
	DWORD diff;
	
	if (new_time >= old_time)
	{
		diff = new_time - old_time;
	}
	else
	{
		diff = new_time + (0xFFFFFFFF- old_time);
	}
	
	return diff;
}
