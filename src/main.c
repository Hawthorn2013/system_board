#include "includes.h"


int main(void)
{
	disable_watchdog();
	init_modes_and_clock();
	initEMIOS_0MotorAndSteer();
	//init_pit();
	init_led();
	init_serial_port_0();
	//init_serial_port_1();
	//init_serial_port_2();
	init_serial_port_3();
	//init_supersonic_receive_0();
	//init_supersonic_receive_1();
	//init_supersonic_receive_2();
	//init_supersonic_receive_3();
	//init_supersonic_trigger_0();
	//init_supersonic_trigger_1();
	//init_supersonic_trigger_2();
	//init_supersonic_trigger_3();
	//init_optical_encoder();
	//init_DSPI_1();
	//init_I2C();
	enable_irq();

	//SD_init();
	//initLCD();
	
	/* Loop forever */
	for (;;)
	{
		if (g_serial_port_0_f)
		{
			g_serial_port_0_f = 0;
			rev_remote_frame(g_serial_port_0_data);
			if (g_remote_frame_state)
			{
				D1 = ~D1;
				g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
				execute_remote_cmd((const BYTE *)(remote_frame_data+5));
			}
		}
	}
}



