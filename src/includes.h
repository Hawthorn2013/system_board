#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#ifndef _TYPEDEF_BYTE_
#define _TYPEDEF_BYTE_
typedef unsigned char BYTE;
#endif

#ifndef _TYPEDEF_SBYTE_
#define _TYPEDEF_SBYTE_
typedef signed char SBYTE;
#endif

#ifndef _TYPEDEF_WORD_
#define _TYPEDEF_WORD_
typedef unsigned short WORD;
#endif

#ifndef _TYPEDEF_SWORD_
#define _TYPEDEF_SWORD_
typedef signed short SWORD;
#endif

#ifndef _TYPEDEF_DWORD_
#define _TYPEDEF_DWORD_
typedef unsigned int DWORD;
#endif

#if 0
#ifndef _TYPEDEF_byte_
#define _TYPEDEF_byte_
typedef unsigned char byte;
#endif

#ifndef _TYPEDEF_word_
#define _TYPEDEF_word_
typedef unsigned short word;
#endif
#endif

#include "MPC5604B_M27V.h"
#include "init.h"
#include "control.h"
#include "supersonic.h"
#include "sdcard.h"
#include "ff.h"
#include "time.h"
#include "lcd.h"
#include "WiFi.h"
#include "UART.h"
#include "L3G4200D.h"
#include "RFIDCard.h"
#include "mag.h"
#include "drifting.h"

///****************LED****************///
#if 1
#define D0 SIU.GPDO[12].B.PDO
#define D1 SIU.GPDO[13].B.PDO
#define D2 SIU.GPDO[14].B.PDO
#define D3 SIU.GPDO[15].B.PDO
#endif


#endif