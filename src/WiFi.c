#define __WIFI_C_
#include "includes.h"


int g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
int g_remote_frame_cnt = 0;
BYTE remote_frame_data[REMOTE_FRAME_LENGTH_MAX];
BYTE remote_frame_data_send[REMOTE_FRAME_LENGTH_MAX];


/*-----------------------------------------------------------------------*/
/* 执行远程命令                                                          */
/*-----------------------------------------------------------------------*/
void execute_remote_cmd(const BYTE *data)
{
	WORD cmd = 0;
	
	cmd = ((WORD)(data[0])<<8) | ((WORD)(data[1]));
	switch (cmd)
	{
		/* 舵机调参 */
		case WIFI_CMD_SET_HELM_TARGET :
		set_steer_helm(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_HELM_KP :
		break;
		case WIFI_CMD_SET_HELM_KI :
		break;
		case WIFI_CMD_SET_HELM_KD :
		break;
		
		/* 电机调参 */
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
		
		/* 陀螺仪 */
		case WIFI_CMD_GET_GYRO_DATA :
		g_remote_control_flags.send_gyro_data = 1;
		break;
		case WIFI_CMD_UNGET_GYRO_DATA :
		g_remote_control_flags.send_gyro_data = 0;
		break;
	}
}


/*-----------------------------------------------------------------------*/
/* 接受远程数据帧                                                      */
/*-----------------------------------------------------------------------*/
int rev_remote_frame(BYTE rev)
{
	if (g_remote_frame_cnt == 0)	//接收帧头
	{
		if (rev == 0xAA)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0xAA;
		}
	}
	else if (g_remote_frame_cnt == 1)	//接收帧头
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
	else if (g_remote_frame_cnt == 2)	//接收地址
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt == 3)	//接收地址
	{
		WORD address = 0x0000;
		
		remote_frame_data[g_remote_frame_cnt++] = rev;
		address |= (WORD)(remote_frame_data[2])<<8;
		address |= (WORD)(remote_frame_data[3]);
		if (address != ((WORD)0x0001<<WIFI_ADDRESS))
		{
			g_remote_frame_cnt = 0;	//不是发给本机的
		}
	}
	else if (g_remote_frame_cnt == 4)	//接收长度
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
		if (rev+6>REMOTE_FRAME_LENGTH_MAX)	//判断是否会导致缓冲区溢出
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt>4 && g_remote_frame_cnt<=remote_frame_data[4]+4)	//接收数据区
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt==remote_frame_data[4]+4+1)	//接收校验字节	
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
/* 发送远程命令                                                          */
/* 参数 : cmd WiFi命令字                                                 */
/*        data发出的数据体，接在cmd后                                    */
/*        data长度                                                       */
/*-----------------------------------------------------------------------*/
void send_remote_frame(WORD cmd, BYTE data[], BYTE length)
{
	WORD i = 0, j = 0;
	
	remote_frame_data_send[i++] = 0xAA;
	remote_frame_data_send[i++] = 0xBB;
	remote_frame_data_send[i++] = (BYTE)((0x0001<<ANDROID_ADDRESS)>>8);
	remote_frame_data_send[i++] = (BYTE)(0x0001<<ANDROID_ADDRESS);
	remote_frame_data_send[i++] = length+2;
	remote_frame_data_send[i++] = (BYTE)(cmd>>8);
	remote_frame_data_send[i++] = (BYTE)cmd;
	for (j = 0;j < length; j++)
	{
		remote_frame_data_send[i++] = data[j];
	}
	remote_frame_data_send[i++] = check_sum(remote_frame_data_send+2, i-3);
	D1 = ~D1;
	serial_port_0_TX_array(remote_frame_data_send, i);
}


/*-----------------------------------------------------------------------*/
/* 异或校验                                                              */
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