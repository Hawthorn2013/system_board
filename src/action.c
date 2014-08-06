#define __ACTION_C_
#include "includes.h"


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
/* �ϸ�˿��                                                              */
/* car 2 & 4                                                             */                                                          
/*-----------------------------------------------------------------------*/
void speed_up_bridge2()
{
    set_speed_target(60);	
}
/*-----------------------------------------------------------------------*/
/* ���ٹ���˿��                                                          */
/* car 2                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void speed_down_bridge2()
{
	set_speed_target(0);
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
/* ������                                                                */
/* ����RFID����                                                          */                                                          
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
/* ������������                                                          */
/* RFID                                                                  */
/* ��1                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_1_action(DWORD site)
{
	if (RFID_CARD_ID_2_3 == site)
	{
		//[implement][CAR_1]ִ��Ư��
		drift_left();
	}
	else if (RFID_CARD_ID_4_1 == site)
	{
		//[implement][CAR_1]ͣ��
		set_speed_target(0);
	}
	else if (RFID_CARD_ID_4_2 == site)
	{
		//[implement][CAR_1]ִ��Ư��
		drift_left();
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
		speed_up_bridge1();
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_2]�������ߣ�������
		g_f_enable_mag_steer_control = 1;
		set_speed_target(10);
		delay_ms(1000);
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
		
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]�����¸�˿�ţ�׼��������
		//[implement][CAR_3]null
		//[implement][CAR_4]�����¸�˿��
		//[implement][CAR_4]-->[Car_3]������
	}
	else if (RFID_CARD_ID_6_5 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]�������ߣ�������
	}
	else if (RFID_CARD_ID_7_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]���������
		//[implement][CAR_4]-->[CAR_1]Ư�ƶ½�
	}
	else if (RFID_CARD_ID_7_2 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]ͣ��
	}
	else if (RFID_CARD_ID_7_3 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]ͣ��
		//[implement][CAR_4]null
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
	else if (RFID_CARD_ID_1_4 == site)//������
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
		//[implement][CAR_3]�������ߣ�������
		//[implement][CAR_4]-->[CAR_1]��ʼƯ��
		//[implement][CAR_4]-->[CAR_2]����
		//[implement][CAR_4]�������ߣ�������
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
		//[implement][CAR_3]�������ߣ�������
		//[implement][CAR_4]-->[CAR_1]��ʼƯ��
		//[implement][CAR_4]-->[CAR_2]����
		//[implement][CAR_4]�������ߣ�������
	}
	else if (RFID_CARD_ID_2_2 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]֪ͨ��������
	}
	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]��ʼ���ٷ�Ծ
		//[implement][CAR_3]��ʼ���ٷ�Ծ
		//[implement][CAR_4]��ʼ���ٷ�Ծ
	}
	else if (RFID_CARD_ID_4_1 == site)
	{
		//[implement][CAR_1]��ά����ֹͣ
		//[implement][CAR_1]�ȴ�<--[CAR_4]Ư�ƶ������
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]�������ߣ�������
		//[implement][CAR_3]׼��������
		//[implement][CAR_4]�������ߣ�������
	}
	else if (RFID_CARD_ID_5_2 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]�����߽���
		//[implement][CAR_3]�ȴ�<--[CAR_4]������
		//[implement][CAR_4]�������ߣ�������
	}
	else if (RFID_CARD_ID_5_3 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]�������
		//[implement][CAR_4]null
	}
	else if (RFID_CARD_ID_5_4 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]�����յ�
		//[implement][CAR_4]null
	}
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]�����ϸ�˿��
		//[implement][CAR_3]null
		//[implement][CAR_4]�����ϸ�˿��
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]���ϸ�˿�ţ�����
		//[implement][CAR_3]null
		//[implement][CAR_4]���ϸ�˿�ţ����٣�׼���߸�˿
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]�߸�˿����
		//[implement][CAR_3]null
		//[implement][CAR_4]֪ͨ��˿�ŶϿ�
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]�����¸�˿�ţ�׼��������
		//[implement][CAR_3]null
		//[implement][CAR_4]�����¸�˿��
		//[implement][CAR_4]-->[Car_3]������
	}
	else if (RFID_CARD_ID_6_5 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]�������ߣ�������
	}
	else if (RFID_CARD_ID_7_1 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]���������
		//[implement][CAR_4]-->[CAR_1]Ư�ƶ½�
	}
	else if (RFID_CARD_ID_7_2 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]ͣ��
	}
	else if (RFID_CARD_ID_7_3 == site)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]ͣ��
		//[implement][CAR_4]null
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
		//[implement][CAR_1]����
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]����
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_3 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]����
		//[implement][CAR_3]����
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_4 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]����
		//[implement][CAR_4]����
	}
	else if (WIFI_CMD_NET_2_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_3]��ʼƯ��
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_2_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_6_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_6_4 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]<--[CAR_3]������
	}
	else if (WIFI_CMD_NET_7_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_4]Ư�ƶ�
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* WiFi                                                                  */
/* ��2                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_2_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_1 == cmd)
	{
		//[implement][CAR_1]����
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]����
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_3 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]����
		//[implement][CAR_3]����
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_4 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]����
		//[implement][CAR_4]����
	}
	else if (WIFI_CMD_NET_2_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_3]��ʼƯ��
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_2_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_6_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_6_4 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]<--[CAR_3]������
	}
	else if (WIFI_CMD_NET_7_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_4]Ư�ƶ�
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* WiFi                                                                  */
/* ��3                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_3_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_1 == cmd)
	{
		//[implement][CAR_1]����
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]����
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_3 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]����
		//[implement][CAR_3]����
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_4 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]����
		//[implement][CAR_4]����
	}
	else if (WIFI_CMD_NET_2_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_3]��ʼƯ��
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_2_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_6_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_6_4 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]<--[CAR_3]������
	}
	else if (WIFI_CMD_NET_7_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_4]Ư�ƶ�
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* WiFi                                                                  */
/* ��4                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_4_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_1 == cmd)
	{
		//[implement][CAR_1]����
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]����
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_3 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]����
		//[implement][CAR_3]����
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_0_4 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]����
		//[implement][CAR_4]����
	}
	else if (WIFI_CMD_NET_2_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_3]��ʼƯ��
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_2_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_6_2 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
	}
	else if (WIFI_CMD_NET_6_4 == cmd)
	{
		//[implement][CAR_1]null
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]<--[CAR_3]������
	}
	else if (WIFI_CMD_NET_7_1 == cmd)
	{
		//[implement][CAR_1]<--[CAR_4]Ư�ƶ�
		//[implement][CAR_2]null
		//[implement][CAR_3]null
		//[implement][CAR_4]null
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