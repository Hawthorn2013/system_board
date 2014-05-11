#include "includes.h"


void change_param(const BYTE * data)
{
	int16_t rev_data = 0;
			
	if (data[0]==4 && data[1]==0x01 && data[2]==0x01)	//修改int16_t类型
	{
		rev_data |= (int16_t)(data[3]);
		rev_data |= (int16_t)(data[4])<<8;
		EMIOS_0.CH[9].CBDR.R = (rev_data+50)*10;
	}
}






#if 0
		if (g_serial_port_0_f)
		{
			D0 = ~D0;
			g_serial_port_0_f = 0;
			if (count == 0)	//读帧头
			{
				if (g_serial_port_0_data == 0xaa)
				{
					cmd[0] = 0xaa;
					count++;
				}
				else
				{
					count = 0;
				}
			}
			else if (count == 1)	//读帧头
			{
				if (g_serial_port_0_data == 0xbb)
				{
					cmd[1] = 0xbb;
					count++;
				}
				else
				{
					count = 0;
				}
			}
			else if (count == 2)	//读数据区长度
			{
				cmd[2] = g_serial_port_0_data;
				count++;
			}
			else if (count>=3 && count<cmd[2]+3)
			{
				cmd[count] = g_serial_port_0_data;
				count++;
			}
			else if (count==cmd[2]+3)	//读校验字节
			{
				D1 = ~D1;
				cmd[count] = g_serial_port_0_data;
				count++;
			}
		}
		
		if (count==cmd[2]+3+1)	//调参
		{
			int16_t rev_data = 0;
			
			if (cmd[2]==4 && cmd[3]==0x01 && cmd[4]==0x01)
			{
				D2 = ~D2;
				rev_data |= (int16_t)(cmd[5]);
				rev_data |= (int16_t)(cmd[6])<<8;
				EMIOS_0.CH[9].CBDR.R = (rev_data+50)*10;
			}
			count = 0;
		}
		
#endif