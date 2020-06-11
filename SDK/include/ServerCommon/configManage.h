#pragma once

#include <map>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include "KernelDefine.h"

//////////////////////////////////////////////////////////
// �������ñ�(�ȳ��Զ�ȡredis��redis��û�ж�ȡ�ڴ��)
#define TBL_BASE_GAME			"gameBaseInfo"
#define TBL_BASE_ROOM			"roomBaseInfo"
#define TBL_BASE_BUY_DESK		"privateDeskConfig"
#define TBL_BASE_OTHER_CONFIG	"otherConfig"
#define TBL_BASE_REDIS_CONFIG	"redisBaseInfo"
#define TBL_BASE_LOGON			"logonBaseInfo"

/////////////////////////////////////////////////////////
// �Զ�����sql��䣬��redis���ݱ��浽���ݿ�
const std::array<const char*, 3> dynamicTbls =
{
	"userInfo",
	"rewardsPool",
	"userBag",
};

// ���ݿ���ֶ����Ͷ���
enum FieldValueType
{
	FIELD_VALUE_TYPE_NONE = 0,
	FIELD_VALUE_TYPE_CHAR,
	FIELD_VALUE_TYPE_INT,
	FIELD_VALUE_TYPE_LONGLONG,
	FIELD_VALUE_TYPE_STR,
	FIELD_VALUE_TYPE_DOUBLE,
	FIELD_VALUE_TYPE_END,
};

struct FieldDescriptor
{
	char fieldName[48];
	int valueType;

	FieldDescriptor()
	{
		memset(this, 0, sizeof(FieldDescriptor));
	}
};

// TODO, ��������һ�������ļ�
//////////////////////////////////////////////////////////////////////
// ������Ϣ��ṹ

// ��Ϸ��Ϣ�ṹ
struct GameBaseInfo
{
	int	gameID;
	char name[64];
	int	deskPeople;
	char dllName[24];
	int	watcherCount;
	int canWatch;
	BYTE canCombineDesk;	//0��ƥ������ģʽ��1������ģʽ
	BYTE multiPeopleGame;	//0��Ĭ�ϣ�1����ʶ2,3,4���Ƿ���ͬһ����Ϸid

	GameBaseInfo()
	{
		memset(this, 0, sizeof(GameBaseInfo));
	}
};
// ���������Ϣ�� roomBaseInfo
enum RoomType
{
	ROOM_TYPE_GOLD = 0,	// ��ҳ�
	ROOM_TYPE_PRIVATE,	// ���ַ�
	ROOM_TYPE_FRIEND,	// ��ҷ�
	ROOM_TYPE_FG_VIP,	// ���ֲ�vip��
	ROOM_TYPE_MATCH,	// ������
};

enum RoomSort
{
	ROOM_SORT_NORMAL = 0,	// ��ͨ
	ROOM_SORT_HUNDRED,		// ������
	ROOM_SORT_SCENE,		// ������
};

enum ServerPlatfromStatus
{
	SERVER_PLATFROM_STATUS_NORMAL = 0,	//����״̬	
	SERVER_PLATFROM_STATUS_CLOSE = 1,	//�ط�
	SERVER_PLATFROM_STATUS_TEST = 2,	//����
};

// ��Ϸ�����ñ�
struct RoomBaseInfo
{
	int		roomID;
	int		gameID;
	char	name[48];
	char	serviceName[48];	// ����������ע�ⳤ�ȳ�����
	int		type;				// ��������(0: ��ҳ� 1: ������)
	int		sort;				// ��������(0: ��ͨ 1: ������)
	int		deskCount;			// ��������
	int		minPoint;			// ��С����(ֻ�����ڽ�ҳ�)
	int		maxPoint;			// ������(ֻ�����ڽ�ҳ�)
	int		basePoint;			// �׷�
	int		gameBeginCostMoney; // ��ҳ��������Ľ��
	char	describe[64];		// ����
	int     roomSign;           // �������ͱ��
	UINT	robotCount;			// ÿ������������
	BYTE	status;				// ����״̬
	int		currPeopleCount;	// ��ǰ��������
	BYTE	level;				// �ȼ������и���
	char	configInfo[2048];	// ����������Ϣ��json��ʽ���������淨������ini�ļ�ͬʱ����

