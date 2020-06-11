#pragma once

#include "RedisCommon.h"
#include <string>

class KERNEL_CLASS CRedisLoader : public CRedisCommon
{
public:
	CRedisLoader();
	virtual ~CRedisLoader();

public:
	virtual bool Init();
	virtual bool Stop();

	//ɾ������
	bool DelPrivateDeskRecord(int deskMixID);
	void DelUserBuyDeskInfoInSet(int userID, const char* passwd);

	//��¼������Ϣ
	bool SetPrivateDeskCurrUserCount(int deskMixID, int userCount);
	bool SetPrivateDeskCurrWatchUserCount(int deskMixID, int watchUserCount);

	//���ÿ�������б�
	bool SetPrivateDeskUserID(int deskMixID, int userID, int type);//0������1ɾ��

	//ʤ�������
	bool SetUserWinCount(int userID);
	bool SetUserTotalGameCount(int userID);

	//����������Ϸ����
	bool SetPrivateDeskGameCount(int deskMixID, int gameCount);

	//����ս��
	bool SetPrivateDeskGrade(GameGradeInfo& gameGradeInfo, long long& gradeID);
	bool SetPrivateDeskSimpleInfo(const std::vector<int>& userIDVec, PrivateDeskGradeSimpleInfo& simpleInfo, const std::vector<long long>& gradeIDVec);

	//����������
	int GetRobotInfoIndex();

	//���õ�ǰ��������
	bool SetRoomServerPeopleCount(int roomID, int peopleCount);

	//�ط���������
	bool SaveFGDeskRoom(const PrivateDeskInfo &privateDeskInfo);

	//������������
	bool CleanRoomAllData(int roomID);

	//��ȡ�������д���������
	bool GetAllDesk(int roomID, std::vector<int>& vecRooms);

	// ���ó�ʱ����
	bool SetPrivateDeskCheckTime(int deskMixID, int checkTime);

	//��ȡ��ҿ��Ʋ���
	bool GetUserControlParam(int userID, int &value);

	//������
	long long GetPartOfMatchIndex();
	bool DelFullPeopleMatchPeople(int gameID, int matchID, const std::vector<MatchUserInfo> &vecPeople);
	bool SetUserMatchStatus(int userID, BYTE matchType, int matchStatus);
	bool SetUserMatchRank(int userID, long long combineMatchID, int curMatchRank);
	bool SetTimeMatchPeopleRank(int matchID, const std::vector<MatchUserInfo> &vecPeople);
	int GetRobotUserID();
private:
	long long GetRoomMaxGradeIndex(int roomID);
	long long GetRoomMaxSimpleGradeIndex(int roomID);
};