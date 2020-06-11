#pragma once

#include "Define.h"

//////////////////////////////////////////////////////////////////////////
const unsigned int MSG_MAIN_TEST = 1;					// ������Ϣ

const unsigned int 	MSG_ASS_TEST = 1;					// ������Ϣ

//////////////////////////////////////////////////////////////////////////
const unsigned int MSG_MAIN_CONECT = 2;					// ���Ӳ���

//////////////////////////////////////////////////////////////////////////

const unsigned int MSG_MAIN_USER_SOCKET_CLOSE = 4;		// ��ҵ���֪ͨ��Ϸ��

// ��Ϣ�����ļ�
// ������Ϣ��ID��Χ			100-499
// ��Ϸ��Ϣ��ID��Χ			500-999


//////////////////////////////////////////////////////////////////////
// ������Ϣ����
//////////////////////////////////////////////////////////////////////

// ע�����
const unsigned int MSG_MAIN_LOGON_REGISTER = 100;


// ��¼���
const unsigned int MSG_MAIN_LOGON_LOGON = 101;

const unsigned int MSG_ASS_LOGON_LOGON = 1; //�˺ŵ�¼
// �������
const unsigned int MSG_ASS_LOGON_DESK = 102;

const unsigned int MSG_ASS_LOGON_BUY_DESK = 1;			// ��������
const unsigned int MSG_ASS_LOGON_ENTER_DESK = 2;		// ��������


// ֪ͨ���
const unsigned int MSG_MAIN_LOGON_NOTIFY = 103;

const unsigned int MSG_NTF_LOGON_USER_SQUEEZE = 1;			// ��ұ����ţ�������
const unsigned int MSG_NTF_LOGON_NOTICE = 2;				// ����
const unsigned int MSG_NTF_LOGON_HORN = 3;					// ����
const unsigned int MSG_NTF_LOGON_USER_EXIST = 4;			// ����ѵ�¼
const unsigned int MSG_NTF_LOGON_RESOURCE_CHANGE = 5;		// ��Դ�仯
const unsigned int MSG_NTF_LOGON_SUPPORT_INFO = 6;			// �Ʋ�������Ϣ����¼�ɹ�֮��������ͣ�
const unsigned int MSG_NTF_LOGON_BUYDESKINFO_CHANGE = 7;	// ֪ͨ�����б���Ϣ�仯
const unsigned int MSG_NTF_LOGON_FRIEND_NOTIFY = 8;			// ����֪ͨ
const unsigned int MSG_NTF_LOGON_FRIEND_ADDOK = 9;			// ��Ӻ��ѳɹ�
const unsigned int MSG_NTF_LOGON_FRIEND_DELOK = 10;			// ɾ�����ѳɹ�
const unsigned int MSG_NTF_LOGON_FIREND_DELNOTIFY = 11;		// ɾ������֪ͨ
const unsigned int MSG_NTF_LOGON_RIEND_LOGINOROUT = 12;		// ֪ͨ���ѵ�¼���ߵǳ�
const unsigned int MSG_NTF_LOGON_FRIEND_REDSPOT = 13;		// ֪ͨ����С���
const unsigned int MSG_NTF_LOGON_CLOSE_SERVER = 14;			// �ط�����֪ͨ
const unsigned int MSG_NTF_LOGON_EMAIL_REDSPOT = 15;		// �ʼ�С���
const unsigned int MSG_NTF_LOGON_NEW_EMAIL = 16;			// ���ʼ�֪ͨ
const unsigned int MSG_NTF_LOGON_REWARD_ACTIVITY = 17;		// ��󽱹���֪ͨ

// ����
const unsigned int MSG_MAIN_LOGON_OTHER = 104;

const unsigned int MSG_ASS_LOGON_OTHER_SEND_HORN = 1;				// ����������
const unsigned int MSG_ASS_LOGON_OTHER_SEND_GIFT = 2;				// ����ת��
const unsigned int MSG_ASS_LOGON_OTHER_SEND_GRADE = 3;				// ����ս��
const unsigned int MSG_ASS_LOGON_OTHER_SEND_ROOMOPENED = 4;			// �����������˿�����Ϣ
const unsigned int MSG_ASS_LOGON_OTHER_USERINFO_FLUSH = 5;			// ˢ�¸�����Ϣ
const unsigned int MSG_ASS_LOGON_OTHER_EMAIL_LIST = 6;				// �ʼ��б���Ϣ
const unsigned int MSG_ASS_LOGON_OTHER_EMAIL_INFO = 7;				// �ʼ�������Ϣ
const unsigned int MSG_ASS_LOGON_OTHER_EMAIL_DELETE = 8;			// ɾ���ʼ�
const unsigned int MSG_ADD_LOGON_OTHER_REQ_SUPPORT = 9;				// �����Ʋ�����
const unsigned int MSG_ASS_LOGON_OTHER_ROBOT_TAKEMONEY = 10;		// ������ȡǮ
const unsigned int MSG_ASS_LOGON_OTHER_REQ_SIMPLE_GRADELIST = 11;	// ����ս���б�(�����)
const unsigned int MSG_ASS_LOGON_OTHER_REQ_SIMPLE_GRADEINFO = 12;	// ����ս����Ϣ(�����)
const unsigned int MSG_ASS_LOGON_OTHER_REQ_GRADELIST = 13;			// ���󵥾���Ϸս���б�
const unsigned int MSG_ASS_LOGON_OTHER_REQ_GRADEINFO = 14;			// ���󵥾���Ϸ��Ϣ
const unsigned int MSG_ASS_LOGON_OTHER_REQ_USERINFO = 15;			// ��ѯ�����Ϣ
const unsigned int MSG_ASS_LOGON_OTHER_REQ_DISSMISS_DESK = 16;		// �����ɢ����
const unsigned int MSG_ASS_LOGON_OTHER_REQ_SAVE_POSITION = 17;		// ���󱣴�λ��
const unsigned int MSG_ASS_LOGON_OTHER_REQ_ENTER_OR_LEAVE_THE_GAMERO = 18;	// ������뷿��
const unsigned int MSG_ASS_LOGON_OTHER_JOIN_MATCH_SCENE = 19;		// �������������
const unsigned int MSG_ASS_LOGON_OTHER_EXIT_MATCH_SCENE = 20;		// �˳�����������

// ����
const unsigned int MSG_MAIN_LOGON_BANK = 105;

const unsigned int MSG_ASS_LOGON_BANK_SAVE_MONEY = 1;				// ��Ǯ
const unsigned int MSG_ASS_LOGON_BANK_TAKE_MONEY = 2;				// ȡǮ
const unsigned int MSG_ASS_LOGON_BANK_TRANSFER = 3;					// ת��
const unsigned int MSG_ASS_LOGON_BANK_SETPASSWD = 4;				// ��������
const unsigned int MSG_ASS_LOGON_BANK_RESETPASSWD = 5;				// �޸�����
const unsigned int MSG_ASS_LOGON_BANK_RECORD = 6;					// ���м�¼

// ����
const unsigned int MSG_MAIN_LOGON_FRIEND = 106;

const unsigned int MSG_ASS_LOGON_FRIEND_LIST = 1;				// ������Ϣ
const unsigned int MSG_ASS_LOGON_FRIEND_ADD = 2;				// ��Ӻ���
const unsigned int MSG_ASS_LOGON_FRIEND_ANSWER_ADD = 3;			// ��Ӧ��Ӻ���
const unsigned int MSG_ASS_LOGON_FRIEND_DEL = 4;				// ɾ������
const unsigned int MSG_ASS_LOGON_FRIEND_REWARD = 5;				// ����
const unsigned int MSG_ASS_LOGON_FRIEND_TAKEREWARD = 6;			// ��ȡ����
const unsigned int MSG_ASS_LOGON_FRIEND_SERACH = 7;				// �������
const unsigned int MSG_ASS_LOGON_FRIEND_CHANGEBATCH = 8;		// ��һ��
const unsigned int MSG_ASS_LOGON_FRIEND_NOTIFYLIST = 9;			// ��ѯ֪ͨ�б�
const unsigned int MSG_ASS_LOGON_FRIEND_DELNOTIFY = 10;			// ɾ��֪ͨ
const unsigned int MSG_ASS_LOGON_FRIEND_INVITEPLAY = 11;		// �������ߺ���(����Ϸ)
const unsigned int MSG_ASS_LOGON_FRIEND_DEL_REDSPOT = 12;		// ����ɾ��С���

// ���ֲ�
const unsigned int MSG_MAIN_LOGON_FRIENDSGROUP = 107;

