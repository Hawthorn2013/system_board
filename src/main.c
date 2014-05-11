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
	//init_serial_port_3();
	//init_supersonic_receive_0();
	//init_supersonic_receive_1();
	//init_supersonic_receive_2();
	//init_supersonic_receive_3();
	//init_supersonic_trigger_0();
	//init_supersonic_trigger_1();
	//init_supersonic_trigger_2();
	//init_supersonic_trigger_3();
	//init_optical_encoder();
	init_DSPI_1();
	//init_DSPI_L3G4200D();
	//initLCD();
	//init_I2C();
	enable_irq();
	
	//init_L3G4200D();
	//SD_init();
	//initLCD();
	//LCD_DISPLAY();
	//delay_us(10);
	//LCD_Fill(0xFF);
	SetMode(NORMAL);
	/* Loop forever */
	for (;;)
	{
		//serial_port_0_TX(L3G4200D_read_write_byte(WHO_AM_I | 0x80));
		serial_port_0_TX(L3G4200D_read_write_byte(0xff));
		//BYTE rev = 0x00;
		//WORD out_y = 0x0000;
		
		//L3G4200D_read_register(OUT_X_L, &rev);
		//out_y |= (WORD)rev;
		//serial_port_0_TX(rev);
		//L3G4200D_read_register(OUT_X_H, &rev);
		//out_y |= ((WORD)rev)<<8;
		//serial_port_0_TX(rev);
		//LCD_PrintoutInt(86, 6, out_y);
		//delay_ms(100);
		//D0 = ~D0;
	}
}



