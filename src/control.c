#define __CONTROL_C_
#include "includes.h"


int g_f_pit = 0;
DWORD g_time_basis_PIT = 0x00000000;	/* ʱ���׼ */
int g_f_enable_mag_steer_control = 0;	/* ���õ��ѭ����־λ */
int g_f_enable_speed_control = 0;	/* �����ٶȿ��Ʊ�־λ */
int g_f_enable_rad_control_1 = 0;		/* ���������ǽǶȿ���Ư�Ʊ�־λ*/
int g_f_enable_rad_control_2 = 0;		/* ���������ǽǶȿ���ת���־λ */
int g_f_enable_speed_control_2 = 0;		/* ���������ǽǶȿ��������±�־λ */
int angle1=0;  /* ���������ǽǶȿ���ת���־λ   ת��Ƕ�ֵ */
int update_steer_helm_basement_to_steer_helm(void);


/*-----------------------------------------------------------------------*/
/* �����ʼ�� 	                                                                      */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* PIT�жϴ�����                                                                 */
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
	if (g_f_enable_speed_control)
	{
		contorl_speed_encoder_pid();
	}

	/* ���ѭ�� */
	if (g_f_enable_mag_steer_control)
	{
		mag_read();
		control_steer_helm();
	}
	
	/* �����ǽǶȿ���Ư��*/
	if (g_f_enable_rad_control_1)
	{
		if (!control_steer_helm_2())
		{
			g_f_enable_rad_control_1 =0;  
			set_steer_helm((WORD)(data_steer_helm.center));	
			set_speed_target(5);
		}
	}
	
	/* �����ǽǶȿ���ת�� */
	if(g_f_enable_rad_control_2)
	{
		if (!control_steer_helm_3(angle1))
		{
			g_f_enable_rad_control_2 =0;  
			set_steer_helm((WORD)(data_steer_helm.center));	
			set_speed_target(5);
		}
	}
	
	/* �����ǿ��������� */
	if(g_f_enable_speed_control_2)
	{
		
	}
	
#if 0
	/* ����λ�� */
	{
		BYTE data[7];
		
		generate_remote_net_frame_to_send_site(WIFI_NET_CMD_CAR_REPORT_CURRENT_SITE, RFID_site_data.site, data);
		generate_remote_frame(WIFI_CMD_NET, data, sizeof(data));
	}
#endif

	PIT.CH[1].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1 
}


/*-----------------------------------------------------------------------*/
/* �����ٶ�PWM                                                                    */
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
/* BangBang�ٶȿ���                                                             */
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
/* ����ٶ�ƫ��                                                                      */
/* ��������Ҷ��                                                                      */
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
/* PID�ٶȿ���                                                                       */
/* ��������Ҷ��                                                                      */                                                          
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
/* ����Ŀ���ٶ�                                                                      */
/*-----------------------------------------------------------------------*/
void set_speed_target(SWORD speed_target)
{
	data_speed_settings.speed_target = speed_target;
}


/*-----------------------------------------------------------------------*/
/* �����ٶ�PID����Pֵ                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_KP(WORD kp)
{
	data_speed_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* �����ٶ�PID����Iֵ                                                             */
