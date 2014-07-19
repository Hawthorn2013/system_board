#define __CONTROL_C_
#include "includes.h"


int g_f_pit = 0;
DWORD g_time_basis_PIT = 0x00000000;	/* 时间基准 */
int g_f_enable_mag_steer_control = 1;	/* 启用电磁循迹标志位 */


/*-----------------------------------------------------------------------*/
/* PIT中断处理函数                                                       */
/*-----------------------------------------------------------------------*/
void PitISR(void)
{
	g_f_pit = 1;
	
	g_time_basis_PIT++;	/* 计时 */
	
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
	
	/* 开始执行速度控制算法 */
	contorl_speed_encoder_pid();

#if 0
	/* 扎气球 */
	punctured_ballon(rfid_site);
#endif

	/* 电磁循迹 */
	if (g_f_enable_mag_steer_control)
	{
		mag_read();
		control_steer_helm();
	}
	
	/* 发送位置 */
	{
		BYTE data[7];
		
		generate_remote_net_frame_to_send_site(WIFI_NET_CMD_CAR_REPORT_CURRENT_SITE, RFID_site_data.site, data);
		generate_remote_frame(WIFI_CMD_NET, data, sizeof(data));
	}
	
	PIT.CH[1].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1 
}


/*-----------------------------------------------------------------------*/
/* 设置速度PWM                                                           */
/*-----------------------------------------------------------------------*/
void set_speed_pwm(int16_t speed_pwm)	//speed_pwm正为向前，负为向后
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
/* BangBang速度控制                                                      */
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
/* 获得速度偏差                                                          */
/* 有问题找叶川                                                          */
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
/* PID速度控制                                                           */
/* 有问题找叶川                                                          */                                                          
/*-----------------------------------------------------------------------*/
void contorl_speed_encoder_pid(void)
{
	float d_speed_pwm;
	SWORD e0=get_e0();
	static SWORD e1=0;
	static SWORD e2=0;
	static SWORD speed_pwm=SPEED_PWM_MIN;
	d_speed_pwm=(float)(data_speed_pid.p*(e0-e1));       //P控制
	d_speed_pwm+=(float)(data_speed_pid.d*(e0+e2-2*e1));
	d_speed_pwm+=(float)(data_speed_pid.i*(e0));		
	if(d_speed_pwm>200)
	      d_speed_pwm=200;
	if(d_speed_pwm<-200)
	      d_speed_pwm=-200;   //限制pwm变化量
	speed_pwm+=d_speed_pwm;
	if(speed_pwm>SPEED_PWM_MAX)
			speed_pwm = SPEED_PWM_MAX;
	else if (speed_pwm<0-SPEED_PWM_MAX)
			speed_pwm =0- SPEED_PWM_MAX;    //防止溢出（造成负数）
	set_speed_pwm(speed_pwm);
	e2=e1;
	e1=e0;	
}


/*-----------------------------------------------------------------------*/
/* 设置目标速度                                                          */
/*-----------------------------------------------------------------------*/
void set_speed_target(SWORD speed_target)
{
	data_speed_settings.speed_target = speed_target;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制P值                                                    */
/*-----------------------------------------------------------------------*/
void set_speed_KP(WORD kp)
{
	data_speed_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制I值                                                    */
/*-----------------------------------------------------------------------*/
void set_speed_KI(WORD ki)
{
	data_speed_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制D值                                                    */
/*-----------------------------------------------------------------------*/
void set_speed_KD(WORD kd)
{
	data_speed_pid.d = kd;
}


/*-----------------------------------------------------------------------*/
/* 设置方向舵机位置                                                      */
/* 面对舵机轴，占空比增大，舵机逆时针旋转，对我们的车是左舵。            */
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
/* 获取两个周期计数的差值，常用故写成函数                                */
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
