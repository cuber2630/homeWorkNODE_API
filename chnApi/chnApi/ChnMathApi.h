#ifndef _CHNMATHAPI_H
#define _CHNMATHAPI_H

#include    "ChnMathBase.h"

typedef struct
{
	int         is_finish;      // �u��1�M0 1��@�⵲��,0��@�⥼����
	long long       bet;	        //�o���n���h�ֿ�
	long long       win;            //�o��Ĺ�h�ֿ��n�j�󵥩�0
	long long       cmd_result;     //1�~�⦨�\,��L����
    //GameMiscellaneous_t gmt;    // jp rtp �p���I demo ���]�w.
	char        jChanceData[MAXIMUM_USER_RECORD_DATA_SIZE];   //�n�s�����v���
	char        out_chancedata[MAXIMUM_HTMLASC_SYNC_DATA_SIZE];//�n�Ǩ�Client�����
    char        recordOrder[MAXIMUM_USER_RECORD_DATA_SIZE];   // �`����
} BetResult;

class ChnMathApi 
{
public:
	ChnMathApi();
	~ChnMathApi();

    void GetVersion(char *version);
	void CreateNewChance(char *recordData);
	void UserLogIn(char *cd, long long credit, char *recordBuf, char *infoData);
	void Bet(const char * in, long long totalbet, long long rngValue, const char * inUserInfo, long long credit, BetResult *res);
	//void Bet(const char * in, long long totalbet, long long rngValue, const char * inUserInfo, long long credit, char *res);
	
};

#endif
