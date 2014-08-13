#include "includes.h"


int main(void)
{
	int flag = 1;
	
	init_all_and_POST();
	read_rad_xyz = 1;
	//reset_rev_data();
	g_f_enable_mag_steer_control = 1;
	set_speed_target(0);

	
	/*等待开始*/
	while(!g_start_all&&(WIFI_ADDRESS_CAR_4 == g_device_NO||WIFI_ADDRESS_CAR_3 == g_device_NO))
	{
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
		D0=~D0;
		delay_ms(500);
	}
	D0=1;
	if((WIFI_ADDRESS_CAR_4 == g_device_NO||WIFI_ADDRESS_CAR_3 == g_device_NO))
		set_speed_target(10);
	else 	set_speed_target(0);
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
		delay_ms(10);
#if 1
		/* 整车动作控制 */
		control_car_action();
#endif
	}
}
















