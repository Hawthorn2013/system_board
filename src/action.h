#ifndef __ACTION_H__
#define __ACTION_H__


#define ACTION_PUNCTURED_BALLON	/* 刺气球 */
#define ACTION_PUNCTURED_BALLON_STEP_NO (0x00)	/* 不执行 */
#define ACTION_PUNCTURED_BALLON_STEP_1 (0x01)	/* 转弯 */
#define ACTION_PUNCTURED_BALLON_STEP_2 (0x02)	/* 前进 */
#define ACTION_PUNCTURED_BALLON_STEP_3 (0x03)	/* 停留 */
#define ACTION_PUNCTURED_BALLON_STEP_4 (0x04)	/* 后退 */
#define ACTION_PUNCTURED_BALLON_STEP_5 (0x05)	/* 反向转弯 */
#define ACTION_PUNCTURED_BALLON_STEP_6 (0x06)	/* 回到线上 */
#define ACTION_PUNCTURED_BALLON_STEP_OK (0x07)	/* 完成 */


/* 刺气球数据 */
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
} data_punctured_ballon;
#endif


#endif