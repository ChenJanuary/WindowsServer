#pragma once

#include <map>
#include <unordered_map>

// ������ҽṹ
struct LogonUserInfo
{
	int userID;
	int socketIdx;
	BYTE isVirtual;
	int roomID;

	LogonUserInfo()
	{
		userID = 0;
		socketIdx = -1;
		isVirtual = 0;
		roomID = 0;
	}
};

// ������ҹ�����
class CLogonUserManage
{
public:
	CLogonUserManage();
	~CLogonUserManage();

public:
	LogonUserInfo* GetUser(int userID);
	bool AddUser(int userID, LogonUserInfo* pUser);
	void DelUser(int userID);
	void Release();
	std::map<int, LogonUserInfo*>& GetLogonUserInfoMap() { return m_logonUserInfoMap; }
	UINT GetUserCount() { return m_logonUserInfoMap.size(); }

private:
	std::map<int, LogonUserInfo*> m_logonUserInfoMap;			// ������ҹ�������ֻ�������ߵ����
};