	RoomBaseInfo()
	{
		memset(this, 0, sizeof(RoomBaseInfo));
	}
};

// ���ط����ñ�
struct LogonBaseInfo
{
	int		logonID;
	char	ip[24];
	char	intranetIP[24];
	int		port;
	int		maxPeople;
	int		curPeople;
	BYTE	status;

	LogonBaseInfo()
	{
		memset(this, 0, sizeof(LogonBaseInfo));
	}
};

// ���ݿ����ýṹ
struct DBConfig
{
	char ip[MAX_IP_ADDR_SIZE];
	char user[MAX_DB_USER_SIZE];
	char passwd[MAX_DB_PASSWD_SIZE];
	char dbName[MAX_DB_NAME_SIZE];
	int port;

	DBConfig()
	{
		memset(this, 0, sizeof(DBConfig));
	}
};

// redis���ýṹ
struct RedisConfig
{
	int redisTypeID;
	char ip[128];
	int port;
	char passwd[128];

	RedisConfig()
	{
		memset(this, 0, sizeof(RedisConfig));
	}
};

// ���ķ���������
struct CenterServerConfig
{
	char ip[MAX_IP_ADDR_SIZE];
	int port;
	int maxSocketCount;

	CenterServerConfig()
	{
		memset(this, 0, sizeof(CenterServerConfig));
	}
};

// ��������������
struct LogonServerConfig
{
	int logonID;

	LogonServerConfig()
	{
		memset(this, 0, sizeof(LogonServerConfig));
	}
};

// ��Ϸ����������
struct LoaderServerConfig
{
	char serviceName[64];
	char logonserverPasswd[128];
	int  recvThreadNumber;

	LoaderServerConfig()
	{
		memset(this, 0, sizeof(LoaderServerConfig));
	}
};

// ͨ������
struct CommonConfig
{
	int logLevel;
	int IOCPWorkThreadNumber;

	CommonConfig()
	{
		memset(this, 0, sizeof(CommonConfig));
	}
};

struct BuyGameDeskInfoKey
{
	int gameID;			//	��ϷID
	int count;			//	����
	int roomType;		//  ��������
	BuyGameDeskInfoKey(int gameID, int count, int roomType)
	{
		this->gameID = gameID;
		this->count = count;
		this->roomType = roomType;
	}

	bool operator < (const BuyGameDeskInfoKey& key_) const
	{
		if (gameID < key_.gameID)
		{
			return true;
		}

		if (gameID == key_.gameID && count < key_.count)
		{
			return true;
		}

		if (gameID == key_.gameID && count == key_.count && roomType < key_.roomType)
		{
			return true;
		}

		return false;
	}
};

struct BuyRoomInfoKey
{
	int gameID;			//	��ϷID
	int roomType;		//  ��������
	BuyRoomInfoKey(int gameID, int roomType)
	{
		this->gameID = gameID;
		this->roomType = roomType;
	}

	bool operator < (const BuyRoomInfoKey& key_) const
	{
		if (gameID < key_.gameID)
		{
			return true;
		}

		if (gameID == key_.gameID && roomType < key_.roomType)
		{
			return true;
		}

		return false;
	}
};

struct BuyGameDeskInfo
{
	int gameID;			//	��ϷID
	int count;			//	����
	int roomType;		//  ��������
	int costResType;	//  RESOURCE_TYPE_MONEY=1:���   RESOURCE_TYPE_JEWEL=2:����
	int costNums;		//	��֧ͨ��������
	int AAcostNums;		//  AA֧������
	int otherCostNums;	//  ����֧��������
	int peopleCount;	//	����

	BuyGameDeskInfo()
	{
		memset(this, 0, sizeof(BuyGameDeskInfo));
	}
};

