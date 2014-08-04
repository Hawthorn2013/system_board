#include "includes.h"


int main(void)
{
	int flag=1;
	init_all_and_POST();
	
	
	/* Loop forever */
	for (;;)
	{
	//control_steer_helm_1();

#if 1
		/* 执行远程命令 */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
#endif
		
		/* 发送当前速度 */
		if (g_remote_control_flags.send_seppd_now)
		{
			SWORD speed_now_tmp;
			
			if (data_encoder.is_forward)
			{
				speed_now_tmp = (SWORD)(data_encoder.speed_now);
			}
			else
			{
				speed_now_tmp = (SWORD)0 - (SWORD)(data_encoder.speed_now);
			}
			generate_remote_frame(WIFI_CMD_GET_SEEED_NOW, (const BYTE *)&speed_now_tmp, sizeof(speed_now_tmp));
		}
	}
}