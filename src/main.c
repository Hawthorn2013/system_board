#include "includes.h"


/* 余婧到此一游 */
/* 叶川到此一游 */
int main(void)
{
	int flag=1;
	init_all_and_POST();
	drift_left();

	/* Loop forever */
	for (;;)
	{

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
			generate_remote_frame_2(g_device_NO, WIFI_ADDRESS_ANDRIUD_ZHOU, WIFI_CMD_GET_SEEED_NOW, sizeof(speed_now_tmp), (const BYTE *)&speed_now_tmp);
		}
		//delay_ms(100);
	}
}