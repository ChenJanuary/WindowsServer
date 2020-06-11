#pragma once

#include "DataLine.h"
#include "Define.h"

/*
������Ϣ��3����:
	1���򵥸�������
	2������ֲ����������������
	3�����½�����������������
*/

#pragma pack(1)

/////////////////////////////// PHP -----> ���ķ� ///////////////////////////////////////////
// ��PHPͨ�ŵ���Ϣ���� 10001-20000
const int PLATFORM_MESSAGE_BEGIN = 10000;

const int PLATFORM_MESSAGE_NOTICE = 10001;					// ����
const int PLATFORM_MESSAGE_REQ_ALL_USER_MAIL = 10002;		// ����ȫ���ʼ�֪ͨ
const int PLATFORM_MESSAGE_CLOSE_SERVER = 10003;			// �ط�����������
const int PLATFORM_MESSAGE_OPEN_SERVER = 10004;				// �������ָ�����
const int PLATFORM_MESSAGE_SEND_HORN = 10005;				// ��������
const int PLATFORM_MESSAGE_MASTER_DISSMISS_DESK = 10006;	// ������ɢ����
const int PLATFORM_MESSAGE_FG_DISSMISS_DESK = 10007;		// ���ֲ�������ɢ����
const int PLATFORM_MESSAGE_RESOURCE_CHANGE = 10008;			// ��֪ͨĳ���ˣ���Դ�仯
const int PLATFORM_MESSAGE_NOTIFY_USERID = 10009;			// ��һ�����������Ϣ
const int PLATFORM_MESSAGE_NOTIFY_FG = 10010;				// ����ֲ��������������Ϣ
const int PLATFORM_MESSAGE_IDENTUSER = 10011;				// �����û����
const int PLATFORM_MESSAGE_RED_SPOT = 10012;				// ����֪ͨС���
const int PLATFORM_MESSAGE_RED_FG_SPOT = 10013;				// ����֪ͨ���ֲ�С���
const int PLATFORM_MESSAGE_RELOAD_GAME_CONFIG = 10014;		// ���¼�����Ϸ����
const int PLATFORM_MESSAGE_PHONE_INFO = 10015;				// �ֻ���֤�ɹ���ע���˺�
const int PLATFORM_MESSAGE_OPEN_ROOM_NOTICE = 10016;		// ����һ���µĿ�����Ϣ
const int PLATFORM_MESSAGE_NOTIFY_MUCH_USERID = 10017;		// �������������Ϣ
const int PLATFORM_MESSAGE_SIGN_UP_MATCH_PEOPLE = 10018;	// ���˱��������˳�������ʵʱ����
const int PLATFORM_MESSAGE_START_MATCH_PEOPLE = 10019;		// ����ʼ����(ʵʱ��)
const int PLATFORM_MESSAGE_MODIFY_TIME_MATCH = 10020;		// �������޸ġ�ɾ����һ����ʱ��
const int PLATFORM_MESSAGE_SIGN_UP_MATCH_TIME = 10021;		// ���˱��������˳���������ʱ����

const int PLATFORM_MESSAGE_END = 20000;

enum PlatformRetCode
{
	PF_SUCCESS = 0,                 // �����ɹ�
	PF_NOUSER = 1,					// �û�������
	PF_SIZEERROR = 2,               // ���ݳ��Ȳ���
	PF_DATA_NULL = 3,               // ���ݰ�Ϊ��
	PF_NOTICE_NUM_NULL = 4,         // ������ֵС��0
	PF_REDIS_NULL = 5,				// redisΪ��
	PF_CLOSE_STATUS_ERR = 6,		// ���͵�ͣ��״̬����ȷ
	PF_DISSMISS_DESK_ERR = 7,       // ��������С��0
	PF_SEND_DATA_ERR = 8,			// ��������ʧ��
	PF_REDSPOT_NOT_EXIST = 9,		// û�м�¼���С�������
};

// ���͹�����Ϣ
struct PlatformNoticeMessage
{
	char	tile[24];		// ����
	char	content[1024];	// ����
	int		interval;		// ���
	int		times;			// ����
	int		type;			// ���� (1:��ͨ 2������ 3:����)

	PlatformNoticeMessage()
	{
		memset(this, 0, sizeof(PlatformNoticeMessage));
	}
};

struct PlatformCloseServerMessage //�ط�
{
	int status;	 // 1:�ط���2������
	PlatformCloseServerMessage()
	{
		memset(this, 0, sizeof(PlatformCloseServerMessage));
	}
};

