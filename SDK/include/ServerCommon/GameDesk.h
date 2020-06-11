#pragma once

#include <map>
#include <vector>
#include <set>
#include "TCPSocket.h"
#include "GameMainManage.h"
#include "NewMessageDefine.h"
#include "PlatformMessage.h"
#include "gameUserManage.h"
#include "Util.h"

//��ʱ��
#define IDT_AGREE_DISMISS_DESK		1		//ͬ���ɢ���Ӷ�ʱ��
#define IDT_FRIEND_ROOM_GAMEBEGIN	2		//���ѷ���Ϸ��ʼ��ʱ��

//��ʱ�����ʱ��
const int CFG_DISMISS_DESK_WAIT_TIME = 120;						// ��ɢ���ӵȴ�ʱ��
const int FRIEND_ROOM_WAIT_BEGIN_TIME = 61;						// ��ҷ���vip����ֵʱ��
const int GOLD_DESK_TIMEOUT_UNAGREE_KICKOUT_SECS = 31;			// ��ҳ���ʱδ׼��T�����ʱ��
static int HUUNDRED_GAME_TIMEOUT_OPERATE_KICKOUT_SECS = 120;	// ��������Ϸ��������Ϸ����ʱ�䲻��������ʱ��

//��Ϸ������־
#define GFF_FORCE_FINISH			0			//ǿ�н��
#define GFF_SAFE_FINISH				1			//�����Ϸ
#define GF_NORMAL					2			//��Ϸ��������
#define GF_SALE						3			//��Ϸ��ȫ����
#define GF_NET_CAUSE                4			//����ԭ����Ϸ����
#define GFF_DISSMISS_FINISH			5			//��ɢ���ӽ���

//��ʼģʽ
enum BeginMode
{
	FULL_BEGIN = 0,				// ���˲ſ�ʼ
	ALL_ARGEE = 1,				// ������ͬ��Ϳ�ʼ
};

enum DismissType
{
	DISMISS_TYPE_DEFAULT = 0,	// Ĭ��
	DISMISS_TYPE_AGREE,			// ͬ���ɢ
	DISMISS_TYPE_DISAGREE,		// ��ͬ���ɢ
};

// ���������Ϣ
struct DeskUserInfo
{
	bool    bNetCut;			// �Ƿ����
	BYTE	deskStation;
	int		userID;
	int		dismissType;		// ��ɢ����
	time_t	lastWaitAgreeTime;	// �ϴεȴ�׼��ʱ��(������һ�ν�����Ϸ, ��Ϸ����)��ʵ���ڽ�ҳ�
	bool	isAuto;				// �Ƿ��й�
	BYTE	isVirtual;			// �Ƿ������
	int		leftRoomUser;		// �Ƿ���ǰ�뿪������(˽�˷���)

	DeskUserInfo()
	{
		clear();
	}

	void clear()
	{
		deskStation = INVALID_DESKSTATION;
		userID = 0;
		dismissType = DISMISS_TYPE_DEFAULT;
		bNetCut = false;
		isAuto = false;
		lastWaitAgreeTime = 0;
		isVirtual = 0;
		leftRoomUser = 0;
	}
};

// �Թ�����Ϣ
struct WatchUserInfo
{
	int userID;
	BYTE deskStation;

	bool friend operator < (WatchUserInfo a, WatchUserInfo b)
	{
		return a.userID >= b.userID;
	}
};

class CGameMainManage;
class KERNEL_CLASS CGameDesk
{
public: //���캯������������
	explicit CGameDesk(BYTE byBeginMode);
	virtual ~CGameDesk();

