#pragma once

#include "Define.h"
#include "basemessage.h"

////////////////////////redis�����ݿⶼ�еı�//////////////////////////////////
// ��̬��(���������ݻᷢ���ı䣬�ᱸ�ݵ����ݿ�)
#define TBL_USER				"userInfo"
#define	TBL_REWARDS_POOL		"rewardsPool"
#define TBL_USER_BAG			"userBag"

//////////////////////////////////�˺����redis��ֵ(����string����,ֻ���ڻ���)////////////////////////////////////////
//�������˺���UID��ӳ���
#define TBL_TRDUSERID			"trdUserID"                          
//�ֻ���½ӳ���
#define TBL_PHONE_TOUSERID		"phoneToUserID"
//���ĵ�½ӳ���
#define TBL_XIANLIAO_TOUSERID	"xianliaoToUserID"
//�ο͵�¼ӳ���
#define TBL_VISITOR_TOUSERID	"visitorToUserID"

//////////////////////////////////////////////////////////
// ֻ�����ڻ����е����ݱ����ݹ���ģ�黮��

// ��������ģ��
#define TBL_CACHE_DESK			"privateDeskInfo"			// �ֶ���Ϣ���PrivateDeskInfo�ṹ�� ��hash��
#define TBL_FG_CLOSE_SAVE_DESK  "FGCloseSaveDesk"			// ��ʱ���������ݿ��е����� ��hash��
#define TBL_CACHE_DESK_PASSWD	"privateDeskPasswd"			// redis key�� string���� ������deskpasswd��deskMixID��ӳ�� ��string��
#define TBL_USER_BUY_DESK		"sameUserBuyDesk"			// ͬ�������������� ��string��
#define TBL_FG_BUY_DESK			"sameFGBuyDesk"				// ͬ�����ֲ������������� ��string��
#define TBL_MARK_DESK_INDEX		"BuyDeskMarkIndex"			// ����������Ҫ�õ������� ��string��
#define CACHE_USER_BUYDESK_SET	"cacheUserBuyDeskSet"		// ��ҿ����б� ��set��

// ս��ģ��
#define TBL_GRADE_DETAIL		"gradeDetailInfo"			// ����ս������ ��hash��
#define TBL_GRADE_SIMPLE		"gradeSimpleInfo"			// �����ս����Ϣ ��hash��
#define TBL_MAX_GRADEKEY		"roomMaxGradeKey"			// ��������ս��ID(ս������ ����ս��) ��string��
#define TBL_MAX_GRADESIMPLEKEY	"roomMaxGradeSimpleKey"		// ��������ս��ID(ս����� �����) ��string��
#define TBL_GRADE_DETAIL_MIN_ID	"clearGradeDetailMinID"		// δ����ĵ���ս��������Сid ��string��
#define TBL_GRADE_SIMPLE_MIN_ID	"clearGradeSimpleMinID"		// δ����Ĵ����ս����Ϣ��Сid ��string��
#define TBL_GRADE_SIMPLE_SET	"gradeSimpleSet"			// �����ս������(����������С�����ID) ��set��
#define TBL_USER_GRADE_SET		"userGradeSet"				// ��ҵĴ����ս������  ��zSet��

// �û�����ģ��
#define TBL_USER_REDSPOT		"userRedSpotCount"			// ���С����ϣ ��hash��
#define TBL_WEB_USER			"webUserInfo"				// php����û����� ��hash����php redis��
#define TBL_ONLINE_USER_SET		"allServerOnlineUserSet"	// ������߼��� ��set��
#define TBL_ONLINE_REALUSER_SET "allServerOnlineRealUserSet"// ������߼���(������������) ��set��
#define TBL_WEB_AGENTMEMBER		"web_agent_member"			// ��Ŵ���ļ��� ��set��
#define TBL_ROBOT_INFO_INDEX	"robotInfoIndex"			// ��������Ϣ���� ��string��
#define TBL_CURMAX_USERID		"curSystemMaxUserID"		// ��ǰϵͳ����userID ��string��

// ���ֲ�ģ��
#define TBL_FG_ROOM_INFO		"FGRoomInfo"				// ���ֲ����䣨������						��string��
#define TBL_FRIENDSGROUP		"friendsGroup"				// ���ֲ���Ϣ								��hash����php redis��
#define TBL_FRIENDSGROUPTOUSER	"friendsGroupToUser"		// ���ֲ���Ա����ȡ���û�ң�				��hash����php redis��
#define TBL_USERTOFRIENDSGROUP	"userToFriendsGroup"		// ��ҶԾ��ֲ���ϣ����ȡ���Ȩ�ޣ�		��hash����php redis��
#define TBL_FG_NOTIFY			"friendsGroupNotify"		// ���ֲ�֪ͨ								��hash����php redis��
#define TBL_FG_TO_USER_SET		"friendsGroupToUserSet"		// ���ֲ���Ա����ȡ���ֲ���ң�				��zSet����php redis��
#define	TBL_USER_FG_NOTIFY_SET	"userToFriendsGroupNotifySet"// ���ֲ�֪ͨ����							��zSet����php redis��
#define	TBL_FG_NOTIFY_ID		"friendsGroupNotifyID"		// ���ֲ�����id								��string����php redis��
#define	TBL_FG_NOTIFY_CLEAR_ID	"friendsGroupNotifyClearID" // ���ֲ�����֪ͨid							��string����php redis��

