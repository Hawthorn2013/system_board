#define __ACTION_C_
#include "includes.h"


/*-----------------------------------------------------------------------*/
/* 飞吊桥                                                                */
/* 不关循迹  car 2 & 3 & 4                                               */                                                          
/*-----------------------------------------------------------------------*/
void speed_up_bridge1()
{
    //g_f_enable_mag_steer_control = 0;
    set_speed_target(260);
	delay_ms(1500);
	set_speed_target(0);
//	g_f_enable_mag_steer_control = 1;
	delay_ms(500);
	set_speed_target(20);	
}
/*-----------------------------------------------------------------------*/
/* 上钢丝桥                                                              */
/* car 2 & 4                                                             */                                                          
/*-----------------------------------------------------------------------*/
void speed_up_bridge2()
{
    set_speed_target(60);	
}
/*-----------------------------------------------------------------------*/
/* 减速过钢丝桥                                                          */
/* car 2                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void speed_down_bridge2()
{
	set_speed_target(0);
	delay_ms(2000);
	set_speed_target(15);	
}
/*-----------------------------------------------------------------------*/
/* 推箱子1_开始推                                                        */
/* car 3                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void push_box1()
{
	g_f_enable_mag_steer_control=0;
	set_steer_helm(STEER_HELM_CENTER);
	set_speed_target(15);	
}
/*-----------------------------------------------------------------------*/
/* 推箱子2_停止推_开始后退                                               */
/* car 3                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void push_box2()
{
	set_speed_target(-10);
	delay_ms(1000);
	set_speed_target(0);
	g_f_enable_mag_steer_control=1;
	set_speed_target(15);
}
/*-----------------------------------------------------------------------*/
/* 戳气球                                                                */
/* 传入RFID卡号                                                          */                                                          
/*-----------------------------------------------------------------------*/
void punctured_ballon(DWORD site)
{
	if (RFID_CARD_ID_PUNCTURED_BALLON_START == site)
	{
		set_speed_target(30);
	}
	else if (RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN1 == site)
	{
		g_f_enable_mag_steer_control = 0;
		set_steer_helm(STEER_HELM_RIGHT);
		set_speed_target(30);
		delay_ms(500);
		set_steer_helm(STEER_HELM_LEFT);
		while (!is_on_mag_line()) { }
		g_f_enable_mag_steer_control = 0;
	}
	else if (RFID_CARD_ID_PUNCTURED_BALLON_STOP == site)
	{
		set_speed_target(0);
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* 车1                                                                   */
/*-----------------------------------------------------------------------*/
void control_car_1_action(DWORD site)
{
	site;
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* 车2                                                                   */
/*-----------------------------------------------------------------------*/
void control_car_2_action(DWORD site)
{
	site;
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* 车3                                                                   */
/*-----------------------------------------------------------------------*/
void control_car_3_action(DWORD site)
{
	if (RFID_CARD_ID_1_2 == site)
	{
		g_f_enable_mag_steer_control = 0;
		set_steer_helm(STEER_HELM_RIGHT);
		set_speed_target(20);
		delay_ms(700);
		set_steer_helm(STEER_HELM_LEFT);
		delay_ms(900);
		set_steer_helm(STEER_HELM_CENTER);
		set_speed_target(10);
		g_f_enable_mag_steer_control = 1;
	}
	else if (RFID_CARD_ID_1_4 == site)
	{
		g_f_enable_mag_steer_control = 0;
		set_steer_helm(STEER_HELM_RIGHT);
		set_speed_target(20);
		delay_ms(700);
		set_steer_helm(STEER_HELM_LEFT);
		delay_ms(900);
		set_steer_helm(STEER_HELM_CENTER);
		set_speed_target(10);
		g_f_enable_mag_steer_control = 1;
	}
	else if (RFID_CARD_ID_2_1 == site)
	{
		//[implement][CAR_4]通知警车开始漂移
	}
	else if (RFID_CARD_ID_2_2 == site)
	{
		//[implement][CAR_4]通知吊桥升起
	}
	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_2]开始加速飞跃
		//[implement][CAR_3]开始加速飞跃
		//[implement][CAR_4]开始加速飞跃
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_2]脱离电磁线，找线
		//[implement][CAR_3]单边走
		//[implement][CAR_4]脱离电磁线，找线
	}
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_2]加速上钢丝桥
		//[implement][CAR_4]加速上钢丝桥
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_2]已上钢丝桥，减速
		//[implement][CAR_4]已上钢丝桥，减速，准备走钢丝
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_2]走钢丝结束
		//[implement][CAR_4]通知钢丝桥断开
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_2]减速下钢丝桥，准备躲箱子
		//[implement][CAR_4]减速下钢丝桥，通知[Car_3]推箱子
	}
	else if (RFID_CARD_ID_6_5 == site)
	{
		//[implement][CAR_4]脱离电磁线，转弯，找线
	}
	else if (RFID_CARD_ID_7_1 == site)
	{
		//[implement][CAR_4]即将入隧道，通知[CAR_1]漂移堵截
	}
	else if (RFID_CARD_ID_7_2 == site)
	{
		//[implement][CAR_4]停车
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* 车4                                                                   */
/*-----------------------------------------------------------------------*/
void control_car_4_action(DWORD site)
{
	if (RFID_CARD_ID_1_1 == site)
	{
		g_f_enable_mag_steer_control = 0;
		set_steer_helm(STEER_HELM_RIGHT);
		set_speed_target(20);
		delay_ms(700);
		set_steer_helm(STEER_HELM_LEFT);
		delay_ms(900);
		set_steer_helm(STEER_HELM_CENTER);
		set_speed_target(10);
		g_f_enable_mag_steer_control = 1;
	}
	else if (RFID_CARD_ID_1_4 == site)
	{
		g_f_enable_mag_steer_control = 0;
		set_steer_helm(STEER_HELM_RIGHT);
		set_speed_target(20);
		delay_ms(700);
		set_steer_helm(STEER_HELM_LEFT);
		delay_ms(900);
		set_steer_helm(STEER_HELM_CENTER);
		set_speed_target(10);
		g_f_enable_mag_steer_control = 1;
	}
	else if (RFID_CARD_ID_2_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]脱离电磁线，找新线
		//[implement][CAR_4]-->[CAR_1]开始漂移
		//[implement][CAR_4]脱离电磁线，找新线
	}
	else if (RFID_CARD_ID_2_2 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]通知吊桥升起
	}
	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]开始加速飞跃
		//[implement][CAR_3]开始加速飞跃
		//[implement][CAR_4]开始加速飞跃
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]脱离电磁线，找新线
		//[implement][CAR_3]准备单边走
		//[implement][CAR_4]脱离电磁线，找新线
	}
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]加速上钢丝桥
		//[implement][CAR_3]null
		//[implement][CAR_4]加速上钢丝桥
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]已上钢丝桥，减速
		//[implement][CAR_3]null
		//[implement][CAR_4]已上钢丝桥，减速，准备走钢丝
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]走钢丝结束
		//[implement][CAR_3]null
		//[implement][CAR_4]通知钢丝桥断开
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]减速下钢丝桥，准备躲箱子
		//[implement][CAR_3]null
		//[implement][CAR_4]减速下钢丝桥
		//[implement][CAR_4]-->[Car_3]推箱子
	}
	else if (RFID_CARD_ID_6_5 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]脱离电磁线，找新线
	}
	else if (RFID_CARD_ID_7_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]即将入隧道
		//[implement][CAR_4]-->[CAR_1]漂移堵截
	}
	else if (RFID_CARD_ID_7_2 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]停车
	}
	else if (RFID_CARD_ID_7_3 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]停车
		//[implement][CAR_4]null
	}
}