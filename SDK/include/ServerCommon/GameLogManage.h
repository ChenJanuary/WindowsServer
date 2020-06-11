#pragma once

#include <map>
#include <string>

struct ThreadLogFiles
{
	std::string errorLog;
	std::string costLog;

	void Clear()
	{
		errorLog.clear();
		costLog.clear();
	}
};

// ��Ϸlog����
class KERNEL_CLASS CGameLogManage
{
private:
	CGameLogManage();
	~CGameLogManage();

public:
	static CGameLogManage* Instance();
	void Release();

	// �ⲿ�ӿ�����߳���־
	void AddLogFile(int threadID, int threadType, int roomID = 0);
	// ��ȡ��Ӧ�̵߳�errorlog
	std::string GetErrorLog(int threadID);
	// ��ȡ��Ӧ�̵߳�costLog
	std::string GetCostLog(int threadID);

	// ����ָ���ļ���fp
	bool AddLogFileFp(std::string strFile, FILE* fp);
	// ��ȡָ���ļ���fp
	FILE* GetLogFileFp(std::string&& strFile);

private:
	// ������ķ����������־�ļ�
	void AddCenterLogFile(int threadID, int threadType);

	// ��Ӵ��������������־�ļ�
	void AddLogonLogFile(int threadID, int threadType);

	// �����Ϸ�����������־�ļ�
	void AddLoaderLogFile(int threadID, int threadType, int roomID);

private:
	// ��Ϸ��־�ļ�map
	std::map<int /*threadID*/, ThreadLogFiles /*logFileName*/> m_loaderLogFilesMap;
	// ������־�ļ�map
	std::map<int /*threadID*/, ThreadLogFiles /*logFileName*/> m_logonLogFilesMap;
	// ���ķ���־�ļ�map
	std::map<int /*threadID*/, ThreadLogFiles /*logFileName*/> m_centerLogFilesMap;

	// �ļ�������map
	std::map<std::string, FILE*> m_filesFpMap;
};

#define GameLogManage()		CGameLogManage::Instance()