// �ʼ�ģ�� 
#define TBL_EMAIL_DETAIL		"emailDetailInfo"			// ÿ���ʼ�����								��hash����php redis��
#define TBL_USER_EMAIL_DETAIL	"userToEmailDetailInfo"		// ��ҵ��ʼ�����(�Ƿ��Ѷ����Ƿ��Ѿ���ȡ)	��hash����php redis��
#define TBL_EMAIL_INFO_MIN_ID	"clearEmailInfoMinID"		// δ������ʼ���Сid						��string����php redis��
#define TBL_EMAIL_INFO_MAX_ID	"emailInfoMaxIDSet"			// �ʼ����id								��string����php redis��
#define TBL_USER_EMAIL_SET		"userEmailSet"				// ��ҵ��ʼ�����							��zSet����php redis��

// ���а�ģ��
#define TBL_RANKING_MONEY		"rankingUserMoney"			// ������а�   ��zSet��  [ zadd  rankingUserMoney  money  userID ]
#define TBL_RANKING_WINCOUNT	"rankingUserWinCount"		// ʤ�����а�   ��zSet��  [ zadd  rankingUserWinCount  12  userID ]
#define TBL_RANKING_JEWELS		"rankingUserJewels"			// ��ʯ���а�   ��zSet��  [ zadd  rankingUserJewels  12  userID ]

// ƽ̨����ģ��
#define TBL_SERVER_STATUS		"ServerPlatfromStatus"		// ������״̬	��string��
#define CACHE_UPDATE_SET		"cacheUpdateSet"			// ��Ҫ���µ����ݼ��� ��set��

// ������
#define TBL_TIME_MATCH_INFO		"timeMatchInfo"				// ��ʱ��������Ϣ ��hash����php redis��
#define TBL_PART_OF_MATCH_INDEX	"partOfMatchIndex"			// �ֲ���������id ��string��
#define TBL_MATCH_ROBOT_USERID_INDEX	"matchRobotUserIDIndex"		// ������������ID������string��
#define TBL_SIGN_UP_MATCH_PEOPLE		"curSignUpMatchPeople"		// ��ʵʱ������ǰ��������	curSignUpMatchPeople|gameID,id	��zSet��  ���ձ���ʱ������
#define TBL_SIGN_UP_TIME_MATCH_PEOPLE	"curSignUpTimeMatchPeople"	// ����ʱ������ǰ��������	curSignUpTimeMatchPeople|id		��zSet��  ���ձ���ʱ������

////////////////////////////��������//////////////////////////////////////////
#define REDIS_STR_DEFAULT		"aa"	// redis���ַ���Ĭ��ֵ
#define	USER_IDENTITY_TYPE_SUPER	1	// ����
#define	USER_IDENTITY_TYPE_WIN		2	// �ڶ�ӮǮ���
#define	USER_IDENTITY_TYPE_FAIL		4	// �ڶ���Ǯ���
#define USER_IDENTITY_TYPE_SEAL		8	// ��ű�ʶ

/////////////////////////////////////////////////////////////////////////
// ��̬��

