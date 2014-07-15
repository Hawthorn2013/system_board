#define __ACTION_C_
#include "includes.h"


/*-----------------------------------------------------------------------*/
/* ´ÁÆøÇò                                                                */
/* ´«ÈëRFID¿¨ºÅ                                                          */                                                          
/*-----------------------------------------------------------------------*/
void punctured_ballon(DWORD site)
{
	if (RFID_CARD_ID_PUNCTURED_BALLON_START == site)
	{
		data_punctured_ballon.time = 0x00000000;
		set_speed_target(30);
	}
	else if (RFID_CARD_ID_PUNCTURED_BALLON_TURN_1 == site)
	{
		if (ACTION_PUNCTURED_BALLON_STEP_NO == data_punctured_ballon.step)
		{
			data_punctured_ballon.time = g_time_basis_PIT;
			data_punctured_ballon.step = ACTION_PUNCTURED_BALLON_STEP_1;
		}
		else if (ACTION_PUNCTURED_BALLON_STEP_1 == data_punctured_ballon.step)
		{
			set_steer_helm(STEER_HELM_RIGHT);
			set_speed_target(20);
			if (diff_time_basis_PIT(g_time_basis_PIT, data_punctured_ballon.time) >= 50)
			{
				data_punctured_ballon.time = g_time_basis_PIT;
				data_punctured_ballon.step = ACTION_PUNCTURED_BALLON_STEP_2;
			}
		}
		else if (ACTION_PUNCTURED_BALLON_STEP_2 == data_punctured_ballon.step)
		{
			set_steer_helm(STEER_HELM_CENTER);
			set_speed_target(20);
			if (diff_time_basis_PIT(g_time_basis_PIT, data_punctured_ballon.time) >= 0)
			{
				data_punctured_ballon.time = g_time_basis_PIT;
				data_punctured_ballon.step = ACTION_PUNCTURED_BALLON_STEP_3;
			}
		}
		else if (ACTION_PUNCTURED_BALLON_STEP_3 == data_punctured_ballon.step)
		{
			set_steer_helm(STEER_HELM_CENTER);
			set_speed_target(0);
			if (diff_time_basis_PIT(g_time_basis_PIT, data_punctured_ballon.time) >= 100)
			{
				data_punctured_ballon.time = g_time_basis_PIT;
				data_punctured_ballon.step = ACTION_PUNCTURED_BALLON_STEP_4;
			}
		}
		else if (ACTION_PUNCTURED_BALLON_STEP_4 == data_punctured_ballon.step)
		{
			set_steer_helm(STEER_HELM_CENTER);
			set_speed_target(-20);
			if (diff_time_basis_PIT(g_time_basis_PIT, data_punctured_ballon.time) >= 60)
			{
				data_punctured_ballon.time = g_time_basis_PIT;
				data_punctured_ballon.step = ACTION_PUNCTURED_BALLON_STEP_5;
			}
		}
		else if (ACTION_PUNCTURED_BALLON_STEP_5 == data_punctured_ballon.step)
		{
			set_steer_helm(STEER_HELM_RIGHT);
			set_speed_target(-20);
			if (diff_time_basis_PIT(g_time_basis_PIT, data_punctured_ballon.time) >= 30)
			{
				data_punctured_ballon.time = g_time_basis_PIT;
				data_punctured_ballon.step = ACTION_PUNCTURED_BALLON_STEP_6;
			}
		}
		else if (ACTION_PUNCTURED_BALLON_STEP_6 == data_punctured_ballon.step)
		{
			set_steer_helm(STEER_HELM_CENTER);
			set_speed_target(20);
			if (diff_time_basis_PIT(g_time_basis_PIT, data_punctured_ballon.time) >= 0)
			{
				data_punctured_ballon.time = g_time_basis_PIT;
				data_punctured_ballon.step = ACTION_PUNCTURED_BALLON_STEP_OK;
				set_steer_helm(STEER_HELM_CENTER);
				set_speed_target(20);
			}
		}
	}
	else if (RFID_CARD_ID_PUNCTURED_BALLON_STOP == site)
	{
		set_speed_target(0);
	}
}