	//�麯�����������صĺ���������һЩ�����������жϵȵ�
public:
	// ��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser) = 0;
	// ������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation) = 0;
	// ��ʼ����Ϸ�߼�
	virtual bool InitDeskGameStation() { return true; }
	// ��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	// ��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	// ��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	// ���ӳɹ���ɢ isDismissMidway=�Ƿ���;��ɢ
	virtual void OnDeskSuccessfulDissmiss(bool isDismissMidway = true);
	// ��ʼ����Ϸ���ݣ�������ʼ��
	virtual void InitDeskGameData() { };
	// �ж���Ϸ���ϵ�ĳ������Ƿ�ʼ��Ϸ��
	virtual bool IsPlayGame(BYTE bDeskStation) { return m_bPlayGame; }
	// �Ƿ���Կ�ʼ��Ϸ
	virtual bool CanBeginGame();
	// �û�ͬ�⿪ʼ
	virtual bool UserAgreeGame(BYTE bDeskStation);
	// ��ɢ��Ϣ����
	virtual bool HandleDissmissMessage(BYTE deskStation, unsigned int assistID, void* pData, int size);
	// �û������뿪
	virtual bool UserNetCut(GameUserInfo *pUser);
	// ��������Ϸ��������������Ϸ��������
	virtual bool OnStart() { return true; }
	// ��������Ϸ,�ж������������Ƿ�ɱ䣨���������ˣ��Լ���ׯ�б���ң�
	virtual bool HundredGameIsInfoChange(BYTE deskStation) { return false; }
	// ������Ϸ״̬
	virtual void SetGameStation(BYTE byGameStation) { m_byGameStation = byGameStation; }
	// �����Ϸ״̬
	virtual BYTE GetGameStation() { return m_byGameStation; }
	// �طű�ʶ��
	virtual bool GetVideoCode(char *pCode, int iLen);
	// �����Ϣ
	virtual bool HandleFrameMessage(BYTE bDeskStation, unsigned int assistID, void* pData, int size, bool bWatchUser = false);
	// ��Ϸ��Ϣ����
	virtual bool HandleNotifyMessage(BYTE bDeskStation, unsigned int assistID, void* pData, int size, bool bWatchUser = false);
	// ����뿪���ӣ�ֱ�ӷ��ش���
	virtual bool UserLeftDesk(GameUserInfo* pUser);
	// ��������й�
	virtual bool OnHandleUserRequestAuto(BYTE deskStation);
	// ���ȡ���й�
	virtual bool OnHandleUserRequestCancelAuto(BYTE deskStation);
	// �Թ����Ƿ��������
	virtual bool WatchCanSit(GameUserInfo* pUser) { return true; }
	// ������Ϸר�ã���������˿�
	virtual void UserBeKicked(BYTE deskStation) {}
	// �����ҵĽ���ܷ��������Ϸ
	virtual int CheckUserMoney(int userID);
	// ��̬���������ļ�����
	virtual void LoadDynamicConfig() {}
	// �������֪ͨ��Ϸ
	virtual bool UserSitDeskActionNotify(BYTE deskStation) { return true; };

	// �ӿں���
public:
	//��ʼ������
	bool Init(int deskIdx, BYTE bMaxPeople, CGameMainManage * pDataManage);
	// ��ʼ��������������
	void InitBuyDeskData();
	// ��ʼ����ɢ����
	void InitDismissData();
	//���ö�ʱ��
	bool SetTimer(UINT uTimerID, int uElapse);
	//ɾ����ʱ��
	bool KillTimer(UINT uTimerID);
	// ��ȡ��������
	int GetRoomType();
	// ��ȡ�������ࣨ���ڰ����ࣩ
	int GetRoomSort();
	// ��ȡ����ȼ����������м����߼���
	int GetRoomLevel();
	// ����������Ϣ
	void LoadPrivateDeskInfo(const PrivateDeskInfo& privateDeskInfo);
	// ������Ϣ��
	void MakeAllUserInfo(char* pBuf, int size, int& realSize);
	// �����๹�������Ϣ
	void HundredMakeAllUserInfo(char* pBuf, int size, int& realSize);
	// �ϳɽ�ɢ������Ϣ
	void MakeDismissData(char* buf, int& size);
	// �ϳ���Ҽ���Ϣ
	bool MakeUserSimpleInfo(BYTE deskStation, UserSimpleInfo& userInfo);
	// �����������������ҵ���Ϣ���ѹ���������ҵ�deskIndex��deskStation��Ϊ��Ч
	void ClearAllData(const PrivateDeskInfo& privateDeskInfo);
	// �������۳�����
	BuyGameDeskInfo ProcessCostJewels();
	// ��Ҫ����ķ��䣬���ս��
	bool AddDeskGrade(const char *pVideoCode, const char * gameData, const char * userInfoList);
	// �ǽ�ҳ����ˣ��Դ���ս����Ӱ�죬�����ṩ�������˿�����Ϸ����
	bool KickOutUser(BYTE deskStation, int ReaSon = REASON_KICKOUT_STAND);
	// ��ȡһ�����Կ�ʼ��Ϸ�����
	void SetBeginUser();
	// ���û��׼�������
	void CheckTimeoutNotAgreeUser(time_t currTime);
	// ���û�в��������
	void CheckTimeoutNotOperateUser(time_t currTime);
	// ֪ͨ��Դ�仯
	void NotifyUserResourceChange(int userID, int resourceType, long long value, long long changeValue);

