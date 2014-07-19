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
		set_speed_target(30);
	}
	else if (RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN1 == site)
	{
		g_f_enable_mag_steer_control = 0;
		set_steer_helm(STEER_HELM_RIGHT);
		set_speed_target(30);
		delay_ms(500);
		set_steer_helm(STEER_HELM_LEFT);
		while (!is_on_mag_line()) { }
		g_f_enable_mag_steer_control = 0;
	}
	else if (RFID_CARD_ID_PUNCTURED_BALLON_STOP == site)
	{
		set_speed_target(0);
	}
}