const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_LIST = 1;				// ���ֲ��б�
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_MEMBER = 2;			// ���ֲ���Ա
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_ALL_NOTIFY_MSG = 3;	// �������֪ͨ��Ϣ��ĳ��ʱ����ǰ��8k��Ϣ��
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_ALL_ROOM_MSG = 4;		// ���ֲ����п�����Ϣ��ĳ��ʱ����ǰ��8k��Ϣ��
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_ALL_ACCO_MSG = 5;		// ���ֲ�����ս����Ϣ��ĳ��ʱ����ǰ��8k��Ϣ��
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_CREATE = 6;			// �������ֲ�
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_REQ_JOIN = 7;			// ���������ֲ�
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_INVITE = 8;			// ������Ѽ�����ֲ�
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_ANSWER_OPER = 9;      // ��Ӧ֪ͨ��Ϣ
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_DEL_MEMBER = 10;		// Ⱥ��ɾ�����ֲ���Ա
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_QUIT = 11;			// �˳����ֲ�
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_DISMISS = 12;			// ��ɢ���ֲ�
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_CHANGE_NAME = 13;		// �ı���ֲ�����
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_DEL_NOTIFY_MSG = 14;	// ɾ��һ��֪ͨ��Ϣ
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_SIMPLE_INFO = 15;		// ��ѯ���ֲ�����Ϣ
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_DEL_REDSPOT = 16;		// ����ɾ��С���
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_SEND_NOTICE = 17;		// ���󷢲�����
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_QUERY_ACCO = 18;		// ��ѯս��ͳ��
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_DESK_LIST_MSG = 19;	// ���ֲ������б�
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_DELETE_DESK_INFO = 20;// ɾ�����ֲ�һ������
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_COST_STATISTICS = 21; // ��ѯ����ͳ��
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_REQ_TRANSFER = 22;	// ����ת�þ��ֲ�
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_REQ_AUTH_MEM = 23;	// ������Ȩ���ֲ���Ա����ȡ���û����
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_MODIFY_WECHAT = 24;	// �����޸�΢��
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_MODIFY_POWER = 25;	// �����޸Ĺ���ԱȨ��
const unsigned int MSG_ASS_LOGON_FRIENDSGROUP_CHANGE_FIRECOIN = 26;	// �����ֵ���߶һ���һ��

// ���ֲ�֪ͨ
const unsigned int MSG_MAIN_FRIENDSGROUP_NOTIFY = 108;

const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_NOTIFY_MSG = 1;		// �������������͵�һ��֪ͨ��Ϣ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_ROOM_MSG = 2;			// �������������͵ľ��ֲ�������Ϣ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_ACCO_MSG = 3;			// �������������͵ľ��ֲ�ս����Ϣ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_REDSPOT = 4;			// ֪ͨ���ֲ�С���
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_MSG_HAVE_CHANGE = 5;	// ���ֲ�������Ϣ�仯֪ͨ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_CREATE_FG = 6;		// ��������������һ�����ֲ�
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_NAME_CHANGE = 7;		// ���ֲ����ֱ仯֪ͨ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_PEOPLE_CHANGE = 8;	// ���ֲ��˱仯֪ͨ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_DISMISS = 9;			// ���ֲ�����ɢ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_NOTICE_CHANGE = 10;	// ���ֲ�����仯֪ͨ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_NEW_DESK_MSG = 11;	// ������Ϣ(����)
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_DESK_INFO_CHANGE = 12;// �����仯֪ͨ�����º�ɾ����
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_STATUS_CHANGE = 13;	// ���ֲ���ݱ仯֪ͨ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_WECHAT_CHANGE = 14;	// ���ֲ�΢�ű仯֪ͨ
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_POWER_CHANGE = 15;	// ���ֲ����Ȩ�ޱ��
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_NEW_VIPROOM_MSG = 16;	// ���͵ľ��ֲ�VIP����(����)
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_VIPROOM_CHANGE = 17;  // vip����仯֪ͨ�����º�ɾ����
const unsigned int MSG_NTF_LOGON_FRIENDSGROUP_FIRECOIN_CHANGE = 18; // ��һ�ұ仯֪ͨ

// ������֪ͨ
const unsigned int MSG_MAIN_MATCH_NOTIFY = 109;

const unsigned int MSG_NTF_LOGON_MATCH_NOTIFY_SIGNUP_CHANGE_PEOPLE = 1;	// ���������仯(ʵʱ��)
const unsigned int MSG_NTF_LOGON_MATCH_NOTIFY_START_MATCH = 2;			// ֪ͨ��������
const unsigned int MSG_NTF_LOGON_MATCH_NOTIFY_SIGNUP_CHANGE_TIME = 3;	// ���������仯(��ʱ��)
const unsigned int MSG_NTF_LOGON_MATCH_NOTIFY_START_MATCH_FAIL = 4;		// ֪ͨ����ʧ��
const unsigned int MSG_NTF_LOGON_MATCH_NOTIFY_START_MATCH_RIGHTAWAY = 5;// ֪ͨ����������ʼ

//////////////////////////////////////////////////////////////////////////
#pragma pack(1)

//ö�ٶ���

// ��Դ�仯ԭ��
enum ResourceChangeReason
{
	// ��Ϸ����Ⱥʹ��
	RESOURCE_CHANGE_REASON_DEFAULT = 0,
	RESOURCE_CHANGE_REASON_CREATE_ROOM,				// ������������
	RESOURCE_CHANGE_REASON_GAME_BEGIN,				// ��Ϸ��ʼ
	RESOURCE_CHANGE_REASON_GAME_FINISHED,			// ��Ϸ����
	RESOURCE_CHANGE_REASON_GAME_SELLETE_ROLLBACK,	// �����û������ս������
	RESOURCE_CHANGE_REASON_GAME_SELLETE_NORAML,		// �������֧ͨ��
	RESOURCE_CHANGE_REASON_GAME_SELLETE_AA,			// ��Ϸ�����AA֧��
	RESOURCE_CHANGE_REASON_GOLD_ROOM_PUMP,			// ��ҷ���ˮ����
	RESOURCE_CHANGE_REASON_ROOM_PUMP_CONSUME,		// �����ˮ�۳�
	RESOURCE_CHANGE_REASON_SYSTEM_SUBSIDY,			// ϵͳ�������(ԭ������Ϸ�۳���ҿ۳��˸���)
	RESOURCE_CHANGE_REASON_REGISTER,				// ע��
	RESOURCE_CHANGE_REASON_MAGIC_EXPRESS,			// ħ������

	// PHP��ʹ��
	RESOURCE_CHANGE_REASON_BANK_SAVE = 1000, 		// ���д�Ǯ
	RESOURCE_CHANGE_REASON_BANK_TAKE,           	// ����ȡǮ
	RESOURCE_CHANGE_REASON_BANK_TRAN,				// ����ת��
	RESOURCE_CHANGE_REASON_GIVE,		 			// ת��
	RESOURCE_CHANGE_REASON_SUPPORT,			 		// �ȼý�
	RESOURCE_CHANGE_REASON_SIGN,			 		// ǩ��
	RESOURCE_CHANGE_REASON_BACK_RECHARE,			// ��̨��ֵ��������
	RESOURCE_CHANGE_REASON_PAY_RECHARE,			 	// �̳�֧����ֵ
	RESOURCE_CHANGE_REASON_AGENT_RECHARE,			// �����ֵ
	RESOURCE_CHANGE_REASON_TURNTABLE,				// ת��
	RESOURCE_CHANGE_REASON_SHARE,					// ����
	RESOURCE_CHANGE_REASON_FRIEND_REWARD,			// ���Ѵ���
	RESOURCE_CHANGE_REASON_BIND_PHONE,				// ���ֻ�
	RESOURCE_CHANGE_REASON_SEND_HORN,				// ����㲥
	RESOURCE_CHANGE_REASON_MAIL_REWARD,				// �ʼ�����
	RESOURCE_CHANGE_REASON_BIND_AGENT,				// �󶨴���
	RESOURCE_CHANGE_REASON_INVITE_ENTER,			// ���������Ϸ
};

enum NOTIFY_MSG_TYPE
{
	SMT_CHAT = 1,				//������Ϣ
	SMT_EJECT = 2,				//������Ϣ
	SMT_GLOBAL = 4,				//ȫ����Ϣ
	SMT_PROMPT = 8,				//��ʾ��Ϣ
	SMT_TABLE_ROLL = 16,		//������Ϣ
};

///////////////////////////////////////////
// �ṹ�嶨��

