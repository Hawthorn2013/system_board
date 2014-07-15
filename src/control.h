#ifndef __CONTROL_H__
#define __CONTROL_H__


extern DWORD g_time_basis_PIT;


/* ������PWM */
#define STEER_HELM_CENTER (3301)
#define STEER_HELM_TEST (1500)
#define STEER_HELM_LEFT (2247)
#define STEER_HELM_RIGHT (4292)
#define STEER_HELM_MIX (1038)
#define STEER_HELM_MAX (6146)

/* �źŶ��PWM */
#define SINGLE_HELM_CENTER (50)

/* �ٶ�PWM */
#define SPEED_PWM_MIN (1)
#define SPEED_PWM_MAX (3199)
#define SPEED_PWM_TEST (20)

/* �ٶȹ�� */
#define SPEED_ENCODER_STOP (0x0000)
#define SPEED_ENCODER_MIN (0x0002)
#define SPEED_ENCODER_MAX (0x0110)
#define SPEED_ENCODER_TEST (11)


extern int g_f_pit;
extern DWORD g_time_basis_PIT;

/* ���������� */
#ifdef __CONTROL_C_
struct
{
	WORD center;
	WORD left_limit;
	WORD right_limit;
} data_steer_helm = { STEER_HELM_CENTER, STEER_HELM_LEFT, STEER_HELM_RIGHT };
#else
extern struct
{
	WORD center;
	WORD left_limit;
	WORD right_limit;
} data_steer_helm;
#endif

/* ������� */
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

/* �ٶ�PID���� */
#ifdef __CONTROL_C_
struct
{
	float p;
	float i;
	float d;
} data_speed_pid = { 15, 1, 5 };
#else
extern struct
{
	float p;
	float i;
	float d;
} data_speed_pid;
#endif

/* �ٶ��������� */
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

/* ������λ������ */
#ifdef __CONTROL_C_
WORD helm_data_record = STEER_HELM_CENTER;
#endif

extern void PitISR(void);
extern void set_speed_pwm(int16_t speed_pwm);
extern void contorl_speed_encoder_bb(void);
extern void set_speed_target(SWORD speed_target);
extern void set_speed_KP(WORD kp);
extern void set_speed_KI(WORD ki);
extern void set_speed_KD(WORD kd);
extern void set_steer_helm(WORD helmData);
extern void contorl_speed_encoder_pid(void);
extern void generate_remote_frame(WORD cmd, BYTE data[], BYTE length);
extern DWORD diff_time_basis_PIT(DWORD new_time, DWORD old_time);
extern void set_steer_helm(WORD helmData);


#endif