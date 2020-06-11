#pragma once

#include "BaseCenterServer.h"
#include "log.h"
#include <map>
#include <unordered_map>

// ��ʱ��ID
enum CenterServerTimerID
{
	CENTER_TIMER_BEGIN = 0,
	CENTER_TIMER_CHECK_REDIS_CONNECTION,			// redis������
	CENTER_TIMER_NORMAL,							// ͨ�ö�ʱ��(s)
	CENTER_TIMER_ROUTINE_CHECK_UNBINDID_SOCKET,		// �����ް�����
	CENTER_TIMER_LOAD_MATCH_INFO,					// ���ر�����Ϣ
	CENTER_TIMER_END,
};

const int CHECK_REDIS_CONNECTION_SECS = 307;		// ���ڼ��redis����(s)
const int ROUTINE_CHECK_UNBINDID_SOCKET = 53;		// ������������(s)
const int NORMAL_TIMER_SECS = 3;					// ͨ�ö�ʱ��(s)
const int CHECK_REDIS_LOAD_MATCH_INFO = 1200;		// ���ر�����Ϣ(s)

//�Ƕ�ʱ��ʱ��
const int CONNECT_TIME_SECS = 20;					// ÿ������������ʱ��

// ���ķ�������ҽṹ
struct CenterUserInfo
{
	int userID;
	UINT socketIdx; //��¼������
	BYTE isVirtual;

	CenterUserInfo()
	{
		userID = 0;
		socketIdx = -1;
		isVirtual = 0;
	}
};

struct ServerBaseInfo
{
	int serverType; // ����������   SERVICE_TYPE_LOGON//����    SERVICE_TYPE_LOADER//��Ϸ
	int serverID;	// ��������logonID����Ϸ����roomID
	ServerBaseInfo()
	{
		serverType = SERVICE_TYPE_BEGIN;
		serverID = 0;
	}
	bool operator<(const ServerBaseInfo &server)const
	{
		if (serverType < server.serverType)
		{
			return true;
		}
		else if (serverType == server.serverType)
		{
			if (serverID < server.serverID)
			{
				return true;
			}
		}
		return false;
	}
};

class CCenterServerManage : public CBaseCenterServer
{
public:
	UINT						m_nPort;				//��½�������˿�
	UINT						m_uMaxPeople;			//֧�����ֲ�ʽ��������

private:
	std::vector<ServerBaseInfo>					m_socketToServerVec;	// socketIdx��������ID��ӳ��(key=socketIdx,value=ServerBaseInfo)
	std::map<ServerBaseInfo, UINT>				m_serverToSocketMap;	// ��������socketIdx��ӳ��  (key=ServerBaseInfo,value=socketIdx)
	std::vector<UINT>							m_logonGroupSocket;		// ��½����Ⱥ(value=socketIdx)
	std::map<int, CenterUserInfo>				m_centerUserInfoMap;	// ��Ⱥϵͳ�������(ֻͳ�Ƶ�¼����)
	std::vector<int>							m_memberUserIDVec;		// ��ʱ����ľ��ֲ���Ա
	time_t										m_lastNormalTimerTime;
	std::map<long long, MatchInfo>				m_timeMatchInfoMap;		// ��ʱ����Ϣ�����ݱ�����ʼʱ������tips�����ж�ʱ��������ʼʱ�䲻����ͬ

public:
	CCenterServerManage();
	CCenterServerManage(CCenterServerManage&);
	CCenterServerManage & operator = (CCenterServerManage &);
	virtual ~CCenterServerManage();

public:
	//���ݹ���ģ������
	virtual bool OnStart();
	//���ݹ���ģ��ر�
	virtual bool OnStop();

