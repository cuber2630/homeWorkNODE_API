#ifndef _MYPROTOCODE_H
#define _MYPROTOCODE_H

#include "Protocol.h"
#pragma pack(push)
#pragma pack(1)

// �C����l��Ƴ]�w�����c, ���e�ۭq, ��Ƶ��c�W�٤ŧ�, �קK�·�
typedef struct
{       
    int m_Version;                // �����ˬd��, ������s.
    // ============= �]�w��� =================================
    char m_LineTables[100];      // �s�u���, �e�����WNo.1, �k�UNo.15
    int m_Odds[50];             // the bigest odds is m_Odds[0][2]    
} GameInfo_t;

#pragma pack(pop)
#endif