	// �����������
public:
	//������Ϸ״̬
	bool SendGameStation(BYTE deskStation, UINT socketID, bool bWatchUser, void * pData, UINT size);
	//�������ݺ���
	bool SendGameData(BYTE deskStation, unsigned int mainID, unsigned int assistID, unsigned int handleCode);
	//�������ݺ��� 
	bool SendGameData(BYTE bDeskStation, void* pData, int  size, unsigned int mainID, unsigned int assistID, unsigned int handleCode);
	//�����Թ�����
	bool SendWatchData(void * pData, int uSize, int mainID, int assistID, int handleCode);
	//����֪ͨ��Ϣ
	bool SendGameMessage(BYTE deskStation, LPCTSTR lpszMessage, int wType = SMT_EJECT);
	// �㲥������Ϣ
	void BroadcastDeskData(void *pData, int size, unsigned int mainID, unsigned int assistID, bool sendVirtual = true, unsigned int handleCode = 0);
	// �������������Ϣ
	void SendAllDeskUserInfo(BYTE deskStation);
	// ���ͷ���Ļ�����Ϣ
	void SendDeskBaseInfo(BYTE deskStation);
	// �㲥����Ļ�����Ϣ
	void BroadcastDeskBaseInfo();
	// �㲥���������Ϣ
	void BroadcastUserSitData(BYTE deskStation);
	// �㲥���������Ϣ�����㲥�Լ���
	void BroadcastUserSitDataExceptMyself(BYTE deskStation);
	// �㲥��Ҽ���Ϣ
	void BroadcastUserSimpleInfo(BYTE deskStation);
	// �㲥����뿪��Ϣ
	bool BroadcastUserLeftData(BYTE deskStation, int reason);
	// ��������뿪��Ϣ
	void SendUserLeftData(BYTE deskStation, int reason);
	// �㲥���ͬ����Ϣ
	void BroadcastUserAgreeData(BYTE deskStation);
	// �ر���Ҷ�����Ϣ
	void BroadcastUserOfflineData(BYTE deskStation);
	// �㲥��ұ�T����Ϣ
	void BroadcastUserKickoutData(BYTE deskStation);
	// ���͵ȴ�׼��ʱ����Ϣ
	void SendLeftWaitAgreeData(BYTE deskStation);
	// ֪ͨ������ҽ�ɢ������Ϣ
	void BroadcastDismissData();
	// ���ͽ�ɢ��Ϣ(���ڶ�������)
	void SendDismissData();
	// �㲥��Ϣ���趨���㲥�ض����
	void BroadcastGameMessageExcept(BYTE deskStation, LPCTSTR lpszMessage, int wType = SMT_EJECT);

