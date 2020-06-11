#pragma once

/***********************************************************************�ں˶���********************************************************************************/

const int MIN_USER_ACCOUNT_LEN = 4;			// ����˺���С����
const int MAX_USER_ACCOUNT_LEN = 48;		// ����˺���󳤶�

const int MAX_USER_NAME_LEN = 64;			// ����ǳ���󳤶�
const int MAX_USER_HEADURL_LEN = 256;		// ���ͷ����󳤶�

const int USER_MD5_PASSWD_LEN = 32;				// ���볤��(����MD5)
const int MAX_USER_MAD_PASSWD_SIZE = 48;		// ������뻺������С

const int MAX_PRIVATE_DESK_PASSWD_LEN = 20;		// ˽�з�������볤��
const int PRIVATE_DESK_PASSWD_LEN = 6;			// ˽�з����볤��

const int MAX_TEMP_SENDBUF_SIZE = 8192;			// ��ʱ���ͻ�������С
const int MAX_REDIS_COMMAND_SIZE = 2048;		// redis���������С
const int MAX_SQL_STATEMENT_SIZE = 2048;		// sql��仺������С

const int MAX_IP_ADDR_SIZE = 128;		// ���IP��ַ����
const int MAX_DB_USER_SIZE = 48;		// ���db_user��������С
const int MAX_DB_PASSWD_SIZE = 48;		// ���db_passwd��������С
const int MAX_DB_NAME_SIZE = 48;		// ���db_name��������С

const int MAX_LOG_BUF_SIZE = 2048;		// log��������С
const int MAX_FILE_NAME_SIZE = 256;		// ����ļ�������
const int MAX_FUNC_NAME_SIZE = 128;		// �����������

const int MAX_BANK_PASSWD_MIN_LEN = 6;	// ����������С����

//////////////////////////////////////////////////////////////////////////
//���ݶ�����󳤶ȣ���������ͷ������Ҫ���� MAX_TEMP_SENDBUF_SIZE=8192
#define LD_MAX_PART						8320

/////////////////////////////////////////////////////////
//web ���
#define  WEB_ADDRESS	"headimgqn.szysx8.cn"
const int MIN_MAN_HEADURL_ID = 1001;
const int MAX_MAN_HEADURL_ID = 3287;
const int MIN_WOMAN_HEADURL_ID = 1000001;
const int MAX_WOMAN_HEADURL_ID = 1003803;

/////////////////////////////////////////////////////////
// ������socket���
#define KEEP_ACTIVE_HEARTBEAT_COUNT				3		// ǰ�˺ͷ�����������
#define BINDID_SOCKET_USERID_TIME				30		// ÿ������socket�������ʱ�䣬��λ��
#define MAX_LOGON_SERVER_COUNT					48		// ����½������

/////////////////////////////////////////////////////////
// ��ʱͳ��
#define MIN_STATISTICS_FUNC_COST_TIME			20000	// ������ʱ��Сͳ��ʱ��(us)

/////////////////////////////////ö��ģ��/////////////////////////////////////////

// ����Ա�
enum UserSex
{
	USER_DEX_FEMALE = 0,//Ů��
	USER_SEX_MALE = 1,  //����
};

// ����������
enum ServiceType
{
	SERVICE_TYPE_BEGIN = 0,
	SERVICE_TYPE_LOGON,		// ����
	SERVICE_TYPE_LOADER,	// ��Ϸ
	SERVICE_TYPE_CENTER,	// ���ķ�
	SERVICE_TYPE_PHP,		// PHP������
	SERVICE_TYPE_END,
};

// �߳�����
enum ThreadType
{
	THREAD_TYPE_MAIN = 1,	// ���߳�
	THREAD_TYPE_LOGIC = 2,	// �߼��߳�
	THREAD_TYPE_ASYNC = 3,	// �첽�߳�
	THREAD_TYPE_ACCEPT = 4,	// �����߳�
	THREAD_TYPE_RECV = 5,	// �����߳�
};

// ��־�ȼ�
enum LogLevel
{
	LOG_LEVEL_INFO = 0,						//info
	LOG_LEVEL_WARNNING,						//warnning
	LOG_LEVEL_ERROR,						//error
	LOG_LEVEL_INFO_CONSOLE,					//info Console
	LOG_LEVEL_ERROR_CONSOLE,				//error Console
	LOG_LEVEL_END,
};

//////////////////////////////////////////////////////////////////////////