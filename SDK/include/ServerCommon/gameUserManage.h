#pragma once

#include <map>
#include <unordered_map>
#include "Define.h"

// ���״̬
enum UserStatus
{
	USER_STATUS_DEFAULT = 0,		// Ĭ��״̬
	USER_STATUS_WATCH,				// �Թ�״̬
	USER_STATUS_SITING,				// ����
	USER_STATUS_AGREE,				// ͬ��
	USER_STATUS_PLAYING,			// ��Ϸ��
	USER_STATUS_STAND,				// վ��
};

// ��Ϸ�����Ϣ
struct GameUserInfo
{
	int		userID;
	int		socketIdx;
	bool	IsOnline;						// �Ƿ�����
	int		deskIdx;						// ������ڵ�deskIdx (��������µ�ʱ������)
	BYTE	deskStation;					// ��ҵ���λ
	int		playStatus;					    // ���״̬
	long long  money;						// ��ҽ��
	int		jewels;							// �����ʯ
	char	name[MAX_USER_NAME_LEN];		// ����ǳ�
	char	headURL[MAX_USER_HEADURL_LEN];
	char	longitude[12];					// ����
	char	latitude[12];					// γ��
	char	address[64];					// ��ַ
	BYTE    userStatus;                     // ������
	char	ip[24];
	BYTE	isVirtual;						// �Ƿ�����
	BYTE	sex;
	BYTE	choiceDeskStation;				// �����ѡ�����λ
	int		fireCoin;						// ��Ҿ��ֲ����
	char	motto[128];						// ����ǩ��
	time_t	lastOperateTime;				// �ϴβ���ʱ��
	int		matchSocre;						// ��������
	int		watchDeskIdx;					// �Թ۵�����

	GameUserInfo()
	{
		Clear();
	}

	void Clear()
	{
		memset(this, 0, sizeof(GameUserInfo));
		socketIdx = -1;
		deskIdx = INVALID_DESKIDX;
		deskStation = INVALID_DESKSTATION;
		choiceDeskStation = INVALID_DESKSTATION;
		lastOperateTime = 0;
	}
};

// ��Ϸ��ҹ�����, ����һ����Ϸ����ҵ���Ϣ
class CGameMainManage;
class KERNEL_CLASS CGameUserManage
{
public:
	CGameUserManage();
	~CGameUserManage();

	void Release();

public:
	bool Init();
	bool UnInit();

public:
	GameUserInfo* GetUser(int userID);
	bool AddUser(GameUserInfo* pUser);
	void DelUser(int userID);
	bool IsUserOnLine(int userID);
	void CheckInvalidStatusUser(CGameMainManage* pManage);
	int GetOnlineUserCount();
	int GetUserCount();

private:
	std::unordered_map<int, GameUserInfo*> m_gameUserInfoMap;
};