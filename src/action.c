#define __ACTION_C_
#include "includes.h"
/*-----------------------------------------------------------------------*/
/* 扎气球                                                                */
/* 关循迹  car 3 & 4                                                     */                                                          
/*-----------------------------------------------------------------------*/
void puncture_ballon()
{
	g_f_enable_mag_steer_control = 0;
	set_steer_helm(data_steer_helm.left_limit);
	set_speed_target(20);
	delay_ms(700);
	set_steer_helm(data_steer_helm.right_limit);
	delay_ms(900);
	set_steer_helm(data_steer_helm.center);
	set_speed_target(10);
	g_f_enable_mag_steer_control = 1;
}
/*-----------------------------------------------------------------------*/
/* 脱离电磁线、左转、找新线                                              */
/* 关循迹  car 3 & 4                                                     */                                                          
/*-----------------------------------------------------------------------*/
void turn_left_1()
{
		g_f_enable_mag_steer_control = 0;
		set_steer_helm(data_steer_helm.left_limit);
		delay_ms(800);
		set_steer_helm(data_steer_helm.center);
		g_f_enable_mag_steer_control = 1;
}
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
/* 脱离电磁线、找新线                                                    */
/* 关循迹  car 2 & 4                                                     */                                                          
/*-----------------------------------------------------------------------*/
void turn_left_2()
{
	
	g_f_enable_mag_steer_control = 0;
	set_steer_helm(data_steer_helm.center);
	set_speed_target(10);
	delay_ms(1000);
	g_f_enable_mag_steer_control = 1;
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
	set_steer_helm(data_steer_helm.center);
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
/* 躲箱子	                                                             */
/* car 2                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void avoid_box()
{
	g_f_enable_mag_steer_control=0;
	set_speed_target(5);
	set_steer_helm(data_steer_helm.center-200);
	delay_ms(1500);
	set_steer_helm(data_steer_helm.center);
	delay_ms(1000);
	set_steer_helm(data_steer_helm.center+200);
	delay_ms(1500);
	g_f_enable_mag_steer_control=1;
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车1                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_1_action(DWORD site)
{
	if (RFID_CARD_ID_2_3 == site)
	{
		//[implement][CAR_1]执行漂移
		g_f_enable_mag_steer_control=0;
		drift_left();
		g_f_enable_mag_steer_control=1;
	}
	else if (RFID_CARD_ID_4_1 == site)
	{
		//[implement][CAR_1]停车
		set_speed_target(0);
	}
	else if (RFID_CARD_ID_4_2 == site)
	{
		//[implement][CAR_1]执行漂移
		g_f_enable_mag_steer_control=0;
		drift_left();
		g_f_enable_mag_steer_control=1;
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车2                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_2_action(DWORD site)
{

	if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_2]开始加速飞跃
		speed_up_bridge1();
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_2]脱离电磁线，找新线
		turn_left_2();
	}
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_2]加速上钢丝桥
		speed_up_bridge2();
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_2]已上钢丝桥，减速，准备走钢丝
		speed_down_bridge2();
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_2]走钢丝结束
		
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_2]减速下钢丝桥，准备躲箱子
		avoid_box();
	}
	else if (RFID_CARD_ID_7_4 == site)
	{
		//[implement][CAR_2]停车
		set_speed_target(0);
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车3                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_3_action(DWORD site)
{
	if (RFID_CARD_ID_1_2 == site)//扎气球
	{
		puncture_ballon();
	}
	else if (RFID_CARD_ID_1_4 == site)//扎气球
	{
		puncture_ballon();
	}
	else if (RFID_CARD_ID_2_1 == site)
	{

		//[implement][CAR_3]脱离电磁线，找新线
		turn_left_1();
	}

	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_3]开始加速飞跃
		speed_up_bridge1();
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_3]准备单边走
		set_speed_target(10);
	}
	else if (RFID_CARD_ID_5_5 == site)
	{
		//[implement][CAR_3]开始单边走
		set_speed_target(0);
		delay_ms(1500);
		set_speed_target(35);
	}
	else if (RFID_CARD_ID_5_2 == site)
	{
		//[implement][CAR_3]单边走结束
		//[implement][CAR_3]等待<--[CAR_4]推箱子
		set_speed_target(0);

	}
	else if (RFID_CARD_ID_5_3 == site)
	{
		//[implement][CAR_3]箱子起点
		push_box1();
	}
	else if (RFID_CARD_ID_5_4 == site)
	{
		//[implement][CAR_3]箱子终点
		push_box2();
	}


	else if (RFID_CARD_ID_7_3 == site)
	{
		//[implement][CAR_3]隧道停车
		set_speed_target(0);
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车4                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_4_action(DWORD site)
{
	if (RFID_CARD_ID_1_1 == site)
	{
		puncture_ballon();
	}
	else if (RFID_CARD_ID_1_4 == site)
	{
		puncture_ballon();
	}
	else if (RFID_CARD_ID_2_1 == site)
	{
		//[implement][CAR_4]-->[CAR_1]开始漂移
		send_net_cmd(WIFI_ADDRESS_CAR_1,WIFI_CMD_NET_0_1);
		//[implement][CAR_4]-->[CAR_2]出发
		send_net_cmd(WIFI_ADDRESS_CAR_2,WIFI_CMD_NET_0_2);
		//[implement][CAR_4]脱离电磁线，找新线
		turn_left_1();
		
	}
	else if (RFID_CARD_ID_2_2 == site)
	{

		//[implement][CAR_4]通知吊桥升起
		send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_2_2);

	}
	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_4]开始加速飞跃
		speed_up_bridge1();
	}

	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_4]脱离电磁线，找新线
		turn_left_2();
	}


	
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_4]加速上钢丝桥
		speed_up_bridge2();
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_4]已上钢丝桥，减速
		set_speed_target(25);
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_4]通知钢丝桥断开、减速
		send_net_cmd(WIFI_ADDRESS_DRAHTBRIDGE,WIFI_CMD_NET_6_3);
		set_speed_target(10);
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_4]-->[Car_3]推箱子
		send_net_cmd(WIFI_ADDRESS_CAR_3,WIFI_CMD_NET_6_4);
	}
	else if (RFID_CARD_ID_6_5 == site)
	{
		//[implement][CAR_4]脱离电磁线，找新线(暂时定为循迹)
	}
	else if (RFID_CARD_ID_7_1 == site)
	{
		//[implement][CAR_4]-->[CAR_1]漂移堵截
		send_net_cmd(WIFI_ADDRESS_CAR_3,WIFI_CMD_NET_7_1);
	}
	else if (RFID_CARD_ID_7_2 == site)
	{
		//[implement][CAR_4]停车
		set_speed_target(0);
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车1                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_1_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_1 == cmd)
	{
		//[implement][CAR_1]启动、准备漂移
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
	else if (WIFI_CMD_NET_7_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_4]启动、准备漂移 堵
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车2                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_2_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_2 == cmd)
	{
		//[implement][CAR_2]启动
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}

}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车3                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_3_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_3 == cmd)
	{
		//[implement][CAR_3]启动
		//[implement][CAR_3]出发
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
	else if (WIFI_CMD_NET_6_4 == cmd)
	{
		//[implement][CAR_3]<--[CAR_4]推箱子 启动
		g_f_enable_mag_steer_control=1;
		set_speed_target(15);
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车4                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_4_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_4 == cmd)
	{
		//[implement][CAR_4]启动
		//[implement][CAR_4]出发
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
}


/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* 负责分配                                                              */
/*-----------------------------------------------------------------------*/
void control_car_action(void)
{
	if (WIFI_ADDRESS_CAR_4 == g_device_NO)
	{
		if (RFID_site_data.is_new_site)
		{
			RFID_site_data.is_new_site = 0;
			
			RFID_control_car_4_action(RFID_site_data.site);
		}
		if (g_net_control_data.is_new_cmd)
		{
			g_net_control_data.is_new_cmd = 0;
			
			WiFi_control_car_4_action(g_net_control_data.cmd);
		}
	}
	else if (WIFI_ADDRESS_CAR_3 == g_device_NO)
	{
		if (RFID_site_data.is_new_site)
		{
			RFID_site_data.is_new_site = 0;
			
			RFID_control_car_3_action(RFID_site_data.site);
		}
		if (g_net_control_data.is_new_cmd)
		{
			g_net_control_data.is_new_cmd = 0;
			
			WiFi_control_car_3_action(g_net_control_data.cmd);
		}
	}
	else if (WIFI_ADDRESS_CAR_2 == g_device_NO)
	{
		if (RFID_site_data.is_new_site)
		{
			RFID_site_data.is_new_site = 0;
			
			RFID_control_car_2_action(RFID_site_data.site);
		}
		if (g_net_control_data.is_new_cmd)
		{
			g_net_control_data.is_new_cmd = 0;
			
			WiFi_control_car_2_action(g_net_control_data.cmd);
		}
	}
	else if (WIFI_ADDRESS_CAR_1 == g_device_NO)
	{
		if (RFID_site_data.is_new_site)
		{
			RFID_site_data.is_new_site = 0;
			
			RFID_control_car_1_action(RFID_site_data.site);
		}
		if (g_net_control_data.is_new_cmd)
		{
			g_net_control_data.is_new_cmd = 0;
			
			WiFi_control_car_1_action(g_net_control_data.cmd);
		}
	}
}