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
#define WIFI_CMD_GET_GYRO_DATA (0x0009)	/* 获取陀螺仪数据 */
#define WIFI_CMD_UNGET_GYRO_DATA (0x000A)	/* 停止获取陀螺仪数据 */
#define WIFI_CMD_SET_STEER_HELM_CENTER (0x000B)	/* 设置方向舵机中值 */
#define WIFI_CMD_SET_STEER_HELM_LEFT (0x000C)	/* 设置方向舵机左极限 */
#define WIFI_CMD_SET_STEER_HELM_RIGHT (0x000D)	/* 设置方向舵机右极限 */
#define WIFI_CMD_WRITE_STEER_HELM_DATA_TO_TF (0x000E)	/* 方向舵机数据写入TF卡 */




#define REMOTE_FRAME_LENGTH (32)
#define WIFI_ADDRESS_NUM (1)
#define REMOTE_FRAME_STATE_OK (1)
#define REMOTE_FRAME_STATE_NOK (0)
#define WIFI_ADDRESS (0x0001)
#define ANDROID_ADDRESS (3)

extern BYTE remote_frame_data[];
extern int g_remote_frame_state;

/* 远程控制标志位 */
#ifdef __WIFI_C_
struct
{
	int send_gyro_data;	/* 发送陀螺仪数据 */
} g_remote_control_flags = { 0, };
#else
extern struct
{
	int send_gyro_data;
} g_remote_control_flags;
#endif

extern BYTE check_sum(const BYTE *data, WORD length);
extern void execute_remote_cmd(const BYTE *data);
extern int rev_remote_frame(BYTE rev);


#endif