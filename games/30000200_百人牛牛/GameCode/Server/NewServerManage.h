#pragma once

#include "GameDesk.h"
#include "GameDefine.h"
#include "UpGradeLogic.h"
#include <vector>
#include <queue>
#include <list>
#include <array>

#define SYSTEMMONEY 88888888
using namespace brnnProto;

class CNewServerManage : public CGameDesk
{
public:
	CNewServerManage();
	virtual ~CNewServerManage();

public:
	// ����һЩ�麯��
	virtual bool InitDeskGameStation();
	virtual bool OnStart();
	virtual bool OnGetGameStation(BYTE deskStation, UINT uSocketID, bool bWatchUser);
	virtual bool HandleNotifyMessage(BYTE deskStation, unsigned int assistID, void* pData, int size, bool bWatchUser = false);
	virtual bool OnTimer(unsigned int timerID);
	virtual bool ReSetGameState(BYTE bLastStation);
	virtual bool UserLeftDesk(GameUserInfo* pUser);
	virtual bool UserNetCut(GameUserInfo* pUser);
	virtual bool IsPlayGame(BYTE deskStation);
	virtual bool IsHundredGameSystemDealer();//��������Ϸ,�ж��Ƿ�ϵͳ��ׯ��
	virtual bool HundredGameIsInfoChange(BYTE deskStation);
	virtual void UserBeKicked(BYTE deskStation);
	virtual void LoadDynamicConfig();
	// ��Ϣ������
private:
	// ������ׯ
	bool OnHandleUserRequestShangZhuang(BYTE deskStation, void* pData, int size);
	// ������ׯ
	bool OnHandleUserRequestXiaZhuang(BYTE deskStation);
	// ������ע
	bool OnHandleUserReuqestNote(BYTE deskStation, void* pData, int size);
	// ������ѹ
	bool OnHandleRequestContinueNote(BYTE deskStation);
	// ��������
	bool OnHandleRequestAreaTrend(BYTE deskStation);
	// ���󳬶�����
	bool OnHandleRequestSuperSet(BYTE deskStation, void* pData, int size);
	// ��������
	bool OnHandleRequestSit(BYTE deskStation, void* pData, int size);

	// ��ʱ�����
private:
	void OnTimerWaitNext();
	void OnTimerNote();
	void OnTimerSendNoteInfo();
	void OnTimerSendCard();
	void OnTimerCompare();
	void OnTimerSettle();

	// ��������
private:
	long long GetNoteByIndex(int index);
	BYTE PopCardFromDesk();
	bool SendAreaCards(BYTE* pCardList);
	void KillAllTimer();
	bool ConfirmDealer();
	time_t GetNextStageLeftSecs();
	int GetRandNum();

private:
	// �ȴ���ʼ�׶�
	void TransToWaitNextStage();
	// ��ע�׶�
	void TransToNoteStage();
	// ���ƽ׶�
	void TransToSendCardStage();
	// ���ƽ׶�
	void TransToCompareStage();
	// �����׶�
	void TransToSettleStage();

private:
	// ֪ͨ��Ϸ״̬
	void BroadcastGameStatus();
	// ֪ͨׯ��Ϣ
	void BroadcastZhuangInfo();
	// ֪ͨ����(��ע)��Ϣ
	void BrodcastNoteInfo(BYTE deskStation = 255);
	// ���ׯ��Ϣ(ׯ����Ϣ+��ׯ�б�)
	bool MakeZhuangInfo(NewGameMessageNtfZhuangInfo& msg);
	// ��䷢����Ϣ
	bool MakeSendCardInfo(NewGameMessageNtfSendCard& msg);
	// ����������Ӯ��Ϣ
	void CalcAreaRate();

