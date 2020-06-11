#pragma once

#include "KernelDefine.h"
#include "GameLogManage.h"
#include <string>
#include <array>

const std::array<const char*, LOG_LEVEL_END> levelNames = { "��INFO��", "��WARNNING��", "��ERROR��", "��INFO��","��ERROR��", };

//// ��־��
class KERNEL_CLASS CLog
{
public:
	CLog();
	~CLog();

public:
	static void Write(const char* pLogFile, int level, const char *pFile, int line, const char* pFuncName, const char *pFormat, ...);

	static void Write(const char* pLogFile, const char*pFuncName, const char* pFormat, ...);

	// ��buf�е�����д��logFile
	static void Write(const char* pLogFile, const char* buf);
};

// sysʱ��Ƶ��
static long long sysFrequency = 0;

// ��⺯������
class KERNEL_CLASS CAutoLog
{
public:
	CAutoLog(const char* pLogFile, const char* pFileName, const char* pFuncName, int line);
	~CAutoLog();

private:
	char m_logFile[MAX_FILE_NAME_SIZE];
	char m_fileName[MAX_FILE_NAME_SIZE];
	char m_funcName[MAX_FUNC_NAME_SIZE];
	int	 m_line;
};

// ͳ�ƺ�����ʱ
class KERNEL_CLASS CAutoLogCost
{
public:
	CAutoLogCost(const char* pLogFile, const char* pFuncName, int microSecs, const char* format, ...);
	~CAutoLogCost();

private:
	int m_microSecs;
	long long m_beginTime;
	long long m_endTime;
	long long m_costTime;

	char m_logFile[MAX_FILE_NAME_SIZE];
	char m_funcName[MAX_FUNC_NAME_SIZE];

	char m_buf[MAX_LOG_BUF_SIZE];
};

#define	AUTOLOG()		CAutoLog autolog(GameLogManage()->GetErrorLog(GetCurrentThreadId()).c_str(), __FILE__, __FUNCTION__, __LINE__);
#define AUTOCOST(...)	CAutoLogCost logCost(GameLogManage()->GetCostLog(GetCurrentThreadId()).c_str(), __FUNCTION__, MIN_STATISTICS_FUNC_COST_TIME, __VA_ARGS__);

// ���������Ϣ ��ֻ������ļ�ϵͳ��
#define ERROR_LOG(...)	{ CLog::Write(GameLogManage()->GetErrorLog(GetCurrentThreadId()).c_str(), LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

// ���������Ϣ ��ֻ������ļ�ϵͳ��
#define INFO_LOG(...)	{ CLog::Write(GameLogManage()->GetErrorLog(GetCurrentThreadId()).c_str(), LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

// �����Ҫ������Ϣ ��ֻ������ļ�ϵͳ��
#define WARNNING_LOG(...)	{ CLog::Write(GameLogManage()->GetErrorLog(GetCurrentThreadId()).c_str(), LOG_LEVEL_WARNNING, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

// ���������Ϣ ��������ļ�ϵͳ�Ϳ���̨��
#define CON_ERROR_LOG(...)	{ CLog::Write(GameLogManage()->GetErrorLog(GetCurrentThreadId()).c_str(), LOG_LEVEL_ERROR_CONSOLE, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

// ���������Ϣ ��������ļ�ϵͳ�Ϳ���̨��
#define CON_INFO_LOG(...)	{ CLog::Write(GameLogManage()->GetErrorLog(GetCurrentThreadId()).c_str(), LOG_LEVEL_INFO_CONSOLE, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

// ������ǰ�Ĵ���
#define WAUTOLOG()		AUTOLOG()
#define WAUTOCOST(...)	AUTOCOST(__VA_ARGS__)
#define WERROR_LOG(...)	ERROR_LOG(__VA_ARGS__)
#define WINFO_LOG(...)	INFO_LOG(__VA_ARGS__)