// ��ұ�
struct UserData
{
	int		userID;
	char	account[64];
	char	passwd[64];
	char	name[64];
	char	phone[24];
	BYTE	sex;
	char	mail[64];
	long long money;
	long long bankMoney;
	char	bankPasswd[20];
	int		jewels;					// ������(��ʯ)
	int		roomID;					// ������ڵ�roomID
	int		deskIdx;				// ������ڵ���������
	char	logonIP[24];			// ��ҵĵ�¼IP
	int		winCount;
	char	headURL[256];			// ���ͷ��
	char	macAddr[64];			// mac��ַ�ַ�����ʽ 
	char    token[64];				// GUID
	BYTE	isVirtual;				// �Ƿ������
	int		status;					// �û����
	int		reqSupportTimes;		// �����Ʋ������Ĵ���
	int		lastReqSupportDate;		// �ϴ������Ʋ�����������
	int		registerTime;			// ע��ʱ��
	char	registerIP[24];			// ע��IP
	BYTE	registerType;			// ע������
	int		buyingDeskCount;		// �Ѿ����˵��ǻ�δ��ɢ��������
	int		lastCrossDayTime;		// �ϴε�½ʱ��ʱ��
	int		totalGameCount;			// �ܾ���
	int		sealFinishTime;			// ��Ž���ʱ�䣬-1 ���÷�ţ�0�޷�ţ�����0��ֹʱ��
	char	wechat[24];				// ΢�ź�
	char	phonePasswd[64];		// �ֻ���¼����
	char	accountInfo[64];		// Ψһ��ʶ
	int		totalGameWinCount;		// ��ʤ����
	char    Lng[12];				// ����
	char	Lat[12];				// γ��
	char	address[64];			// ��ַ
	long long lastCalcOnlineToTime; // �ϴμ�������ʱ��ʱ��
	long long allOnlineToTime;		// �ۼ�����ʱ��ʱ��
	BYTE	IsOnline;				// �Ƿ�����
	char	motto[128];				// ����ǩ��
	char	xianliao[64];			// ����Ψһ��Ϣ
	int		controlParam;			// ���˿���Ȩ�أ�������Ϸʹ��
	int		ModifyInformationCount; // �޸���Ϣ����
	BYTE	matchType;				// �������ͣ�MatchType����
	long long combineMatchID;		// �����ı���id��gameID*MAX_GAME_MATCH_ID + id
	BYTE	matchStatus;			// ����״̬ ֵΪUserMatchStatus
	int		curMatchRank;			// ��ǰ�μӵı�������

	UserData()
	{
		memset(this, 0, sizeof(UserData));
		deskIdx = INVALID_DESKIDX;
		controlParam = -1;
	}
};

////////////////////////////////////////////////////////////////////////
// redis �б������Ϣ(����)
struct PrivateDeskInfo
{
	int	roomID;
	int	deskIdx;
	int masterID;
	char passwd[MAX_PRIVATE_DESK_PASSWD_LEN];
	int	buyGameCount;
	char gameRules[256];
	time_t createTime;
	time_t checkTime;
	BYTE masterNotPlay;
	BYTE payType;
	int	currDeskUserCount;
	int	maxDeskUserCount;
	int	currWatchUserCount;
	int	maxWatchUserCount;
	int	friendsGroupID;				// ���ӵľ��ֲ�ID
	int friendsGroupDeskNumber;		// ���ֲ����Ӻ��룬<=0�������б�>0���ֲ�����id
	char arrUserID[128];			// ������������
	int curGameCount;				// ��ǰ��Ϸ���� 0��δ��ʼ��>=1������Ϸ��

	PrivateDeskInfo()
	{
		memset(this, 0, sizeof(PrivateDeskInfo));
		memcpy(passwd, REDIS_STR_DEFAULT, strlen(REDIS_STR_DEFAULT));
		memcpy(gameRules, REDIS_STR_DEFAULT, strlen(REDIS_STR_DEFAULT));
	}
};

// һ����Ϸս��
struct GameGradeInfo
{
	long long id;
	int roomID;
	int deskPasswd;
	int masterID;
	time_t currTime;
	int inning;
	char videoCode[20];
	char userInfoList[192];
	char gameData[1024];

	GameGradeInfo()
	{
		memset(this, 0, sizeof(GameGradeInfo));
		memcpy(gameData, REDIS_STR_DEFAULT, strlen(REDIS_STR_DEFAULT) + 1);
	}
};

// һ������(����)ս���ļ���Ϣ(��Ӧ����������)
struct PrivateDeskGradeSimpleInfo
{
	long long id;
	int roomID;
	int passwd;
	int masterID;
	int time;
	int gameCount;
	int maxGameCount;
	char gameRules[256];
	char userInfoList[192];

	PrivateDeskGradeSimpleInfo()
	{
		memset(this, 0, sizeof(PrivateDeskGradeSimpleInfo));
	}
};

/*
json�ֶζ���:

��ע����
noteRate        ����������עռ��(ռׯ����ׯ���) 1-100ֵ

�����˸���Ƶ��
updateRate	�����˸���Ƶ�� 1-100��ֵ

��ׯ������
zMinC   ��ׯ��������С����
zMaxC   ��ׯ�������������

����������
sMinC   ������������С����
sMaxC   �����������������

������
minC   ��������С����
maxC   �������������
minM   ��������С���
maxM   �����������
*/
//���ر�
struct RewardsPoolInfo
{
	int roomID;						//����id
	long long poolMoney;			//ÿ������Ľ���
	long long gameWinMoney;			//ÿ������ϵͳ����ӮǮ
	long long percentageWinMoney;	//��Ϸ��ˮ��õ�Ǯ
	long long otherWinMoney;		//��������ӮǮ
	long long allGameWinMoney;		//�ܹ���ÿ������ϵͳ����ӮǮ
	long long allPercentageWinMoney;//�ܹ�����Ϸ��ˮ��õ�Ǯ
	long long allOtherWinMoney;		//�ܹ�����������ӮǮ
	BYTE platformCtrlType;			//ƽ̨������Ӯ���ͣ�0������platformCtrlPercent
	int platformCtrlPercent;		//���Ϳ�����Ӯ�ٷֱȣ���Χ0-1000
	int realPeopleFailPercent;		//������������������ʣ���Χ0-1000
	int realPeopleWinPercent;		//��������������Ӯ���ʣ���Χ0-1000
	char detailInfo[1024];          //��Ϸ��������Ƹ��� feng

