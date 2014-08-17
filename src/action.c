#define __ACTION_C_
#include "includes.h"
int flag_5_2=0;
int flag_c_2_2=0;
int flag_c_4_1=0;
int flag_c_4_2=0;
/*-----------------------------------------------------------------------*/
/* ������  2                                                              */
/* ��ѭ��  car 3 & 4                                                     */                                                          
/*-----------------------------------------------------------------------*/
void puncture_ballon()
{
	g_f_enable_mag_steer_control = 0;
	set_steer_helm(data_steer_helm.left_limit);
	set_speed_target(20);
	delay_ms(650);
	set_steer_helm(data_steer_helm.right_limit);
	set_speed_target(15);
	delay_ms(1400);
	set_steer_helm(data_steer_helm.center);
	set_speed_target(10);
	g_f_enable_mag_steer_control = 1;
}
/*-----------------------------------------------------------------------*/
/* ������ 1                                                               */
/* ��ѭ��  car 3 & 4                                                     */                                                          
/*-----------------------------------------------------------------------*/
void puncture_ballon_2()
{
	g_f_enable_mag_steer_control = 0;
	set_steer_helm(data_steer_helm.left_limit);
	set_speed_target(20);
	delay_ms(650);
	set_steer_helm(data_steer_helm.right_limit);
	set_speed_target(15);
	delay_ms(500);
	set_steer_helm(data_steer_helm.left_limit);
	delay_ms(500);
	set_steer_helm(data_steer_helm.center);
	set_speed_target(15);
	find_mag_back_box = 1;
}
/*-----------------------------------------------------------------------*/
/* �������ߡ���ת��������                                              */
/* ��ѭ��  car 3 & 4                                                     */                                                          
/*-----------------------------------------------------------------------*/
void turn_left_1()
{
		g_f_enable_mag_steer_control = 0;
		control_angle_steer_helm(45);
		set_speed_target(20);
		find_mag_back_box=1;
}
/*-----------------------------------------------------------------------*/
/* �ɼ��ٹ����ţ�ƽ��                                                                */
/* ����ѭ��  car 3                                               */                                                          
/*-----------------------------------------------------------------------*/
void speed_up_bridge1()
{
    //g_f_enable_mag_steer_control = 0;
    control_speed_motor(20);
//	g_f_enable_mag_steer_control = 1;

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
	g_f_enable_steer_bridge=1;
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
	g_f_enable_mag_steer_control=0;
	set_steer_helm(data_steer_helm.center);
	set_speed_target(-10);
	delay_ms(2500);
	set_speed_target(15);
	control_angle_steer_helm(20);
	find_mag_back_box=1;
}
/*-----------------------------------------------------------------------*/
/* ������	                                                             */
/* car 2                                                                 */                                                          
/*-----------------------------------------------------------------------*/
void avoid_box()
{
	g_f_enable_mag_steer_control=0;
	set_speed_target(10);
	control_angle_steer_helm(30);
	delay_ms(2000);
	g_f_enable_rad_control_2=0;
	set_speed_target(15);
	set_steer_helm(data_steer_helm.right_limit*0.85);
	find_mag_back_box=1;
}
/*-----------------------------------------------------------------------*/
/* ����	                                                             */
/* car 2,,4                                                                 */                                                          
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
	if (RFID_CARD_ID_2_4 == site)
	{
		//[implement][CAR_1]ִ��Ư��
		g_f_enable_mag_steer_control=0;
		drift_left(1);
	}	
	else if (RFID_CARD_ID_2_3 == site)
	{
		//[implement][CAR_1]ִ��Ư��
		g_f_enable_mag_steer_control=0;
		drift_left(2);
	}
	else if (RFID_CARD_ID_4_1 == site)
	{
		//[implement][CAR_1]ͣ��
		if(flag_c_4_1==0)
		{
			g_f_enable_mag_steer_control=0;
			set_steer_helm(0);
			set_speed_target(5);
			send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_4_1);
		}
		if(flag_c_4_1==1)
		{
			set_steer_helm(data_steer_helm.left_limit);
			set_speed_target(15);
			delay_ms(1000);
			g_f_enable_mag_steer_control=1;
			delay_ms(1500);
			g_f_enable_mag_steer_control=0;
			set_speed_target(0);
			delay_ms(1000);
			set_steer_helm(0);
			set_speed_target(-10);
			flag_c_4_1=2;
			
		}
		
	}
	else if (RFID_CARD_ID_4_2 == site)
	{
		//[implement][CAR_1]ͣ��
		if(flag_c_4_2==0)
		{
			flag_c_4_2=1;
			flag_c_4_1=1;
			set_speed_target(-5);
			return;
		}

		if(flag_c_4_2==1)
		{
			set_speed_target(0);
			flag_c_4_2=2;
			send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_4_2);
		}
	}
	else if (RFID_CARD_ID_4_3 == site)
	{
		//[implement][CAR_1]ִ��Ư��
		if(flag_c_4_2==2)
		{
			g_f_enable_mag_steer_control=0;
			drift_left(3);
		}
	}
}


