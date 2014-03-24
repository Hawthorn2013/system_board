#ifndef __CONTROL_H__
#define __CONTROL_H__

///****************舵机pwm****************///
#define STEER_HELM_CENTER (500)

///****************速度pwm****************///
#define SPEED_PWM_MIN (3)
#define SPEED_PWM_MAX (60)
#define SPEED_PWM_TEST (20)

///****************速度光编****************///
#define SPEED_ENCODER_STOP (0x0000)
#define SPEED_ENCODER_MIN (0x0002)
#define SPEED_ENCODER_MAX (0x0110)
#define SPEED_ENCODER_TEST (11)


extern int cnt_pit;
extern int f_pit;
typedef struct
{
	WORD cnt_old;
	WORD cnt_new;
	WORD speed_now;
	int16_t speed_target;
	int is_forward;
} Data_encoder;
extern Data_encoder data_encoder;


extern void PitISR(void);
extern void set_speed_pwm(int16_t speed_pwm);
extern void contorl_speed_encoder_bb(void);


#endif