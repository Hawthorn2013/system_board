#ifndef __ACTION_H__
#define __ACTION_H__


#define ACTION_PUNCTURED_BALLON	/* ������ */
#define ACTION_PUNCTURED_BALLON_STEP_NO (0x00)	/* ��ִ�� */
#define ACTION_PUNCTURED_BALLON_STEP_1 (0x01)	/* ת�� */
#define ACTION_PUNCTURED_BALLON_STEP_2 (0x02)	/* ǰ�� */
#define ACTION_PUNCTURED_BALLON_STEP_3 (0x03)	/* ͣ�� */
#define ACTION_PUNCTURED_BALLON_STEP_4 (0x04)	/* ���� */
#define ACTION_PUNCTURED_BALLON_STEP_5 (0x05)	/* ����ת�� */
#define ACTION_PUNCTURED_BALLON_STEP_6 (0x06)	/* �ص����� */
#define ACTION_PUNCTURED_BALLON_STEP_OK (0x07)	/* ��� */

#define ACTION_PUNCTURED_BALLON_STEP_CAR3_TURN1 (0x08)	/* ��3��1��ת�� */
#define ACTION_PUNCTURED_BALLON_STEP_CAR4_TURN1 (0x09)	/* ��4��1��ת�� */
#define ACTION_PUNCTURED_BALLON_STEP_CAR3_TURN2 (0x0a)	/* ��3��2��ת�� */
#define ACTION_PUNCTURED_BALLON_STEP_CAR4_TURN2 (0x0b)	/* ��4��2��ת�� */


/* ���������� */
#ifdef __ACTION_C_
struct
{
	DWORD time;
	int step;
} data_punctured_ballon = { 0x00000000, };
#else
extern struct
{
	DWORD time;
	int step;
} data_punctured_ballon;
#endif


#endif