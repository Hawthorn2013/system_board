#define __MAIN_C_
#include "includes.h"


int main(void)
{
	int i = 0;
	
	disable_watchdog();
	init_modes_and_clock();
	initEMIOS_0MotorAndSteer();
	init_pit();
	init_led();
	init_serial_port_0();
	init_serial_port_1();
	//init_serial_port_2();
	//init_serial_port_3();
	//init_supersonic_receive_0();
	//init_supersonic_receive_1();
	//init_supersonic_receive_2();
	//init_supersonic_receive_3();
	//init_supersonic_trigger_0();
	//init_supersonic_trigger_1();
	//init_supersonic_trigger_2();
	//init_supersonic_trigger_3();
	init_optical_encoder();
	init_DSPI_1();
	//init_I2C();
	enable_irq();
	
	//SD_init();
	initLCD();
	LCD_DISPLAY();
	LCD_Fill(0x00);
	
	set_speed_target(20);
	
	/* Loop forever */
	for (;;)
	{
		//delay_ms(2);	/* 不加延时会停 */
		if (g_f_pit)
		{
			g_f_pit = 0;
			
			contorl_speed_encoder_pid();
		}
#if 0
		if (g_serial_port_0_f)
		{
			g_serial_port_0_f = 0;
			
			rev_remote_frame(g_serial_port_0_data);
		}
#endif
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			D0 = ~D0;
			execute_remote_cmd(remote_frame_data+5);
		}
	}
}



