
#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#define  MAXIMUM_CLIENT_INPUT_BUF_SIZE		256
#define  MAXIMUM_USER_BUFFER_SIZE           10240
#define  MAXIMUM_USER_RECORD_DATA_SIZE			16384
#define  MAXIMUM_HTMLASC_SYNC_DATA_SIZE     10240

#define  MAXIMUM_BET_TABLE    30

typedef struct
{
    int a;
    short b;
	short c;               // 0=false, 1=true
}   GameGameData_t;
    
typedef struct
{
    int userId;
    short userSeat;
	short demo;               // 0=false, 1=true
	short rtp;           // 12345 = 123.45%(1/10000) default payoutrate, 不會變動的. mik4 20180302
    short otherPool;         // 1000 = 10%
	short line;           // line count
    short betTableLength;
	long long betTable[MAXIMUM_BET_TABLE];  
    GameGameData_t gameData; 
}   GameConfig_t;


#endif