// ��������
struct PlatformHorn
{
	int userID;
	char userName[64];
	char content[1024];

	PlatformHorn()
	{
		memset(this, 0, sizeof(PlatformHorn));
	}
};

struct PlatformDissmissDesk
{
	int userID;
	int deskMixID;
	bool bDelete;
	int roomID;

	PlatformDissmissDesk()
	{
		memset(this, 0, sizeof(PlatformDissmissDesk));
	}
};

struct PlatformIdentUser
{
	BYTE type;			// 0 ȡ�� 1 ����
	BYTE statusValue;	// 1 ���� 2 Ӯ��� 4 ����� 8 ���
	int otherValue;     // ��Ź��ܣ�������д���ʱ�䣨��λ�룩��-1���÷��

	PlatformIdentUser()
	{
		memset(this, 0, sizeof(PlatformIdentUser));
	}
};

struct PlatformPHPRedSpotNotify //֪ͨС���
{
	enum PHPRedSpotType
	{
		PHP_REDSPOT_TYPE_EMAIL = 0,			//�ʼ�С���
		PHP_REDSPOT_TYPE_FIREND = 1,		//����С���
		PHP_REDSPOT_TYPE_FG = 2,			//���ֲ�С���
	};
	BYTE redspotType;

	PlatformPHPRedSpotNotify()
	{
		memset(this, 0, sizeof(PlatformPHPRedSpotNotify));
	}
};

struct PlatformPHPReloadGameConfig //������Ϸ����
{
	int roomID;

	PlatformPHPReloadGameConfig()
	{
		memset(this, 0, sizeof(PlatformPHPReloadGameConfig));
	}
};

//�ֻ�ע����Ϣ
struct PlatformPhoneInfo
{
	char phone[24];
	char passwd[64];
	char name[64];
	char headURL[256];			// ���ͷ��
	char sex;

	PlatformPhoneInfo()
	{
		memset(this, 0, sizeof(PlatformPhoneInfo));
	}
};

//���������˳�������ʵʱ����
struct PlatformPHPSignUpMatchPeople
{
	int gameID;			//��������Ϸ
	int matchID;		//������Ϸ�ı��
	int curSignUpCount;	//��ǰ��������
	int peopleCount;	//�������˿���
	int allSignUpCount;	//�ۼƱ�������

	PlatformPHPSignUpMatchPeople()
	{
		memset(this, 0, sizeof(PlatformPHPSignUpMatchPeople));
	}
};

//����(ʵʱ��)
struct PlatformPHPReqStartMatchPeople
{
	int gameID;			//��������Ϸ
	int matchID;		//������Ϸ�ı��
	int matchRound;		//��������
	int peopleCount;	//�������˿�

	PlatformPHPReqStartMatchPeople()
	{
		memset(this, 0, sizeof(PlatformPHPReqStartMatchPeople));
	}
};

//�������޸ġ�ɾ����һ����ʱ��
struct PlatformPHPModifyTimeMatch
{
	BYTE type;			//0��������1��ɾ����2���޸�
	int matchID;		//������Ϸ�ı��
	int gameID;			//��Ϸid
	int minPeople;		//����������С����
	BYTE costResType;	//������Դ����
	int costResNums;	//��Ϣ��Դ����
	long long startTime;//������ʼʱ��

	PlatformPHPModifyTimeMatch()
	{
		memset(this, 0, sizeof(PlatformPHPModifyTimeMatch));
	}
};

//���������˳���������ʱ����
struct PlatformPHPSignUpMatchTime
{
	int matchID;		//������Ϸ�ı��
	int curSignUpCount;	//��ǰ��������

	PlatformPHPSignUpMatchTime()
	{
		memset(this, 0, sizeof(PlatformPHPSignUpMatchTime));
	}
};

///////////////////////////////// �����ķ� -----> ��½����  �����ķ� -----> ��Ϸ���� ///////////////////////////////////
const int CENTER_MESSAGE_COMMON_BEGIN = 20000;

// ���ķ����ͣ�С��20100������TcpConnect.h��
const int CENTER_MESSAGE_COMMON_REPEAT_ID = 20101;			// ������Ψһid�ظ�
const int CENTER_MESSAGE_COMMON_LOGON_GROUP_INFO = 20102;	// ��½����Ⱥ��Ϣ
const int CENTER_MESSAGE_COMMON_RESOURCE_CHANGE = 20103;	// ��֪ͨĳ���ˣ���Դ�仯
const int CENTER_MESSAGE_COMMON_AUTO_CREATEROOM = 20104;	// ֪ͨ��½���������Զ�����
const int CENTER_MESSAGE_COMMON_START_MATCH_TIME = 20105;	// ֪ͨ��Ϸ����ʼ��ʱ��

