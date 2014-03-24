#include "includes.h"


int main(void)
{
	BYTE test[5] = {0xbc, 0xbc, 0xbc, 0xbc, 0xbc}, test_1 = 0x6b;
	
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
	
#if 0
	test_1 = (BYTE)I2C_read_multiple_byte_from_time_module(0x00, &test, 1);
	serial_port_0_TX(test);
	serial_port_0_TX(test_1);
#endif

#if 0
	test_1 = I2C_write_byte_to_time_module(0x08, 0x09);
	serial_port_0_TX(test_1);	
#endif

#if 0
	I2C_read_byte_from_time_module_2(0x02, test);
	serial_port_0_TX(test[0]);
	serial_port_0_TX(test[1]);
	serial_port_0_TX(test[2]);
	serial_port_0_TX(test[3]);
#endif
	
	/* Loop forever */
	for (;;)
	{
#if 1
		init_I2C();
		serial_port_0_TX((BYTE)I2C_read_byte_from_time_module_3(0x02, test, 3));
		serial_port_0_TX(test[0]);
		serial_port_0_TX(test[1]);
		serial_port_0_TX(test[2]);

		delay_ms(900);
#endif
	}
}



