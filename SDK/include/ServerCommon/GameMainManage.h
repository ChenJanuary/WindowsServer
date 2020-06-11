#pragma once

#include <map>
#include "BaseMainManage.h"
#include "gameUserManage.h"
#include "GameDesk.h"
#include "TCPSocket.h"
#include "GameDataBaseHandle.h"
#include "tableDefine.h"
#include "GameLogManage.h"
#include "log.h"
#include "configManage.h"

#pragma comment(lib, "json_vc71_libmt.lib")

using namespace std;

#define TIME_SPACE						50			//��Ϸ ID ���
#define TIME_START_ID					100			//��ʱ����ʼ ID

#define OFFLINE_CHANGE_AGREE_STATUS					//���߸ı�׼��״̬��ע�͵����߲��ı�׼��״̬

#define G_CHANGEDESK_MAX_COUNT			12			//��Ϸ�����������������

// ��ʱ��ID
enum LoaderServerTimerID
{
	LOADER_TIMER_BEGIN = 0,
	LOADER_TIMER_SAVE_ROOM_PEOPLE_COUNT,		// д�뵱ǰ��������
	LOADER_TIMER_CHECK_REDIS_CONNECTION,		// ���redis������
	LOADER_TIMER_CHECK_INVALID_STATUS_USER,		// �����Ч״̬�����
	LOADER_TIMER_CHECK_TIMEOUT_DESK,			// ��鳬ʱ������
	LOADER_TIMER_COMMON_TIMER,					// ͨ�ö�ʱ��
	LOADER_TIMER_HUNDRED_GAME_START,			// ��������Ϸ����֪ͨ�¼�(һ����)
	LOADER_TIMER_SCENE_GAME_START,				// ��������Ϸ����֪ͨ�¼�(һ����)
	LOADER_TIMER_COMBINE_DESK_GAME_BENGIN,		// ������Ϸ��ʼ��ʱ��
	LOADER_TIMER_END,
};

const int CHECK_SAVE_ROOM_PEOPLE_COUNT = 37;		// ���ڱ�����Ϸ����
const int CHECK_REDIS_CONNECTION_SECS = 307;		// ���redis������ʱ��
const int CHECK_INVALID_STATUS_USER_SECS = 67;		// �����Ч״̬����Ҽ��
const int CHECK_TIMEOUT_DESK_SECS = 127;			// ��鳬ʱ���ӵ�ʱ��
const int CHECK_COMMON_TIMER_SECS = 3;				// ͨ�ö�ʱ��ʱ��
const int CHECK_COMBINE_DESK_GAME_BENGIN_SECS = 3;	// ������Ϸ��ʼ��ʱ�����޸����ʱ����Կ������ƥ���ٶȣ�

//�������ݹ�����
class CGameDesk;
class CBaseMainManage;
class CGameUserManage;
class KERNEL_CLASS CGameMainManage : public CBaseMainManage
{
public:
	CGameMainManage();
	virtual ~CGameMainManage();
	CGameMainManage(CGameMainManage&) {}
	CGameMainManage& operator=(CGameMainManage&);

private:
	//���ݹ���ģ���ʼ��
	virtual bool OnInit(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData);
	//���ݹ���ģ��ж��
	virtual bool OnUnInit();
	//���ݹ���ģ������
	virtual bool OnStart();
	//���ݹ���ģ��ر�
	virtual bool OnStop();
	//���ݹ���ģ��ˢ��
	virtual bool OnUpdate();

private:
	//������չ�ӿں��� ���������̵߳��ã�
	//SOCKET ���ݶ�ȡ
	virtual bool OnSocketRead(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	//SOCKET �ر�
	virtual bool OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, UINT uConnectTime);
	//�첽�̴߳�����
	virtual bool OnAsynThreadResult(AsynThreadResultLine * pResultData, void * pData, UINT uSize);
	//��ʱ����Ϣ
	virtual bool OnTimerMessage(UINT uTimerID);

public:
	// ��������idȡ����ָ�����
	virtual CGameDesk* GetDeskObject(int deskIdx);

private:
	//��ȡ��Ϸ������Ϣ
	virtual CGameDesk * CreateDeskObject(UINT uInitDeskCount, UINT & uDeskClassSize) = 0;
	void KillAllTimer();
	//��ʼ����Ϸ��
	bool InitGameDesk(UINT uDeskCount, UINT	uDeskType);

private:
	//��ҵ���
	bool OnUserSocketClose(int userID, UINT uSocketIndex);

private:
	////////////////////////////////////////////////
	// ��¼���
	bool OnHandleLogonMessage(unsigned int assistID, void* pData, int size, unsigned int accessIP, unsigned int socketIdx, int userID);
	// ��ҵ�½
	bool OnUserRequestLogon(void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, int userID);

