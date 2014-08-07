#ifndef __RFIDCARD_H__
#define __RFIDCARD_H__

#define RFID_CMD_READ_CARD (0x20)
#define RFID_CMD_READ_DATA_BLOCK (0x21)
#define RFID_CMD_ENERGETIC_MODE (0x41)

#define RFID_CARD_ID
#define RFID_CARD_ID_PUNCTURED_BALLON_START (0x0A1E4748)
#define RFID_CARD_ID_PUNCTURED_BALLON_TURN_1 (0x4AF14448)
#define RFID_CARD_ID_PUNCTURED_BALLON_STOP (0x1250DD76)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR4_TURN1 (0xCA7C00BD)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN1 (0xDA8220BD)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR4_TURN2 (0xDE913FC9)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN2 (0x4AF14448)
#define RFID_CARD_ID_1 (0xDAA23548)
#define RFID_CARD_ID_2 (0xFA8320BD)

#define RFID_CARD_ID_1_1 (0x00000000)
#define RFID_CARD_ID_1_2 (0x00000000)
#define RFID_CARD_ID_1_3 (0x00000000)
#define RFID_CARD_ID_1_4 (0x00000000)
#define RFID_CARD_ID_2_1 (0x00000000)
#define RFID_CARD_ID_2_2 (0x00000000)
#define RFID_CARD_ID_2_3 (0x00000000)
#define RFID_CARD_ID_3_1 (0x1250DD76)
#define RFID_CARD_ID_4_1 (0x00000000)
#define RFID_CARD_ID_4_2 (0x00000000)
#define RFID_CARD_ID_5_1 (0xDAA23548)
#define RFID_CARD_ID_5_2 (0x00000000)
#define RFID_CARD_ID_5_3 (0x00000000)
#define RFID_CARD_ID_5_4 (0x00000000)
#define RFID_CARD_ID_5_5 (0x00000000)
#define RFID_CARD_ID_6_1 (0xFA8320BD)
#define RFID_CARD_ID_6_2 (0x2A8420BD)
#define RFID_CARD_ID_6_3 (0xCA72FFBC)
#define RFID_CARD_ID_6_4 (0xDE913FC9)
#define RFID_CARD_ID_6_5 (0x00000000)
#define RFID_CARD_ID_6_6 (0x00000000)
#define RFID_CARD_ID_7_1 (0x00000000)
#define RFID_CARD_ID_7_2 (0x00000000)
#define RFID_CARD_ID_7_3 (0x00000000)
#define RFID_CARD_ID_7_4 (0xCA7C00BD)

#define RFID_FRAME_LENGTH_MAX (256)
#define RFID_FRAME_STATE_OK (1)
#define RFID_FRAME_STATE_NOK (0)

#define RFID_MODUL_TYPE_UNKNOW (0)
#define RFID_MODUL_TYPE_OLD (1)
#define RFID_MODUL_TYPE_NEW (2)

extern const BYTE rfid_cmd_get_id[];
extern const BYTE rfid_cmd_get_data[];
extern const BYTE rfid_cmd_energetic_mode_enable[];
extern const BYTE rfid_cmd_energetic_mode_enable_new[];
extern const BYTE rfid_cmd_energetic_mode_disable[];
extern const BYTE rfid_cmd_energetic_mode_disable_new[];
//extern DWORD rfid_site;


/* λ������ */
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
extern void explane_RFID_ret_data(const BYTE *data, WORD length);
extern void explane_RFID_ret_cardID(DWORD id);
extern int init_RFID_modul_type(void);

#endif