// ����ע��
struct LogonRequestRegister
{
	BYTE		byFromPhone;			//0-��PCע�ᣬ1-���ƶ���ע��
	BYTE		byFastRegister;			//0-����ע�ᣬ1-��ͨע��,2-΢��ע�ᣬ3-QQע�ᣬ4-�ֻ�ע��
	char		szAccount[64];			//�û��� ΢���ǳ� �ֻ�����
	char		szPswd[64];				//���� ΢�š�QQ��¼��ʶ
	BYTE		byStatus;			    //0 ע�� 1 ��¼ ����˺ŵ�¼	
	BYTE		bySex;					//�Ա�
	char		szHeadUrl[256];			//ͷ��URL
	char		szAccountInfo[64];		//Ψһ��ʶ

	// ѡ���Է���
	BYTE		byPlatformType;			//Ĭ��0   1��ios  2��׿   3��win32
	LogonRequestRegister()
	{
		memset(this, 0, sizeof(LogonRequestRegister));
	}
};

// ��Ӧע��
struct LogonResponseRegister
{
	int  userID;			//�û�ID
	char szToken[64];		//�û�Token��sessioncode��
	char szAccountInfo[64];	//Ψһ��ʶ
	BYTE byFastRegister;	//1-��ͨע��,2-΢��ע�ᣬ3-QQע��

	LogonResponseRegister()
	{
		memset(this, 0, sizeof(LogonResponseRegister));
	}
};

// �����¼����
struct LogonRequestLogon
{
	char		szMacAddr[64];						//mac��ַ
	int		    iUserID;							//�û�ID��¼�����ID>0��ID��¼
	char        szToken[64];                        //�û�Token��sessioncode��
	char        szVersions[24];                     //�汾��

	// ѡ���Է���
	BYTE		byPlatformType;						//Ĭ��0   1��ios  2��׿   3��win32
	LogonRequestLogon()
	{
		memset(this, 0, sizeof(LogonRequestLogon));
	}
};

// ��¼��������
struct LogonResponseLogon
{
	char	name[64];
	char	phone[24];
	BYTE	sex;
	long long money;
	int		jewels;					// ������(��ʯ)
	int		roomID;					// ������ڵ�roomID
	int		deskIdx;				// ������ڵ���������
	char	logonIP[24];			// ��ҵĵ�¼IP
	char	headURL[256];			// ���ͷ��
	long long bankMoney;			// ���н��
	char	bankPasswd[20];			// ��������
	char	longitude[12];			// ����
	char	latitude[12];			// γ��
	char	address[64];			// ��ַ
	char	motto[128];				// ����ǩ��
	BYTE	visitor;				// �Ƿ����ο�
	long long combineMatchID;		// �����ı���id��gameID*MAX_GAME_MATCH_ID + id
	BYTE	matchStatus;			// ����״̬ ֵΪUserMatchStatus

	LogonResponseLogon()
	{
		memset(this, 0, sizeof(LogonResponseLogon));
	}
};

struct LogonResponseFail
{
	BYTE byType; //0 �˺ű���
	int iRemainTime;//ʣ��ʱ�� ��-1���÷��
	LogonResponseFail()
	{
		memset(this, 0, sizeof(LogonResponseFail));
	}
};

// ����������
struct LogonRequestBuyDesk
{
	int		userID;				// �û�ID
	int		roomType;			// �������ͣ�1�����֣�2����ҷ���3��vip����
	int		gameID;				// ��ϷID
	int		count;				// �������
	BYTE	needExt;			// �Ƿ��Զ���չ��0:���Զ���չ�����־ͽ�ɢ 1:�ǣ�
	char	gameRules[256];		// ��Ϸ���� json ��ʽ����ϷDLL�˺Ϳͻ������ж��� {"zhuang":"0"}
	BYTE	masterNotPlay;		// �Ƿ������˿���(1:Yes 0:No)
	int		friendsGroupID;     // ���ӵľ��ֲ�ID
	int		friendsGroupDeskNumber;	// ���룬0��vip����(vip����ĺ�����ϵͳ�Զ�����)������0������

	LogonRequestBuyDesk()
	{
		memset(this, 0, sizeof(LogonRequestBuyDesk));
	}
};

// �������ӽ��
struct LogonResponseBuyDesk
{
	int		userID;					// �û�ID
	int		roomID;					// �����
	int		deskIdx;				// ���Ӻ�
	char	passwd[20];				// ��������
	int		passwdLen;				// ���볤��
	int		jewels;					// ��������ʯ
	BYTE	byMasterNotPlay;		// �Ƿ������˿���(1:Yes 0:No)

	LogonResponseBuyDesk()
	{
		memset(this, 0, sizeof(LogonResponseBuyDesk));
	}
};

//����VIP����
struct LogonRequestEnterDesk
{
	int		userID;
	int		passwdLen;
	char	passwd[20];

	LogonRequestEnterDesk()
	{
		memset(this, 0, sizeof(LogonRequestEnterDesk));
	}
};

//����VIP���ӽ��
struct LogonResponseEnterDesk
{
	int		userID;
	int		roomID;
	int		deskIdx;

	LogonResponseEnterDesk()
	{
		memset(this, 0, sizeof(LogonResponseEnterDesk));
	}
};

// ֪ͨ����
struct LogonNotifyNotice
{
	int		interval;		// ���
	int		times;			// ����
	int		type;			// ����(1:��ͨ 2:����)
	char	tile[24];		// ����
	int		sizeCount;		// �����ֽ�����
	char	content[1024];	// ����

	LogonNotifyNotice()
	{
		memset(this, 0, sizeof(LogonNotifyNotice));
	}
};

// ֪ͨ����
struct LogonNotifyHorn
{
	int userID;
	char userName[64];
	int sizeCount;     //content�ֽ�����
	char content[1024];

	LogonNotifyHorn()
	{
		memset(this, 0, sizeof(LogonNotifyHorn));
	}
};

// �������
struct LogonNotifyActivity
{
	int		sizeCount;		// �����ֽ�����
	char	content[1024];	// ����

	LogonNotifyActivity()
	{
		memset(this, 0, sizeof(LogonNotifyActivity));
	}
};

struct LogonNotifyResourceChange
{
	int resourceType;
	long long value;
	int reason;				// ԭ��
	int param1;				// ����
	long long changeValue;  // �仯��

	LogonNotifyResourceChange()
	{
		memset(this, 0, sizeof(LogonNotifyResourceChange));
	}
};

//������Ϣˢ��
struct LogonUserInfoFlush
{
	int iJewels;//����
	long long money; //���

	LogonUserInfoFlush()
	{
		memset(this, 0, sizeof(LogonUserInfoFlush));
	}
};

// ֪ͨ�����б���Ϣ�仯
struct LogonNotifyBuyDeskInfoChange
{
	enum ChangeType
	{
		CHANGE_REASON_USERCOUNT_CHANGE = 1,		// �����仯
		CHANGE_REASON_DEL = 2,					// ɾ�����ӣ�����㣩
		CHANGE_REASON_BEGIN = 3,				// ��Ϸ��ʼ�������仯��
	};

	int changeType;		//��ӦChangeType
	char passwd[20];
	int currUserCount;

	LogonNotifyBuyDeskInfoChange()
	{
		memset(this, 0, sizeof(LogonNotifyBuyDeskInfoChange));
	}
};

//������ȡǮ
struct _LogonResponseRobotTakeMoney
{
	int iMoney;
	int iRoomID;
	_LogonResponseRobotTakeMoney()
	{
		memset(this, 0, sizeof(_LogonResponseRobotTakeMoney));
	}
};

struct _LogonRobotTakeMoneyRes
{
	int iMoney;
	_LogonRobotTakeMoneyRes()
	{
		memset(this, 0, sizeof(_LogonRobotTakeMoneyRes));
	}
};

struct LogonRequestUserInfo
{
	int userID;

	LogonRequestUserInfo()
	{
		memset(this, 0, sizeof(LogonRequestUserInfo));
	}
};

struct LogonResponseUserInfo
{
	char name[64];
	char headURL[256];
	BYTE sex;
	int gameCount;
	int winCount;
	BYTE onlineStatus;
	char longitude[12];
	char latitude[12];
	char address[64];
	int jewels;
	long long money;
	char ip[24];
	char motto[128];// ����ǩ��

	LogonResponseUserInfo()
	{
		memset(this, 0, sizeof(LogonResponseUserInfo));
	}
};

// �ʼ�С���֪ͨ
struct LogonNotifyEmailRedSpot
{
	int notReadCount;	 //δ���ʼ�����
	int notReceivedCount;//δ��ȡ�ʼ�����
	LogonNotifyEmailRedSpot()
	{
		memset(this, 0, sizeof(LogonNotifyEmailRedSpot));
	}
};

