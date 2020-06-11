#pragma once
#include "../GameMessage/UpGradeLogic.h"
class CGameTaskLogic :public CUpGradeGameLogic
{
public:
	CGameTaskLogic(void);
public:
	~CGameTaskLogic(void);

private:

	BYTE   m_byLastCardList[20] ;  ///���һ���Ƶ��б�
	BYTE   m_byLastCardCount ;     ///���һ���Ƶ�����

	BYTE   m_byBackCardType  ;   ///�������� 

	BYTE   m_byTaskType  ;       ///���������
	BYTE   m_bySpecifyShape ;    ///ָ��������
	BYTE   m_bySpecifyCard ;     ///ָ������

public:
	///��ʼ�������
	void   InitGameTask(void) ; 
	///��ȡ��������
	bool   GetBackCardType(BYTE bbackCardList[] , int  cbCardCount) ;  
    ///�����Ƿ�Ϊ˳��
	bool   IsBackCardStraight(BYTE iCardList[],int iCardCount) ; 
	// ��ȡָ������ֵ�Ƶ�����
	int    GetCardNumCount(BYTE iCardList[], int iCardCount, BYTE bCardNum) ; 
    ///��ȡ�������
	void   GetRandTask(int inDex) ;  ///��ȡ�������
	///    �����������
	void   SetGameTask(GameTaskStruct &gameTask) ; 
	///�������һ���Ƶ�����
	void   SetLastCardData(BYTE iCardList[], BYTE iCardCount)  ;
	///�ж��Ƿ����������
	bool   IsFinishTask(void) ; 
	///��ȡ������
	int   GetTaskMutiple(bool bFinish ) ;
	///��ȡ���Ʊ���
	int   GetBackCardMytiple(void) ; 

};
