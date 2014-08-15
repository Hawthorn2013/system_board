#define __WIFI_C_
#include "includes.h"


int g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
int g_remote_frame_cnt = 0;
int g_start_all=0;
BYTE remote_frame_data[REMOTE_FRAME_LENGTH];
BYTE remote_frame_data_send[REMOTE_FRAME_LENGTH];
BYTE g_device_NO = WIFI_ADDRESS_CAR_1;	/* �豸�� ��WiFi��ַ */


/*-----------------------------------------------------------------------*/
/* ִ��Զ������                                                                      */
/*-----------------------------------------------------------------------*/
void execute_remote_cmd(const BYTE *data)
{
	WORD cmd = 0;
	
	cmd = ((WORD)(data[0])<<8) | ((WORD)(data[1]));
	switch (cmd)
	{
		/* ������� */
		case WIFI_CMD_SET_HELM_TARGET :
		set_steer_helm_basement(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_HELM_KP :
		break;
		case WIFI_CMD_SET_HELM_KI :
		break;
		case WIFI_CMD_SET_HELM_KD :
		break;
		case WIFI_CMD_SET_STEER_HELM_CENTER :
		set_steer_helm_basement_center(*((WORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_STEER_HELM_LEFT :
		set_steer_helm_basement_left_limit(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_STEER_HELM_RIGHT :
		set_steer_helm_basement_right_limit(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_WRITE_STEER_HELM_DATA_TO_TF :
		if (!update_steer_helm_basement_to_steer_helm())
		{
			SWORD tmp_read_rad_xyz = (SWORD)read_rad_xyz;	/* �ݴ������Ƕ�ȡ��־λ */
			
			read_rad_xyz = 0;	/* �ݴ���������Ƕ�ȡ����ֹ����TF����д */
			write_steer_helm_data_to_TF();	/* ��Լ60ms */
			read_rad_xyz = tmp_read_rad_xyz;	/* �ָ������Ƕ�ȡ��־λ */
		}
		else
		{
			//������ݲ�����
		}
		break;
		case WIFI_CMD_SEND_STEER_HELM_DATA_FROM_TF :
		generate_remote_frame_2(g_device_NO, ANDROID_ADDRESS, WIFI_CMD_SEND_STEER_HELM_DATA_FROM_TF, sizeof(data_steer_helm_basement), (const BYTE *)&data_steer_helm_basement);
		break;
		
		
		/* ������� */
		case WIFI_CMD_SET_MOTOR_TARGET :
		set_speed_target(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_MOTOR_KP :
		set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_MOTOR_KI :
		set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_MOTOR_KD :
		set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		
		/* ������ */
		case WIFI_CMD_GET_GYRO_DATA :
		g_remote_control_flags.send_gyro_data = 1;
		break;
		case WIFI_CMD_UNGET_GYRO_DATA :
		g_remote_control_flags.send_gyro_data = 0;
		break;
		
		case WIFI_CMD_GET_SEEED_NOW :
		g_remote_control_flags.send_seppd_now = 1;
		break;
		case WIFI_CMD_UNGET_SPEED_NOW :
		g_remote_control_flags.send_seppd_now = 0;
		break;
		
		case WIFI_CMD_NET :
		g_net_control_data.is_new_cmd = 1;
		g_net_control_data.cmd = *((WORD *)(&(data[2])));
		break;
		/*��ʼȫ������*/
		case WIFI_CMD_START_ACTIVE:
		
		g_start_all=1;
		break;
	}
}


/*-----------------------------------------------------------------------*/
/* ����Զ������֡                                                        */
/* �ڶ���                                                                */
/* �޸ĵ�ַλ����:Դ��ַ(1B)+Ŀ�ĵ�ַ(1B)                                */
/* AA BB Դ��ַ(1B) Ŀ�ĵ�ַ(1B) ����(1B) ������ У����(1B)              */
/* δ�ı�ԭ����֡�Ĵ󲿷ֶ��弰����                                      */
/*-----------------------------------------------------------------------*/
int rev_remote_frame_2(BYTE rev)
{
	if (g_remote_frame_cnt == 0)	//����֡ͷ
	{
		if (rev == 0xAA)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0xAA;
		}
	}
	else if (g_remote_frame_cnt == 1)	//����֡ͷ
	{
		if (rev == 0xBB)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0xBB;
		}
		else
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt == 2)	//����Դ��ַ
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt == 3)	//����Ŀ�ĵ�ַ
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
		if (rev != g_device_NO && rev != 0xFF)
		{
			g_remote_frame_cnt = 0;	//���Ƿ���������
		}
	}
	else if (g_remote_frame_cnt == 4)	//���ճ���
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
		if (rev+6>REMOTE_FRAME_LENGTH)	//�ж��Ƿ�ᵼ�»��������
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt>4 && g_remote_frame_cnt<=remote_frame_data[4]+4)	//����������
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt==remote_frame_data[4]+4+1)	//����У���ֽ�	
	{
		BYTE sum;
		remote_frame_data[g_remote_frame_cnt++] = rev;
		sum = check_sum((const BYTE *)(remote_frame_data+2), (WORD)(remote_frame_data[4]+3));
		if (sum != remote_frame_data[remote_frame_data[4]+5])
		{
			g_remote_frame_cnt = 0;	//CheckSum Fail
		}
		else
		{
			g_remote_frame_cnt = 0;
			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
		}
	}
	
	return g_remote_frame_state;
}


/*-----------------------------------------------------------------------*/
/* ���ɲ�����Զ������                                                    */
/* �ڶ���                                                                */
/* �޸ĵ�ַλ����:Դ��ַ(1B)+Ŀ�ĵ�ַ(1B)                                */
/* AA BB Դ��ַ(1B) Ŀ�ĵ�ַ(1B) ����(1B) ������ У����(1B)              */
/* δ�ı�ԭ����֡�Ĵ󲿷ֶ��弰����                                      */
/* ���� : scr Դ��ַ                                                     */
/*        des Ŀ���ַ                                                   */
/*        cmd WiFi������                                                 */
/*        length data����                                                */
/*        data�����������壬����cmd��                                    */
/*-----------------------------------------------------------------------*/
void generate_remote_frame_2(BYTE scr, BYTE des, WORD cmd, BYTE length, const BYTE data[])
{
	WORD i = 0, j = 0;
	remote_frame_data_send[i++] = 0xAA;
	remote_frame_data_send[i++] = 0xBB;
	remote_frame_data_send[i++] = scr;
	remote_frame_data_send[i++] = des;
	remote_frame_data_send[i++] = length+2;
	remote_frame_data_send[i++] = (BYTE)(cmd>>8);
	remote_frame_data_send[i++] = (BYTE)cmd;
	for (j = 0; j < length; j++)
	{
		remote_frame_data_send[i++] = data[j];
	}
	remote_frame_data_send[i++] = check_sum(remote_frame_data_send+2, i-2);
	for (; i < REMOTE_FRAME_LENGTH; i++)	/* ���δʹ������ */
	{
		remote_frame_data_send[i] = 0x00;
	}
	serial_port_0_TX_array(remote_frame_data_send, REMOTE_FRAME_LENGTH);
}


/*-----------------------------------------------------------------------*/
/* �������������������                                                          */
/* ����generate_remote_frame_2()                                          */
/*-----------------------------------------------------------------------*/
void send_net_cmd(BYTE des, WORD net_cmd)
{
	generate_remote_frame_2(g_device_NO, des, WIFI_CMD_NET, sizeof(net_cmd), (const BYTE *)(&net_cmd));
}


/*-----------------------------------------------------------------------*/
/* ���У��                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_sum(const BYTE *data, WORD length)
{
	int i;
	BYTE res = 0;
	
	for (i=0; i<length; i++)
	{
		res ^= data[i];
	}
	
	return res;
}


/*-----------------------------------------------------------------------*/
/* ��������                                                                            */
/*-----------------------------------------------------------------------*/
void report_online(void)
{
	WORD online = WIFI_CMD_NET_ONLINE;
	
	if (200 < diff_time_basis_PIT(g_time_basis_PIT, g_net_control_data.last_report_online_time))	/* 2�뱨��һ�� */
	{
		generate_remote_frame_2(g_device_NO, WIFI_ADDRESS_BROADCAST, WIFI_CMD_NET, sizeof(online), (const BYTE *)(&online));
		g_net_control_data.last_report_online_time = g_time_basis_PIT;
	}
	
}