	// ����ׯ��
private:
	// ����ϵͳ������Դ
	void ChangeSystemZhuangInfo();
	// ɾ����ׯ�б�ĳ�����
	void DelShangZhuangUser(int userID);
	// �ж�ĳ������Ƿ�����ׯ�б���
	bool IsShangZhuangList(int userID);
	// ����ׯ���б�
	void ClearShangZhuangList(int userID, BYTE deskStation);

public:
	/////////////////////////////////////////��ֵ/////////////////////////////////
///��������Ӯ�Զ�����
	void AiWinAutoCtrl();
	//��ȡ���ֿ����仹�ǿ���Ӯ��1������Ӯ��-1�������䣬0��������
	int GetCtrlRet();
	//���㵱ǰϵͳӮǮ
	long long CalcSystemWinMoney(int iRateValue);
	//�õ���СӮǮ
	void GetMinWinMoney(int iRateValue, BYTE byAllCard[MAX_AREA_COUNT + 1][AREA_CARD_COUNT], int iWinArry[][5], long long llWinMoney[], int &iCount, int iCtrlType);
	//ִ�г�������
	void SuperSetChange();
	//���������������
	void ExchangeCard(BYTE area1, BYTE area2);

	//�����˿��ƽӿ�
private:
	// �жϻ������Ƿ��������ׯ  0����ׯ   1����ׯ
	bool IsVirtualCanShangXiazZhuang(BYTE deskStation, BYTE type);
	// ������ע
	bool IsVirtualCanNote(BYTE deskStation, long long note);
	// �жϻ������Ƿ����������վ��  0������   1��վ��
	bool IsVirtualCanSit(BYTE deskStation, BYTE type);

	//��λ���
private:
	bool IsDeskUser(BYTE deskStation);
	bool DelDeskUser(BYTE deskStation);
	bool SetDeskUser(BYTE deskStation, BYTE byIndex);
	bool IsDeskHaveUser(BYTE byIndex);
	void ClearDeskUser();
	//ÿ�ֿ��������Ļ�������
	void UpSitPlayerCount();
	std::vector<BYTE> m_gameDesk;

private:
	GameConfig m_gameConfig;				// ��Ϸ����

private:
	CPokerGameLogic m_logic;
	std::vector<BrnnUserInfo> m_userInfoVec;			// �����Ϣ����(������ׯ��)
	DealerInfo m_dealerInfo;							// ׯ����Ϣ
	std::list<ShangZhuangUser> m_shangZhuangList;		// ��ׯ�б�
	std::array<AreaInfo, MAX_AREA_COUNT> m_areaInfoArr;	// ������Ϣ
	std::queue<BYTE> m_deskCardQueue;					// ���ϵ���
	int m_gameStatus;									// ��Ϸ״̬
	BYTE m_dealerDeskStation;							// ׯ��λ��(254Ϊϵͳ)
	int m_currDealerGameCount;							// ��ǰׯ�ҽ��еľ���(���������Ч)
	bool m_xiaZhuangRightNow;							// �Ƿ�������ׯ
	time_t m_startTime;									// "��ʼ"ʱ��(�ȴ���ʼ�׶ο�ʼ��ʱ)

	ChouMaInfo m_tempChouMaData[MAX_CHOU_MA_COUNT];		// ����ͳ��
	int m_tempChouMaDataCount;							// ��������
	int m_areaChouMaCount[MAX_AREA_COUNT][MAX_NOTE_SORT];	// ÿ�������������

	//////////////////////////////////ׯ����Ϣ////////////////////////////////////////
	long long m_dealerResNums;
	char m_dealerName[64];
	char m_dealerHeadURL[256];

	//////////////////////////////////��ֵ����������//////////////////////////////////
	bool					m_bIsSuper[PLAY_COUNT];			// �Ƿ��ǳ���
	BYTE m_bySuperSetArea;								// ������������255:Ĭ��ֵ��0��ׯΪ��1���м�1������
	BYTE m_bySuperSetType;								// �����������ͣ�0��Ĭ��ֵ��1��Ӯ��2����
	std::vector<CtrlSystemParam>	m_ctrlParam;		// һ����Ʋ���ֵ
	int m_curCtrlWinCount;
	int m_runCtrlCount;
	int m_ctrlParamIndex;
	char m_szLastCtrlParam[256];
};