// ֪ͨ����С���
struct LogonNotifyFriendRedSpot
{
	int friendListRedSpotCount;		// �����б�������
	int notifyListRedSpotCount;		// ֪ͨ�б�������
	LogonNotifyFriendRedSpot()
	{
		memset(this, 0, sizeof(LogonNotifyFriendRedSpot));
	}
};


//////////////////////////////////////////////////////////////////////////
//���ֲ����Э��

//��ҵľ��ֲ��б�
struct OneFriendsGroupInfo
{
	int friendsGroupID;
	BYTE status;		//���ֲ�����ݣ�1Ⱥ����0��ͨ��Ա��2����Ա
	int currOnlineCount;//��ǰ��������
	int peopleCount;	//���ֲ�����
	int deskCount;		//�ѿ���������
	int VIPRoomCount;	//�ѿ�VIP��������
	int frontMember[9];	//ǰ�漸�����userID
	int createTime;
	int masterID;
	char name[24];		//���ֲ�����
	char notice[128];	//���ֲ�����
	char wechat[48];	//΢��Ⱥ
	BYTE power;			//������ֲ���Ȩ��
	int carryFireCoin;	//��������ֲ�Я���Ļ��
	OneFriendsGroupInfo()
	{
		memset(this, 0, sizeof(OneFriendsGroupInfo));
	}
};

//������ֲ���Ա�Լ�������ֲ�����Ϣ
struct LogonRequestFriendsInfo
{
	int friendsGroupID;		// ���ֲ�id
	int page;				// �ڼ�ҳ����һ��Ĭ��0
};
struct OneFriendsGroupUserInfo
{
	int userID;
	int joinTime;
	BYTE onlineStatus;		//����״̬ 0:������ 1������
	int score;
	long long money;
	int fireCoin;			//�ۼ���Ӯ�Ļ��
	BYTE status;			//���ֲ�����ݣ�1Ⱥ����0��ͨ��Ա��2����Ա
	int carryFireCoin;		//����Я�����
	BYTE power;				//��ҵ�Ȩ��
	OneFriendsGroupUserInfo()
	{
		memset(this, 0, sizeof(OneFriendsGroupUserInfo));
	}
};
struct LogonResponseFriendsGroupMember
{
	int allPage;							//�ܹ�ҳ
	int curPage;							//��ǰҳ
	int pageDataCount;						//��ǰҳ������
	OneFriendsGroupUserInfo userInfo[200];
	LogonResponseFriendsGroupMember()
	{
		memset(this, 0, sizeof(LogonResponseFriendsGroupMember));
	}
};
struct LogonResponseFriendsGroupSimple
{
	int friendsGroupID;
	char name[24];//���ֲ�����
	int masterID; //Ⱥ��ID
	char notice[128]; //���ֲ�����
	LogonResponseFriendsGroupSimple()
	{
		memset(this, 0, sizeof(LogonResponseFriendsGroupSimple));
	}
};

//������ֲ�������֪ͨ��Ϣ
struct LogonRequestFriendsGroupAllNotify
{
	int time; //ʱ��㣬time=0:�������µ�����
};
struct OneFriendsGroupNotify
{
	int id; //��Ϣid��Ψһ��ʶ
	int userID;
	int targetFriendsGroupID;
	char name[24];
	BYTE type;//��Ϣ����
	int time; //��Ϣ����ʱ��
	int param1;
	int param2;
	BYTE alreadyOper; //�Ƿ�������Ҳ���,0:��1����
	OneFriendsGroupNotify()
	{
		memset(this, 0, sizeof(OneFriendsGroupNotify));
	}
};

//������ֲ������б���Ϣ
struct LogonRequestFriendsGroupRoomMsg
{
	int friendsGroupID; //���ֲ�id
	int time; //ʱ��㣬time=0:�������µ�����
};
struct OneFriendsGroupRoomMsg //һ�����ֲ�������Ϣ
{
	int friendsGroupID;//���ֲ�id
	int userID;		   //������Ϣ��˭���͵�
	int id;            //��ϢID,ÿ�����ֲ���ϢidΨһ
	int time;          //��Ϣ����ʱ��
	int roomID;		   //����ID
	int roomCount;	   //���¿�����������ǰ���ֲ����еĿ����������ṩ���ͻ��˸��¿�������
	int curPeopleCount;//��ǰ����
	int allPeopleCount;//������
	int playCount;     //��Ϸ����
	char passwd[20];   //��������
	char gameRules[256];//��Ϸ����
	OneFriendsGroupRoomMsg()
	{
		memset(this, 0, sizeof(OneFriendsGroupRoomMsg));
	}
};

//������ֲ���ս����Ϣ
struct LogonRequestFriendsGroupAccoMsg
{
	int friendsGroupID; //���ֲ�id
	int firstTime;		//��ʼʱ��
	int endTime;		//��ֹʱ�䡣��ֹʱ��Ϊ0����ʾ����
	BYTE roomType;		//0���У�1:���ַ���2����ҷ���3��VIP��
};
struct OneFriendsGroupAccountMsg //һ�����ֲ�������Ϣ
{
	int friendsGroupID;		//���ֲ�id
	int userID;				//������Ϣ��˭���͵ģ�userID=0��ս��Ĭ��userID=0
	int time;				//��Ϣ����ʱ��
	int roomID;				//����ID
	int realPlayCount;		//������Ϸ����
	int playCount;			//�����Ϸ����
	BYTE playMode;			//��Ϸģʽ����Ӧjson["gameIdx"]��������Ϸ����ͳһ
	char passwd[20];		//��������
	char userInfoList[192];	//ս���б�
	BYTE roomType;			//��������
	BYTE srcType;			//����
	OneFriendsGroupAccountMsg()
	{
		memset(this, 0, sizeof(OneFriendsGroupAccountMsg));
	}
};

//���󴴽����ֲ�
struct LogonRequestCreateFriendsGroup
{
	char name[24];//���ֲ�����
};

//���������ֲ�
struct LogonRequestJoinFriendsGroup
{
	int friendsGroupID; //���ֲ�id
};

//������Ѽ�����ֲ�
struct LogonRequestAddFriendJoin
{
	int friendID;       //������ĺ���id
	int friendsGroupID; //Ҫ����ľ��ֲ�id
};

//�����Ӧ֪ͨ��Ϣ
struct LogonRequestAnswerOper
{
	int id;				//��������Ϣ��Ψһid
	BYTE operType;		//��������,1ͬ�⣬2�ܾ�
	LogonRequestAnswerOper()
	{
		memset(this, 0, sizeof(LogonRequestAnswerOper));
		operType = 1;
	}
};

//ɾ�����ֲ���Ա
struct LogonRequestDelMember
{
	int friendsGroupID; //���ֲ�id
	int targetID;		//Ҫɾ����ԱID
};

//�˳����ֲ�
struct LogonRequestQuit
{
	int friendsGroupID; //���ֲ�id
};

//��ɢ���ֲ�
struct LogonRequestDismiss
{
	int friendsGroupID; //���ֲ�id
};

//�ı���ֲ�����
struct LogonRequestChangeName
{
	int friendsGroupID; //���ֲ�id
	char newName[24];//���ֲ�����
};

//ɾ��һ����Ϣ֪ͨ
struct LogonRequestDelNotiy
{
	int id;
	BYTE reply;//�Ƿ���Ҫ�ظ���0�����ظ���1���ظ�
};

// ����ɾ��С���
struct LogonFriendsGroupRequestDelRedSpot
{
	enum RedSpotType
	{
		REDSPOT_TYPE_NOTIFYLIST = 1,	// ֪ͨ�б�
		REDSPOT_TYPE_ROOM_MSG = 2,		// ������Ϣ
		REDSPOT_TYPE_ACCO_MSG = 3,		// ս����Ϣ
		REDSPOT_TYPE_DESK_MSG = 4,		// ������Ϣ
		REDSPOT_TYPE_VIP_ROOM_MSG = 5,	// VIP������Ϣ
	};

	int redSpotType;
	int friendsGroupID; //���ֲ�id
};

// ���󷢲����ֲ�����
struct LogonFriendsGroupRequestSendNotice
{
	int friendsGroupID; //���ֲ�id
	char notice[128];   //֪ͨ
};

// ս��ͳ��
struct LogonFriendsGroupRequestQueryAcco
{
	int friendsGroupID; //���ֲ�id
	int firstTime;		//��ʼʱ��
	int endTime;		//��ֹʱ�䡣��ֹʱ��Ϊ0����ʾ����
};
struct OneSimlpeFriendsGroupAcco
{
	int userID;
	int score;
	long long money;
	int fireCoin;
};
struct LogonFriendsGroupRequestQueryAccoRsp
{
	int friendsGroupID; //���ֲ�id
	int peopleCount;    //����
	OneSimlpeFriendsGroupAcco accos[200];
	LogonFriendsGroupRequestQueryAccoRsp()
	{
		memset(this, 0, sizeof(LogonFriendsGroupRequestQueryAccoRsp));
	}
};

