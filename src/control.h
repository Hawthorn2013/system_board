#ifndef __CONTROL_H__
#define __CONTROL_H__


/* 舵机PWM */
#define STEER_HELM_CENTER (500)
#define STEER_HELM_TEST (1500)

/* 信号舵机PWM */
#define SINGLE_HELM_CENTER (50)

/* 速度PWM */
#define SPEED_PWM_MIN (3)
#define SPEED_PWM_MAX (60)
#define SPEED_PWM_TEST (20)

/* 速度光编 */
#define SPEED_ENCODER_STOP (0x0000)
#define SPEED_ENCODER_MIN (0x0002)
#define SPEED_ENCODER_MAX (0x0110)
#define SPEED_ENCODER_TEST (11)


extern int cnt_pit;
extern int f_pit;

/* 光编数据 */
#ifdef __CONTROL_C_
struct
{
	WORD cnt_old;
	WORD cnt_new;
	WORD speed_now;
	SWORD is_forward;
} data_encoder = { 0x0000, 0x0000, 0x0000, 0, };
#else
extern struct
{
	WORD cnt_old;
	WORD cnt_new;
	WORD speed_now;
	SWORD is_forward;
} data_encoder;
#endif

/* 速度PID数据 */
#ifdef __CONTROL_C_
struct
{
	WORD p;
	WORD i;
	WORD d;
} data_speed_pid = { 0x0000, 0x0000, 0x0000, };
#else
extern struct
{
	WORD p;
	WORD i;
	WORD d;
} data_speed_pid;
#endif

/* 速度设置数据 */
#ifdef __CONTROL_C_
struct
{
	SWORD speed_pwm;
	SWORD speed_target;
} data_speed_settings = { 0x0000, 0x0000, };
#else
extern struct
{
	SWORD speed_pwm;
	SWORD speed_target;
} data_speed_settings;
#endif

extern void PitISR(void);
extern void set_speed_pwm(int16_t speed_pwm);
extern void contorl_speed_encoder_bb(void);
extern void set_speed_target(SWORD speed_target);

#endif