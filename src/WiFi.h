#ifndef __WIFI_H__
#define __WIFI_H__


#define WIFI_CMD_SET_HELM_TARGET (0x0001)	/* Զ���޸Ķ��Ŀ��ֵ */
#define WIFI_CMD_SET_HELM_KP (0x0002)	/* Զ���޸Ķ��Pֵ */
#define WIFI_CMD_SET_HELM_KI (0x0003)	/* Զ���޸Ķ��Iֵ */
#define WIFI_CMD_SET_HELM_KD (0x0004)	/* Զ���޸Ķ��Dֵ */
#define WIFI_CMD_SET_MOTOR_TARGET (0x0005)	/* Զ���޸ĵ��Ŀ��ֵ */
#define WIFI_CMD_SET_MOTOR_KP (0x0006)	/* Զ���޸ĵ��Pֵ */
#define WIFI_CMD_SET_MOTOR_KI (0x0007)	/* Զ���޸ĵ��Iֵ */
#define WIFI_CMD_SET_MOTOR_KD (0x0008)	/* Զ���޸ĵ��Dֵ */
#define WIFI_CMD_GET_GYRO_DATA (0x0009)	/* ��ȡ���������� */
#define WIFI_CMD_UNGET_GYRO_DATA (0x000A)	/* ֹͣ��ȡ���������� */
#define WIFI_CMD_SET_STEER_HELM_CENTER (0x000B)	/* ���÷�������ֵ */
#define WIFI_CMD_SET_STEER_HELM_LEFT (0x000C)	/* ���÷��������� */
#define WIFI_CMD_SET_STEER_HELM_RIGHT (0x000D)	/* ���÷������Ҽ��� */
#define WIFI_CMD_WRITE_STEER_HELM_DATA_TO_TF (0x000E)	/* ����������д��TF�� */
#define WIFI_CMD_SEND_STEER_HELM_DATA_FROM_TF (0x000F)	/* ���ͷ�������������λ�� */
#define WIFI_CMD_NET (0x0010)	/* ��װ�������� */
#define WIFI_CMD_GET_SEEED_NOW (0x0011)	/* ��ȡ��ǰ�ٶ� */
#define WIFI_CMD_UNGET_SPEED_NOW (0x0012)	/* ֹͣ��ȡ��ǰ�ٶ� */

#define WIFI_NET_CMD_CAR_REPORT_CURRENT_SITE (0x0001)	/* �����浱ǰ������λ�� */
#define WIFI_NET_CMD_CAR_REPORT_CACHE_SITE (0x0002)	/* �����滺���λ�� �����һ�ζ�����λ�� */


#define REMOTE_FRAME_LENGTH (32)
#define REMOTE_FRAME_STATE_OK (1)
#define REMOTE_FRAME_STATE_NOK (0)

//#define WIFI_ADDRESS (1)
#define ANDROID_ADDRESS (3)

#define WIFI_ADDRESS_CAR_1 (1)	/* ����1 Ư�� */
#define WIFI_ADDRESS_CAR_2 (2)	/* ����2 */
#define WIFI_ADDRESS_CAR_3 (3)	/* �˳�1 */
#define WIFI_ADDRESS_CAR_4 (4)	/* �˳�2 */
#define WIFI_ADDRESS_DRAWBRIDGE (5)	/* ���� */
#define WIFI_ADDRESS_DRAHTBRIDGE (6)	/* ��˿�� */
#define WIFI_ADDRESS_PC (7)	/* PC��λ�� */
#define WIFI_ADDRESS_ANDRIUD_ZHOU (8)	/* Andriud��λ�� of ��˹�� */
#define WIFI_ADDRESS_ANDRIUD_YE (9)	/* Andriud��λ�� of Ҷ�� */
#define WIFI_ADDRESS_ANDRIUD_JIAO (10)	/* Andriud��λ�� of ���� */
#define WIFI_ADDRESS_ANDRIUD_YU (11)	/* Andriud��λ�� of ��� */

extern BYTE remote_frame_data[];
extern int g_remote_frame_state;
extern BYTE g_device_NO;

/* Զ�̿��Ʊ�־λ */
#ifdef __WIFI_C_
struct
{
	int send_gyro_data;	/* �������������� */
	int send_seppd_now;	/* ���͵�ǰ�ٶ� */
} g_remote_control_flags = { 0, 0 };
#else
extern struct
{
	int send_gyro_data;
	int send_seppd_now;
} g_remote_control_flags;
#endif

extern BYTE check_sum(const BYTE *data, WORD length);
extern void execute_remote_cmd(const BYTE *data);
extern int rev_remote_frame(BYTE rev);
extern int rev_remote_frame_2(BYTE rev);
extern void generate_remote_frame(WORD cmd, const BYTE data[], BYTE length);
extern void generate_remote_frame_2(BYTE scr, BYTE des, WORD cmd, BYTE length, const BYTE data[]);
extern void generate_remote_net_frame_to_send_site(WORD cmd, DWORD site, BYTE data[]);


#endif