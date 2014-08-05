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
	set_steer_helm(STEER_HELM_CENTER);
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
/* ��1                                                                   */
/*-----------------------------------------------------------------------*/
void control_car_1_action(DWORD site)
{
	site;
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* ��2                                                                   */
/*-----------------------------------------------------------------------*/
void control_car_2_action(DWORD site)
{
	site;
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* ��3                                                                   */
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
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* ��4                                                                   */
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
		//[implement]֪ͨ������ʼƯ��
	}
	else if (RFID_CARD_ID_2_2 == site)
	{
		//[implement]֪ͨ��������
	}
	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement]��ʼ���ٷ�Ծ
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_2]�������ߣ�����
		//[implement][CAR_3]������
		//[implement][CAR_4]�������ߣ�����
	}
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_2]�����ϸ�˿��
		//[implement][CAR_4]�����ϸ�˿��
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_2]���ϸ�˿�ţ�����
		//[implement][CAR_4]���ϸ�˿�ţ����٣�׼���߸�˿
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_2]�߸�˿����
		//[implement][CAR_4]֪ͨ��˿�ŶϿ�
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_2]�����¸�˿�ţ�׼��������
		//[implement][CAR_4]�����¸�˿�ţ�֪ͨ[Car_3]������
	}
	else if (RFID_CARD_ID_6_5 == site)
	{
		//[implement][CAR_4]�������ߣ�ת�䣬����
	}
	else if (RFID_CARD_ID_7_1 == site)
	{
		//[implement][CAR_4]�����������֪ͨ[CAR_1]Ư�ƶ½�
	}
	else if (RFID_CARD_ID_7_2 == site)
	{
		//[implement][CAR_4]ͣ��
	}
}