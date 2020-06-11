#pragma once

#include "RedisCommon.h"
#include <vector>
#include <string>
#include "configManage.h"
#include "NewMessageDefine.h"

class KERNEL_CLASS CRedisLogon : public CRedisCommon
{
public:
	CRedisLogon();
	virtual ~CRedisLogon();

public:
	// ��ʼ��
	virtual bool Init();
	// �ر�
	virtual bool Stop();
	// ��ȡredis Context
	redisContext* GetRedisContext();
public:
	// ע���û�
	int  Register(const UserData& userData, BYTE registerType);

	// �����������
	int  GetDeskMixIDByPasswd(const char* passwd);
	std::string CreatePrivateDeskRecord(int userID, int roomID, BYTE masterNotPlay, int maxDeskCount, int buyGameCount,
		const char* pGameRules, int maxUserCount, int payType, int maxWatchUserCount, int	friendsGroupID, int friendsGroupDeskNumber);
	int  GetMarkDeskIndex(int roomID);

	//���ߺ���
	bool SetUserInfo(int userID, std::string strLine);
	int GetRelevanceTrdUid(std::string triID);	// ��������¼��ȡuserID
	int GetVisitorID(std::string triID);		// �͵�¼��ȡuserID
	bool ClearAllUserWinCount();
	void AddUserBuyDeskSet(int userID, const std::string& passwd);
	std::string GetRandUnRepeatedDeskPasswd();
	bool ClearAllUserRanking(const char* ranking);


	//���ز���
	bool ClearOneDayWinMoney();
	bool CountOneDayPoolInfo();

	///////////////////////��½ע��/////////////////////////////
	// �ֻ��ŵ�¼
	int GetUserIDByPhone(const char* phone);
	// ��ȡ��ǰ���id
	int  GetCurMaxUserID();
	// ���ĵ�½
	int GetUserIDByXianLiao(const char* xianliao);

	// ���õ�¼������
	bool SetLogonServerCurrPeopleCount(int logonID, int peopleCount);

	///////////////////////////////////ս��ģ��///////////////////////////////////////
	bool GetGradeSimpleInfo(long long id, PrivateDeskGradeSimpleInfo& simpleInfoVec);
	bool GetGradeDetailInfo(long long id, GameGradeInfo& gameGradeInfo);
	void ClearGradeInfo();
	void ClearUserGradeInfo(int userID);
	void ClearGradeSimpleInfoByRoomID(int roomID);
	void ClearGradeDetailInfoByRoomID(int roomID);

	///////////////////////////////////���ֲ�����ģ��///////////////////////////////////////
	int IsCanCreateFriendsGroupRoom(int userID, int friendsGroupID, BYTE userPower, int friendsGroupDeskNumber);
	bool CreateFriendsGroupDeskInfo(int friendsGroupID, int friendsGroupDeskNumber, const std::string &deskPasswd,
		int gameID, int roomType, OneFriendsGroupDeskInfo &deskInfo, bool &bHaveRedSpot);
	int GetFGDeskMixID(const char * asskey);
	bool GetTempFgDesk(const char * asskey, SaveRedisFriendsGroupDesk & desk);  //�ָ������õ�
	int GetAllTempFgDesk(std::vector<SaveRedisFriendsGroupDesk>& vecFGDesk);    //�ָ������õ�
public:
	// ����redis�е����ݵ�DB
	void RountineSaveRedisDataToDB(bool updateAll);
	bool SaveRedisDataToDB(const char* key, const char* tableName, int id, int mode);

	//�ֲ�ʽ
	bool IsMainDistributedSystem(); //�ж��Ƿ��ǵ�ǰ��Ҫ��Ⱥϵͳ
	bool IsDistributedSystemCalculate(long long calcID); //���id�Ƿ�ǰϵͳ����
public:
	//�ֲ�ʽ�������
	UINT m_uLogonGroupIndex;		//��½����Ⱥ����
	UINT m_uLogonGroupCount;		//��½����Ⱥ����
	UINT m_uMainLogonGroupIndex;	//��ǰ��Ҫ��½����Ⱥ���������������������Ⱥ����������ݣ�
private:
	std::vector<FieldRealInfo> m_vecFields;
};