#define __RFIDCARD_C_
#include "includes.h"


const BYTE rfid_cmd_get_id[] = { 0xAA, 0xBB, 0x02, 0x20, 0x22 };
const BYTE rfid_cmd_get_data[] = { 0xAA, 0xBB, 0x0A, 0x21, 0x00, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F };
const BYTE rfid_cmd_energetic_mode_enable[] = { 0xAA, 0xBB, 0x03, 0x41, 0x15, 0x57 };
const BYTE rfid_cmd_energetic_mode_disable[] = { 0xAA, 0xBB, 0x03, 0x41, 0x00, 0x42 };
int g_rfid_frame_state = REMOTE_FRAME_STATE_NOK;
int g_rfid_frame_cnt = 0;
BYTE rfid_frame_data[RFID_FRAME_LENGTH_MAX];
BYTE rfid_frame_data_send[RFID_FRAME_LENGTH_MAX];
//DWORD rfid_site = 0x00000000;


/*-----------------------------------------------------------------------*/
/* Send RFID cmd                                                         */
/*-----------------------------------------------------------------------*/
void send_RFID_cmd(const BYTE cmd[])
{
	serial_port_2_TX_array(cmd, (WORD)(cmd[2]+3));
}


/*-----------------------------------------------------------------------*/
/* Receive RFID frame                                                    */
/*-----------------------------------------------------------------------*/
int rev_RFID_frame(BYTE rev)
{
	if (g_rfid_frame_cnt == 0)	//Receive frame head
	{
		if (rev == 0xAA)
		{
			rfid_frame_data[g_rfid_frame_cnt++] = 0xAA;
		}
	}
	else if (g_rfid_frame_cnt == 1)	//Receive frame head
	{
		if (rev == 0xBB)
		{
			rfid_frame_data[g_rfid_frame_cnt++] = 0xBB;
		}
		else
		{
			g_rfid_frame_cnt = 0;
		}
	}
	else if (g_rfid_frame_cnt == 2)	//Receive length
	{
		rfid_frame_data[g_rfid_frame_cnt++] = rev;
		if (rev+3>RFID_FRAME_LENGTH_MAX)	//Overflow or not
		{
			g_rfid_frame_cnt = 0;
		}
	}
	else if (g_rfid_frame_cnt>2 && g_rfid_frame_cnt<=rfid_frame_data[2]+1)	//Receive data area
	{
		rfid_frame_data[g_rfid_frame_cnt++] = rev;
	}
	else if (g_rfid_frame_cnt==rfid_frame_data[2]+2)	//Receive check BYTE
	{
		BYTE sum;
		
		rfid_frame_data[g_rfid_frame_cnt++] = rev;
		sum = check_sum((const BYTE *)(rfid_frame_data+2), (WORD)(rfid_frame_data[2]));
		if (sum != rfid_frame_data[rfid_frame_data[2]+2])
		{
			g_rfid_frame_cnt = 0;	//CheckSum Fail
		}
		else
		{
			g_rfid_frame_cnt = 0;
			g_rfid_frame_state = RFID_FRAME_STATE_OK;	//CheckSum Success
			explane_RFID_ret_data((const BYTE *)(rfid_frame_data+3));
		}
	}
	
	return g_rfid_frame_state;
}


/*-----------------------------------------------------------------------*/
/* Explane RFID return data aera                                         */
/*-----------------------------------------------------------------------*/
void explane_RFID_ret_data(const BYTE *data)
{
	WORD cmd = 0;
	DWORD cardID = 0x00000000;
	
	cmd = data[0];
	switch (cmd)
	{
		case RFID_CMD_ENERGETIC_MODE :
		cardID = *(DWORD *)(data+1);
		explane_RFID_ret_cardID(cardID);
		break;
	}
}


/*-----------------------------------------------------------------------*/
/* Explane RFID card ID                                                  */
/*-----------------------------------------------------------------------*/
void explane_RFID_ret_cardID(DWORD id)
{
	RFID_site_data.site = id;
	RFID_site_data.is_new_site = 1;
	RFID_site_data.time = g_time_basis_PIT;
	//serial_port_1_TX_array((BYTE *)&id, sizeof(id));
}