const int CENTER_MESSAGE_COMMON_END = 30000;

struct PlatformResourceChange //֪ͨ(��Ϸ���ʹ�����)�����Դ�仯
{
	int resourceType;
	long long value;		// ȫ��
	long long changeValue;	// ����
	int reason;				// ԭ��
	int reserveData;		// Ԥ���ֶΣ���Ҫ������ֲ�id
	BYTE isNotifyRoom;		// �Ƿ�֪ͨ����Ϸ

	PlatformResourceChange()
	{
		memset(this, 0, sizeof(PlatformResourceChange));
	}
};

struct PlatformDistributedSystemInfo
{
	UINT logonGroupIndex;		//��½����Ⱥ����
	UINT logonGroupCount;		//��½����Ⱥ����
	UINT mainLogonGroupIndex;	//��ǰ��Ҫ��½����Ⱥ���������������������Ⱥ����������ݣ�

	PlatformDistributedSystemInfo()
	{
		memset(this, 0, sizeof(PlatformDistributedSystemInfo));
	}
};

struct PlatformRepeatUserLogon
{
	int userID;

	PlatformRepeatUserLogon()
	{
		memset(this, 0, sizeof(PlatformRepeatUserLogon));
	}
};

struct PlatformAutoCreateRoom
{
	int masterID;
	int	friendsGroupID;			// ���ӵľ��ֲ�ID
	int friendsGroupDeskNumber;	// ���ֲ����Ӻ��룬<=0�������б�>0���ֲ�����id
	int roomType;				//��������
	int gameID;			//��ϷID
	int	maxCount;		//�����Ϸ����
	char gameRules[256];//��Ϸ����

	PlatformAutoCreateRoom()
	{
		memset(this, 0, sizeof(PlatformAutoCreateRoom));
	}
};

//����(��ʱ��)
struct PlatformReqStartMatchTime
{
	int gameID;			//��Ϸid
	int matchID;		//������Ϸ�ı��
	int minPeople;		//����������С����
	BYTE costResType;	//������Դ����
	int costResNums;	//��Ϣ��Դ����

	PlatformReqStartMatchTime()
	{
		memset(this, 0, sizeof(PlatformReqStartMatchTime));
	}
};

//////////////////////////// ��½�� -----> ���ķ� ////////////////////////////////////
const int CENTER_MESSAGE_LOGON_BEGIN = 30000;

const int CENTER_MESSAGE_LOGON_RESOURCE_CHANGE = 30001;			// ��Դ�仯
const int CENTER_MESSAGE_LOGON_USER_LOGON_OUT = 30002;			// ��ҵ�¼�����ߵ���
const int CENTER_MESSAGE_LOGON_REPEAT_USER_LOGON = 30003;		// ����ظ���¼
const int CENTER_MESSAGE_LOGON_RELAY_USER_MSG = 30004;			// ת����Ϣ������
const int CENTER_MESSAGE_LOGON_RELAY_FG_MSG = 30005;			// ת����Ϣ�����ֲ�
const int CENTER_MESSAGE_LOGON_REQ_FG_REDSPOT_MSG = 30006;		// ��������ֲ�������ҷ���С���
const int CENTER_MESSAGE_LOGON_MASTER_DISSMISS_DESK = 30007;	// �����ɢ����

const int CENTER_MESSAGE_LOGON_END = 40000;


struct PlatformUserLogonLogout
{
	int userID;
	BYTE type; //����˵�� 0:���� 1���Ƴ�
	BYTE isVirtual;

	PlatformUserLogonLogout()
	{
		memset(this, 0, sizeof(PlatformUserLogonLogout));
	}
};

//���������͵�С�������
struct PlatformFriendsGroupPushRedSpot
{
	int friendsGroupID;			//���ֲ�id
	int deskMsgRedSpotCount;    //������Ϣ����
	int VIPRoomMsgRedSpotCount;	//VIP������Ϣ����

	PlatformFriendsGroupPushRedSpot()
	{
		memset(this, 0, sizeof(PlatformFriendsGroupPushRedSpot));
	}
};

////////////////////////////��Ϸ�� -----> ���ķ�////////////////////////////////////
const int CENTER_MESSAGE_LOADER_BEGIN = 40000;