	//������չ�ӿں���
private:
	//��ȡ��Ϣ����
	virtual bool PreInitParameter(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData);
	//SOCKET ���ݶ�ȡ
	virtual bool OnSocketRead(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	//SOCKET �ر�
	virtual bool OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, UINT uConnectTime);
	//��ʱ����Ϣ
	virtual bool OnTimerMessage(UINT uTimerID);

public:///////////////////////////////////�����������///////////////////////////////////////
	// �����(��¼��)��������
	bool SendData(int userID, UINT msgID, void* pData, int size, unsigned int mainID, unsigned int assistID, unsigned int handleCode);
	// ���������������
	bool SendData(ServerBaseInfo * pServer, UINT msgID, void* pData, UINT size, int userID = 0, UINT mainID = 0, UINT assistID = 0, UINT handleCode = 0);
	// ���������������
	bool SendData(UINT uScoketIndex, UINT msgID, void* pData, UINT size, UINT handleCode = 0);
	// ֪ͨ��Դ�仯
	void NotifyResourceChange(int userID, int resourceType, long long value, long long changeValue, int reason, int reserveData = 0, BYTE isNotifyRoom = 0);
	// ����¼����������Ⱥ���͵�ǰ��Ⱥ��Ϣ
	void SendDistributedSystemInfo();
	// ����������ʼ�������б�����ҷ�����Ϣ֪ͨ
	void SendNotifyMatchStart(const MatchInfo &matchInfo);
private: /////////////////////////�ڲ�ϵͳ////////////////////////
	// ��ʼ����Ҫ���ڼ����¼�
	void InitRounteCheckEvent();
	// ���redis������
	void CheckRedisConnection();
	// ͨ�ö�ʱ��
	void OnNormalTimer();
	// ��������
	void RoutineCheckUnbindIDSocket();
	// ����
	void OnServerCrossDay();
	// ����
	void OnServerCrossWeek();
	// ����Ƿ�����
	bool IsUserOnline(int userID);
	// ��鶨ʱ��
	void CheckTimeMatch(const time_t &currTime);

private:
	////////////////////////////////����PHP����Ϣ//////////////////////////////////////////
	bool OnHandlePHPMessage(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	// ƽ̨�������
	bool OnHandlePHPNoticeMessage(UINT socketIdx, void* pData, int size);
	// ȫ���ʼ�֪ͨ
	bool OnHandlePHPAllUserMailMessage(UINT socketIdx, void* pData, int size);
	// �ط�
	bool OnHandlePHPCloseServerMessage(UINT socketIdx, void*pData, int size);
	// ����
	bool OnHandlePHPOpenServerMessage(UINT socketIdx, void*pData, int size);
	// ��������
	bool OnHandlePHPHornMessage(UINT socketIdx, void*pData, int size);
	// ��ɢ����
	bool OnHandlePHPDissmissDeskMessage(UINT socketIdx, UINT msgID, void*pData, int size);
	// ֪ͨĳ������Դ�仯
	bool OnHandlePHPNotifyResChangeMessage(UINT socketIdx, int userID, void*pData, int size);
	// ��ĳ�����������Ϣ
	bool OnHandlePHPNotifyOneUserMessage(UINT socketIdx, int userID, void*pData, int size, unsigned int mainID, unsigned int assistID);
	// ����ֲ����������Ϣ
	bool OnHandlePHPNotifyFGMessage(UINT socketIdx, int friendsGroupID, void*pData, int size, unsigned int mainID, unsigned int assistID);
	// ����������
	bool OnHandlePHPSetUserMessage(UINT socketIdx, int userID, void* pData, int size);
	// ֪ͨС���
	bool OnHandlePHPNotifyUserRedspotMessage(UINT socketIdx, int userID, void* pData, int size);
	// ֪ͨ���ֲ�С���
	bool OnHandlePHPNotifyUserRedFGspotMessage(UINT socketIdx, int friendsGroupID, void* pData, int size);
	// ������Ϸ����
	bool OnHandlePHPReloadGameConfigMessage(UINT socketIdx, void* pData, int size);
	// �ֻ�ע����֤�ɹ���ע���˺�
	bool OnHandlePHPPhoneInfoMessage(UINT socketIdx, void* pData, int size);
	// ���˱��������˳�������ʵʱ����
	bool OnHandlePHPSignUpMatchMessage(UINT socketIdx, void*pData, int size);
	// ����ʼ����
	bool OnHandlePHPReqStartMatchMessage(UINT socketIdx, void*pData, int size);
	// �������޸ġ�ɾ����һ����ʱ��
	bool OnHandlePHPReqModifyTimeMatchMessage(UINT socketIdx, void*pData, int size);
	// ��ұ��������˳���������ʱ����
	bool OnHandlePHPTimeMatchPeopleChangeMessage(UINT socketIdx, void*pData, int size);

	////////////////////////////////����ͨ��(���غ���Ϸ��������PHP)��Ϣ//////////////////////////////////////////
	bool OnHandleCommonMessage(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	// Ϊÿ����������socketIdx
	bool OnHandleCommonServerVerifyMessage(void* pData, UINT size, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);


	////////////////////////////////�����������Ϣ//////////////////////////////////////////
	bool OnHandleLogonMessage(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	// ��½����Դ�仯
	bool OnHandleLogonResChangeMessage(int userID, void* pData, UINT size);
	// ���������
	bool OnHandleLogonUserStatusMessage(void* pData, UINT size, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	// ��������¼���������ݣ��ߵ��ɷ��������
	bool OnHandleLogonRepeatUserMessage(CenterServerMessageHead * pCenterHead, void* pData, UINT size, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	// ת����Ϣ��ĳ����
	bool OnHandleLogonRelayUserMessage(NetMessageHead * pNetHead, void* pData, UINT size, int userID);
	// ת����Ϣ�����ֲ�
	bool OnHandleLogonRelayFGMessage(NetMessageHead * pNetHead, void* pData, UINT size, int friendsGroupID);
	// ��������ֲ�������ҷ���С���
	bool OnHandleLogonReqFGRedSpotMessage(void* pData, UINT size, int friendsGroupID);
	// �����ɢ����
	bool OnHandleLogonReqDissmissDeskessage(void* pData, UINT size);


	////////////////////////////////������Ϸ����Ϣ//////////////////////////////////////////
	bool OnHandleLoaderMessage(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	// ��Ϸ����Դ�仯����Һ���ʯ��
	bool OnHandleLoaderResChangeMessage(int userID, void* pData, UINT size);
	// �����б���Ϣ�仯(�����仯)
	bool OnHandleLoaderBuyDeskInfoChangeMessage(void* pData, UINT size);
	// ��Ϸ�����
	bool OnHandleLoaderDeskDissmissMessage(void* pData, UINT size);
	// �����仯
	bool OnHandleLoaderDeskStatusChangeMessage(void* pData, UINT size);
	// ��ұ仯֪ͨ
	bool OnHandleLoaderFireCoinChangeMessage(int userID, void* pData, UINT size);
	// ��֪ͨ
	bool OnHandleLoaderRewardMessage(void* pData, UINT size);
	// ��ʼ����
	bool OnHandleLoaderNotifyStartMatchMessage(void* pData, UINT size);
	// ��ʼ����ʧ��
	bool OnHandleLoaderNotifyStartMatchFailMessage(int userID, void* pData, UINT size);
};