	//��������¼��
	bool OnPrivateLogonLogic(void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, int userID);
	//��ҳ���¼�߼�
	bool OnMoneyLogonLogic(void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, int userID);
	//��������½�߼�
	bool OnMatchLogonLogic(void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, int userID);

	// ��ҵǳ�
	bool OnUserRequestLogout(void* pData, UINT size, ULONG uAccessIP, UINT uIndex, int userID);

	//////////////////////////////////////////////
	// �������
	bool OnHandleActionMessage(int userID, unsigned int assistID, void* pData, int size);
	// �������
	bool OnHandleUserRequestSit(int userID, void* pData, int size);
	// ��Ҵ�������
	bool OnHandleUserRequestMatchSit(int userID, void* pData, int size);
	// ���վ��
	bool OnHandleUserRequestStand(int userID, void* pData, int size);
	// ����������ȡǮ
	bool OnHandleRobotRequestTakeMoney(int userID, void* pData, int size);
	// ������Ϸ��ʼ
	bool OnHandleUserRequestGameBegin(int userID);
	// �����������
	bool OnHandleUserRequestCombineSit(int userID, void* pData, int size);
	// ���ȡ����������
	bool OnHandleUserRequestCancelSit(int userID, void* pData, int size);
	///////////////////////////////////////////////
	// �����Ϣ
	bool OnHandleFrameMessage(int userID, unsigned int assistID, void* pData, int size);

	///////////////////////////////////////////////
	// ��Ϸ��Ϣ
	bool OnHandleGameMessage(int userID, unsigned int assistID, void* pData, int size);
	// ��ɢ��Ϣ
	bool OnHandleDismissMessage(int userID, unsigned int assistID, void* pData, int size);
	// �����������
	bool OnHandleVoiceAndTalkMessage(int userID, unsigned int assistID, void* pData, int size);

	//////////////////////////////////////////////////////////////////////////
	//��������Ϣ
	bool OnHandleMatchMessage(int userID, unsigned int assistID, void* pData, int size, long handleID);
	//������������״̬
	bool OnHandleMatchAllDeskStatusMessage(int userID, void* pData, int size, long handleID);
	//�Թ���������
	bool OnHandleMatchEnterWatchDeskMessage(int userID, void* pData, int size);
	//�˳��Թ�
	bool OnHandleMatchQuitWatchDeskMessage(int userID, void* pData, int size);

public: //���ķ�������Ϣ
	bool SendMessageToCenterServer(UINT msgID, void* pData, UINT size, int userID = 0);
	// ֪ͨ������Դ�仯����Һ���ʯ��
	void SendResourcesChangeToLogonServer(int userID, int resourceType, long long value, int reason, long long changeValue);
	// ֪ͨ������Դ�仯����ң�
	void SendFireCoinChangeToLogonServer(int friendsGroupID, int userID, long long value, int reason, long long changeValue);
	// ֪ͨȫ���󽱻��߹���
	void SendRewardActivityNotify(const char * rewardMsg);

	//���ķ���Ϣ
	virtual bool OnCenterServerMessage(UINT msgID, NetMessageHead * pNetHead, void* pData, UINT size, int userID);
	// ��Դ�仯
	bool OnCenterMessageResourceChange(void* pData, int size, int userID);
	// ����Ա��ɢ����
	bool OnCenterMessageFGDissmissDesk(void* pData, int size);
	// �����б��ɢ����
	bool OnCenterMessageMasterDissmissDesk(void* pData, int size);
	// �ط�����ɢ��������
	void OnCenterCloseServerDissmissAllDesk(void* pData, int size);
	// ���¼�����������
	bool OnCenterMessageReloadGameConfig(void* pData, int size);
	// ��ʼ����(ʵʱ��)
	bool OnCenterMessageStartMatchPeople(void* pData, int size);
	// ��ʼ����(��ʱ��)
	bool OnCenterMessageStartMatchTime(void* pData, int size);

