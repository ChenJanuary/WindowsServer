#pragma once

#include "BaseLogonServer.h"
#include "LogonUserManage.h"
#include "LogonGServerManage.h"
#include "log.h"
#include <map>

// ��ʱ��ID
enum LogonServerTimerID
{
	LOGON_TIMER_BEGIN = 0,
	LOGON_TIMER_CHECK_HEARTBEAT,				// ������ʱ��
	LOGON_TIMER_CHECK_REDIS_CONNECTION,			// redis������
	LOGON_TIMER_ROUTINE_CHECK_UNBINDID_SOCKET,	// ���ڼ��û�а����ID����Ч����
	LOGON_TIMER_ROUTINE_SAVE_REDIS,				// ���ڴ洢redis����
	LOGON_TIMER_NORMAL,							// ͨ�ö�ʱ��(s)
	LOGON_TIMER_END,
};

const int CHECK_HEAETBEAT_SECS = 15;				// ������ʱ��ʱ��(s)
const int CHECK_REDIS_SAVE_DB = 61;					// ���ڴ洢redis����(s)
const int CHECK_REDIS_CONNECTION_SECS = 307;		// ���ڼ��redis����(s)
const int ROUTINE_CHECK_UNBINDID_SOCKET = 41;		// ���ڼ��δ��¼������(s)
const int NORMAL_TIMER_SECS = 2;					// ͨ�ö�ʱ��(s)

// ��½��socket
struct LogonServerSocket
{
	BYTE type;			// �������͵�socket 1����ҵ�socket��2����Ϸ����socket
	int identityID;		// ���id����roomID

	LogonServerSocket()
	{
		type = LOGON_SERVER_SOCKET_TYPE_NO;
		identityID = 0;
	}

	LogonServerSocket(BYTE type, int identityID)
	{
		this->type = type;
		this->identityID = identityID;
	}
};

class CGameLogonManage : public CBaseLogonServer
{
public:
	UINT						m_nPort;					//��½�������˿�
	UINT						m_uMaxPeople;				//֧���������������gserver������

private:
	CLogonUserManage*				m_pUserManage;			// ��ҹ�����
	CLogonGServerManage*			m_pGServerManage;		// ��Ϸ��������
	std::vector<LogonServerSocket>	m_socketInfoVec;		// socket������identityID��ӳ���
	std::vector<int>				m_buyRoomVec;
	time_t							m_lastNormalTimerTime;
	time_t							m_lastSendHeartBeatTime;// �ϴη�������ʱ��

private:
	std::set<UINT>					m_scoketMatch;			// �ڱ��������ҳ������

public:
	CGameLogonManage();
	CGameLogonManage(CGameLogonManage&);
	CGameLogonManage & operator = (CGameLogonManage &);
	virtual ~CGameLogonManage();

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
	virtual bool OnSocketRead(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	//SOCKET �ر�
	virtual bool OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, UINT uConnectTime);
	//�첽�̴߳�����
	virtual bool OnAsynThreadResult(AsynThreadResultLine * pResultData, void * pData, UINT uSize);
	//��ʱ����Ϣ
	virtual bool OnTimerMessage(UINT uTimerID);

	// ��½֪ͨ
private:
	void NotifyUserInfo(const UserData &userData);
private:
	// ���ע�����
	bool OnHandleUserRegister(unsigned int assistID, void* pData, int size, unsigned long accessIP, unsigned int socketIdx, long handleID);

	// ��ҵ�½���
	bool OnHandleUserLogonMessage(int assistID, void* pData, int size, unsigned long accessIP, unsigned int socketIdx, long handleID);

	bool OnHanleUserLogon(void* pData, int size, unsigned long accessIP, unsigned int socketIdx, long handleID);

	// �������
	bool OnHandleGameDeskMessage(int assistID, void* pData, int size, unsigned long accessIP, unsigned int socketIdx, long handleID);

	bool OnHandleUserBuyDesk(int userID, void* pData, int size);
	bool OnHandleUserEnterDesk(int userID, void* pData, int size);

	// �������
	bool OnHandleOtherMessage(int assistID, void* pData, int size, unsigned long accessIP, unsigned int socketIdx, long handleID);

	//ˢ�¸�����Ϣ
	bool OnHandleUserInfoFlush(int userID, void* pData, int size);
	//������ȡǮ
	bool OnHandleRobotTakeMoney(int userID, void* pData, int size);
	// ���������Ϣ
	bool OnHandleReqUserInfo(int userID, void* pData, int size);
	// ������������ҳ��
	bool OnHandleJoinMatchScene(UINT uIndex);
	// �����˳�������ҳ��
	bool OnHandleExitMatchScene(UINT uIndex);

