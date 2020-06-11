#include "pch.h"
#include "Math.h"
#include "DataBase.h"
#include "Function.h"
#include "TCPSocket.h"
#include "OleDBErr.h"
#include "ConfigManage.h"
#include "GameLogManage.h"
#include "log.h"

//�����߳̽ṹ����
struct DBThreadParam
{
	HANDLE						hEvent;									//�˳��¼�
	HANDLE						hCompletionPort;						//��ɶ˿�
	CDataBaseManage			* pDataManage;							//���ݿ������ָ��
};

CDataBaseManage::CDataBaseManage()
{
	m_bInit = false;
	m_bRun = false;
	m_hThread = NULL;
	m_hCompletePort = NULL;
	m_pInitInfo = NULL;
	m_pKernelInfo = NULL;
	m_pHandleService = NULL;
	m_pMysqlHelper = NULL;

	m_sqlClass = 0;
	m_nPort = 0;
	m_bsqlInit = FALSE;

	m_host[0] = '\0';
	m_user[0] = '\0';
	m_name[0] = '\0';
	m_passwd[0] = '\0';
}

CDataBaseManage::~CDataBaseManage()
{
	m_bInit = false;
	m_pInitInfo = NULL;
	m_hThread = NULL;
	m_hCompletePort = NULL;
	m_pKernelInfo = NULL;
	m_pHandleService = NULL;
}

//��ʼ����
bool CDataBaseManage::Start()
{
	INFO_LOG("DataBaseManage start begin ...");

	if (m_bRun == true || m_bInit == false)
	{
		ERROR_LOG("DataBaseManage already running or not inited m_bRun=%d m_bInit=%d", m_bRun, m_bInit);
		return false;
	}

	m_bRun = true;

	//�����¼�
	HANDLE StartEvent = CreateEvent(FALSE, TRUE, NULL, NULL);

	//������ɶ˿�
	m_hCompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (m_hCompletePort == NULL)
	{
		ERROR_LOG("CreateIoCompletionPort failed err=%d", GetLastError());
		return false;
	}

	m_DataLine.SetCompletionHandle(m_hCompletePort);

	SQLConnectReset();
	//SQLConnect();

	//�������ݴ����߳�
	unsigned dwThreadID = 0;
	DBThreadParam ThreadStartData;

	ThreadStartData.pDataManage = this;
	ThreadStartData.hEvent = StartEvent;
	ThreadStartData.hCompletionPort = m_hCompletePort;

	int roomID = 0;
	if (m_pInitInfo)
	{
		roomID = m_pInitInfo->uRoomID;
	}

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, DataServiceThread, &ThreadStartData, 0, &dwThreadID);
	if (m_hThread == NULL)
	{
		ERROR_LOG("_beginthreadex DataServiceThread failed");
		return false;
	}

	// ������־�ļ�
	GameLogManage()->AddLogFile(dwThreadID, THREAD_TYPE_ASYNC, roomID);
	// �ȴ����̶߳�ȡ�̲߳���
	WaitForSingleObject(StartEvent, INFINITE);

	ResetEvent(StartEvent);

	INFO_LOG("DataBaseManage start end.");

	return true;
}

bool CDataBaseManage::Stop()
{
	INFO_LOG("DataBaseManage stop begin...");

	if (!m_bRun)
	{
		ERROR_LOG("DataBaseManage is not running...");
		return false;
	}

	m_bRun = false;


	// �ȹر���ɶ˿�
	if (m_hCompletePort)
	{
		PostQueuedCompletionStatus(m_hCompletePort, 0, NULL, NULL);
		CloseHandle(m_hCompletePort);
		m_hCompletePort = NULL;
	}

	// ����dataline
	m_DataLine.SetCompletionHandle(NULL);
	m_DataLine.CleanLineData();

	// �ر����ݿ⴦���߳̾��
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	//�ر����ݿ�����
	if (m_pMysqlHelper)
	{
		m_pMysqlHelper->disconnect();
		delete m_pMysqlHelper;
		m_pMysqlHelper = NULL;
	}

	INFO_LOG("DataBaseManage stop end.");

	return true;
}

//ȡ����ʼ��
bool CDataBaseManage::UnInit()
{
	return true;
}

//���봦�����
bool CDataBaseManage::PushLine(DataBaseLineHead * pData, UINT uSize, UINT uHandleKind, UINT uIndex, DWORD dwHandleID)
{
	//��������
	pData->dwHandleID = dwHandleID;
	pData->uIndex = uIndex;
	pData->uHandleKind = uHandleKind;
	return m_DataLine.AddData(&pData->DataLineHead, uSize, 0) != 0;
}

