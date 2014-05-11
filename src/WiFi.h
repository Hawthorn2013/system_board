#ifndef __WIFI_H__
#define __WIFI_H__


#define WIFI_CMD_SET_HELM (0x0100)
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