struct OtherConfig
{
	int supportTimesEveryDay;
	int supportMinLimitMoney;
	int supportMoneyCount;
	int registerGiveMoney;
	int registerGiveJewels;
	int logonGiveMoneyEveryDay;

	int sendHornCostJewels;			// ��������㲥���ĵķ���

	int useMagicExpressCostMoney;	// ʹ��ħ�����黨��

	int buyingDeskCount;

	int friendRewardMoney;			// ���Ѵ��ͻ�ý��
	int friendRewardMoneyCount;     // ÿ������ȡ�Ĵ���
	int friendTakeRewardMoneyCount; // ÿ������ȡ�Ĵ���

	BYTE byIsIPRegisterLimit;		// ע��ip����
	int IPRegisterLimitCount;		// ÿ��ipע������

	BYTE byIsDistributedTable;		// �Ƿ�ֲ�ʽ��¼��

	char http[128];					// http�������������ip

	BYTE byIsOneToOne;				// �Ƿ���1:1ƽ̨

	OtherConfig()
	{
		supportTimesEveryDay = 2;
		supportMinLimitMoney = 2000;
		supportMoneyCount = 4000;
		registerGiveMoney = 100000;
		registerGiveJewels = 20;
		logonGiveMoneyEveryDay = 0;
		sendHornCostJewels = 5;
		useMagicExpressCostMoney = 300;
		buyingDeskCount = 5;
		friendRewardMoney = 100;
		friendRewardMoneyCount = 5;
		friendTakeRewardMoneyCount = 5;

		byIsIPRegisterLimit = 0;
		IPRegisterLimitCount = 20;

		byIsDistributedTable = 0;

		memset(http, 0, sizeof(http));

		byIsOneToOne = 0;
	}
};

struct BankConfig
{
	long long minSaveMoney;
	int saveMoneyMuti;
	long long minTakeMoney;
	int takeMoneyMuti;
	long long minTransferMoney;
	int transferMoneyMuti;

	BankConfig()
	{
		minSaveMoney = 10000;
		saveMoneyMuti = 1000;
		minTakeMoney = 10000;
		takeMoneyMuti = 1000;
		minTransferMoney = 10000;
		transferMoneyMuti = 1000;
	}
};

//  ת���������
struct SendGiftConfig
{
	long long myLimitMoney;
	int myLimitJewels;

	long long sendMinMoney;
	int sendMinJewels;

	double rate;		//˰�� 

	SendGiftConfig()
	{
		myLimitMoney = 500000;
		myLimitJewels = 50;

		sendMinMoney = 10000;
		sendMinJewels = 10;

		rate = 0.02;
	}
};

//  ���ֲ��������
struct FriendsGroupConfig
{
	int groupCreateCount; //ÿ������ഴ�����ֲ�ʱ��
	int groupMemberCount; //ÿ�����ֲ���Ա�������
	int groupJoinCount;   //ÿ������������ֲ������������Լ������ͼ���ģ�
	int groupManageRoomCount;//�ھ��ֲ��У�Ⱥ����ഴ����������
	int groupRoomCount;		//�ھ��ֲ��У���ͨ��Ա��ഴ����������
	int groupAllAlterNameCount; //���ֲ��ܹ�����������
	int groupEveAlterNameCount; //���ֲ�ÿ������������
	int groupTransferCount;		//���ֲ��������Ȩ�Ĺ���Ա����

	FriendsGroupConfig()
	{
		groupCreateCount = 5;
		groupMemberCount = 150;
		groupJoinCount = 10;
		groupManageRoomCount = 5;
		groupRoomCount = 1;
		groupAllAlterNameCount = 3;
		groupEveAlterNameCount = 1;
		groupTransferCount = 3;
	}
};

// ftp����
struct FtpConfig
{
	char ftpIP[24];
	char ftpUser[64];
	char ftpPasswd[64];

	FtpConfig()
	{
		memset(this, 0, sizeof(FtpConfig));
	}
};

struct RobotPositionInfo
{
	std::string ip;
	std::string longitude;
	std::string latitude;
	std::string address;
	std::string name;
	std::string headUrl;
	BYTE sex;