//������ֲ������б�(����VIP����)
struct LogonFriendsGroupRequestDeskList
{
	int friendsGroupID; //���ֲ�id
	BYTE type;			//0��������������0������vip�����б�
};
struct OneFriendsGroupDeskInfo //һ�����ֲ�����������vip���䣩
{
	int friendsGroupID; //���ֲ�id
	int friendsGroupDeskNumber; //����id ,ȡֵ��Χ{1-(MAX_FRIENDSGROUP_DESK_LIST_COUNT+MAX_FRIENDSGROUP_VIP_ROOM_COUNT)}
	int time;          //���Ӵ���ʱ��
	int roomID;		   //����ID
	int gameID;		   //��ϷID
	int	roomType;	   //��������
	int curPeopleCount;//��ǰ����
	int allPeopleCount;//������
	int playCount;     //��Ϸ����
	BYTE gameStatus;   //��Ϸ״̬��0��δ��ʼ��1������Ϸ��
	char passwd[20];   //��������
	char gameRules[256];//��Ϸ����
	int userID[MAX_PLAYER_GRADE]; //����б�
	OneFriendsGroupDeskInfo()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0, sizeof(OneFriendsGroupDeskInfo));
	}
};

//�����ɢ���ֲ�һ������������vip���䣩
struct LogonFriendsGroupRequestDeleteDeskInfo
{
	int friendsGroupID; //���ֲ�id
	int friendsGroupDeskNumber; //���Ӻ�
};

// ����ͳ��
struct LogonFriendsGroupRequestQueryDeskAcco
{
	int friendsGroupID; //���ֲ�id
	int firstTime; //��ʼʱ��
	int endTime;   //��ֹʱ�䡣��ֹʱ��Ϊ0����ʾ����
};
struct OneSimlpeFriendsGroupDeskAcco
{
	long long costMoney;			//���Ľ������
	int costJewels;					//������ʯ
	long long fireCoinRecharge;		//��ҳ�ֵ
	long long fireCoinExchange;		//��Ҷһ�
	long long moneyPump;			//��ҳ�ˮ
	long long fireCoinPump;			//��ҳ�ˮ
	OneSimlpeFriendsGroupDeskAcco()
	{
		memset(this, 0, sizeof(OneSimlpeFriendsGroupDeskAcco));
	}
};
struct LogonFriendsGroupRequestQueryDeskAccoRsp
{
	int friendsGroupID;  //���ֲ�id
	OneSimlpeFriendsGroupDeskAcco accos[1];
	LogonFriendsGroupRequestQueryDeskAccoRsp()
	{
		memset(this, 0, sizeof(LogonFriendsGroupRequestQueryDeskAccoRsp));
	}
};

// ����ת�þ��ֲ�
struct LogonFriendsGroupRequestTransfer
{
	int friendsGroupID; //���ֲ�id
	int tarUserID;
};

// ������Ȩ���ֲ���Ա
struct LogonFriendsGroupRequestAuthMembers
{
	int friendsGroupID; //���ֲ�id
	int tarUserID;
	BYTE status; //0��ͨ��Ա��2����Ա
};

// �����޸ľ��ֲ�΢��
struct LogonFriendsGroupRequestModifyWechat
{
	int friendsGroupID; //���ֲ�id
	char wechat[48];    //΢��
};

// �����޸�ĳ����ҵ�Ȩ��
struct LogonFriendsGroupRequestModifyUserPower
{
	int friendsGroupID; //���ֲ�id
	int targetUserID;
	BYTE newPower;		//�µ�Ȩ��
};

// ����ҳ�ֵ���߶һ����
struct LogonFriendsGroupRequestModifyUserFireCoin
{
	int friendsGroupID; //���ֲ�id
	int targetUserID;
	int changeFireCoint;//�仯ֵ������0���ӻ�ң�С��0���ٻ��
};


//////////////֪ͨ��Ϣ//////////////////////////////
//���ֲ�������Ϣ�仯֪ͨ
struct LogonChatMsgChange
{
	int friendsGroupID; //���ֲ�id
	BYTE operType; //0�����£�1��ɾ��
	BYTE msgType; // 0������Ϣ��1��ս����Ϣ
	int id; //��ϢID,ÿ�����ֲ���ϢidΨһ
	int roomCount;	   //���¿�������
	int curPeopleCount;//��ǰ����
	LogonChatMsgChange()
	{
		memset(this, 0, sizeof(LogonChatMsgChange));
	}
};

//���ֲ����ֱ仯֪ͨ
struct LogonFriendsGroupNameChange
{
	int friendsGroupID; //���ֲ�id
	char name[24];		//�µľ��ֲ�����
	LogonFriendsGroupNameChange()
	{
		memset(this, 0, sizeof(LogonFriendsGroupNameChange));
	}
};

//���ֲ��˱仯֪ͨ
struct LogonFriendsGroupPeopleChange
{
	int friendsGroupID; //���ֲ�id
	BYTE type;  //0���£�1���ӣ�2ɾ��
	OneFriendsGroupUserInfo userInfo;
	LogonFriendsGroupPeopleChange()
	{
		memset(this, 0, sizeof(LogonFriendsGroupPeopleChange));
	}
};

//���������͵�һ�����ֲ�֪ͨ
struct LogonFriendsGroupPushNotify
{
	OneFriendsGroupNotify notify;
};

//���������͵�һ�����ֲ�������Ϣ
struct LogonFriendsGroupPushRoomMsg
{
	OneFriendsGroupRoomMsg notify;

};
//���������͵�һ�����ֲ�ս����Ϣ
struct LogonFriendsGroupPushAccoMsg
{
	OneFriendsGroupAccountMsg notify;
};

//���������͵�һ�����ֲ��б���Ϣ
struct LogonFriendsGroupPushFGMsg
{
	OneFriendsGroupInfo notify;
};

//�������������͵ľ��ֲ���ɢ��Ϣ
struct LogonFriendsGroupPushDismiss
{
	int friendsGroupID; //���ֲ�id
	LogonFriendsGroupPushDismiss()
	{
		memset(this, 0, sizeof(LogonFriendsGroupPushDismiss));
	}
};

//���������͵�С�������
struct LogonFriendsGroupPushRedSpot
{
	int notifyListRedSpotCount;		//֪ͨ�б�����
	struct MsgRedSpot
	{
		int friendsGroupID;			//���ֲ�id
		int deskMsgRedSpotCount;    //������Ϣ����
		int VIPRoomMsgRedSpotCount;	//VIP������Ϣ����
	};
	int friendsGroupCount;
	MsgRedSpot msgRedSpot[20];
	LogonFriendsGroupPushRedSpot()
	{
		memset(this, 0, sizeof(LogonFriendsGroupPushRedSpot));
	}
};

//���ֲ�����仯֪ͨ
struct LogonFriendsGroupNoticeChange
{
	int friendsGroupID; //���ֲ�id
	char notice[128];	//�µľ��ֲ�����
	LogonFriendsGroupNoticeChange()
	{
		memset(this, 0, sizeof(LogonFriendsGroupNoticeChange));
	}
};

/////����������vip���䣩
//��������
struct LogonFriendsGroupAddDeskInfo
{
	OneFriendsGroupDeskInfo desk;
};
//�����仯
struct LogonFriendsGroupDeskInfoChange
{
	int friendsGroupID; //���ֲ�id
	BYTE operType;		//0�����£�1��ɾ��
	int friendsGroupDeskNumber; //���Ӻ��룬����MAX_FRIENDSGROUP_DESK_LIST_COUNT��vip����
	int curPeopleCount;//��ǰ����
	BYTE gameStatus;   //��Ϸ״̬��0��δ��ʼ��1������Ϸ��
	BYTE updateType;   //0�����ӣ�1�����٣�2��gameStatus״̬�仯
	int userID;		   //���ӻ������ҵ�ID
	LogonFriendsGroupDeskInfoChange()
	{
		memset(this, 0, sizeof(LogonFriendsGroupDeskInfoChange));
	}
};

//���ֲ������ݱ仯֪ͨ
struct LogonFriendsGroupUserStatusChange
{
	int friendsGroupID; //���ֲ�id
	int peopleCount;

