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
		report_online();//添加3、4号车等待时候报告已上电
		delay_ms(10);
	}
 	
	if(WIFI_ADDRESS_CAR_3 == g_device_NO)
		set_speed_target(10);
	else if(WIFI_ADDRESS_CAR_4 == g_device_NO)
	{
		delay_ms(1000);
		set_speed_target(10);
	}
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

		
		delay_ms(10);
#if 1
		/* 整车动作控制 */
		control_car_action();
#endif

		/* 报告在线 */
		report_online();
	}
}
