const int CENTER_MESSAGE_LOADER_RESOURCE_CHANGE = 40001;			// ��Դ�仯(��Һ���ʯ�仯)
const int CENTER_MESSAGE_LOADER_BUYDESKINFO_CHANGE = 40002;			// ������Ϣ�����仯(�����仯)
const int CENTER_MESSAGE_LOADER_DESK_DISSMISS = 40003;				// ���ӽ�ɢ
const int CENTER_MESSAGE_LOADER_DESK_STATUS_CHANGE = 40004;			// ���Ӿ����仯
const int CENTER_MESSAGE_LOADER_FIRECOIN_CHANGE = 40005;			// ��Դ�仯(��ұ仯)
const int CENTER_MESSAGE_LOADER_REWARD_ACTIVITY = 40006;			// ���֪ͨ
const int CENTER_MESSAGE_LOADER_NOTIFY_START_MATCH = 40007;			// ֪ͨǰ�˽�����Ϸ��ʼ����
const int CENTER_MESSAGE_LOADER_NOTIFY_START_MATCH_FAIL = 40008;	// ֪ͨǰ�˱�����ʼʧ��

const int CENTER_MESSAGE_LOADER_END = 50000;

//���ֲ���Ϣ�ṹ��
struct PlatformFriendsGroupMsg
{
	int	friendsGroupID;			// ���ӵľ��ֲ�ID
	int friendsGroupDeskNumber;	// ���ֲ����Ӻ��룬<=0�������б�>0���ֲ�����id
	bool bDeleteDesk;			// ���ֲ������Ƿ��������

	PlatformFriendsGroupMsg()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0, sizeof(PlatformFriendsGroupMsg));
	}
};

// �����仯
struct PlatformDeskPeopleCountChange
{
	int	friendsGroupID;			// ���ӵľ��ֲ�ID
	int friendsGroupDeskNumber;	// ���ֲ����Ӻ��룬<=0�������б�>0���ֲ�����id
	int masterID;
	char roomPasswd[20];
	int currUserCount;
	BYTE updateType;			// 0:���ӣ�1������
	int userID;					// ���ӻ������ҵ�ID
	PlatformDeskPeopleCountChange()
	{
		memset(this, 0, sizeof(PlatformDeskPeopleCountChange));
	}
};

// ��Ϸ�����
struct PlatformDeskDismiss
{
	int masterID;
	int	friendsGroupID;			// ���ӵľ��ֲ�ID
	int friendsGroupDeskNumber;	// ���ֲ����Ӻ��룬<=0�������б�>0���ֲ�����id
	char passwd[20];			// �����
	BYTE bDeleteDesk;			// ���ֲ������Ƿ��������

	// �Զ�����ʹ��
	int roomType;				//��������
	int gameID;					//��ϷID
	int	maxCount;				//�����Ϸ����
	char gameRules[256];		//��Ϸ����

	PlatformDeskDismiss()
	{
		memset(this, 0, sizeof(PlatformDeskDismiss));
	}
};

// �����仯
struct PlatformDeskGameCountChange
{
	int	friendsGroupID;			// ���ӵľ��ֲ�ID
	int friendsGroupDeskNumber;	// ���ֲ����Ӻ��룬<=0�������б�>0���ֲ�����id
	BYTE gameStatus;			// ��Ϸ״̬��0��δ��ʼ��1������Ϸ��
	PlatformDeskGameCountChange()
	{
		memset(this, 0, sizeof(PlatformDeskGameCountChange));
	}
};

// ֪ͨ����
struct PlatformLoaderNotifyStartMatch
{
	int gameID;			//��������Ϸ
	BYTE matchType;		//��������
	int matchID;		//������Ϸ�ı��
	int roomID;
	int peopleCount;	//��������
	int userID[MAX_MATCH_PEOPLE_COUNT];

	PlatformLoaderNotifyStartMatch()
	{
		memset(this, 0, sizeof(PlatformLoaderNotifyStartMatch));
	}
};

// ֪ͨ����ʧ��
struct PlatformLoaderNotifyStartMatchFail
{
	int gameID;			//��������Ϸ
	BYTE matchType;		//��������
	int matchID;		//������Ϸ�ı��
	BYTE reason;		//����ʧ��ԭ��

	PlatformLoaderNotifyStartMatchFail()
	{
		memset(this, 0, sizeof(PlatformLoaderNotifyStartMatchFail));
	}
};

#pragma pack()
