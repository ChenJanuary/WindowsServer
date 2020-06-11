#pragma once

#include "RedisBase.h"

// redis ������
class KERNEL_CLASS CRedisCommon : public CRedisBase
{
public:
	CRedisCommon();
	virtual ~CRedisCommon();

public:
	//////////////////////////////////������������////////////////////////////////////////
	// ��ȡ��������
	bool GetPrivateDeskRecordInfo(int deskMixID, PrivateDeskInfo& deskRecordInfo);
	// �����������������
	bool SetMarkDeskIndex(int roomID, int iMarkIndex);
	// ɾ�����ֲ�����
	bool DelFGDeskRoom(int friendsGroupID, int friendsGroupDeskNumber);

	//////////////////////////////////��������////////////////////////////////////////
	//������ұ�����Ϣ
	bool GetUserBag(int userID, UserBag& userBagInfo);
	//��ȡָ��������������
	int GetUserBagCount(int userID, const char * resName);
	//������ұ���
	bool SetUserBag(int userID, const char * resName, int changeResNums, bool bAdd);

	/////////////////////////////////�û�����/////////////////////////////////////////
	// ����˺��Ƿ����
	bool IsAccountExists(const char* account);
	// ͨ������˺Ż�ȡID
	int GetUserIDByAccount(const char* account);
	// ��ȡ�������
	bool GetUserData(int userID, UserData& userData);
	// ������ҵ�roomID
	bool SetUserRoomID(int userID, int roomID);
	// �������deskIdx
	bool SetUserDeskIdx(int userID, int deskIdx);
	// ������ҽ��
	bool SetUserMoney(int userID, long long money);
	// ������ҽ�ң��·���
	bool SetUserMoneyEx(int userID, long long money, bool bAdd = true, int reason = 0, int roomID = 0, long long rateMoney = 0, BYTE isVirtual = 0, int friendsGroupID = 0, int roomType = -1);
	// ������ҷ���
	bool SetUserJewels(int userID, int jewels);
	// �µ�������ҷ���
	bool SetUserJewelsEx(int userID, int jewels, bool bAdd = true, int reason = 0, int roomID = 0, int rateJewels = 0, BYTE isVirtual = 0, int friendsGroupID = 0, int roomType = -1);
	// ��ȡ�������ֵ��ֻ�ܻ�ȡ��ֵ����
	long long GetUserResNums(int userID, const char * resName);
	// �����������ֵ��ֻ��������ֵ����
	bool SetUserResNums(int userID, const char * resName, long long resNums);
	// ������Դֵ��ֻ��������ֵ����
	long long AddUserResNums(int userID, const char * resName, long long changeResNums);
	// ������ҹ����е����Ӵ���
	bool SetUserBuyingDeskCount(int userID, int count, bool bAdd = true);
	// ������ҵĿ���ʱ��
	bool SetUserCrossDayTime(int useID, int time);
	// �������token
	bool SetUserToken(int userID, const char* token);
	// ��ȡ���С�����Ϣ
	bool GetUserRedSpot(int userID, UserRedSpot &userRedSpot);
	// ���õ�ǰ����û�id
	bool SetCurMaxUserID(int iUserID);
	// ��ͨ�û�����account-userID������, �������û�����key-userID����
	bool FixAccountIndexInfo(const char* account, const char* passwd, int userID, int registerType);

	/////////////////////////////////��������/////////////////////////////////////////
	bool GetRewardsPoolInfo(int roomID, RewardsPoolInfo& poolInfo);
	bool SetRoomPoolMoney(int roomID, long long money, bool bAdd = false);
	bool SetRoomGameWinMoney(int roomID, long long money, bool bAdd = false);
	bool SetRoomPercentageWinMoney(int roomID, long long money, bool bAdd = false);
	bool SetRoomPoolData(int roomID, const char * fieldName, long long money, bool bAdd = false);
	long long GetRoomPoolData(int roomID, const char * fieldName);

	/////////////////////////////////��������������/////////////////////////////////////////
	// ��ȡ������״̬
	bool GetServerStatus(int &status);
	// ���÷�����״̬
	bool SetServerStatus(int status);
	// ��ȡOtherConfig
	bool GetOtherConfig(OtherConfig &config);
	// ��ȡroomBaseInfo
	bool GetRoomBaseInfo(int roomID, RoomBaseInfo &room);
	// ��ȡ���򷿿����� privateDeskConfig
	bool GetBuyGameDeskInfo(const BuyGameDeskInfoKey &buyKey, BuyGameDeskInfo &buyInfo);

	////////////////////////////////������//////////////////////////////////////////
	// ��ȡ���˿��������
	bool GetFullPeopleMatchPeople(int gameID, int matchID, int peopleCount, std::vector<MatchUserInfo> &vecPeople);
	// ��ȡ��ʱ���������
	bool GetTimeMatchPeople(int matchID, std::vector<MatchUserInfo> &vecPeople);

protected:
	// ��Ҫ���浽dbָ��ӿڣ�Ŀǰֻ��������� �������Ŀǰ����ʹ��redis lock ʹ��redis lock��ʱ���ɵ����߾���
	bool InnerHINCRBYCommand(const char* key, const char* redisCmd, long long &retValue, int mode = REDIS_EXTEND_MODE_DEFAULT);	// ֻ�����HINCRBYָ�ֻ����ֵ����
	bool InnerHMSetCommand(const char* key, const char* redisCmd, int mode = REDIS_EXTEND_MODE_DEFAULT);						// ֻ�����HMSETָ��

public:
	//�������ά��
	bool IsUserOnline(int userID);
	bool RemoveOnlineUser(int userID, BYTE isVirtual);
	bool AddOnlineUser(int userID, BYTE isVirtual);
	bool ClearOnlineUser();

	// ���ϲ���
	bool AddKeyToSet(const char* key, const char * assKey);
	bool AddKeyToZSet(const char* key, long long socre, long long value);
	bool GetZSetSocreByRank(const char* key, int rank, bool order, long long &socre); //true���Ӵ�С
public:
	// ��ȡָ��λ�������������λ��Ϊ0
	static int GetRandDigit(int digit);
};