#ifndef __INCLUDES_H__
#define __INCLUDES_H__


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;


#include "MPC5604B_M27V.h"
#include "init.h"
#include "control.h"
#include "supersonic.h"
#include "sdcard.h"


///****************LED****************///

#define D0 SIU.GPDO[12].B.PDO
#define D1 SIU.GPDO[13].B.PDO
#define D2 SIU.GPDO[14].B.PDO
#define D3 SIU.GPDO[15].B.PDO


#endif