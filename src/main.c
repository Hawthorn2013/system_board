#include "includes.h"


int main(void)
{
	int flag = 1;
	
	init_all_and_POST();
	read_rad_xyz = 1;
	//reset_rev_data();
	g_f_enable_mag_steer_control = 1;
	set_speed_target(0);

	
	/*�ȴ���ʼ*/
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
		/* ִ��Զ������ */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
#endif

		
		delay_ms(10);
#if 1
		/* ������������ */
		control_car_action();
#endif

		/* �������� */
		report_online();
	}
}
