//���ݿ⴦���߳�
unsigned __stdcall CDataBaseManage::DataServiceThread(LPVOID pThreadData)
{
	Sleep(1);

	INFO_LOG("DataServiceThread starting...");

	//���ݶ���
	DBThreadParam		* pData = (DBThreadParam *)pThreadData;		//�߳���������ָ��
	CDataBaseManage		* pDataManage = pData->pDataManage;				//���ݿ����ָ��
	CDataLine				* pDataLine = &pDataManage->m_DataLine;			//���ݶ���ָ��
	IDataBaseHandleService	* pHandleService = pDataManage->m_pHandleService;	//���ݴ���ӿ�
	HANDLE					hCompletionPort = pData->hCompletionPort;			//��ɶ˿�

	//�߳����ݶ�ȡ���
	::SetEvent(pData->hEvent);

	//�ص�����
	void					* pIOData = NULL;									//����
	DWORD					dwThancferred = 0;								//��������
	ULONG_PTR				dwCompleteKey = 0L;								//�ص� IO ��ʱ����
	LPOVERLAPPED			OverData;										//�ص� IO ��ʱ����

	//���ݻ���
	BOOL					bSuccess = FALSE;
	BYTE					szBuffer[LD_MAX_PART];

	while (pDataManage->m_bRun == true)
	{
		//�ȴ���ɶ˿�
		bSuccess = GetQueuedCompletionStatus(hCompletionPort, &dwThancferred, &dwCompleteKey, (LPOVERLAPPED *)&OverData, INFINITE);
		if (bSuccess == FALSE || dwThancferred == 0)
		{
			//_endthreadex(0);
			continue;
		}

		while (pDataLine->GetDataCount())
		{
			try
			{
				//������ɶ˿�����
				if (pDataLine->GetData((DataLineHead *)szBuffer, sizeof(szBuffer)) < sizeof(DataBaseLineHead))
				{
					continue;
				}

				pHandleService->HandleDataBase((DataBaseLineHead *)szBuffer);
			}
			catch (...)
			{
				ERROR_LOG("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
				continue;
			}
		}
	}

	INFO_LOG("DataServiceThread exit.");

	return 0;
}

//�������ݿ�
bool CDataBaseManage::SQLConnectReset()
{
	if (m_bsqlInit == FALSE)
	{
		const DBConfig& dbConfig = ConfigManage()->GetDBConfig();

		m_nPort = dbConfig.port;

		strcpy(m_host, dbConfig.ip);
		strcpy(m_user, dbConfig.user);
		strcpy(m_name, dbConfig.dbName);
		strcpy(m_passwd, dbConfig.passwd);

		//m_szDetectTable = TEXT("");
		m_bsqlInit = TRUE;
	}

	if (m_pMysqlHelper)
	{
		delete m_pMysqlHelper;
	}

	m_pMysqlHelper = new CMysqlHelper;

	//��ʼ��mysql���󲢽�������
	m_pMysqlHelper->init(m_host, m_user, m_passwd, m_name, "", m_nPort);
	try
	{
		m_pMysqlHelper->connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		ERROR_LOG("�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}

bool CDataBaseManage::CheckSQLConnect()
{
	if (!m_pMysqlHelper)
	{
		return false;
	}

	char buf[128] = "";
	sprintf(buf, "select * from %s LIMIT 1", TBL_BASE_GAME);
	bool bConect = false;

	try
	{
		m_pMysqlHelper->sqlExec(buf);
	}
	catch (...)
	{
		bConect = true;
	}

	if (bConect)
	{
		try
		{
			m_pMysqlHelper->connect();
		}
		catch (MysqlHelper_Exception& excep)
		{
			ERROR_LOG("�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
			return false;
		}
	}

	return true;
}

//***********************************************************************************************//
CDataBaseHandle::CDataBaseHandle()
{
	m_pInitInfo = NULL;
	m_pKernelInfo = NULL;
	m_pRusultService = NULL;
	m_pDataBaseManage = NULL;
}

CDataBaseHandle::~CDataBaseHandle()
{
}

bool CDataBaseHandle::SetParameter(IAsynThreadResultService * pRusultService, CDataBaseManage * pDataBaseManage, ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)
{
	m_pInitInfo = pInitData;
	m_pKernelInfo = pKernelData;
	m_pRusultService = pRusultService;
	m_pDataBaseManage = pDataBaseManage;

	return true;
}

//��ʼ������
bool CDataBaseManage::Init(ManageInfoStruct* pInitInfo, KernelInfoStruct * pKernelInfo, IDataBaseHandleService * pHandleService, IAsynThreadResultService * pResultService)
{
	if (!pInitInfo || !pKernelInfo || !pHandleService || !pResultService)
	{
		throw new CException(TEXT("CDataBaseManage::Init ��������!"), 0x407);
	}

	//Ч�����
	if (m_bInit == true || m_bRun == true)
	{
		throw new CException(TEXT("CDataBaseManage::Init ״̬Ч��ʧ��"), 0x408);
	}

	//��������
	m_pInitInfo = pInitInfo;
	m_pKernelInfo = pKernelInfo;
	m_pHandleService = pHandleService;
	m_DataLine.CleanLineData();

	//��������
	m_bInit = true;

	return true;
}
