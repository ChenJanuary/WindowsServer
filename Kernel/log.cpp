#include "pch.h"
#include "log.h"
#include "configManage.h"

CLog::CLog()
{
}

CLog::~CLog()
{

}

void CLog::Write(const char* pLogfile, int level, const char * pFile, int line, const char * pFuncName, const char * pBuf, ...)
{
	if (!pLogfile || !pFile || !pFuncName || !pBuf)
	{
		return;
	}

	if (level >= (int)levelNames.size())
	{
		return;
	}

	const char* levelName = levelNames[level];
	if (!levelName)
	{
		return;
	}

	char buf[MAX_LOG_BUF_SIZE] = "";

	// �߳�ID��level
	int threadID = GetCurrentThreadId();
	sprintf(buf, "%d %s ", threadID, levelName);

	// ʱ��
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	int millisecs = sysTime.wSecond * 1000 + sysTime.wMilliseconds;

	sprintf(buf + strlen(buf), "%04d-%02d-%02d %02d:%02d:%05d ", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, millisecs);

	// ����
	va_list args;
	va_start(args, pBuf);

	vsprintf(buf + strlen(buf), pBuf, args);
	va_end(args);

	// ���������̨
	if (level == LOG_LEVEL_INFO_CONSOLE)
	{
		std::cout << buf << std::endl;
	}
	else if (level == LOG_LEVEL_ERROR_CONSOLE)
	{
		std::cout << buf << "{func=" << pFuncName << " line=" << line << "}\n";
	}

	sprintf(buf + strlen(buf), "{%s %s %d}\n", pFile, pFuncName, line);

	std::string strFile = pLogfile;
	FILE* fp = GameLogManage()->GetLogFileFp(std::move(strFile));
	if (!fp)
	{
		fp = fopen(pLogfile, "a+");
		if (!fp)
		{
			return;
		}
		GameLogManage()->AddLogFileFp(pLogfile, fp);
	}

	fputs(buf, fp);
	fflush(fp);
}

void CLog::Write(const char * pLogFile, const char * pFuncName, const char * pFormat, ...)
{
	if (!pLogFile || !pFuncName || !pFormat)
	{
		return;
	}

	char buf[MAX_LOG_BUF_SIZE] = "";

	// �߳�ID��level
	int threadID = GetCurrentThreadId();
	const char* levelName = levelNames[LOG_LEVEL_INFO];
	sprintf(buf, "%d %s ", threadID, levelName);

	// ʱ��
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	int millisecs = sysTime.wSecond * 1000 + sysTime.wMilliseconds;

	sprintf(buf + strlen(buf), "%04d-%02d-%02d %02d:%02d:%05d ", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, millisecs);

	// ����
	va_list args;
	va_start(args, pFormat);

	vsprintf(buf + strlen(buf), pFormat, args);
	va_end(args);

	sprintf(buf + strlen(buf), " %s\n", pFuncName);

	std::string strFile = pLogFile;
	FILE* fp = GameLogManage()->GetLogFileFp(std::move(strFile));
	if (!fp)
	{
		fp = fopen(pLogFile, "a+");
		if (!fp)
		{
			return;
		}
		GameLogManage()->AddLogFileFp(pLogFile, fp);
	}

	fputs(buf, fp);
	fflush(fp);
}

void CLog::Write(const char * pLogFile, const char * buf)
{
	if (!pLogFile || !buf)
	{
		return;
	}

	std::string strFile = pLogFile;
	FILE* fp = GameLogManage()->GetLogFileFp(std::move(strFile));
	if (!fp)
	{
		fp = fopen(pLogFile, "a+");
		if (!fp)
		{
			return;
		}
		GameLogManage()->AddLogFileFp(pLogFile, fp);
	}

	fputs(buf, fp);
	fflush(fp);
}

CAutoLog::CAutoLog(const char* pLogFile, const char* pFileName, const char* pFuncName, int line)
{
	memset(m_logFile, 0, sizeof(m_logFile));
	memset(m_fileName, 0, sizeof(m_fileName));
	memset(m_funcName, 0, sizeof(m_funcName));

	if (pLogFile && pFileName && pFuncName)
	{
		strcpy(m_logFile, pLogFile);
		strcpy(m_fileName, pFileName);
		strcpy(m_funcName, pFuncName);
	}

	m_line = line;

	// ��ӡ��ʼ��Ϣ
	CLog::Write(m_logFile, LOG_LEVEL_INFO, pFileName, line, pFuncName, "AUTOLOG Begin:");
}

CAutoLog::~CAutoLog()
{
	CLog::Write(m_logFile, LOG_LEVEL_INFO, m_fileName, m_line, m_funcName, "AUTOLOG End��");
}

CAutoLogCost::CAutoLogCost(const char* plogFile, const char* pFuncName, int microSecs, const char* pFormat, ...)
{
	memset(m_buf, 0, sizeof(m_buf));
	memset(m_logFile, 0, sizeof(m_logFile));
	memset(m_funcName, 0, sizeof(m_funcName));

	m_microSecs = microSecs;

	LARGE_INTEGER largeInteger;

	if (sysFrequency == 0)
	{
		QueryPerformanceFrequency(&largeInteger);
		sysFrequency = largeInteger.QuadPart;
	}

	// ��¼��ʼʱ��
	QueryPerformanceCounter(&largeInteger);
	m_beginTime = largeInteger.QuadPart;

	// ѡ��log�ļ�
	if (plogFile)
	{
		strcpy(m_logFile, plogFile);
	}
	else
	{
		strcpy(m_logFile, "cost.log");
	}

	//////// ���buf����������
	// ʱ��
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	int millisecs = sysTime.wSecond * 1000 + sysTime.wMilliseconds;

	sprintf(m_buf, "%04d-%02d-%02d %02d:%02d:%05d ", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, millisecs);

	// �䳤����
	va_list args;
	va_start(args, pFormat);

	vsprintf(m_buf + strlen(m_buf), pFormat, args);
	va_end(args);

	// ������
	sprintf(m_buf + strlen(m_buf), " %s ", pFuncName);
}

CAutoLogCost::~CAutoLogCost()
{
	LARGE_INTEGER largeInteger;
	QueryPerformanceCounter(&largeInteger);
	m_endTime = largeInteger.QuadPart;

	if (sysFrequency > 0)
	{
		m_costTime = (m_endTime - m_beginTime) * 1000 * 1000 / sysFrequency;

		if (m_costTime >= m_microSecs)
		{
			sprintf(m_buf + strlen(m_buf), "costTime:��%I64dus��\n", m_costTime);
			CLog::Write(m_logFile, m_buf);
		}
	}
}