	// ���㺯��
protected:
	// ˽�з����㺯��
	bool ChangeUserPointPrivate(long long *arPoint, bool *bCut, char *pVideoCode = NULL, bool bGrade = false, char * gameData = NULL);
	// ˽�з����㺯��
	bool ChangeUserPointPrivate(int *arPoint, bool *bCut, char *pVideoCode = NULL, bool bGrade = false, char * gameData = NULL);
	// �ı���һ��֣������ڽ�ҳ�
	bool ChangeUserPoint(long long *arPoint, bool *bCut, long long * rateMoney = NULL);
	// �ı���һ��֣������ڽ�ҳ�
	bool ChangeUserPoint(int *arPoint, bool *bCut, long long * rateMoney = NULL);
	// �ı���ұ������ݣ���ʱ����ʹ��
	bool ChangeUserBage(BYTE deskStation, char * resName, int changeNum = 1, bool add = true);
	// �ı���ҽ��
	bool ChangeUserPoint(BYTE deskStation, long long point, long long rateMoney = 0, bool notify = true);
	// �ı���һ��
	bool ChangeUserFireCoin(long long *arPoint, char *pVideoCode = NULL, bool bGrade = false, char * gameData = NULL);
	// �ı���һ��
	bool ChangeUserFireCoin(int *arPoint, char *pVideoCode = NULL, bool bGrade = false, char * gameData = NULL);
	// ��ҷ����㺯��
	bool ChangeUserPointGoldRoom(long long *arPoint, char *pVideoCode = NULL, bool bGrade = false, char * gameData = NULL, long long * rateMoney = NULL);
	// ��ҷ����㺯��
	bool ChangeUserPointGoldRoom(int *arPoint, char *pVideoCode = NULL, bool bGrade = false, char * gameData = NULL, long long * rateMoney = NULL);
	// ��ʯ�����㺯��
	bool ChangeUserPointJewelsRoom(long long *arPoint, char *pVideoCode = NULL, bool bGrade = false, char * gameData = NULL, long long * rateJewels = NULL);
	// ��ʯ�����㺯��
	bool ChangeUserPointJewelsRoom(int *arPoint, char *pVideoCode = NULL, bool bGrade = false, char * gameData = NULL, long long * rateJewels = NULL);

	// �����жϺ���
public:
	// �����Ƿ���Ч
	inline bool IsEnable() { return m_enable; }
	// ����Ƿ��й�
	bool IsAuto(BYTE deskStation);
	// �ж�����Ƿ����
	bool IsNetCut(BYTE deskStation);
	// �ж�����Ƿ������
	bool IsVirtual(BYTE deskStation);
	// �ж��Ƿ��ǳ���
	bool IsSuperUser(BYTE deskStation);
	// �ж��Ƿ����ο�
	bool IsVisitorUser(BYTE deskStation);
	// �Ƿ񷿿���
	bool IsPrivateRoom() { return GetRoomType() == ROOM_TYPE_PRIVATE; }
	// �Ƿ��ҳ�
	bool IsGoldRoom() { return GetRoomType() == ROOM_TYPE_GOLD; }
	// �Ƿ�˽�˷�
	bool IsFriendRoom() { return GetRoomType() == ROOM_TYPE_FRIEND; }
	// �Ƿ���ֲ�VIP����
	bool IsFGVIPRoom() { return GetRoomType() == ROOM_TYPE_FG_VIP; }
	//�Ƿ��������
	bool IsCanSitDesk();
	// �Ƿ��Թ���
	bool IsWatcher(int userID);
	// �Ƿ�����λ��
	bool IsHaveDeskStation(int userID, const PrivateDeskInfo &info);
	// �Ƿ�ȫ����ҵ���
	bool IsAllUserOffline();
	// ��Ϸ�ж��Ƿ���Ҫ��һ��������(��Ҫ�ǰ�����ʹ��)
	bool IsKickOutVirtual(BYTE deskStation);