	// �ӿں���
public:
	void SendData(int userID, void* pData, int size, unsigned int mainID, unsigned int assistID, unsigned int handleCode);
	//����֪ͨ��Ϣ
	bool SendErrNotifyMessage(int userID, LPCTSTR lpszMessage, int wType);
	// ��ȡ���
	GameUserInfo* GetUser(int userID);
	// ��ҵǳ�
	bool OnUserLogout(int userID);
	// ɾ�����
	void DelUser(int userID);
	// ��ȡroomID
	int GetRoomID();
	// �Ƴ��������
	bool RemoveOfflineUser(int userID);
	// �Ƴ��Թ����
	bool RemoveWatchUser(int userID);
	// ��ȡ��������
	int GetRoomType();
	// ��ȡ���״̬
	int  GetUserPlayStatus(int userID);
	// �������״̬
	bool SetUserPlayStatus(int userID, int status);
	// ��ȡ�����������
	int GetOnlineUserCount();
	// �Ƿ����Թ�
	bool IsCanWatch();
	// �Ƿ���������Ϸ
	bool IsCanCombineDesk();
	// �Ƿ������Ϸ����һ����Ϸid
	bool IsMultiPeopleGame();
	// ֪ͨ��ҳ�Ǯ
	void NotifyUserRechargeMoney(int userID, long long rechargeMoney, int leftSecs);
	// �ж�ĳ������Ƿ����ο�
	bool IsVisitorUser(int userID);
	// ������Ԥ���Ƿ�������
	bool GoldRoomIsHaveDeskstation();
	// ���ؽ�������
	bool LoadPoolData();
	// ��ȡ�������ã�û������Ĭ�Ͼ���0
	int GetPoolConfigInfo(const char * fieldName);
	// ��ȡ�������ã���Ҫ�ǻ�ȡ��������
	bool GetPoolConfigInfoString(const char * fieldName, int * pArray, int size, int &iArrayCount);
	// �Ƿ���1��1ƽ̨
	bool IsOneToOnePlatform();

	// ��ʱִ�к���
private:
	// �����Ч״̬�����
	void OnTimerCheckInvalidStatusUser();
	// ��鳬ʱ������
	void CheckTimeOutDesk();
	// ͨ�ö�ʱ��
	void OnCommonTimer();
	// ���redis������
	void CheckRedisConnection();
	// ��鳬ʱû��׼�������
	void CheckTimeoutNotAgreeUser();
	// ��鳬ʱ���������
	void CheckTimeoutNotOperateUser();
	// ��������Ϸ����֪ͨ
	void OnHundredGameStart();
	// ���ڱ��浱ǰ��������
	void OnSaveRoomPeopleCount();
	// ��������Ϸ����֪ͨ
	void OnSceneGameStart();
	// Ϊ�Ŷ��б����ƥ�����ӣ�����ʼ��Ϸ
	void OnCombineDeskGameBegin();

	// ������غ���
private:
	// �����ҵ���������
	bool AddCombineDeskUser(int userID, BYTE isVirtual);
	// ������ɾ�����
	bool DelCombineDeskUser(int userID, BYTE isVirtual);

	// ������
public:
	// Ϊ��������һ������������
	bool GetMatchDesk(int needDeskCount, std::list<int> &listDesk);
	// Ϊ������Ա�������ӣ�������
	void AllocDeskStartMatch(const std::list<int> &matchDeskList, const std::vector<MatchUserInfo> &vecPeople);
	// ��ʱ�����Կ�ʼ����������
	void CheckDeskStartMatch();
	// �Ƿ�ȫ�����Ӷ���ɱ���
	bool IsAllDeskFinishMatch(long long llPartOfMatchID);
	// ĳһ��������Ϸ����
	void MatchDeskFinish(long long llPartOfMatchID, int deskIdx);
	// ����������һ��
	void MatchNextRound(long long llPartOfMatchID, int iCurMatchRound, int iMaxMatchRound);
	// ��������
	void MatchEnd(long long llPartOfMatchID, int iCurMatchRound, int iMaxMatchRound);
	// ���ݻ��֣�����û����̭�����
	int MatchSortUser(std::vector<MatchUserInfo> &vecPeople, int iCurMatchRound);
	// ��ȡһ����ȷ�Ļ����˲μӱ���
	int MatchGetRobotUserID();
	// ���ͱ����ʼ�����
	void SendMatchGiftMail(int userID, int gameID, BYTE matchType, int gameMatchID, int ranking);
	// ���ͱ���ʧ�ܣ��˱����ѣ��Լ��������״̬
	void SendMatchFailMail(BYTE failReason, int userID, BYTE matchType, int gameMatchID);
	// �������״̬
	void ClearMatchStatus(BYTE failReason, int userID, BYTE matchType, int gameMatchID);
	// �����������ڴ�
	void ClearMatchUser(int gameMatchID, const std::vector<MatchUserInfo> &vecPeople);

private:
	//ƥ��������ض���
	std::set<int>		m_combineUserSet;					// ������Ҽ��ϣ����ٲ�ѯ��
	std::vector<int>	m_combineRealUserVec;				// ������ң����ˣ����ϣ�ƥ��������
	std::vector<int>	m_combineRobotUserVec;				// ������ң������ˣ����ϣ�ƥ��������
	std::vector<int>	m_allCombineDeskUserVec;			// �����˵ļ���

