#include "includes.h"


int main(void)
{
	int flag=1;
	init_all_and_POST();
	//drift_left();
	set_speed_target(30);
	delay_ms(500);
	/* ����ת90�� */
	//control_angle_steer_helm(-30);
	/* ѭ�� */
	//g_f_enable_mag_steer_control = 1;
//	g_f_enable_speed_control_2=1;

	/* Loop forever */
	for (;;)
	{
	read_rev_data();
//	LCD_PrintoutInt(0, 0, rad.x);
//	LCD_PrintoutInt(0, 2, rad.y);	


#if 1
		/* ִ��Զ������ */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
#endif
		/* ���͵�ǰ�ٶ� */
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

#if 0
		/* ������������ */
		control_car_action();
#endif
	}
}
















