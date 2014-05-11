#ifndef __SUPERSONIC_H__
#define __SUPERSONIC_H__


#define SS_DIS_20CM (0x00000000)

extern void init_supersonic_trigger_0(void);
extern void init_supersonic_trigger_1(void);
extern void init_supersonic_trigger_2(void);
extern void init_supersonic_trigger_3(void);

extern void trigger_supersonic_0(void);
extern void trigger_supersonic_1(void);
extern void trigger_supersonic_2(void);
extern void trigger_supersonic_3(void);

extern void init_supersonic_receive_0(void);
extern void init_supersonic_receive_1(void);
extern void init_supersonic_receive_2(void);
extern void init_supersonic_receive_3(void);

extern void intc_get_supersonic_time_0(void);
extern void intc_get_supersonic_time_1(void);
extern void intc_get_supersonic_time_2(void);
extern void intc_get_supersonic_time_3(void);


#endif