/*-----------------------------------------------------------------------*/
void set_speed_KI(WORD ki)
{
	data_speed_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* �����ٶ�PID����Dֵ                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_KD(WORD kd)
{
	data_speed_pid.d = kd;
}

/*-----------------------------------------------------------------------*/
/* �����ǿ���ת��   //Ҷ�����                                            */
/*-----------------------------------------------------------------------*/
void control_angle_steer_helm(int angle_target)
{
		g_f_enable_rad_control_2=1;
		angle1=angle_target;
}



/*-----------------------------------------------------------------------*/
/* ���÷�����λ��                                                                */
/* ͳһ������ʽӿ�                                                                */
/* ������棬�����Ҷ棬����ֵ                                                 */
/*-----------------------------------------------------------------------*/
void set_steer_helm(SWORD helmData)
{
	if(helmData <= data_steer_helm.left_limit)
	{
		helmData = data_steer_helm.left_limit;
	}
	else if(helmData >= data_steer_helm.right_limit)
	{
		helmData = data_steer_helm.right_limit;
	}
	helm_data_record = helmData;
	helmData = (WORD)(helmData*data_steer_helm_basement.direction + data_steer_helm_basement.center);
	set_steer_helm_basement(helmData);
}

/*-----------------------------------------------------------------------*/
/* ���÷�����λ��                                                                */
/* ���ڰ�ɫ�ź��ߵĶ����                                                       */
/* ��Զ���ᣬռ�ձ����󣬶����ʱ����ת�������ǵĳ������    */
/* ���ڳ�ɫ�ź��ߵĶ����                                                       */
/* �෴                                                                                  */
/* ֱ�ӷ������Ĵ���                                                             */
/* ���޷�                                                                               */
/*-----------------------------------------------------------------------*/
void set_steer_helm_basement(WORD helmData)
{
	if(helmData <= 1500)
	{
		helmData = 1500;
	}
	else if(helmData >= 5000)
	{
		helmData = 5000;
	}
	EMIOS_0.CH[9].CBDR.R = helmData;
}


/*-----------------------------------------------------------------------*/
/* ���÷������ײ����� ��ֵ                                                   */
/* ���ķ������Ĵ���                                                             */
/*-----------------------------------------------------------------------*/
void set_steer_helm_basement_center(WORD helmData)
{
	data_steer_helm_basement.center = helmData;
	set_steer_helm_basement(helmData);
}


/*-----------------------------------------------------------------------*/
/* ���÷������ײ����� ����                                                */
/* ���ķ������Ĵ���                                                             */
/*-----------------------------------------------------------------------*/
void set_steer_helm_basement_left_limit(WORD helmData)
{
	data_steer_helm_basement.left_limit = helmData;
	set_steer_helm_basement(helmData);
}


/*-----------------------------------------------------------------------*/
/* ���÷������ײ����� �Ҽ���                                                */
/* ���ķ������Ĵ���                                                             */
/*-----------------------------------------------------------------------*/
void set_steer_helm_basement_right_limit(WORD helmData)
{
	data_steer_helm_basement.right_limit = helmData;
	set_steer_helm_basement(helmData);
}


/*-----------------------------------------------------------------------*/
/* ���������ײ����ݸ��µ��������ϲ�����                            */
/* У�������Ƿ����                                                                */
/* �������޸� ����0                                                                */
/* ������ܾ��޸� ����1                                                          */
/*-----------------------------------------------------------------------*/
int update_steer_helm_basement_to_steer_helm(void)
{
	if(data_steer_helm_basement.left_limit < data_steer_helm_basement.center && data_steer_helm_basement.center < data_steer_helm_basement.right_limit)
	{
		data_steer_helm_basement.direction = 1;
		data_steer_helm.left_limit = (SWORD)(data_steer_helm_basement.left_limit - data_steer_helm_basement.center);
		data_steer_helm.right_limit = (SWORD)(data_steer_helm_basement.right_limit - data_steer_helm_basement.center);
	}
	else if (data_steer_helm_basement.left_limit > data_steer_helm_basement.center && data_steer_helm_basement.center > data_steer_helm_basement.right_limit)
	{
		data_steer_helm_basement.direction = -1;
		data_steer_helm.left_limit = (SWORD)(data_steer_helm_basement.center - data_steer_helm_basement.left_limit);
		data_steer_helm.right_limit = (SWORD)(data_steer_helm_basement.center - data_steer_helm_basement.right_limit);
	}
	else
	{
		return 1;
	}
	return 0;
}


/*-----------------------------------------------------------------------*/
/* ��ȡ�������ڼ����Ĳ�ֵ�����ù�д�ɺ���                               */
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