	struct FriendsGroupUserStatusChangeStruct
	{
		int userID;
		BYTE status; //���ֲ�����ݣ�0��ͨ��Ա ,1Ⱥ����2����Ա
		BYTE power;  //���ֲ�Ȩ��
	};

	FriendsGroupUserStatusChangeStruct user[10];

	LogonFriendsGroupUserStatusChange()
	{
		memset(this, 0, sizeof(LogonFriendsGroupUserStatusChange));
	}
};

//���ֲ�΢�ű仯֪ͨ
struct LogonFriendsGroupWechatChange
{
	int friendsGroupID; //���ֲ�id
	char wechat[48];	//�µľ��ֲ�΢��
	LogonFriendsGroupWechatChange()
	{
		memset(this, 0, sizeof(LogonFriendsGroupWechatChange));
	}
};

//���ֲ���ҵ�Ȩ�ޱ仯
struct LogonFriendsGroupUserPowerChange
{
	int userID;
	int friendsGroupID; //���ֲ�id
	BYTE newPower;		//�µ�Ȩ��
	LogonFriendsGroupUserPowerChange()
	{
		memset(this, 0, sizeof(LogonFriendsGroupUserPowerChange));
	}
};

//���ֲ���ұ仯֪ͨ
struct LogonFriendsGroupUserFireCoinChange
{
	struct LogonFriendsGroupUserFireCoinChangeInfo
	{
		int userID;
		int newFireCoin;
	};
	int friendsGroupID; //���ֲ�id
	LogonFriendsGroupUserFireCoinChangeInfo user[MAX_PLAYER_GRADE];
	LogonFriendsGroupUserFireCoinChange()
	{
		memset(this, 0, sizeof(LogonFriendsGroupUserFireCoinChange));
	}
};

//////////////////////////////////////////////////////////////////////////
//���������ͣ�ʵʱ����
struct LogonPeopleMatchSignUpPeopleChange
{
	int gameID;			//��������Ϸ
	int matchID;		//������Ϸ�ı��
	int curSignUpCount;	//��ǰ��������
	int peopleCount;	//�������˿���
	int allSignUpCount;	//�ۼƱ�������

	LogonPeopleMatchSignUpPeopleChange()
	{
		memset(this, 0, sizeof(LogonPeopleMatchSignUpPeopleChange));
	}
};

struct LogonMatchJoinRoom
{
	int gameID;			//��������Ϸ
	BYTE matchType;		//��������
	int matchID;		//������Ϸ�ı��
	int roomID;

	LogonMatchJoinRoom()
	{
		memset(this, 0, sizeof(LogonMatchJoinRoom));
	}
};

//���������ͣ���ʱ����
struct LogonTimeMatchSignUpPeopleChange
{
	int matchID;		//������Ϸ�ı��
	int curSignUpCount;	//��ǰ��������

	LogonTimeMatchSignUpPeopleChange()
	{
		memset(this, 0, sizeof(LogonTimeMatchSignUpPeopleChange));
	}
};

//// ����ʧ��ԭ��
//enum MatchFailReason
//{
//	MATCH_FAIL_REASON_NOT_ENOUGH_PEOPLE = 1,	//��������
//	MATCH_FAIL_REASON_PLAYING = 2,				//��Ϸ��
//	MATCH_FAIL_REASON_SYSTEM_ERROR = 3			//ϵͳ�쳣
//};
//֪ͨ����ʧ��
struct LogonStartMatchFail
{
	int gameID;			//��������Ϸ
	BYTE matchType;		//��������
	int matchID;		//������Ϸ�ı��
	BYTE reason;		//����ʧ��ԭ��

	LogonStartMatchFail()
	{
		memset(this, 0, sizeof(LogonStartMatchFail));
	}
};

//֪ͨ����������ʼ
struct LogonNotifyStartMatchRightAway
{
	int gameID;			//��������Ϸ
	int matchID;		//������Ϸ�ı��
	long long startTime;//��ʼʱ��

	LogonNotifyStartMatchRightAway()
	{
		memset(this, 0, sizeof(LogonNotifyStartMatchRightAway));
	}
};

//================================================================================
#pragma pack()

//////////////////////////////////////////////////////////////////////
// ��Ϸ��Ϣ����
//////////////////////////////////////////////////////////////////////
// ��¼���
const unsigned int MSG_MAIN_LOADER_LOGON = 500;

const unsigned int MSG_ASS_LOADER_LOGON = 1;	// ��¼
const unsigned int MSG_ADD_LOADER_LOGOUT = 2;	// �ǳ�

// �������
const unsigned int MSG_MAIN_LOADER_ACTION = 501;

const unsigned int MSG_ASS_LOADER_ACTION_SIT = 1;			// ����
const unsigned int MSG_ASS_LOADER_ACTION_STAND = 2;			// վ����
const unsigned int MSG_ASS_LOADER_ACTION_MATCH_SIT = 3;		// ��������
const unsigned int MSG_ASS_LOADER_ROBOT_TAKEMONEY = 4;		// �����˱��ߣ�����ȡǮ����
const unsigned int MSG_ASS_LOADER_GAME_BEGIN = 5;			// ������Ϸ��ʼ
const unsigned int MSG_ASS_LOADER_ACTION_COMBINE_SIT = 6;	// ������������
const unsigned int MSG_ASS_LOADER_ACTION_CANCEL_SIT = 7;	// ȡ����������

// �������Ϣ
const unsigned int MSG_MAIN_LOADER_FRAME = 502;

const unsigned int MSG_ASS_LOADER_DESK_INFO = 1;	// ����������Ϣ(���ӵĻ�����Ϣ�������Ϣ)
const unsigned int MSG_ASS_LOADER_GAME_INFO = 2;	// ��Ϸ��Ϣ(������Ϸ)
const unsigned int MSG_ASS_LOADER_GAME_STATION = 3; // ��Ϸ״̬
const unsigned int MSG_ASS_LOADER_DESK_USERINFO = 4;// ���������Ϣ
const unsigned int MSG_ASS_LOADER_DESK_ONE_USERINFO = 5;// ���󵥸����������Ϣ

// ��Ϸ��Ϣ
const unsigned int MSG_MAIN_LOADER_GAME = 503;

const unsigned int MSG_ASS_LOADER_GAME_AGREE = 1;			// ���׼��
const unsigned int MSG_ASS_LOADER_GAME_AUTO = 2;			// �й�
const unsigned int MSG_ASS_LOADER_GAME_CANCEL_AUTO = 3;		// ȡ���й�
const unsigned int MSG_ASS_LOADER_GAME_MAGICEXPRESS = 4;	// ħ������


//���ӽ�ɢ��Ϣ
const unsigned int MSG_MAIN_LOADER_DESKDISSMISS = 504;

const unsigned int MSG_ASS_LOADER_REQ_DESKDISSMISS = 1;			// �����ɢ����
const unsigned int MSG_ASS_LOADER_ANSWER_DESKDISMISS = 2;		// ͬ���ɢ����

// ֪ͨ����Ϣ�����ӣ�
const unsigned int MSG_MAIN_LOADER_NOTIFY = 505;