	//���������
public:
	std::map<long long, std::list<int> >				m_matchGameDeskMap;	// ÿ����������������������
	std::map<long long, std::vector<MatchUserInfo> >	m_matchUserMap;		// ÿ�����������������
	std::map<long long, int>							m_matchMainIDMap;	// ÿ�������������Ĵ�ID
	std::map<long long, BYTE>							m_matchTypeMap;		// ÿ���������������� MatchType

public:
	UINT				m_uNameID;							// ��Ϸ���� ID ����
	RewardsPoolInfo		m_rewardsPoolInfo;					// �����ν�������
protected:
	CGameUserManage*	m_pGameUserManage;					// ��ҵĹ������
	UINT				m_uDeskCount;						// ��Ϸ����Ŀ
	CGameDesk			* * m_pDesk;						// ��Ϸ��ָ��
	CGameDesk			* m_pDeskArray;						// ��Ϸ��ָ��
};

//��Ϸ���ݹ�����ģ��
template <class GameDeskClass, UINT uBasePoint, UINT uLessPointTimes> class CGameMainManageTemplate : public CGameMainManage
{
public:
	CGameMainManageTemplate() {}
	virtual ~CGameMainManageTemplate() {}

private:
	//��ȡ��Ϣ���� ���������أ�
	virtual bool PreInitParameter(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)
	{
		//����ʹ������
		pKernelData->bStartTCPSocket = TRUE;

		//�������ݿ���Ϣ
		pKernelData->bLogonDataBase = TRUE;
		pKernelData->bNativeDataBase = TRUE;
		pKernelData->bStartSQLDataBase = TRUE;

		//������Ϸ��Ϣ
		pKernelData->uNameID = pInitData->uNameID;
		GameBaseInfo* pGameBaseInfo = ConfigManage()->GetGameBaseInfo(pInitData->uNameID);
		pKernelData->uDeskPeople = pGameBaseInfo->deskPeople;

		pKernelData->uMinDeskPeople = pKernelData->uDeskPeople;

		//��������
		if ((pInitData->uDeskCount*pGameBaseInfo->deskPeople + 50) < pInitData->uMaxPeople)
		{
			pInitData->uMaxPeople = pInitData->uDeskCount*pGameBaseInfo->deskPeople + 50;
		}

		//���ڱ�����
		if (lstrcmp(pInitData->szLockTable, TEXT("N/A")) == 0) pInitData->szLockTable[0] = 0;
		if (lstrcmp(pInitData->szIPRuleTable, TEXT("N/A")) == 0) pInitData->szIPRuleTable[0] = 0;
		if (lstrcmp(pInitData->szNameRuleTable, TEXT("N/A")) == 0) pInitData->szNameRuleTable[0] = 0;

		return true;
	};

	//��ȡ��Ϸ������Ϣ
	virtual CGameDesk* CreateDeskObject(UINT uInitDeskCount, UINT & uDeskClassSize)
	{
		uDeskClassSize = sizeof(GameDeskClass);
		return new GameDeskClass[uInitDeskCount];
	};
};

/*******************************************************************************************************/
//��Ϸģ����ģ��
template <class GameDeskClass, UINT uBasePoint, UINT uLessPointTimes> class CGameModuleTemplate : public IModuleManageService
{
public:
	CGameDataBaseHandle														m_DataBaseHandle;	//���ݿ⴦��ģ��
	CGameMainManageTemplate<GameDeskClass, uBasePoint, uLessPointTimes>		m_GameMainManage;	//��Ϸ���ݹ���

public:
	CGameModuleTemplate() {}
	virtual ~CGameModuleTemplate() {}

public:
	virtual bool InitService(ManageInfoStruct * pInitData)
	{
		KernelInfoStruct KernelData;
		return m_GameMainManage.Init(pInitData, &m_DataBaseHandle);
	}

	virtual bool UnInitService() { return m_GameMainManage.UnInit(); }
	virtual bool StartService(UINT &errCode) { return m_GameMainManage.Start(); }
	virtual bool StoptService() { return m_GameMainManage.Stop(); }
	virtual bool DeleteService() { delete this; return true; }
	virtual bool UpdateService() { return m_GameMainManage.Update(); }
};