/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/* RFID                                                                  */
/* ��2                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_2_action(DWORD site)
{
	int i;
	if (RFID_CARD_ID_2_2 == site)
	{
		//[implement][CAR_2]ͣ�µȴ��ĺų��ɹ�����
		if(flag_c_2_2==0)
		{
			set_speed_target(0);
			flag_c_2_2= 2;
		}
		if(flag_c_2_2==1)
		{
			g_f_enable_mag_steer_control=1;
			set_speed_target(15);
			flag_c_2_2= 1;
		}
	}
	    else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_2]��ʼ���ٷ�Ծ
		fly_bridge();
	}
	else if (RFID_CARD_ID_3_3 == site)
	{
		//����U
		set_speed_target(15);
		reset_rev_data();
		g_f_big_U=1;
		
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_2]�������ߣ�������
		g_f_big_U=0;
		turn_left_2();
		for(i=0;i<5;i++)
		send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_5_1_2);
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
		g_f_enable_steer_bridge=0;
		
		set_speed_target(5);	
	}
	else if (RFID_CARD_ID_6_4 == site)
	{
		//[implement][CAR_2]�����¸�˿�ţ�׼��������
		avoid_box();
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_6_3_2);
	}
	else if (RFID_CARD_ID_7_1== site)
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
	int i;
	if (RFID_CARD_ID_1_2 == site)//������
	{
		puncture_ballon();
	}
	else if (RFID_CARD_ID_1_4 == site)//������
	{
		puncture_ballon_2();
	}
	else if (RFID_CARD_ID_2_1 == site)
	{

		//[implement][CAR_3]�������ߣ�������
		turn_left_1();
	}

	else if (RFID_CARD_ID_3_1 == site)
	{
		g_f_enable_mag_steer_control = 0;
		set_speed_target(0);
		delay_ms(1000);		
		set_steer_helm((WORD)(data_steer_helm.right_limit));
		delay_ms(500);
		set_steer_helm((WORD)(data_steer_helm.left_limit));
		delay_ms(500);		
		set_steer_helm((WORD)(data_steer_helm.right_limit));
		delay_ms(500);
		set_steer_helm((WORD)(data_steer_helm.left_limit));
		delay_ms(1000);
		set_steer_helm((WORD)(data_steer_helm.center));
		delay_ms(500);
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_3_1);
		g_f_enable_mag_steer_control = 1;
		delay_ms(500);		
		//[implement][CAR_3]��ʼ���ٹ����ţ�ƽ��֪ͨ��1��������
		speed_up_bridge1();
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_1,WIFI_CMD_NET_3_1);
	}
	else if (RFID_CARD_ID_3_2 == site)
	{
		//��ָ��������
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_3_2);
		//[implement][CAR_3]-->[CAR_2]����
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_2,WIFI_CMD_NET_0_2);	
		reset_rev_data();
		set_speed_target(17);
		g_f_enable_speed_control_2=0;
		
	}
		else if (RFID_CARD_ID_3_3 == site)
	{
		//����U
		set_speed_target(12);
		reset_rev_data();
		g_f_big_U=1;
		
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_3]׼��������
		set_speed_target(10);
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_5_1);
		
	}
	else if (RFID_CARD_ID_5_5 == site)
	{
		//[implement][CAR_3]��ʼ������
		g_f_big_U=0;
		g_f_enable_single_bridge_control=1;
		control_speed_motor(40);
	}
	else if (RFID_CARD_ID_5_2 == site&&!flag_5_2)
	{
		//[implement][CAR_3]�����߽���
		//[implement][CAR_3]�ȴ�<--[CAR_4]������
		flag_5_2=1;
		g_f_enable_single_bridge_control=0;
		g_f_enable_speed_control_2=0;
		set_speed_target(0);
		for(i=0;i<5;i++)
		send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_5_2);
		
	}
	else if (RFID_CARD_ID_5_3 == site)
	{
		//[implement][CAR_3]�������
	//	push_box1();
	}
	else if (RFID_CARD_ID_5_4 == site)
	{	//[implement][CAR_3]-->[CAR_1]Ư�ƶ½�
		for(i=0;i<5;i++)
			{
				send_net_cmd(WIFI_ADDRESS_CAR_1,WIFI_CMD_NET_7_1);
				send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_7_1);
			}

		//[implement][CAR_3]�����յ�
		
		push_box2();
	}


	else if (RFID_CARD_ID_7_2 == site)
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
	else if (RFID_CARD_ID_1_3 == site)
	{
	//[implement][CAR_4]-->[CAR_1]����
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_1,WIFI_CMD_NET_0_1);
		puncture_ballon_2();
	
	}
	else if (RFID_CARD_ID_2_1 == site)
	{
	//[implement][CAR_4]�������ߣ�������
		turn_left_1();
	}
	else if (RFID_CARD_ID_2_2 == site)
	{
		g_f_enable_fly_bridge=1;
		if(flag_c_2_2) return;
		//[implement][CAR_4]ͣ�µȴ���������
		set_speed_target(0);
		flag_c_2_2= 1;

	}
	else if (RFID_CARD_ID_3_1 == site)
	{
		//[implement][CAR_4]��ʼ���ٷ�Ծ
		fly_bridge();
	}
	else if (RFID_CARD_ID_3_3 == site)
	{
		//����U
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_2,WIFI_CMD_NET_3_3);
		set_speed_target(15);
		reset_rev_data();
		g_f_big_U=1;
		
	}
	else if (RFID_CARD_ID_5_1 == site)
	{
		//[implement][CAR_4]�������ߣ�������
		g_f_big_U=0;
		turn_left_2();
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_DRAWBRIDGE,WIFI_CMD_NET_5_1_3);	
	}
	else if (RFID_CARD_ID_6_1 == site)
	{
		//[implement][CAR_4]�����ϸ�˿��
		speed_up_bridge2();
	}
	else if (RFID_CARD_ID_6_2 == site)
	{
		//[implement][CAR_4]���ϸ�˿�ţ�����
		g_f_enable_speed_control_2=0;
		set_speed_target(25);
	}
	else if (RFID_CARD_ID_6_3 == site)
	{
		//[implement][CAR_4]֪ͨ��˿�ŶϿ�������
		set_speed_target(10);
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_DRAHTBRIDGE,WIFI_CMD_NET_6_3);
	}
	else if (RFID_CARD_ID_6_4 == site)
	{

		//[implement][CAR_4]-->[Car_3]������
		for(i=0;i<5;i++)
			send_net_cmd(WIFI_ADDRESS_CAR_3,WIFI_CMD_NET_6_4);
		g_f_enable_mag_steer_control=0;
		delay_ms(1000);
		find_mag_back_box_2=1;
		control_angle_steer_helm(-180);
	}
/*	else if (RFID_CARD_ID_6_5 == site)
	{
		//[implement][CAR_4]�������ߣ�������(��ʱ��Ϊѭ��)
	}*/

	else if (RFID_CARD_ID_7_3 == site)
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
		set_speed_target(30);
	}
	else if (WIFI_CMD_NET_3_1 == cmd)
	{
		//[implement][CAR_1]����
		if(g_f_enable_rad_control_1==2)
		{
			g_f_enable_rad_control_1=1;
		}
		else
		{
			g_f_enable_mag_steer_control=1;
			set_speed_target(20);
		}
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
		set_steer_helm(0);
		g_f_enable_mag_steer_control=0;
		set_speed_target(20);
//		delay_ms(1000);
		g_f_enable_mag_steer_control=1;
		set_speed_target(20);
	}
	if (WIFI_CMD_NET_3_3 == cmd)
	{
		//[implement][CAR_2] ��c_2_2������ͣ��
		flag_c_2_2= 1;
		set_speed_target(15);
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
		delay_ms(2000);
		set_speed_target(20);
	}
	else if (WIFI_CMD_NET_6_4 == cmd)
	{
		//[implement][CAR_3]<--[CAR_4]������ ����
		g_f_enable_mag_steer_control=1;
		delay_ms(2000);
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
	else if(WIFI_CMD_NET_BRIDGE == cmd)
	{
		//��ʼ׼������
		g_f_enable_mag_steer_control=1;
		set_speed_target(15);
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