	// ��Ϣ�������
public:
	// ������ҵ�¼
	bool OnPrivateUserLogin(int userID, const PrivateDeskInfo& info);
	// �����������������Ϣ
	bool OnHandleUserRequestDeskInfo(BYTE deskStation);
	// �������������Ϸ��Ϣ
	bool OnHandleUserRequestGameInfo(BYTE deskStation);
	// ��������������������Ϣ
	bool OnHandleUserRequestALLUserInfo(BYTE deskStation);
	// �������󵥸������Ϣ
	bool OnHandleUserRequestOneUserInfo(BYTE deskStation, void* pData, int size);
	// ����������������ɢ
	bool OnHandleUserRequestDissmiss(BYTE deskStation);
	// ������һ�Ӧ��ɢ��Ϣ
	bool OnHandleUserAnswerDissmiss(BYTE deskStation, void* pData, int size);
	// �����ɢʧ��
	bool OnDeskDismissFailed();
	// ���ͬ���ɢ
	bool OnUserAgreeDismiss(BYTE deskStation);
	// �������������Ϣ
	bool OnHandleTalkMessage(BYTE deskStation, void* pData, int size);
	// �������������Ϣ
	bool OnHandleVoiceMessage(BYTE deskStation, void* pData, int size);
	// �������ʹ��ħ������
	bool OnHandleUserRequestMagicExpress(BYTE deskStation, void* pData, int size);
	// ������Ϸ��ʼ
	bool OnUserRequsetGameBegin(int userID);

	// ����������
protected:
	// ͨ����λ�Ż�ȡ���ID
	int GetUserIDByDeskStation(BYTE deskStation);
	// ͨ�����ID��ȡ��λ��
	BYTE GetDeskStationByUserID(int userID);
	// ֪ͨ���ӺŻ�ȡ�������
	bool GetUserData(BYTE deskStation, UserData& userData);
	// ֪ͨ���ӺŻ�ȡ�������
	bool GetUserData(BYTE deskStation, GameUserInfo& userData);
	// ��ȡ��ұ�������
	bool GetUserBagData(BYTE bDeskStation, UserBag & userBagData);
	// ͨ��key��ȡ���ĳ��������
	int GetUserBagDataByKey(BYTE bDeskStation, const char * resName);
	// ��ȡ��ǰ��ҵĽ�������߻������
	bool GetRoomResNums(BYTE deskStation, long long &resNums);
	// ������һ���
	bool SetUserScore(BYTE deskStation, long long score);
	// ��ȡ��һ���
	long long GetUserScore(BYTE deskStation);
	// ��ȡ��ҿ��Ʋ���
	int GetUserControlParam(BYTE deskStation);

	// �����������
public:
	// ��ȡ������Ϸ״̬
	bool GetPlayGame() { return m_bPlayGame; }
	// ��ȡ׼���������
	int AgreePeople();
	// ��ȡ�������
	int GetRealPeople();
	// ��ȡ���������
	UINT GetRobotPeople();
	// ��ȡ�׷�(����)
	int GetBasePoint();
	// ��ȡ��ǰ�������
	int GetUserCount();
	// ��ȡ�����������
	int GetOfflineUserCount();
	// ��ȡ��׼�����������
	int GetAgreeUserCount();
	// ����˽�˷�������ͽ��
	void SetRemovePlayerMinPoint(int point) { m_RemoveMinPoint = point; }
	// ������ͳ�������Ϸ�����ó�ʱʱ��
	void SetOperationTimeout(int time) { HUUNDRED_GAME_TIMEOUT_OPERATE_KICKOUT_SECS = time; }

	// ��Ҷ������(���¡��뿪�͵���)
public:
	// �������������
	bool UserSitDesk(GameUserInfo* pUser);
	// �����������߼�
	bool PrivateSitDeskLogic(GameUserInfo* pUser);
	// ��ҳ������߼�
	bool MoneySitDeskLogic(GameUserInfo* pUser);
	// �������뿪�����߼�
	bool PrivateUserLeftDeskLogic(GameUserInfo * pUser);
	// ��ҳ��뿪�����߼�
	bool MoneyUserLeftDeskLogic(GameUserInfo * pUser);
	// ��������Ϸ�뿪�����߼�
	bool HundredGameUserLeftLogic(GameUserInfo* pUser);
	// ��������Ϸ�����߼�
	bool HundredGameUserNetCutLogic(GameUserInfo* pUser);
	// �Թ����뿪
	bool OnWatcherLeftDesk(GameUserInfo* pUser, const PrivateDeskInfo & deskInfo);
	// ��������뿪
	bool OnDeskUserLeftDesk(GameUserInfo* pUser, const PrivateDeskInfo& deskInfo);
	// ��������Թ�
	bool ProcessUserWatch(GameUserInfo* pUser, const PrivateDeskInfo& privateDeskInfo);
	// �����������
	bool ProcessPrivateUserSitDesk(GameUserInfo* pUser, const PrivateDeskInfo& privateDeskInfo);
	// �Թ��ߵ���
	bool OnWatchUserOffline(int userID);

