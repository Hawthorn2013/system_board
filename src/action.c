#define __ACTION_C_
#include "includes.h"
/*-----------------------------------------------------------------------*/
/* ������                                                                */
/* ��ѭ��  car 3 & 4                                                     */                                                          
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
/* �������ߡ���ת��������                                              */
/* ��ѭ��  car 3 & 4                                                     */                                                          
/*-----------------------------------------------------------------------*/
void turn_left_1()
{
		g_f_enable_mag_steer_control = 0;
		control_angle_steer_helm(90);
		set_speed_target(20);
		delay_ms(1000);
		g_f_enable_rad_control_2=0;
		g_f_enable_mag_steer_control = 1;
}
/*-----------------------------------------------------------------------*/
/* �ɵ���                                                                */
/* ����ѭ��  car 2 & 3 & 4                                               */                                                          
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
/* �������ߡ�������                                                    */
/* ��ѭ��  car 2 & 4                                                     */                                                          
/*-----------------------------------------------------------------------*/
void turn_left_2()
{
	
	g_f_enable_mag_steer_control = 0;
	set_steer_helm(data_steer_helm.center);
	set_speed_target(10);
	delay_ms(1500);
	control_angle_steer_helm(30);
	delay_ms(1000);
	g_f_enable_rad_control_2=0;
	g_f_enable_mag_steer_control = 1;
}
/*-----------------------------------------------------------------------*/
/* �ϸ�˿��                                                              */
/* car 2 & 4                                                             */                                                          
/*-----------------------------------------------------------------------*/
void speed_up_bridge2()
{
    control_speed_motor(15);	
}
/*-----------------------------------------------------------------------*/
/* ���ٹ���˿��                                                          */
/* car 2                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void speed_down_bridge2()
{
	set_speed_target(0);
	g_f_enable_speed_control_2=0;
	delay_ms(2000);
	set_speed_target(15);	
}
/*-----------------------------------------------------------------------*/
/* ������1_��ʼ��                                                        */
/* car 3                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void push_box1()
{
	g_f_enable_mag_steer_control=0;
	set_steer_helm(data_steer_helm.center);
	set_speed_target(15);	
}
/*-----------------------------------------------------------------------*/
/* ������2_ֹͣ��_��ʼ����                                               */
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
/* ������	                                                             */
/* car 2                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void avoid_box()
{
	g_f_enable_mag_steer_control=0;
	set_speed_target(5);
	control_angle_steer_helm(30);
	delay_ms(2000);
	g_f_enable_rad_control_2=0;
	g_f_enable_mag_steer_control=1;
	set_speed_target(15);
}
/*-----------------------------------------------------------------------*/
/* ����	                                                             */
/* car 2,3,4                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void fly_bridge()
{
	g_f_enable_mag_steer_control=1;
	g_f_enable_fly_bridge = 1;
	control_speed_motor(150);
}

/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* RFID                                                                  */
/* ��1                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_1_action(DWORD site)
{
	if (RFID_CARD_ID_2_3 == site)
	{
		//[implement][CAR_1]ִ��Ư��
		g_f_enable_mag_steer_control=0;
		drift_left();
		g_f_enable_mag_steer_control=1;
	}
	else if (RFID_CARD_ID_4_1 == site)
	{
		//[implement][CAR_1]ͣ��
		set_speed_target(0);
	}
	else if (RFID_CARD_ID_4_2 == site)
	{
		//[implement][CAR_1]ִ��Ư��
		g_f_enable_mag_steer_control=0;
		drift_left();
		g_f_enable_mag_steer_control=1;
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* RFID                                                                  */
/* ��2                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_2_action(DWORD site)
{

	if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_2]��ʼ���ٷ�Ծ
		fly_bridge();
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_2]�������ߣ�������
		turn_left_2();
	}
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_2]�����ϸ�˿��
		speed_up_bridge2();
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_2]���ϸ�˿�ţ����٣�׼���߸�˿
		speed_down_bridge2();
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_2]�߸�˿����
		set_speed_target(5);	
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_2]�����¸�˿�ţ�׼��������
		avoid_box();
	}
	else if (RFID_CARD_ID_7_4 == site)
	{
		//[implement][CAR_2]ͣ��
		set_speed_target(0);
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* RFID                                                                  */
/* ��3                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_3_action(DWORD site)
{
	if (RFID_CARD_ID_1_2 == site)//������
	{
		puncture_ballon();
	}
	else if (RFID_CARD_ID_1_4 == site)//������
	{
		puncture_ballon();
	}
	else if (RFID_CARD_ID_2_1 == site)
	{

		//[implement][CAR_3]�������ߣ�������
		turn_left_1();
	}

	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_3]��ʼ���ٷ�Ծ
		speed_up_bridge1();
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_3]׼��������
		set_speed_target(10);
	}
	else if (RFID_CARD_ID_5_5 == site)
	{
		//[implement][CAR_3]��ʼ������
		set_speed_target(0);
		delay_ms(1500);
		set_speed_target(35);
	}
	else if (RFID_CARD_ID_5_2 == site)
	{
		//[implement][CAR_3]�����߽���
		//[implement][CAR_3]�ȴ�<--[CAR_4]������
		set_speed_target(0);

	}
	else if (RFID_CARD_ID_5_3 == site)
	{
		//[implement][CAR_3]�������
		push_box1();
	}
	else if (RFID_CARD_ID_5_4 == site)
	{
		//[implement][CAR_3]�����յ�
		push_box2();
	}


	else if (RFID_CARD_ID_7_3 == site)
	{
		//[implement][CAR_3]���ͣ��
		set_speed_target(0);
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* RFID                                                                  */
/* ��4                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_4_action(DWORD site)
{
	int i;
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
		//[implement][CAR_4]-->[CAR_1]��ʼƯ��
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_1,WIFI_CMD_NET_0_1);
		//[implement][CAR_4]-->[CAR_2]����
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_2,WIFI_CMD_NET_0_2);
		//[implement][CAR_4]�������ߣ�������
		turn_left_1();
		
	}
	else if (RFID_CARD_ID_2_2 == site)
	{

		//[implement][CAR_4]֪ͨ��������
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_2_2);

	}
	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_4]��ʼ���ٷ�Ծ
		speed_up_bridge1();
	}

	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_4]�������ߣ�������
		turn_left_2();
	}


	
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_4]�����ϸ�˿��
		speed_up_bridge2();
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_4]���ϸ�˿�ţ�����
		set_speed_target(25);
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_4]֪ͨ��˿�ŶϿ�������
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_DRAHTBRIDGE,WIFI_CMD_NET_6_3);
		set_speed_target(10);
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		
		//[implement][CAR_4]-->[Car_3]������
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_3,WIFI_CMD_NET_6_4);
	}
	else if (RFID_CARD_ID_6_5 == site)
	{
		//[implement][CAR_4]�������ߣ�������(��ʱ��Ϊѭ��)
	}
	else if (RFID_CARD_ID_7_1 == site)
	{
		//[implement][CAR_4]-->[CAR_1]Ư�ƶ½�
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_3,WIFI_CMD_NET_7_1);
	}
	else if (RFID_CARD_ID_7_2 == site)
	{
		//[implement][CAR_4]ͣ��
		set_speed_target(0);
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* WiFi                                                                  */
/* ��1                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_1_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_1 == cmd)
	{
		//[implement][CAR_1]������׼��Ư��
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
	else if (WIFI_CMD_NET_7_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_4]������׼��Ư�� ��
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* WiFi                                                                  */
/* ��2                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_2_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_2 == cmd)
	{
		//[implement][CAR_2]����
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}

}
/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* WiFi                                                                  */
/* ��3                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_3_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_3 == cmd)
	{
		//[implement][CAR_3]����
		//[implement][CAR_3]����
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
	else if (WIFI_CMD_NET_6_4 == cmd)
	{
		//[implement][CAR_3]<--[CAR_4]������ ����
		g_f_enable_mag_steer_control=1;
		set_speed_target(15);
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* WiFi                                                                  */
/* ��4                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_4_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_4 == cmd)
	{
		//[implement][CAR_4]����
		//[implement][CAR_4]����
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* �������                                                              */
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