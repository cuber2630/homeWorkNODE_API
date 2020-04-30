

#include "GameBase.h"
#include <stdio.h>
//extern ChnMathBase *NewMath();
//extern    void DelMath(ChnMathBase *);
//extern    void GetVersion(char *);
//extern    int NewCreateUser(char *recdata);
//========================================================================
// �H�U���禡 �bclass�٨S������e, ���F���o static ���(GameInfo).
GameInfo_t GameBase::m_GameInfo;
ChnMathBase *NewMath()
{
    return new GameBase();
}

void DelMath( ChnMathBase *math)
{
    delete math;
}

void GetVersion(char *version)
{
    GameBase::GetVersion(version);// static ���.
}

int NewCreateUser(char *recordData)
{
    GameBase::InitialGameInfo();  //�ح����O static ���
//
    return GameBase::NewCreateUser( recordData);
	// return 0;
}

//======================================
GameBase::GameBase(int num)
{	
	GameBase::showNumber = num;
}

GameBase::GameBase()
{	
	//GameBase::showNumber = 0;
}

GameBase::~GameBase()
{
	
}

//
void GameBase::InitialGameInfo()
{
	// multi-threa prevant multi-times initial.
    if((m_GameInfo.m_Version%10000) == (GAME_VERSION_DATA%10000))
	{
		//�̦h�Q��ơA����j��2147483647
		//10�X 2�~.2��.2��.4����(�睊���~�|�k�s)
		m_GameInfo.m_Version = GAME_VERSION_DATA;	//��s���
		return;
	}
	m_GameInfo.m_Version = GAME_VERSION_DATA;     //��s����A�P����
}


// �� server �Ϊ�, m_GameInfo ���@�˪� ���v�v  �Цۦ�ק�
void GameBase::GetVersion(char *version) 
{
    sprintf(version, "%d", m_GameInfo.m_Version);
}

int GameBase::NewCreateUser(char *recordData) 
{
    return 0;
}


// �`:������, server ��, �i�P�_�O�_�ݭn��s.
void GameBase::GetMathInfo(char *data)
{

}

// �`:������, server ��, �i�P�_�O�_�ݭn��s.
void GameBase::UserLogIn(char *data)
{

}

// �`:������, server ��, �i�P�_�O�_�ݭn��s.
long long GameBase::GameBets(char *data)
{
	SYSTEM_DATA sys_data={0};
	GameLoop(&sys_data);
	return 1;
}

int GameBase::GameLoop(SYSTEM_DATA *sys_data)
{
	GameBase::showNumber++;
	return 1;
}
