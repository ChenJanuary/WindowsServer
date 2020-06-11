#include "pch.h"
#include "BillManage.h"
#include "Define.h"
#include "Util.h"
#include "log.h"
#include "InternalMessageDefine.h"

CBillManage::CBillManage()
{
}

CBillManage::~CBillManage()
{
}

CBillManage* CBillManage::Instance()
{
	static CBillManage manage;
	return &manage;
}

void CBillManage::Release()
{
}

void CBillManage::WriteUserBill(int userID, int billType, const char* pFormat, ...)
{
	//AUTOCOST("WriteUserBill");

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	char path[128] = "";
	sprintf(path, "%s/%04d-%02d-%02d", BILLS_DIR, sysTime.wYear, sysTime.wMonth, sysTime.wDay);

	CUtil::MkdirIfNotExists(path);

	int currTime = (int)time(NULL);

	char buf[2048] = "";
	sprintf(buf, "%d|%d|%d|", billType, currTime, userID);

	va_list args;
	va_start(args, pFormat);

	vsprintf(buf + strlen(buf), pFormat, args);
	va_end(args);

	// bill�ļ�����ÿ10����һ�Σ�
	char fileName[128] = "";
	sprintf(fileName, "%04d-%02d-%02d-%02d-%d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute / 10);

	// ���·��+�ļ���
	char fullFileName[128] = "";
	sprintf(fullFileName, "%s/%s", path, fileName);

	FILE* fp = fopen(fullFileName, "a+");
	if (!fp)
	{
		return;
	}

	// д�˵�
	fputs(buf, fp);
	fputc('\n', fp);
	fclose(fp);
}

void CBillManage::WriteCommonBill(int billType, const char* pFormat, ...)
{
	//AUTOCOST("WriteUserBill");

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	char path[128] = "";
	sprintf(path, "%s/%04d-%02d-%02d", BILLS_DIR, sysTime.wYear, sysTime.wMonth, sysTime.wDay);

	CUtil::MkdirIfNotExists(path);

	int currTime = (int)time(NULL);

	char buf[2048] = "";
	sprintf(buf, "%d|%d|", billType, currTime);

	va_list args;
	va_start(args, pFormat);

	vsprintf(buf + strlen(buf), pFormat, args);
	va_end(args);

	// bill�ļ�����ÿ10����һ�Σ�
	char fileName[128] = "";
	sprintf(fileName, "%04d-%02d-%02d-%02d-%d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute / 10);

	// ���·��+�ļ���
	char fullFileName[128] = "";
	sprintf(fullFileName, "%s/%s", path, fileName);

	FILE* fp = fopen(fullFileName, "a+");
	if (!fp)
	{
		return;
	}

	// д�˵�
	fputs(buf, fp);
	fputc('\n', fp);
	fclose(fp);
}

void CBillManage::WriteBill(CDataBaseManage* m_pDBManage, const char* pFormat, ...)
{
	if (m_pDBManage == NULL)
	{
		ERROR_LOG("���ݾ��m_pDBManage=NULL");
		return;
	}

	char sql[MAX_SQL_STATEMENT_SIZE] = "";

	va_list args;
	va_start(args, pFormat);

	vsprintf(sql, pFormat, args);

	va_end(args);

	InternalSqlStatement msg;
	memcpy(msg.sql, sql, sizeof(sql));

	if (!m_pDBManage->PushLine(&msg.head, sizeof(InternalSqlStatement), DTK_GP_SQL_STATEMENT, 0, 0))
	{
		ERROR_LOG("Ͷ�ݶ���ʧ�ܣ�%s", sql);
	}
}