const unsigned int MSG_NTF_LOADER_DESK_GAMEBEGIN = 1;			// ֪ͨ��Ϸ��ʼ
const unsigned int MSG_NTF_LOADER_DESK_BASEINFO = 2;			// ֪ͨ������Ϣ
const unsigned int MSG_NTF_LOADER_DESK_DISMISS_OK = 3;			// ֪ͨ���ӳɹ���ɢ
const unsigned int MSG_NTF_LOADER_DESK_DISMISS_FAILED = 4;		// ֪ͨ���ӽ�ɢʧ��
const unsigned int MSG_NTF_LOADER_DESK_ALL_USERINFO = 5;		// �������������Ϣ(ĳ������)
const unsigned int MSG_NTF_LOADER_DESK_DISMISS_INFO = 6;		// ֪ͨͬ�����ӽ�ɢ
const unsigned int MSG_NTF_LOADER_DESK_TALK = 7;				// ֪ͨ��������
const unsigned int MSG_NTF_LOADER_DESK_VOICE = 8;				// ֪ͨ��������
const unsigned int MSG_NTF_LOADER_DESK_GAMEFINISH = 9;			// ֪ͨ��Ϸ����
const unsigned int MSG_NTF_LOADER_DESK_GRADE = 10;              // ֪ͨ������Ϸս��
const unsigned int MSG_NTF_LOADER_DESK_MONEY = 11;				// ֪ͨ������Ϸ��ҽ���
const unsigned int MSG_NTF_LOADER_DESK_LEFT_WAITAGREE_TIME = 12;// ֪ͨʣ��ȴ�׼��ʱ��
const unsigned int MSG_NTF_LOADER_DESK_ISAUTO = 13;				// ֪ͨ����Ƿ��й�
const unsigned int MSG_NTF_LOADER_DESK_MAGICEXPRESS = 14;		// ֪ͨħ������
const unsigned int MSG_NTF_LOADER_DESK_SITFULL = 15;			// ֪ͨ�������������
const unsigned int MSG_NTF_LOADER_DESK_CANBEGIN = 16;			// ֪ͨ���������ܿ�ʼ��
const unsigned int MSG_NTF_LOADER_DESK_STOP_JOIN = 17;			// ��Ϸ�Ѿ���ʼ����ֹ����
const unsigned int MSG_NTF_LOADER_RECHARGE = 18;				// ��ֵ����ʱ
const unsigned int MSG_NTF_LOADER_DESK_HUNDRED_ALL_USER = 19;	// ��������Ϸ��������Ҽ���Ϣ
const unsigned int MSG_NTF_LOADER_DESK_DISMISS_USERID = 20;		// ���ӳɹ���ɢ��������Ӧ����λ�����id
const unsigned int MSG_NTF_LOADER_ERR_MSG = 21;					// ��Ϸ��������ʾ��Ϣ
const unsigned int MSG_NTF_LOADER_DESK_JEWELS = 22;				// ֪ͨ������Ϸ��ʯ����
const unsigned int MSG_NTF_LOADER_NO_USER = 23;					// �ڴ��в����ڸ���ң�ǰ��ֱ�ӷ��ش���															
const unsigned int MSG_NTF_LOADER_DESK_MATCH_STATUS = 24;		// ֪ͨ����������״̬
const unsigned int MSG_NTF_LOADER_DESK_FINISH_MATCH = 25;		// ֪ͨ������ɱ���
const unsigned int MSG_NTF_LOADER_MATCH_RANK = 26;				// ֪ͨ�������
const unsigned int MSG_NTF_LOADER_DESK_MATCH_GRADE = 27;        // ֪ͨ���ֱ��������ֱ仯

// ֪ͨĳ�������Ϣ 
const unsigned int MSG_MAIN_LOADER_NOTIFY_USER = 506;

const unsigned int MSG_NTF_LOADER_DESK_USER_INFO = 1;			// ֪ͨ����(ĳ��)�����Ϣ
const unsigned int MSG_NTF_LOADER_DESK_USER_SIT = 2;			// ����
const unsigned int MSG_NTF_LOADER_DESK_USER_AGREE = 3;			// ֪ͨ�Ѿ�׼��
const unsigned int MSG_NTF_LOADER_DESK_USER_LEFT = 4;			// ֪ͨ����뿪
const unsigned int MSG_NTF_LOADER_DESK_USER_OFFLINE = 5;		// ֪ͨ��Ҷ���
const unsigned int MSG_NTF_LOADER_DESK_USER_BE_KICKOUT = 6;		// ֪ͨ��ұ�T��
const unsigned int MSG_NTF_LOADER_DESK_USER_NOFIND_DESK = 7;	// û�ҵ�����

// ������������Ϣ
const unsigned int MSG_MAIN_LOADER_VOICEANDTALK = 507;

const unsigned int MSG_ASS_LOADER_TALK = 1;			// ����
const unsigned int MSG_ASS_LOADER_VOICE = 2;		// ����

// ��Դ�仯���
const unsigned int MSG_MAIN_LOADER_RESOURCE_CHANGE = 508; // (��ʱû��СID)

// ���������
const unsigned int MSG_MAIN_LOADER_MATCH = 509;

const unsigned int MSG_ASS_LOADER_MATCH_ALL_DESK_DATA = 1;		// ������������״̬
const unsigned int MSG_ASS_LOADER_MATCH_ENTER_WATCH_DESK = 2;	// �Թ���������
const unsigned int MSG_ASS_LOADER_MATCH_QUIT_WATCH_DESK = 3;	// �˳��Թ�


// Э�������ID
const unsigned int MSG_MAIN_LOADER_MAX = 520;

// �ṹ�嶨��
#pragma pack(1)

//��Ϸ�����½
struct LoaderRequestLogon
{
	int			roomID;					// ����ID

	LoaderRequestLogon()
	{
		memset(this, 0, sizeof(LoaderRequestLogon));
	}
};

struct LoaderResponseLogon
{
	int	 deskIdx;
	int	 deskPasswdLen;
	char deskPasswd[20];
	bool isInDesk;
	int iRoomID;
	int playStatus;	// ���״̬

	LoaderResponseLogon()
	{
		memset(this, 0, sizeof(LoaderResponseLogon));
	}
};

//�û��������£���ҳ���������
struct LoaderRequestMatchSit
{
	int		deskIdx;			// ���ڷ�������˵��������Ѿ�û��ʲô������

	LoaderRequestMatchSit()
	{
		memset(this, 0, sizeof(LoaderRequestMatchSit));
	}
};

//�û���������(���ַ��ͽ�ҷ�)
struct LoaderRequestSit
{
	BYTE deskStation;   // ѡ�����λ��

	LoaderRequestSit()
	{
		deskStation = INVALID_DESKSTATION;
	}
};

struct LoaderNotifyWaitAgree
{
	int leftWaitAgreeSecs;		// ʣ��׼��ʱ��
	int cfgWaitAgreeSecs;		// ���õ�ʣ��׼��ʱ��

	LoaderNotifyWaitAgree()
	{
		memset(this, 0, sizeof(LoaderNotifyWaitAgree));
	}
};

///��Ϸ��Ϣ
struct LoaderGameInfo
{
	BYTE bGameStation;	//��Ϸ״̬
	char gameRules[256];

	LoaderGameInfo()
	{
		memset(this, 0, sizeof(LoaderGameInfo));
	}
};

//�����˽�Ҳ��㱻�ߣ�����ȡǮ
struct _LoaderRobotTakeMoney
{
	int iMoney;
	int iRoomID;
	_LoaderRobotTakeMoney()
	{
		memset(this, 0, sizeof(_LoaderRobotTakeMoney));
	}
};

//�ظ�������
struct _LoaderRobotTakeMoneyRes
{
	BYTE byCode; //0�ɹ�
	int iMoney;
	_LoaderRobotTakeMoneyRes()
	{
		memset(this, 0, sizeof(_LoaderRobotTakeMoneyRes));
	}
};

//============================================================================================
// ��Ҽ���Ϣ
struct UserSimpleInfo
{
	int		userID;
	char	name[64];
	char	headURL[256];
	bool	isOnline;
	BYTE	deskStation;
	int		playStatus;
	int		score;
	int		iDeskIndex;
	long long money;
	char	longitude[12];
	char	latitude[12];
	char	address[64];
	char	ip[24];
	BYTE	sex;
	bool	isAuto;
	int		jewels;
	char	motto[128];// ����ǩ��

	UserSimpleInfo()
	{
		Init();
	}

	void Init()
	{
		memset(this, 0, sizeof(UserSimpleInfo));
		deskStation = INVALID_DESKSTATION;
	}
};

// ֪ͨ������Ϣ(���������Ϣ)
struct LoaderNotifyDeskInfo
{
	int		deskIdx;
	int		deskPasswdLen;
	char	deskPasswd[20];
	int		masterID;
	char	masterName[64];
	int		runGameCount;
	int		totalGameCount;
	bool	isDismissStatus;		// �Ƿ��ڽ�ɢ״̬

	LoaderNotifyDeskInfo()
	{
		memset(this, 0, sizeof(LoaderNotifyDeskInfo));
	}
};

// ֪ͨ���������Ϣ
struct LoaderNotifyDeskUserInfo
{
	int		userCount;
	UserSimpleInfo userInfo[1];

	LoaderNotifyDeskUserInfo()
	{
		memset(this, 0, sizeof(LoaderNotifyDeskUserInfo));
	}
};

struct LoaderNotifyAgree
{
	BYTE deskStation;
	int userStatus;

	LoaderNotifyAgree()
	{
		memset(this, 0, sizeof(LoaderNotifyAgree));
	}
};

struct DeskDismissType
{
	BYTE deskStation;
	int dismissType;

	DeskDismissType()
	{
		deskStation = INVALID_DESKSTATION;
		dismissType = 0;
	}
};

// ֪ͨ��ɢ
struct LoaderNotifyDismiss
{
	BYTE	deskStation;			// �����ɢ��
	int		leftDismissTime;		// �����ɢ��ʱ��
	int		cfgWaitDismissTime;		// �ȴ���ɢʱ��
	int		deskUserCount;			// �����������

	DeskDismissType dismissType[1];

	LoaderNotifyDismiss()
	{
		memset(this, 0, sizeof(LoaderNotifyDismiss));
	}
};