	//////////////////////////////��Ϸ�����////////////////////////////////////////////
	// ��֤
	bool OnHandleGServerVerifyMessage(void* pData, int size, unsigned int socketIdx);
	// ǰ�� ----> ��Ϸ��
	bool OnHandleGServerToGameMessage(int userID, NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	// ��Ϸ�� ----> ǰ��
	bool OnHandleGServerToUserMessage(int roomID, NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);

public:
	// ͨ��socketIdx��ȡsocket��Ϣ
	LogonServerSocket GetIdentityIDBySocketIdx(int socketIdx);
	// ɾ��socketIdx����
	void DelSocketIdx(int socketIdx);
	// ����ҷ�������
	bool SendData(int userID, void* pData, int size, unsigned int mainID, unsigned int assistID, unsigned int handleCode, unsigned int uIdentification = 0);
	// �㲥ȫ����ң�������gserver��
	bool SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT bAssistantID, UINT uHandleCode);
	// ֪ͨ��Դ�仯, valueΪ��ֵ�����Ǳ仯ֵ
	void NotifyResourceChange(int userID, int resourceType, long long value, int reason, long long changeValue);
	// �����ķ�����������Ϣ
	bool SendMessageToCenterServer(UINT msgID, void* pData, UINT size, int userID = 0, UINT mainID = 0, UINT assistID = 0, UINT handleCode = 0);
public:
	// �ж�roomID�ķ������Ƿ����
	bool IsRoomIDServerExists(int roomID);
	// ɾ���������ɵ���Ч�ļ�
	void DeleteExpireFile();
	// ����Ƿ�����
	bool IsUserOnline(int userID);
	// �Զ�����
	bool AutoCreateRoom(const SaveRedisFriendsGroupDesk &deskInfo);
	// �жϴ�ip�Ƿ����ע��
	bool IsIpRegister(const OtherConfig &otherConfig, const char * ip);
	// �ж�ĳ������Ƿ����ο�
	bool IsVisitorUser(int userID);
	// ��ȡ���ͷ��
	std::string GetRandHeadURLBySex(BYTE sex);
private:
	// ��ʼ����Ҫ���ڼ����¼�
	void InitRounteCheckEvent();
	// ���redis������
	void CheckRedisConnection();
	// �������
	void CheckHeartBeat(time_t llLastSendHeartBeatTime, int iHeartBeatTime);
	// ��ʱ�洢redis���ݵ�DB updateAll=�Ƿ�ȫ������
	void RountineSaveRedisDataToDB(bool updateAll);
	// ���ڼ��û�а����ID������
	void RoutineCheckUnbindIDSocket();
	// ͨ�ö�ʱ��
	void OnNormalTimer();
	// ����
	void OnServerCrossDay();
	// �賿12��
	void OnServerCrossDay12Hour();
	// ����
	void OnServerCrossWeek();
	// ���ʤ�ְ�
	void CleanAllUserWinCount();
	// ��������ʯ���а�
	void CleanAllUserMoneyJewelsRank();
	// ������ҵ�¼
	void OnUserLogon(const UserData &userData);
	// ������ҵǳ�
	void OnUserLogout(int userID);
	// �������ע��
	void OnUserRegister(const UserData &userData);
	// ��ҿ���
	void OnUserCrossDay(int userID, int time);
	// �������ݿ��������
	void ClearDataBase();
	// �����±��¼�˵�����
	void AutoCreateNewTable(bool start);

	// ���ķ���Ϣ���
private:
	// �������ķ���������Ϣ
	virtual bool OnCenterServerMessage(UINT msgID, NetMessageHead * pNetHead, void* pData, UINT size, int userID);
	// ������id�ظ�����
	bool OnCenterRepeatIDMessage(void* pData, int size);
	// �ֲ�ʽϵͳ��Ϣ
	bool OnCenterDistributedSystemInfoMessage(void* pData, int size);
	// �ߵ���������ң���ͬ�豸ͬʱ��½ʹ�ã�
	bool OnCenterKickOldUserMessage(void* pData, UINT size);
	// ���˵�����Ϣ(���ʹ��)
	bool OnCenterKickUserMessage(void* pData, int size, int userID);
	// ��������
	bool OnCenterNoticeMessage(void* pData, int size);
	// ȫ���ʼ�֪ͨ
	bool OnCenterAllUserMailMessage(void* pData, int size);
	// �ط�����
	bool OnCenterCloseServerMessage(void* pData, int size);
	// ��������
	bool OnCenterOpenServerMessage(void* pData, int size);
	// ��ҷ�������
	bool OnCenterSendHornMessage(void* pData, UINT size);
	// ��ĳ�����������Ϣ
	bool OnCenterNotifyUserMessage(NetMessageHead * pNetHead, void* pData, int size, int userID);
	// ���ķ��Զ�����
	bool OnCenterAutoCreateRoomMessage(void* pData, UINT size);
	// �ֻ�ע��
	bool OnCenterPhoneInfoMessage(void* pData, int size);
	// ����ȫ��֪ͨ
	bool OnCenterActivityRewardsMessage(void* pData, int size);
	// ���˱��������˳�������ʵʱ����
	bool OnCenterSignUpMatchMessage(void* pData, UINT size);
	// ������֪ͨ��ҽ������
	bool OnCenterLoaderStartMatchMessage(void* pData, UINT size);
	// ���˱��������˳�����(��ʱ��)
	bool OnCenterTimeMatchPeopleChangeMessage(void* pData, UINT size);

	//��PHP����ӿ����
private:
	// ��php������Ϣ�ӿ�
	void SendHTTPMessage(int userID, const std::string &url, BYTE postType);
	// ���ע�ᷢ��http����
	bool SendHTTPUserRegisterMessage(int userID);
	// ��ҵ�½�͵ǳ�֪ͨ type���ͣ�0��½��1�ǳ�
	bool SendHTTPUserLogonLogout(int userID, BYTE type);
};
