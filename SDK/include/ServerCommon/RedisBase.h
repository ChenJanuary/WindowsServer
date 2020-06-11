#pragma once

#include "hiredis.h"
#include "configManage.h"
#include "tableDefine.h"
#include <string>
#include <vector>
#include <unordered_map>

#pragma comment(lib, "hiredis.lib")

// redis�����������ʵ����redis���Դ洢2^32-1��key-value
#define MAX_REDIS_CMD_WORD_COUNT	512		

//redis hash����չ�ֶ�ģʽ����ʱɨ���Ǹ��ݶ�Ӧ���ֶ�����Ӧ�Ĳ�����
enum RedisExtendMode
{
	REDIS_EXTEND_MODE_DEFAULT = 0,
	REDIS_EXTEND_MODE_UPDATE,
	REDIS_EXTEND_MODE_INSERT,
	REDIS_EXTEND_MODE_DELETE,
	REDIS_EXTEND_MODE_END,
};

//redis ��Ӧ��key������
enum RedisKeyType
{
	REDIS_KEY_TYPE_NONE = 0,	// ������(����)
	REDIS_KEY_TYPE_HASH,		// ��ϣ
};

class CDataBaseManage;
// CRedisBase �����
class KERNEL_CLASS CRedisBase
{
public:
	CRedisBase();
	virtual ~CRedisBase();

public:
	// ��ʼ��
	virtual bool Init() = 0;
	virtual bool Stop() = 0;

public:
	static std::string MakeKey(const char* tableName, long long id);
	// �ϳ�redis�е�key"tableName|id"��ʽ
	static std::string MakeKey(const char* tableName, int id);
	// �ϳ�redis�е�key"tableName|id"��ʽ
	static std::string MakeKey(const char* tableName, const char* id);
	// ����redis�е�key, "tableName|id"��ʽ
	static bool ParseKey(const char* key, char* tableName, int& id);
	// ����redis�е�key, "tableName|id"��ʽ
	static bool ParseKey(const char* key, char* tableName, long long& id);

public:
	// redis �е�key�Ƿ����
	bool IsKeyExists(const char* key);
	// redis �е�key�Ƿ����
	bool IsKeyExists(const char* mainKey, int mainID, int assID);
	// redis �е�key�Ƿ����
	bool IsKeyExists(const char* mainKey, int mainID);
	// redis �е�key�Ƿ����
	bool IsKeyExists(const char* mainKey, const char* assKey);
	// ɾ��ָ����key
	bool DelKey(const char* key);
	// ��ȡkey������
	int GetKeyType(const char* key);
	// ɾ�����ϣ����򼯺ϣ���Ԫ��
	bool SremMember(const char* key, const char* member);
	// ɾ�����ϣ����򼯺ϣ�Ԫ��
	bool ZremMember(const char* key, const std::vector<long long> &memberVec);
	// ɾ�����ϣ����򼯺ϣ�Ԫ��(int����)
	bool ZremMember(const char* key, const std::vector<int> &memberVec);
	// ��ȡ���򼯺ϵĴ�С
	int GetZSetSize(const char* key);
	// �趨��ʱʱ��
	bool SetExpire(const char* key, int secs);
	// ���������
	void CheckConnection(const RedisConfig& redisConfig);
	// ����redis��д����
	void TestRedis(const char * name);
	// ��ȡĳ����ֵ����
	long long GetKeyIndex(const char * key);
	// ����ĳ����ֵ����
	bool SetKeyIndex(const char * key, long long llIndex);
	// ��ȡ�����ַ���
	long long GetIncrKeyIndex(const char * key);
	// ���ݿ�
	void SetDBManage(CDataBaseManage* pDBManage);
protected:
	// ��ȡ�ֶ���չģʽ
	int  GetExtendMode(const char* key);
	// ������չģʽ
	bool SetExtendMode(const char* key, int mode);
	// ����
	bool ReConnect(const RedisConfig& redisConfig);
	// ��֤
	bool Auth(const char* passwd);
public:
	// ���ù�ϣ������
	bool hmset(const char* table, int id, std::unordered_map<std::string, std::string>& fieldInfoUMap, int mode = REDIS_EXTEND_MODE_DEFAULT, const char * updateSet = NULL);
	bool hmset(const char* table, long long id, std::unordered_map<std::string, std::string>& fieldInfoUMap, int mode = REDIS_EXTEND_MODE_DEFAULT, const char * updateSet = NULL);
	bool hmset(const char* table, const char * id, std::unordered_map<std::string, std::string>& fieldInfoUMap, int mode = REDIS_EXTEND_MODE_DEFAULT, const char * updateSet = NULL);
public:
	int m_sleepTime;
protected:
	redisContext* m_pContext;
protected: //���ݿ����
	CDataBaseManage* m_pDBManage;
};

// redis ��
class KERNEL_CLASS CRedisLock
{
public:
	CRedisLock(redisContext* pContext, const char* key, int sleepTime = 0, bool bAutoLock = true);
	~CRedisLock();

public:
	void Lock();
	void Unlock();

private:
	redisContext* m_pContext;
	char m_key[128];
	int m_sleepTime;
};

// redis������
#define REDIS_CHECKF(pReply, redisCmd)	{if (!pReply){ERROR_LOG("redisCommand return NULL redisCmd=%s context err=%d errstr=%s", redisCmd, m_pContext->err, m_pContext->errstr);return 0;}}
#define REDIS_CHECK(pReply, redisCmd)	{if (!pReply){ERROR_LOG("redisCommand return NULL redisCmd=%s context err=%d errstr=%s", redisCmd, m_pContext->err, m_pContext->errstr);return;}}
#define ASSERT_REDIS_REPLY_ERRORF(pReply, redisCmd)	{if (pReply->type==REDIS_REPLY_ERROR){ERROR_LOG("redisCommand return Error redisCmd=%s errstr=%s", redisCmd, pReply->str);freeReplyObject(pReply); return 0;}}
#define ASSERT_REDIS_REPLY_ERROR(pReply, redisCmd)	{if (pReply->type==REDIS_REPLY_ERROR){ERROR_LOG("redisCommand return Error redisCmd=%s errstr=%s", redisCmd, pReply->str);freeReplyObject(pReply);}}
#define ASSERT_REDIS_REPLY_NILF(pReply, redisCmd)	{if (pReply->type==REDIS_REPLY_NIL){ERROR_LOG("redisCommand return Error redisCmd=%s errstr=%s", redisCmd, pReply->str);freeReplyObject(pReply); return 0;}}
#define ASSERT_REDIS_REPLY_NIL(pReply, redisCmd)	{if (pReply->type==REDIS_REPLY_NIL){ERROR_LOG("redisCommand return Error redisCmd=%s errstr=%s", redisCmd, pReply->str);freeReplyObject(pReply);}}