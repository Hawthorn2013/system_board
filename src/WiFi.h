#ifndef __WIFI_H__
#define __WIFI_H__


#define WIFI_CMD_SET_HELM_TARGET (0x0001)	/* 远程修改舵机目标值 */
#define WIFI_CMD_SET_HELM_KP (0x0002)	/* 远程修改舵机P值 */
#define WIFI_CMD_SET_HELM_KI (0x0003)	/* 远程修改舵机I值 */
#define WIFI_CMD_SET_HELM_KD (0x0004)	/* 远程修改舵机D值 */
#define WIFI_CMD_SET_MOTOR_TARGET (0x0005)	/* 远程修改电机目标值 */
#define WIFI_CMD_SET_MOTOR_KP (0x0006)	/* 远程修改电机P值 */
#define WIFI_CMD_SET_MOTOR_KI (0x0007)	/* 远程修改电机I值 */
#define WIFI_CMD_SET_MOTOR_KD (0x0008)	/* 远程修改电机D值 */

#define REMOTE_FRAME_LENGTH_MAX (32)
#define WIFI_ADDRESS_NUM (1)
#define REMOTE_FRAME_STATE_OK (1)
#define REMOTE_FRAME_STATE_NOK (0)
#define WIFI_ADDRESS (0x0001)

extern BYTE remote_frame_data[];
extern int g_remote_frame_state;

extern BYTE check_sum(const BYTE *data, WORD length);
extern void execute_remote_cmd(const BYTE *data);
extern int rev_remote_frame(BYTE rev);


#endif