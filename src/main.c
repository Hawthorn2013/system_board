#include "includes.h"


int main(void)
{
	int i = 0;
	int flag=1;
	
	init_all_and_POST();
	
	
	/* Loop forever */
	for (;;)
	{
#if 0
		u8_t status;
		
		/* µ˜ ‘Õ”¬›“« */
		if (MEMS_SUCCESS == GetSatusReg(&status))
		{
			if (status & 80)
			{
				AngRateRaw_t rev;
				GetAngRateRaw(&rev);
				LCD_PrintoutInt(0, 0, (rev.x));
				LCD_PrintoutInt(0, 2, (rev.y));
				LCD_PrintoutInt(0, 4, (rev.z));
				if (g_remote_control_flags.send_gyro_data)
				{
					generate_remote_frame(WIFI_CMD_GET_GYRO_DATA, (BYTE *)&rev, sizeof(rev));
				}
			}
			//serial_port_0_TX(TestWhoAmI());
		}
#endif

#if 1
		/* ÷¥––‘∂≥Ã√¸¡Ó */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
#endif
	}
}