	// ��Ϸ�������
protected:
	// ���Թ��߷�����λ
	BYTE AllocWatcherDeskStation();
	// �������ӵ���λ��
	BYTE AllocDeskStation(int userID);
	// ��������Ϸ��ʼ
	bool HundredGameBegin();
	// ��������Ϸ����
	bool HundredGameFinish();
	// ��ҳ����ֿ۽��
	void ProcessDeduceMoneyWhenGameBegin();
	// ƽ̨��Ϸ����㷢���������
	void OnDeskDissmissFinishSendData();

	// ��ʱ�����
private:
	// ���ѷ���ʱ��ʼ
	bool OnTimerFriendRoomGameBegin();

	// ��ˮ����
protected:
	// ��ȡϵͳ��ˮֵ
	double GetDeskPercentage();
	// ���ó�ˮ֮��ķ���
	bool SetDeskPercentageScore(long long * arPoint, long long * ratePoint = NULL, bool arPointIsChange = true);
	// ���ó�ˮ֮��ķ���
	bool SetDeskPercentageScore(int * arPoint, int * ratePoint = NULL, bool arPointIsChange = true);

	// ���ķ����
public:
	// ������ɢ������Ϣ����Ϸ�Ѿ���ʼ�Ͳ���ɢ
	void LogonDissmissDesk(int userID, bool bDelete);
	// ������ɢ������Ϣ(ǿ�н�ɢ)
	void LogonDissmissDesk();
	// ֪ͨ����������Ϣ�����仯(�����仯)
	void NotifyLogonBuyDeskInfoChange(int masterID, int userCount, int userID, BYTE updateType, int deskMixID);
	// �����
	void NotifyLogonDeskDissmiss(const PrivateDeskInfo& privateDeskInfo);
	// �����仯
	void NotifyLogonDeskStatusChange();

	// ƽ̨�����ͽ�������
protected:
	// ����ϵͳ����
	bool SetServerRoomPoolData(const char * fieldName, long long fieldValue, bool bAdd);
	// ����ϵͳ����
	bool SetServerRoomPoolData(const char * fieldName, const char * fieldValue);
	// ��ȡ������������
	bool GetRoomConfigInfo(char configInfo[2048], int size);
	// �Ƿ���1��1ƽ̨
	bool IsOneToOnePlatform();
	// ƽ̨����
	int GetPlatformMultiple();

