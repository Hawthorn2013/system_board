#ifndef __INIT_H__
#define __INIT_H__


extern void init_modes_and_clock(void);
extern void disable_watchdog(void);
extern void initEMIOS_0MotorAndSteer(void);
extern void init_led(void);
extern void init_pit(void);
extern void init_encoder(void);
extern void enable_irq(void);
extern int is_big_endian();

extern void init_serial_port_0(void);
extern void serial_port_0_TX(BYTE data);
extern void intc_serial_port_0_RX(void);

extern void init_serial_port_1(void);
extern void serial_port_1_TX(BYTE data);
extern void intc_serial_port_1_RX();

extern void init_serial_port_2(void);
extern void serial_port_2_TX(BYTE data);
extern void intc_serial_port_2_RX();

extern void init_serial_port_3(void);
extern void serial_port_3_TX(BYTE data);
extern void intc_serial_port_3_RX();

extern void init_supersonic_trigger_0(void);
extern void trigger_supersonic_0(void);
extern void init_supersonic_receive_0(void);
extern void intc_get_supersonic_time_0(void);

extern void init_optical_encoder(void);
extern void delayms(int ms);

#endif