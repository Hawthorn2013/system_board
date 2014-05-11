#include "includes.h"


int g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
int g_remote_frame_cnt = 0;
BYTE remote_frame_data[REMOTE_FRAME_LENGTH_MAX];


void execute_remote_cmd(const BYTE *data)
{
	WORD cmd = 0;
	
	D2 = ~D2;
	cmd = (WORD)(data[0]) | ((WORD)(data[1])<<8);
	switch (cmd)
	{
		case WIFI_CMD_SET_HELM :
		D3 = ~D3;
		STEER_HELM_DATA = (WORD)(data[2]) | ((WORD)(data[3])<<8);
		break;
	}
}


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
		address |= (WORD)(remote_frame_data[2]);
		address |= (WORD)(remote_frame_data[3])<<8;
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
			D0 = ~D0;
			g_remote_frame_cnt = 0;
			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
		}
	}
	
	return g_remote_frame_state;
}


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