	// ������
public:
	// ��ʼ������������
	void InitDeskMatchData();
	// ��������Ϸ����
	bool MatchRoomGameBegin();
	// ��������Ϸ����
	bool MatchRoomGameFinish();
	// ���͵�ǰ���ӵı���״̬
	void SendMatchDeskStatus(int userID);
	// �����������߼�
	bool MatchRoomSitDeskLogic(GameUserInfo* pUser);
	// ����������뿪����
	bool MatchRoomUserLeftDeskLogic(GameUserInfo * pUser);
	// ��������ȡһ�������������id
	int MatchGetDeskUserID(int arrUserID[MAX_PLAYER_GRADE]);
	// ���������뱾���Թ�
	bool MatchEnterMyDeskWatch(GameUserInfo * pUser, long long partOfMatchID);
	// �������˳������Թ�
	bool MatchQuitMyDeskWatch(GameUserInfo * pUser, int socketIdx, BYTE result);
	// �Ƿ������
	bool IsMatchRoom() { return GetRoomType() == ROOM_TYPE_MATCH; }
	// ���������㺯��
	bool ChangeUserPointMatchRoom(long long *arPoint);
	// ���������㺯��
	bool ChangeUserPointMatchRoom(int *arPoint);

public:
	// init ʱ���ʼ�������ݣ��������ڴ���Ϸ��������Ϸ����
	CGameMainManage				* m_pDataManage;		// ������ָ��
	BYTE						m_byMaxPeople;			// ��Ϸ����
	BYTE						m_byBeginMode;			// ͬ��ģʽ
	int							m_deskIdx;				// ��������
	bool						m_needLoadConfig;		// ��Ϸ���Ƿ���Ҫ��������
	int							m_iRunGameCount;		// ��Ϸ���еľ���
	std::string					m_ctrlParmRecordInfo;	// ������Ϣ����Ϸ���Ƶ�ʱ�����룬�����д�����ݿ�

public:
	// ���������ʱ����Ҫ���õ�����
	int							m_iVipGameCount;								// �������Ӿ���(�ɱ�)
	char						m_szGameRules[MAX_BUY_DESK_JSON_LEN];			// ��Ϸ����json ��ʽ��������Ϸ����
	char						m_szDeskPassWord[MAX_PRIVATE_DESK_PASSWD_LEN];	// ��������
	int						m_masterID;										// ����ID
	bool						m_isMasterNotPlay;                              // �Ƿ������˿���
	int							m_iConfigCount;				// ��������������������ã�
	int							m_MinPoint;					// �볡���ƣ���ҷ��;��ֲ�vip���䣩
	int							m_RemoveMinPoint;			// �������ޣ���ҷ��;��ֲ�vip���䣩
	int							m_basePoint;				// ��ע����ҷ���
	BYTE						m_roomTipType;				// ��ˮ��ʽ
	BYTE						m_roomTipTypeNums;			// ��ˮ��
	BYTE						m_payType;					// ֧������
	BYTE						m_playMode;					// ��Ϸ�淨����������ׯ
	PlatformFriendsGroupMsg		m_friendsGroupMsg;			// ���ӵľ��ֲ���Ϣ

private:
	// �����ɢ�������
	bool						m_isDismissStatus;			// �Ƿ��ڽ�ɢ״̬(���������ɢ�����ǻ�δ������ɢ)
	BYTE						m_reqDismissDeskStation;	// �����ɢ����
	time_t						m_reqDismissTime;			// �����ɢ�����ʱ��

private:
	// ��Ϸ�����е�����
	bool						m_bPlayGame;				// ��Ϸ�Ƿ�ʼ��־,��Ҫ��ʶ������Ϸ
	BYTE						m_byGameStation;			// ��Ϸ״̬
	bool						m_enable;					// �Ƿ���Ч
	time_t						m_beginTime;
	time_t						m_finishedTime;
	int							m_beginUserID;				// ��ʼ��Ϸ���

public:
	// ������
	long long					m_llPartOfMatchID;			// �������ڵı���id��С��
	int							m_iCurMatchRound;			// �������е�����
	int							m_iMaxMatchRound;			// ����������
	time_t						m_llStartMatchTime;			// ������ʼʱ��
	bool						m_bFinishMatch;				// �����Ƿ���ɱ���
	std::set<int>				m_matchWatchUserID;			// �����Թ�����Ϣ

private:
	// ��������
	std::vector<DeskUserInfo>	m_deskUserInfoVec;			// �����Ϣ����
	std::vector<long long>		m_gradeIDVec;				// ս���б�
	std::set<WatchUserInfo>		m_watchUserInfoSet;			// �Թ�����Ϣ����
	int							m_finishedGameCount;		// ��ɵ���Ϸ����
	bool						m_isBegin;					// ��Ϸ�Ƿ�ʼ, ��������һ�ֿ�ʼ֮����Ϊ��ʼ�������������ɢ�������һ�ֽ���
	int							m_autoBeginMode;			// �Զ���ʼģʽ(0��������ʼ N:��N�˿�ʼ)
	bool						m_bGameStopJoin;			// �Ƿ���;��ֹ����
	int							m_iBuyGameCount;			// �������Ӿ�������������̶������ɱ䣩
	long long					m_uScore[MAX_PLAYER_GRADE];	// ������һ���
	int							m_gameWinCount[MAX_PLAYER_GRADE];	// ÿ����ҵ�ʤ����
};
