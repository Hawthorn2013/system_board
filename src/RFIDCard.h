#ifndef __RFIDCARD_H__
#define __RFIDCARD_H__

#define RFID_CMD_READ_CARD (0x20)
#define RFID_CMD_READ_DATA_BLOCK (0x21)
#define RFID_CMD_ENERGETIC_MODE (0x41)

#define RFID_CARD_ID
#define RFID_CARD_ID_PUNCTURED_BALLON_START (0x1250DD76)
#define RFID_CARD_ID_PUNCTURED_BALLON_TURN_1 (0x4AF14448)
#define RFID_CARD_ID_PUNCTURED_BALLON_STOP (0xCA7C00BD)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR4_TURN1 (0xCA7C00BD)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN1 (0xDA8220BD)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR4_TURN2 (0xDE913FC9)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN2 (0x4AF14448)

#define RFID_FRAME_LENGTH_MAX (256)
#define RFID_FRAME_STATE_OK (1)
#define RFID_FRAME_STATE_NOK (0)


extern const BYTE rfid_cmd_get_id[];
extern const BYTE rfid_cmd_get_data[];
extern const BYTE rfid_cmd_energetic_mode_enable[];
extern const BYTE rfid_cmd_energetic_mode_disable[];
//extern DWORD rfid_site;

/* Î»ÖÃÊý¾Ý */
#ifdef __CONTROL_C_
struct
{
	DWORD site;
	DWORD time;
	int is_new_site;
} RFID_site_data = { 0x00000000, 0x00000000, 0 };
#else
extern struct
{
	DWORD site;
	DWORD time;
	int is_new_site;
} RFID_site_data;
#endif

extern int rev_RFID_frame(BYTE rev);
extern void explane_RFID_ret_data(const BYTE *data);
extern void explane_RFID_ret_cardID(DWORD id);


#endif