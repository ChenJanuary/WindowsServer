#pragma once

#include "KernelDefine.h"

/***********************************************************************ȫ�ֶ���********************************************************************************/

//////////////////////////////////////////////////////////////////////////
#define TMLcopyright  TEXT("����:�����л�������Ƽ����޹�˾")	///��Ȩ

//////////////////////////////////////////////////////////////////////////
#define SAFECHECK_MESSAGE(pMessage,MessageType,pData,iSize) \
if(sizeof(MessageType) != iSize)\
	return false;\
MessageType *pMessage = (MessageType *)pData;\
if(!pMessage)\
	return false;

//////////////////////////////////////////////////////////
// ��Ϸ������
#define INVALID_DESKIDX						(-1)			// ��Ч������ID
#define INVALID_DESKSTATION					0xFF			// ��Ч����λ��
#define MAX_PLAYER_GRADE					9				// ս����ʾ���������
#define MAX_ROOM_HAVE_DESK_COUNT			100000			// ÿ�����������������
#define MAX_PEOPLE							180				// �����Ϸ����
#define MAX_BUY_DESK_JSON_LEN				256				// ��������json��󳤶�

#define MAKE_DESKMIXID(roomID, deskIdx)		(roomID * MAX_ROOM_HAVE_DESK_COUNT + deskIdx)	// ����redis�У���������

#define PRIVATE_DESK_TIMEOUT_SECS			(10 * 60)		// ���������ӳ�ʱʱ��(s) TODOs

#define MAX_ROOM_GRADE_COUNT				10000000		// ÿ��roomID���ս������

#define GRADE_SIMPLE_EXPIRE_TIME			(7*24*60*60)	// �����ս������ʱ��
#define GRADE_EXPIRE_TIME					(GRADE_SIMPLE_EXPIRE_TIME + (1*24*60*60))		// ����ս��ʧЧʱ��

#define SAVE_JSON_PATH						"C:/web/json/"	// ����json��Ŀ¼

////////////////////////////////////////////////////////////
// �˵����
#define BILLS_DIR							"bills"

////////////////////////////////////////////////////////////
// ���ֲ����
#define GROUP_NOTIY_MSG_EXPIRE_TIME			(7*24*60*60)	// ���ֲ�֪ͨ��Ϣ����ʱ��
#define MAX_FRIENDSGROUP_DESK_LIST_COUNT	20				// �����������
#define MAX_FRIENDSGROUP_VIP_ROOM_COUNT		20				// ���VIP��������

//////////////////////////////////////////////////////////////////////////
// �ʼ�ϵͳ
#define EMAIL_SIMPLE_EXPIRE_TIME			(10*24*60*60)	// ÿ���û����ʼ�id����ʱ��
#define EMAIL_EXPIRE_TIME					(11*24*60*60)	// �ʼ��������ʱ��

//////////////////////////////////////////////////////////////////////////
// ���а�
#define MAX_RANK_COUNT						10				// ���а������ʾ����

//////////////////////////////////////////////////////////////////////////
// ���������
#define MAX_MATCH_ROUND							10						// ��������������
#define MAX_GAME_MATCH_ID						10000000				// �����еı�������
#define MAX_MATCH_PEOPLE_COUNT					512						// ����������
#define MIN_ROBOT_USERID						117700					// ��������Сid
#define MAX_ROBOT_USERID						117999					// ���������id
#define MATCH_START_NOTIFY_TIME					180						// ����֪ͨʱ��

// ��������
enum MatchType
{
	MATCH_TYPE_NORMAL = 0,		//0Ĭ��ֵ��û�б�������
	MATCH_TYPE_PEOPLE = 1,		//ʵʱ��
	MATCH_TYPE_TIME = 2,		//��ʱ��
};

// ��ұ���״̬
enum UserMatchStatus
{
	USER_MATCH_STATUS_NORMAL = 0,		//û�б��������߱�������
	USER_MATCH_STATUS_SIGN_UP = 1,		//�Ѿ���������Ϸδ��ʼ
	USER_MATCH_STATUS_AFTER_BEGIN = 2,	//�Ѿ�����������������ʼ
	USER_MATCH_STATUS_PLAYING = 3,		//����������
};

// ��ұ�������״̬
enum DeskMatchStatus
{
	DESK_MATCH_STATUS_NORMAL = 0,	//�ȴ����������߽���
	DESK_MATCH_STATUS_FAIL = 1,		//�Ѿ�����̭
};

// ����ʧ��ԭ��
enum MatchFailReason
{
	MATCH_FAIL_REASON_NOT_ENOUGH_PEOPLE = 1,	//��������
	MATCH_FAIL_REASON_PLAYING = 2,				//��Ϸ��
	MATCH_FAIL_REASON_SYSTEM_ERROR = 3			//ϵͳ�쳣
};