	RewardsPoolInfo()
	{
		memset(this, 0, sizeof(RewardsPoolInfo));
	}
};

//С���
struct UserRedSpot
{
	int notEMRead;			//δ���ʼ�����
	int notEMReceived;		//δ��ȡ�ʼ�����

	int friendList;			//�����б�������
	int friendNotifyList;	//֪ͨ�б�������

	int FGNotifyList;		//���ֲ�֪ͨ�б�����

	UserRedSpot()
	{
		memset(this, 0, sizeof(UserRedSpot));
	}
};

//����
struct SaveRedisFriendsGroupDesk
{
	int userID;			//Ⱥ��id
	int friendsGroupID;	//���ֲ�id
	int friendsGroupDeskNumber; //����id(1-9)
	int roomType;		//��������
	int gameID;			//��ϷID
	int	maxCount;		//�����Ϸ����
	char gameRules[256];//��Ϸ����
	SaveRedisFriendsGroupDesk()
	{
		memset(this, 0, sizeof(SaveRedisFriendsGroupDesk));
	}
};

//���ֲ�Ȩ��
enum FriendsGroupPowerType
{
	FRIENDSGROUP_POWER_TYPE_NO = 0,			// ��Ȩ��

	FRIENDSGROUP_POWER_TYPE_DEL = 1,		// ɾ����Ա
	FRIENDSGROUP_POWER_TYPE_DESK = 2,		// �����ͽ�ɢ����
	FRIENDSGROUP_POWER_TYPE_VIPROOM = 4,	// �����ͽ�ɢVIP��
	FRIENDSGROUP_POWER_TYPE_FIRE_COIN = 8,	// �һ�����ֵ���
	FRIENDSGROUP_POWER_TYPE_SET = 16,		// ���þ��ֲ�΢�Ż��߹�����߸���

	FRIENDSGROUP_POWER_TYPE_ALL = 31,		// ȫ��Ȩ�ޣ�����Ȩ�޻�����õ�
};

//���ʼ�����
struct EmailSimpleInfo
{
	long long emailID;	//�ʼ���Ψһid
	int sendtime;		//����ʱ��
	EmailSimpleInfo()
	{
		memset(this, 0, sizeof(EmailSimpleInfo));
	}
};

struct FieldRealInfo
{
	const char* field;
	const char* value;
	int	type;
};

//�û�����
struct UserBag
{
	int		userID;				//�û�ID
	int		skillFrozen;		//������������
	int		skillLocking;		//����Ŀ�꼼��
	int		redBag;				//�������
	int		phoneBillCard1;		//���ѿ�1Ԫ
	int		phoneBillCard5;		//���ѿ�5Ԫ
	int		goldenArmor;		//�ƽ�ս����̨����һ�����ã�
	int		mechatroPioneer;	//�����ȷ棨��һ�����ã�
	int		deepSeaArtillery;	//����ڣ���һ�����ã�
	int		octopusCannon;		//������ڣ���һ�����ã�
	int		goldenDragon;		//�ƽ�ʥ������һ�����ã�
	int		lavaArmor;			//����ս�ף���һ�����ã�

	UserBag()
	{
		memset(this, 0, sizeof(UserBag));
	}
};


//������
struct MatchUserInfo
{
	int userID;
	BYTE byMatchStatus;		//����״̬  ö��DeskMatchStatus
	time_t signUpTime;		//��������ʱ��

	MatchUserInfo()
	{
		memset(this, 0, sizeof(MatchUserInfo));
	}
};

//������Ϣ
struct MatchInfo
{
	int matchID;		//������Ϸ�ı��
	int gameID;			//��Ϸid
	int minPeople;		//����������С����
	BYTE costResType;	//������Դ����
	int costResNums;	//��Ϣ��Դ����
	long long startTime;//������ʼʱ��
	BYTE matchStatus;	//����״̬  MatchStatus���� ��0�������У�1�������У�2������������
	BYTE isNotified;	//�Ƿ�֪ͨ������������ʼ

	MatchInfo()
	{
		memset(this, 0, sizeof(MatchInfo));
	}
};