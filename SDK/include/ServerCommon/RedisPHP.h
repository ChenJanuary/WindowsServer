#pragma once

#include "RedisBase.h"

class KERNEL_CLASS CRedisPHP : public CRedisBase
{
public:
	CRedisPHP();
	virtual ~CRedisPHP();

public:
	// ��ʼ��
	virtual bool Init();
	// �ر�
	virtual bool Stop();
	// ��ȡredis Context
	redisContext* GetRedisContext();
public:
	/////////////////////////////////���ֲ����/////////////////////////////////////////
	bool GetFGMember(int friendsGroupID, std::vector<int> &memberUserIDVec);
	// ��ȡ��Ҿ��ֲ����
	bool GetUserFriendsGroupMoney(int friendsGroupID, int userID, long long &money);
	// ������Ҿ��ֲ����
	bool SetUserFriendsGroupMoney(int friendsGroupID, int userID, long long money, bool bAdd = true, int reason = 0, int roomID = 0, int rateFireCoin = 0);
	// ��ȡ���Ȩ��
	int GetUserPower(int friendsGroupID, int userID);
	// ��ȡ���ֲ�Ⱥ��
	int GetFriendsGroupMasterID(int friendsGroupID);
	// �Ƿ���Լ�����ֲ�����
	bool IsCanJoinFriendsGroupRoom(int userID, int friendsGroupID);
	// ������Ҿ��ֲ���Դ����
	bool SetUserFriendsGroupResNums(int friendsGroupID, int userID, const char * resName, long long resNums, bool bAdd = false);
	// ��ȡ֪ͨʱ��
	bool GetFGNotifySendTime(long long llIndex, time_t &sendTime);
	// ������Ҿ��ֲ�֪ͨ
	void ClearUserFGNotifySet(int userID);
	//  ��������֪ͨ
	void ClearAllFGNotifyInfo();
	//��ȡ���ֲ��󶨵Ĺ���ԱID
	bool GetfriendsGroupToUser(int friendsGroupID, int userID, int &ChouShui, int&ManagerId);


	/////////////////////////////////�ʼ�ģ��/////////////////////////////////////////
	bool GetEmailSendTime(long long llIndex, int &sendTime);
	bool GetUserAllEmailID(int userID, std::vector<EmailSimpleInfo> &simpleEmailID, bool Asc = true);
	bool DelUserEmailInfo(int userID, long long emailID);
	void ClearUserEmailSet(int userID);
	void ClearAllEmailInfo();

	//////////////////////////////////����ͳ��////////////////////////////////////////
	long long AddUserResNums(int userID, const char * resName, long long changeResNums);

	//////////////////////////////////������////////////////////////////////////////
	// ��redis������Ϣ�����ص��ڴ�
	bool LoadAllMatchInfo(std::map<long long, MatchInfo> & matchInfoMap);
	// ��ȡ������Ϣ
	bool GetMatchInfo(int matchID, MatchInfo& matchInfo);
	// ���ñ���״̬
	bool SetMatchStatus(int matchID, BYTE matchStatus);
};