//����״̬  0�������У�1�������У�2����������
enum MatchStatus
{
	MATCH_STATUS_SIGN_UP = 0,	//������
	MATCH_STATUS_PLAYING = 1,	//������
	MATCH_STATUS_GAME_OVER = 2,	//��������
};

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////ö��ģ��/////////////////////////////////////////

// �ߵ�(����)ԭ��
enum ReasonKickout
{
	REASON_KICKOUT_DEFAULT = 0,		// Ĭ��
	REASON_KICKOUT_STAND,			// վ����
	REASON_KICKOUT_STAND_MINLIMIT,	// ���С���������
	REASON_KICKOUT_STAND_MAXLIMIT,  // ��Ҵ����������
	REASON_KICKOUT_NOTAGREE,		// ��ʱ��δ׼��
	REASON_KICKOUT_STAND_FIRECOIN_MINLIMIT,		 // ���С���������
	REASON_KICKOUT_STAND_FIRECOIN_MAXLIMIT,		 // ��Ҵ����������
	REASON_KICKOUT_LONG_TIME_NOOPERATION,		//��ʱ��δ����
};

// ��Դ����
enum ResourceType
{
	RESOURCE_TYPE_NONE = 0,
	RESOURCE_TYPE_MONEY,		// ���
	RESOURCE_TYPE_JEWEL,		// ����
	RESOURCE_TYPE_BANKMONEY,	// ���н��
	RESOURCE_TYPE_FIRECOIN,		// ���ֲ����
	RESOURCE_TYPE_END,
};

// ��½ע������
enum LogonType
{
	LOGON_QUICK,	// ���ٵ�¼
	LOGON_NORMAL,	// ��ͨ��¼
	LOGON_WEICHAT,	// ΢�ŵ�¼
	LOGON_QQ,		// QQ��¼
	LOGON_TEL_PHONE,// �ֻ���¼
	LOGON_TEL_XIANLIAO,// ���ĵ�¼
	LOGON_VISITOR,	// �ο͵�¼
	LOGON_NR		// ����±�
};

// ����״̬
enum LogonStatus
{
	LOGON_REGISTER,  //ע��
	LOGON_LOGIN,     //��¼
};

// ֧����ʽ
enum PayType
{
	PAY_TYPE_NORMAL = 1,	// ��֧ͨ��
	PAY_TYPE_AA = 2,		// AA֧��
};

// ���������
enum RoomTipType
{
	ROOM_TIP_TYPE_NO = 0,		//����С��
	ROOM_TIP_TYPE_ALL_WIN = 1,	//��ȡ���е�Ӯ��
	ROOM_TIP_TYPE_MAX_WIN = 2,	//��ȡ���Ĵ�Ӯ��
};

// redis��������
enum RedisDataType
{
	REDIS_DATA_TYPE_GAME = 1,	// ������Ϸ����
	REDIS_DATA_TYPE_PHP = 2,	// ����PHP����
};

// redis�д�ȡ����������
enum SystemConfigType
{
	SYSTEM_CONFIG_TYPE_OTHER = 1,	// ��������
	SYSTEM_CONFIG_TYPE_BANK = 2,	// ����
	SYSTEM_CONFIG_TYPE_SENDGIFT = 3,// ת��
	SYSTEM_CONFIG_TYPE_FG = 4,		// ���ֲ�
	SYSTEM_CONFIG_TYPE_FTP = 5,		// ftp
};

// ��½��socket����
enum LogonServerSocketType
{
	LOGON_SERVER_SOCKET_TYPE_NO = 0,			//������
	LOGON_SERVER_SOCKET_TYPE_USER = 1,			//���
	LOGON_SERVER_SOCKET_TYPE_GAME_SERVER = 2,	//��Ϸ��
};

// �첽�̴߳���������
enum AnsyThreadResultType
{
	ANSY_THREAD_RESULT_TYPE_NO = 0,			//������
	ANSY_THREAD_RESULT_TYPE_DATABASE = 1,	//���ݿ�
	ANSY_THREAD_RESULT_TYPE_HTTP = 2,		//http��https������
	ANSY_THREAD_RESULT_TYPE_LOG = 3,		//��־
};

// HTTP��������
enum HTTP_POST_TYPE
{
	HTTP_POST_TYPE_NO = 0,			//������
	HTTP_POST_TYPE_REGISTER = 1,	//ע��
	HTTP_POST_TYPE_LOGON = 2,		//��½
	HTTP_POST_TYPE_LOGOUT = 3,		//�ǳ�
	HTTP_POST_TYPE_REQ_DATA = 4,	//��ȡҳ������
	HTTP_POST_TYPE_MATCH_GIFT = 5,	//��������
	HTTP_POST_TYPE_MATCH_FAIL = 6,	//����ʧ��
};

//////////////////////////////////////////////////////////////////////////