// ֪ͨ�Ƿ��й�
struct LoaderNotifyIsAuto
{
	int userID;
	bool isAuto;

	LoaderNotifyIsAuto()
	{
		memset(this, 0, sizeof(LoaderNotifyIsAuto));
	}
};

struct LoaderRequestAnswerDismiss
{
	bool isAgree;

	LoaderRequestAnswerDismiss()
	{
		isAgree = false;
	}
};

// ��������
struct LoaderRequestTalk
{
	int sizeCount;		//�ֽ�����
	char words[1024];

	LoaderRequestTalk()
	{
		memset(this, 0, sizeof(LoaderRequestTalk));
	}
};

// ֪ͨ����
struct LoaderNotifyTalk
{
	int userID;
	int sizeCount;		//�ֽ�����
	char words[1024];

	LoaderNotifyTalk()
	{
		memset(this, 0, sizeof(LoaderNotifyTalk));
	}
};

// ��������
struct LoaderRequestVoice
{
	int voiceID;

	LoaderRequestVoice()
	{
		voiceID = 0;
	}
};

struct LoaderNotifyVoice
{
	int userID;
	int voiceID;

	LoaderNotifyVoice()
	{
		memset(this, 0, sizeof(LoaderNotifyVoice));
	}
};

struct LoaderNotifyUserLeft
{
	int reason;
	UserSimpleInfo userInfo;

	LoaderNotifyUserLeft()
	{
		reason = 0;
	}
};

struct LoaderNotifyDismissSuccess
{
	bool isDismissMidway;			// �Ƿ���;��ɢ

	LoaderNotifyDismissSuccess()
	{
		memset(this, 0, sizeof(LoaderNotifyDismissSuccess));
	}
};

struct LoaderNotifyDismissSuccessData
{
	int	userID[MAX_PLAYER_GRADE];   // ÿ����λ�����id
	char name[MAX_PLAYER_GRADE][64];
	char headURL[MAX_PLAYER_GRADE][256];

	LoaderNotifyDismissSuccessData()
	{
		memset(this, 0, sizeof(LoaderNotifyDismissSuccessData));
	}
};

struct LoaderNotifyUserGrade
{
	int grade[MAX_PLAYER_GRADE];               //���ս��

	LoaderNotifyUserGrade()
	{
		memset(grade, 0, sizeof(grade));
	}
};

struct LoaderRequestMagicExpress
{
	int userID;
	int magicType;

	LoaderRequestMagicExpress()
	{
		userID = 0;
		magicType = 0;
	}
};

struct LoaderNotifyMagicExpress
{
	int srcUserID;
	int targetUserID;
	int magicType;

	LoaderNotifyMagicExpress()
	{
		memset(this, 0, sizeof(LoaderNotifyMagicExpress));
	}
};

struct LoaderNotifyResourceChange
{
	int userID;
	int resourceType;
	long long value;			// ����
	long long changeValue;		// �仯��

	LoaderNotifyResourceChange()
	{
		memset(this, 0, sizeof(LoaderNotifyResourceChange));
	}
};

// ��Ҽ���Ϣ
struct HundredGameUserSimpleInfo
{
	int  userID;
	bool isOnline;
	BYTE deskStation;
	int  userStatus;
	long long money;
	bool isAuto;

	HundredGameUserSimpleInfo()
	{
		Init();
	}

	void Init()
	{
		memset(this, 0, sizeof(HundredGameUserSimpleInfo));
		deskStation = INVALID_DESKSTATION;
	}
};

struct LoaderRequestOneUserSimpleInfo
{
	BYTE	deskStation;

	LoaderRequestOneUserSimpleInfo()
	{
		memset(this, 0, sizeof(LoaderRequestOneUserSimpleInfo));
	}
};

struct LoaderNotifyGameFinish
{
	int rechargeUserID[MAX_PLAYER_GRADE]; //��Ҳ��㣬��Ҫ��ֵ�����id

	LoaderNotifyGameFinish()
	{
		memset(rechargeUserID, 0, sizeof(rechargeUserID));
	}
};

struct LoaderNotifyRecharge
{
	int leftSecs;					// ʣ��ʱ��
	long long needRechargeMoney;	// ��Ҫ���Ǯ

	LoaderNotifyRecharge()
	{
		leftSecs = 0;
		needRechargeMoney = 0;
	}
};

struct LoaderNotifyERRMsg
{
	int msgType;
	int sizeCount;
	char notify[1024];

	LoaderNotifyERRMsg()
	{
		memset(this, 0, sizeof(LoaderNotifyERRMsg));
	}
};

//////////////////////////////////////////////////////////////////////////
// ���������Э��
struct LoaderNotifyDeskMatchStatus
{
	long long llPartOfMatchID;	//�������ڵı���id��С��
	int	iCurMatchRound;			//�������е�����
	int	iMaxMatchRound;			//����������
	BYTE status;				//0�������У�1�����������Ѿ�������2����������Ҫ��ʼ������remainTime��ʣ��ʱ��
	int remainTime;				//ʣ�࿪ʼʱ��

	LoaderNotifyDeskMatchStatus()
	{
		memset(this, 0, sizeof(LoaderNotifyDeskMatchStatus));
	}
};

struct LoaderNotifyDeskFinishMatch
{
	int deskIdx; //���ӱ��

	LoaderNotifyDeskFinishMatch()
	{
		memset(this, 0, sizeof(LoaderNotifyDeskFinishMatch));
	}
};

struct LoaderNotifyMatchRank
{
	struct LoaderNotifyMatchRankUser
	{
		int userID;
		int rank;

		LoaderNotifyMatchRankUser()
		{
			memset(this, 0, sizeof(LoaderNotifyMatchRankUser));
		}
	};

	int gameID;
	int gameMatchID;
	int	iCurMatchRound;		//�������е�����
	int	iMaxMatchRound;		//����������
	BYTE type;				//0����̭��1��������2�����վ���
	int rankMatch;			//����

	int peopleCount;
	LoaderNotifyMatchRankUser user[MAX_MATCH_PEOPLE_COUNT];  //Э�����peopleCount��������

	LoaderNotifyMatchRank()
	{
		memset(this, 0, sizeof(LoaderNotifyMatchRank));
	}
};

//�����������������ս��
struct LoaderRequestMatchAllDeskDataInfo
{
	long long llPartOfMatchID;

	LoaderRequestMatchAllDeskDataInfo()
	{
		memset(this, 0, sizeof(LoaderRequestMatchAllDeskDataInfo));
	}
};

struct LoaderResponseMatchAllDeskDataInfo
{
	struct DeskInfo
	{
		int deskIdx;		//���ӱ��
		BYTE status;		//0�������У�1�����������Ѿ�������2����������Ҫ��ʼ����
	};

	int iCurPeopleCount;
	int iMaxPeopleCount;
	int	iCurMatchRound;		//�������е�����
	int	iMaxMatchRound;		//����������
	int deskCount;
	DeskInfo desk[256];		//���256������ͬʱ����

	LoaderResponseMatchAllDeskDataInfo()
	{
		memset(this, 0, sizeof(LoaderResponseMatchAllDeskDataInfo));
	}
};

// �Թ���������
struct LoaderRequestMatchEnterWatch
{
	long long llPartOfMatchID;	//�������ڵı���id��С��
	int deskIdx;

	LoaderRequestMatchEnterWatch()
	{
		memset(this, 0, sizeof(LoaderRequestMatchEnterWatch));
	}
};
struct LoaderResponseMatchEnterWatch
{
	BYTE result;		//0���ɹ���1������״̬�����Թۣ�2���Ѿ������Թۣ�3��������Ϸδ������4���쳣����
	int peopleCount;
	UserSimpleInfo user[MAX_PLAYER_GRADE];

	LoaderResponseMatchEnterWatch()
	{
		memset(this, 0, sizeof(LoaderResponseMatchEnterWatch));
	}
};

// �˳��Թ�
struct LoaderMatchQuitWatch
{
	BYTE result;		//�����0��Ĭ��  1�������˳�   2����Ϸ�����˳�

	LoaderMatchQuitWatch()
	{
		memset(this, 0, sizeof(LoaderMatchQuitWatch));
	}
};

struct LoaderNotifyUserMatchRoomGrade
{
	int changeGrade[MAX_PLAYER_GRADE];	//�仯ֵ
	int grade[MAX_PLAYER_GRADE];		//�仯֮��ı�������

	LoaderNotifyUserMatchRoomGrade()
	{
		memset(this, 0, sizeof(LoaderNotifyUserMatchRoomGrade));
	}
};

#pragma pack()