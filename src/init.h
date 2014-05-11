#ifndef __INIT_H__
#define __INIT_H__


#define PERIPH_SET_1_CLK_16M


extern void init_modes_and_clock(void);
extern void disable_watchdog(void);
extern void initEMIOS_0MotorAndSteer(void);
extern void init_led(void);
extern void init_pit(void);
extern void init_encoder(void);
extern void enable_irq(void);
extern int is_big_endian();


extern void init_supersonic_trigger_0(void);
extern void trigger_supersonic_0(void);
extern void init_supersonic_receive_0(void);
extern void intc_get_supersonic_time_0(void);

extern void init_optical_encoder(void);
void delay_us(DWORD us);
void delay_ms(DWORD ms);


#endif