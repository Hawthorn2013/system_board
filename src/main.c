#include "includes.h"


int main(void)
{
	BYTE test[16] = {0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, 0xbc, }, test_1 = 0x6b;
	BYTE test_2[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	disable_watchdog();
	init_modes_and_clock();
	//initEMIOS_0MotorAndSteer();
	//init_pit();
	init_led();
	init_serial_port_0();
	//init_serial_port_1();
	//init_serial_port_2();
	//init_supersonic_receive_0();
	//init_supersonic_trigger_0();
	//init_optical_encoder();
	//init_DSPI_2();
	//init_DSPI_1();
	//init_I2C();
	enable_irq();

	//SD_init();
	init_I2C();
	
	/* Loop forever */
	for (;;)
	{
		if (g_serial_port_0_f==1)
		{
			g_serial_port_0_f=0;
			switch (g_serial_port_0_data)
			{
				case 'T':
				serial_port_0_TX((BYTE)I2C_write_byte_to_time_module_2(0x02, test_2, 7));
				break;
				case 'R':
				serial_port_0_TX((BYTE)I2C_read_byte_from_time_module_2(0x00, test, 16));
				serial_port_0_TX(test[0]);
				serial_port_0_TX(test[1]);
				serial_port_0_TX(test[2]);
				serial_port_0_TX(test[3]);
				serial_port_0_TX(test[4]);
				serial_port_0_TX(test[5]);
				serial_port_0_TX(test[6]);
				serial_port_0_TX(test[7]);
				serial_port_0_TX(test[8]);
				serial_port_0_TX(test[9]);
				serial_port_0_TX(test[10]);
				serial_port_0_TX(test[11]);
				serial_port_0_TX(test[12]);
				serial_port_0_TX(test[13]);
				serial_port_0_TX(test[14]);
				serial_port_0_TX(test[15]);

				break;
			}
		}
		D0 = ~ D0;
		delay_ms(900);
	}
}