	RobotPositionInfo()
	{
		ip = "127.0.0.1";
		address = "default";
		latitude = "0.0";
		longitude = "0.0";
		name = "default";
		headUrl = "default";//"http://39.108.166.3/head/man/1001.jpg";
		sex = 0;
	}
};

struct ServerConfigInfo
{
	int port;
	std::string ip;

	ServerConfigInfo(int port, std::string ip)
	{
		this->port = port;
		this->ip = ip;
	}

	bool operator < (const ServerConfigInfo& info) const
	{
		if (port < info.port)
		{
			return true;
		}

		if (port == info.port && ip < info.ip)
		{
			return true;
		}

		return false;
	}
};

class CMysqlHelper;
// ���ù���, ���������л�����ͨ�������ļ�������
class KERNEL_CLASS CConfigManage
{
private:
	CConfigManage();
	~CConfigManage();

public:
	static CConfigManage* Instance();
	void Release();

public:
	bool Init();
	// ����DB����
	bool LoadDBConfig();
	// �������ķ���������
	bool LoadCenterServerConfig();
	// ���ر������ط���������
	bool LoadLogonServerConfig();
	// ������Ϸ����������
	bool LoadLoaderServerConfig();
	// ����ͨ������
	bool LoadCommonConfig();
	// ����DB���ֶ���������
	bool LoadTableFiledConfig();
	// ���ػ�������
	bool LoadBaseConfig();
	// ������������
	bool LoadOtherConfig();
	// ���÷���������
	void SetServiceType(int type);
	// ��ȡ����ip
	static bool GetInternetIP(char* ip, int size);
	// ��ȡmac��ַ
	static int GetMacByNetBIOS(char* mac, int size);
	// ������֤
	static bool RequestAuth();
	// ����json����
	static std::string ParseJsonValue(const std::string& src, const char* key);
private:
	// �������ݿ�����
	bool ConnectToDatabase();
	// ������Ϸ��������
	bool LoadGameBaseConfig();
	// ���ع�����������
	bool LoadBuyGameDeskConfig();
	// ���ط����������
	bool LoadRoomBaseConfig();
	// ���ػ�����λ������
	bool LoadRobotPositionConfig();
	// �������ֿ�
	bool LoadDirtyWordsConfig();
	// ����db�������
	bool LoadTablesPrimaryKey();
	// ��������������
	bool LoadLogonBaseConfig();
	// ����redis��������
	bool LoadRedisConfig();
public:
	// ��ȡDB����
	const DBConfig& GetDBConfig();
	// ��ȡcenterserver����
	const CenterServerConfig& GetCenterServerConfig();
	// ��ȡlogonserver����
	const LogonServerConfig& GetLogonServerConfig();
	// ��ȡͨ������
	const CommonConfig& GetCommonConfig();
	// ��ȡftp����
	const FtpConfig& GetFtpConfig();
	// ��ȡ����ĳ���ֶε�����
	int GetFieldType(const char* tableName, const char* filedName);
	// ��ȡdll�ļ���
	const char* GetDllFileName(int gameID);
	// ��ȡ�����������
	RoomBaseInfo* GetRoomBaseInfo(int roomID);
	// ͨ��gameID��ȡ˽�з���roomID
	bool GetPrivateRoomIDByGameID(int gameID, std::vector<int>& roomID);
	// ��ȡ��Ϸ��������
	GameBaseInfo* GetGameBaseInfo(int GameID);
	// ��ȡ������������
	BuyGameDeskInfo* GetBuyGameDeskInfo(const BuyGameDeskInfoKey& buyKey);
	// ��ȡ����������Ϣ
	const OtherConfig& GetOtherConfig();
	// ��ȡת������
	const SendGiftConfig& GetSendGiftConfig();
	// ��ȡ��������
	const BankConfig& GetBankConfig();
	// ��ȡ���ֲ�����
	const FriendsGroupConfig& GetFriendsGroupConfig();
	// ��ȡ������������
	LogonBaseInfo* GetLogonBaseInfo(int logonID);
	// ��ȡredis����
	const RedisConfig& GetRedisConfig(int redisTypeID);
	// ��ȡ����roomID��Ϣ
	void GetBuyRoomInfo(int gameID, int roomType, std::vector<int>& roomIDVec);
	// ��÷ֱ�ı���
	bool GetTableNameByDate(const char* name, char* dateName, size_t size);

public:
	// ͨ�������˵�ID��ȡ��Ϣ
	bool GetRobotPositionInfo(int robotID, RobotPositionInfo& info);
	// ��ȡ���ݱ��ֶ��Լ�������Ϣ
	const std::vector<FieldDescriptor>& GetTableFiledDescVec(const std::string& tableName);
	// ��ȡ��������ֶ���
	const char* GetTablePriamryKey(const std::string& tableName);
public:
	static bool sqlGetValue(std::map<std::string, std::string>& data, const char* szFieldName, int& iValue);
	static bool sqlGetValue(std::map<std::string, std::string>& data, const char* szFieldName, UINT& uValue);
	static bool sqlGetValue(std::map<std::string, std::string>& data, const char* szFieldName, long& lValue);
	static bool sqlGetValue(std::map<std::string, std::string>& data, const char* szFieldName, long long& llValue);
	static bool sqlGetValue(std::map<std::string, std::string>& data, const char* szFieldName, double& dValue);
	static bool sqlGetValue(std::map<std::string, std::string>& data, const char* szFieldName, bool& bValue);
	static bool sqlGetValue(std::map<std::string, std::string>& data, const char* szFieldName, BYTE& dValue);
	static bool sqlGetValue(std::map<std::string, std::string>& data, const char* szFieldName, char szBuffer[], UINT uSize);
public:
	// DB����
	DBConfig m_dbConfig;
	// ���ķ���������
	CenterServerConfig m_centerServerConfig;
	// ��������������
	LogonServerConfig m_logonServerConfig;
	// ��Ϸ����������
	LoaderServerConfig m_loaderServerConfig;
	// ͨ������
	CommonConfig m_commonConfig;
	// ftp����
	FtpConfig m_ftpConfig;
	// ��������
	OtherConfig m_otherConfig;
	// ת������
	SendGiftConfig m_sendGiftConfig;
	// ��������
	BankConfig m_bankConfig;
	// ���ֲ��������
	FriendsGroupConfig m_friendsGroupConfig;
	// ����������
	int m_serviceType;

public:
	// ��Ϸ������Ϣ																	
	std::map<int, GameBaseInfo> m_gameBaseInfoMap;
	// ��������
	std::map<int, RoomBaseInfo> m_roomBaseInfoMap;
	// ������Ϸ����������Ϣ(��������)
	std::map<BuyGameDeskInfoKey, BuyGameDeskInfo> m_buyGameDeskInfoMap;
	// ������Ϸ����roomID��Ϣ(roomID����)
	std::map<BuyRoomInfoKey, std::vector<int> > m_buyRoomInfoMap;
	// redis���ݷ���
	std::map<int, RedisConfig> m_redisConfigMap;
	// ����������
	std::map<int, LogonBaseInfo> m_logonBaseInfoMap;
	// ���ݱ��ֶ������Ϣ(�������ñ���Ҫ)
	std::map<std::string, std::vector<FieldDescriptor> > m_tableFieldDescMap;

public:
	std::vector<RobotPositionInfo> m_robotPositionInfoVec;
	std::vector<std::string> m_dirtyWordsVec;
	std::map<std::string, std::string> m_tablePrimaryKeyMap;
	std::map<std::string, int> m_nickName;
private:
	CMysqlHelper* m_pMysqlHelper;		// ���ݿ�ģ��
private:
	std::set<ServerConfigInfo>  m_logonPortSet;
public:
	void GetOtherConfigKeyValue(std::string& strKey, std::string& strValue);
};

#define ConfigManage()		CConfigManage::Instance()