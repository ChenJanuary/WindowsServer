#include "main.h"
#include "BaseLogonServer.h"
#include "Exception.h"
#include "RedisLogon.h"
#include "log.h"
#include "PlatformMessage.h"
#include "InternalMessageDefine.h"
#include "Function.h"
#include "Util.h"

#pragma warning (disable: 4355)

//�����߳������ṹ
struct WindowThreadStartStruct
{
	//��������
	HANDLE								hEvent;						//�����¼�
	BOOL								bSuccess;					//�����ɹ���־
	CBaseLogonServer				* pMainManage;				//���ݹ���ָ��
};

//�����߳������ṹ
struct HandleThreadStartStruct
{
	//��������
	HANDLE								hEvent;						//�����¼�
	HANDLE								hCompletionPort;			//��ɶ˿�
	CBaseLogonServer				* pMainManage;				//���ݹ���ָ��
};

/*****************************************************************************************************************/
CBaseLogonServer::CBaseLogonServer()
{
	m_bInit = false;
	m_bRun = false;
	m_hWindow = NULL;
	m_hHandleThread = NULL;
	m_hWindowThread = NULL;
	m_hCompletePort = NULL;
	m_connectCServerHandle = NULL;
	::memset(&m_DllInfo, 0, sizeof(m_DllInfo));
	::memset(&m_InitData, 0, sizeof(m_InitData));
	::memset(&m_KernelData, 0, sizeof(m_KernelData));

	m_pRedis = NULL;
	m_pRedisPHP = NULL;
	m_pTcpConnect = NULL;
}

CBaseLogonServer::~CBaseLogonServer()
{
	SAFE_DELETE(m_pRedis);
	SAFE_DELETE(m_pRedisPHP);
	SAFE_DELETE(m_pTcpConnect);
}

//��ʼ������ 
bool CBaseLogonServer::Init(ManageInfoStruct * pInitData, IDataBaseHandleService * pDataHandleService)
{
	INFO_LOG("BaseMainManageForZ Init begin...");

	if (m_bInit == true)
	{
		ERROR_LOG("is already have been inited...");
		return false;
	}

	if (!pInitData || !pDataHandleService)
	{
		ERROR_LOG("invalid params input pInitData=%p pDataHandleService=%p", pInitData, pDataHandleService);
		return false;
	}

	if (m_bInit == true)
	{
		ERROR_LOG("is already been inited");
		return false;
	}

	srand((unsigned)time(NULL));

	//��������
	m_InitData = *pInitData;

	bool ret = false;

	// ���ز���
	ret = PreInitParameter(&m_InitData, &m_KernelData);
	if (!ret)
	{
		ERROR_LOG("PreInitParameter failed");
		return false;
	}

	// redis ���
	m_pRedis = new CRedisLogon;
	if (!m_pRedis)
	{
		ERROR_LOG("create redis object failed");
		return false;
	}

	ret = m_pRedis->Init();
	if (!ret)
	{
		ERROR_LOG("����redisʧ��");
		return false;
	}

	// redis ���
	m_pRedisPHP = new CRedisPHP;
	if (!m_pRedisPHP)
	{
		ERROR_LOG("create m_pRedisPHP object failed");
		return false;
	}

	ret = m_pRedisPHP->Init();
	if (!ret)
	{
		ERROR_LOG("����PHP redisʧ��");
		return false;
	}

	// ��ʼ������
	ret = m_TCPSocket.Init(this, m_InitData.uMaxPeople, m_InitData.uListenPort, m_InitData.iSocketSecretKey);
	if (!ret)
	{
		ERROR_LOG("TCPSocket Init failed");
		return false;
	}

	// DB���
	ret = pDataHandleService->SetParameter(this, &m_SQLDataManage, &m_InitData, &m_KernelData);
	if (!ret)
	{
		ERROR_LOG("DataHandleService SetParameter failed");
		return false;
	}

	ret = m_SQLDataManage.Init(&m_InitData, &m_KernelData, pDataHandleService, this);
	if (!ret)
	{
		ERROR_LOG("SQLDataManage Init failed");
		return false;
	}

	// ���ķ���������
	m_pTcpConnect = new CTcpConnect;
	if (!m_pTcpConnect)
	{
		throw new CException(TEXT("BaseMainManageForZ::Init new CTcpConnect failed"), 0x43A);
	}

	ret = OnInit(&m_InitData, &m_KernelData);
	if (!ret)
	{
		ERROR_LOG("OnInit failed");
		return false;
	}

	m_bInit = true;

	INFO_LOG("BaseMainManageForZ Init end");

	return true;
}

//ȡ����ʼ������ 
bool CBaseLogonServer::UnInit()
{
	////ֹͣ����
	//if (m_bRun)
	//{
	//	Stop();
	//}

	////���ýӿ�
	//OnUnInit();

	//m_bInit = false;
	//m_TCPSocket.UnInit();
	//m_SQLDataManage.UnInit();

	////��������
	//memset(&m_DllInfo, 0, sizeof(m_DllInfo));
	//memset(&m_InitData, 0, sizeof(m_InitData));
	//memset(&m_KernelData, 0, sizeof(m_KernelData));

	return true;
}

//��������
bool CBaseLogonServer::Start()
{
	INFO_LOG("BaseMainManageForZ Start begin...");

	if (m_bInit == false || m_bRun == true)
	{
		ERROR_LOG("BaseMainManageForZ already been inited or running");
		return false;
	}

	m_bRun = true;

	//�����¼�
	HANDLE StartEvent = CreateEvent(FALSE, TRUE, NULL, NULL);

	//������ɶ˿�
	m_hCompletePort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (!m_hCompletePort)
	{
		ERROR_LOG("CreateIoCompletionPort failed err=%d", GetLastError());
		return false;
	}

	m_DataLine.SetCompletionHandle(m_hCompletePort);

	//���������߳�
	UINT uThreadID = 0;

	HandleThreadStartStruct	ThreadStartData;

	ThreadStartData.pMainManage = this;
	ThreadStartData.hCompletionPort = m_hCompletePort;
	ThreadStartData.hEvent = StartEvent;

	m_hHandleThread = (HANDLE)_beginthreadex(NULL, 0, LineDataHandleThread, &ThreadStartData, 0, &uThreadID);
	if (!m_hHandleThread)
	{
		ERROR_LOG("begin LineDataHandleThread failed");
		return false;
	}

	// ��������ҵ���߼��߳����Ӧ��־�ļ�
	GameLogManage()->AddLogFile(uThreadID, THREAD_TYPE_LOGIC);

	WaitForSingleObject(StartEvent, INFINITE);

	bool ret = false;

	//��������ģ��
	ret = CreateWindowsForTimer();
	if (!ret)
	{
		ERROR_LOG("CreateWindowsForTimer failed");
		return false;
	}

	// ����DBģ��
	ret = m_SQLDataManage.Start();
	if (!ret)
	{
		ERROR_LOG("SQLDataManage start failed");
		return false;
	}

	// ��������ģ��
	ret = m_TCPSocket.Start(SERVICE_TYPE_LOGON);
	if (!ret)
	{
		ERROR_LOG("TCPSocket Start failed");
		return false;
	}

	const CenterServerConfig & centerServerConfig = ConfigManage()->GetCenterServerConfig();
	ret = m_pTcpConnect->Start(&m_DataLine, centerServerConfig.ip, centerServerConfig.port, SERVICE_TYPE_LOGON, ConfigManage()->GetLogonServerConfig().logonID);		// TODO	
	if (!ret)
	{
		throw new CException(TEXT("BaseMainManageForZ::m_TCPSocket.Start ����ģ������ʧ��"), 0x433);
	}

	m_connectCServerHandle = (HANDLE)_beginthreadex(NULL, 0, TcpConnectThread, this, 0, NULL);
	if (!m_connectCServerHandle)
	{
		throw new CException(TEXT("BaseMainManageForZ::m_TCPSocket.Start �����̺߳�������ʧ��"), 0x434);
	}

	//���ýӿ�
	ret = OnStart();
	if (!ret)
	{
		ERROR_LOG("OnStart failed");
		return false;
	}

	INFO_LOG("BaseMainManageForZ Start end.");

	return true;
}

//ֹͣ����
bool CBaseLogonServer::Stop()
{
	INFO_LOG("BaseMainManageForZ Stop begin...");

	if (m_bRun == false)
	{
		ERROR_LOG("is not running...");
		return false;
	}

	m_bRun = false;

	m_DataLine.SetCompletionHandle(NULL);

	// �ȹر�����ģ��
	m_TCPSocket.Stop();

	//�ر������ķ�����������
	m_pTcpConnect->Stop();

	//�ر���ɶ˿�
	if (m_hCompletePort)
	{
		PostQueuedCompletionStatus(m_hCompletePort, 0, NULL, NULL);
		CloseHandle(m_hCompletePort);
		m_hCompletePort = NULL;
	}

	// �ر�linedate�߳̾��
	if (m_hHandleThread)
	{
		WaitForSingleObject(m_hHandleThread, INFINITE);
		CloseHandle(m_hHandleThread);
		m_hHandleThread = NULL;
	}

	// �ر����ķ������߳�
	if (m_connectCServerHandle)
	{
		//WaitForSingleObject(m_connectCServerHandle, INFINITE);
		CloseHandle(m_connectCServerHandle);
		m_connectCServerHandle = NULL;
	}

	// �ϲ�ӿ�
	OnStop();		// ���̹߳رղ�������Դ(������Ҫ����redis��db)

	// �ر�DBģ��
	m_SQLDataManage.Stop();

	// �ر�redis
	m_pRedis->Stop();
	m_pRedisPHP->Stop();

	//�رմ���
	if (m_hWindow != NULL && IsWindow(m_hWindow) == TRUE)
	{
		SendMessage(m_hWindow, WM_CLOSE, 0, 0);
	}

	// �ȴ������߳�
	if (m_hWindowThread)
	{
		WaitForSingleObject(m_hWindowThread, INFINITE);
		CloseHandle(m_hWindowThread);
		m_hWindowThread = NULL;
	}

	INFO_LOG("BaseMainManageForZ Stop end.");

	return true;
}

//����رմ���
bool CBaseLogonServer::OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, UINT uConnectTime)
{
	SocketCloseLine SocketClose;
	SocketClose.uConnectTime = uConnectTime;
	SocketClose.uIndex = uIndex;
	SocketClose.uAccessIP = uAccessIP;
	return (m_DataLine.AddData(&SocketClose.LineHead, sizeof(SocketClose), HD_SOCKET_CLOSE) != 0);
}

//������Ϣ����
bool CBaseLogonServer::OnSocketReadEvent(CTCPSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID)
{
	if (!pNetHead)
	{
		return false;
	}

	SocketReadLine SocketRead;

	SocketRead.uHandleSize = uSize;
	SocketRead.uIndex = uIndex;
	SocketRead.dwHandleID = dwHandleID;
	SocketRead.uAccessIP = 0;		//TODO
	SocketRead.NetMessageHead = *pNetHead;
	return m_DataLine.AddData(&SocketRead.LineHead, sizeof(SocketRead), HD_SOCKET_READ, pData, uSize) != 0;
}

//��ʱ��֪ͨ��Ϣ
bool CBaseLogonServer::WindowTimerMessage(UINT uTimerID)
{
	WindowTimerLine WindowTimer;
	WindowTimer.uTimerID = uTimerID;
	return (m_DataLine.AddData(&WindowTimer.LineHead, sizeof(WindowTimer), HD_TIMER_MESSAGE) != 0);
}

//�첽�߳̽������
bool CBaseLogonServer::OnAsynThreadResultEvent(UINT uHandleKind, UINT uHandleResult, void * pData, UINT uResultSize, UINT uDataType, UINT uHandleID)
{
	AsynThreadResultLine resultData;

	//��װ����
	resultData.LineHead.uSize = uResultSize;
	resultData.LineHead.uDataKind = uDataType;

	resultData.uHandleKind = uHandleKind;
	resultData.uHandleResult = uHandleResult;
	resultData.uHandleID = uHandleID;

	//�������
	return (m_DataLine.AddData(&resultData.LineHead, sizeof(resultData), HD_ASYN_THREAD_RESULT, pData, uResultSize) != 0);
}

//�趨��ʱ��
bool CBaseLogonServer::SetTimer(UINT uTimerID, UINT uElapse)
{
	if ((m_hWindow != NULL) && (IsWindow(m_hWindow) == TRUE))
	{
		::SetTimer(m_hWindow, uTimerID, uElapse, NULL);
		return true;
	}
	return false;
}

//�����ʱ��
bool CBaseLogonServer::KillTimer(UINT uTimerID)
{
	if ((m_hWindow != NULL) && (::IsWindow(m_hWindow) == TRUE))
	{
		::KillTimer(m_hWindow, uTimerID);
		return true;
	}
	return false;
}

//��������Ϊ�����ɶ�ʱ��
bool CBaseLogonServer::CreateWindowsForTimer()
{
	if ((m_hWindow != NULL) && (::IsWindow(m_hWindow) == TRUE))
	{
		return false;
	}

	//�����¼�
	HANDLE StartEvent = CreateEvent(FALSE, TRUE, NULL, NULL);

	//�����߳�
	WindowThreadStartStruct ThreadData;

	ThreadData.bSuccess = FALSE;
	ThreadData.pMainManage = this;
	ThreadData.hEvent = StartEvent;

	UINT uThreadID = 0;
	m_hWindowThread = (HANDLE)::_beginthreadex(NULL, 0, WindowMsgThread, &ThreadData, 0, &uThreadID);
	if (m_hWindowThread == NULL)
	{
		ERROR_LOG("_beginthreadex WindowMsgThread failed");
		return false;
	}
	WaitForSingleObject(ThreadData.hEvent, INFINITE);

	if (ThreadData.bSuccess == FALSE)
	{
		ERROR_LOG("ThreadData.bSuccess == FALSE");
		return false;
	}

	return true;
}

//�������ݴ����߳�
unsigned __stdcall CBaseLogonServer::LineDataHandleThread(LPVOID pThreadData)
{
	// �����߳�һ��ʱ��
	Sleep(1);

	INFO_LOG("LineDataHandleThread start...");

	HandleThreadStartStruct* pData = (HandleThreadStartStruct *)pThreadData;
	if (!pData)
	{
		return -1;
	}

	CBaseLogonServer	* pThis = pData->pMainManage;
	if (!pThis)
	{
		return -2;
	}

	CDataLine* pDataLine = &pThis->m_DataLine;
	if (!pDataLine)
	{
		return -3;
	}

	//��ɶ˿�
	HANDLE hCompletionPort = pData->hCompletionPort;
	if (!hCompletionPort)
	{
		return -4;
	}

	//���б�־
	bool& bRun = pThis->m_bRun;

	::SetEvent(pData->hEvent);

	// ��ʼ�����������
	srand((unsigned)time(NULL));

	// ��ʼ���쳣����
	CWin32Exception::SetWin32ExceptionFunc();

	//�ص�����
	DWORD dwThancferred = 0;
	ULONG dwCompleteKey = 0;
	LPOVERLAPPED OverData;

	//���ݻ���
	BYTE szBuffer[LD_MAX_PART] = "";
	DataLineHead* pDataLineHead = (DataLineHead *)szBuffer;

	while (pThis->m_bRun)
	{
		BOOL bSuccess = ::GetQueuedCompletionStatus(hCompletionPort, &dwThancferred, &dwCompleteKey, (LPOVERLAPPED *)&OverData, INFINITE);
		if (bSuccess == FALSE || dwThancferred == 0)
		{
			//ERROR_LOG("GetQueuedCompletionStatus failed err = %d", GetLastError());
			continue;
		}

		while (pDataLine->GetDataCount())
		{
			try
			{
				unsigned int bytes = pDataLine->GetData(pDataLineHead, sizeof(szBuffer));
				if (bytes == 0)
				{
					// ȡ���������ݴ�СΪ0����̫����
					ERROR_LOG("GetDataCount data size = 0");
					continue;
				}

				switch (pDataLineHead->uDataKind)
				{
				case HD_SOCKET_READ:		// socket ���ݶ�ȡ
				{
					SocketReadLine * pSocketRead = (SocketReadLine *)pDataLineHead;
					void* pBuffer = NULL;
					unsigned int size = pSocketRead->uHandleSize;
					if (size > 0)
					{
						pBuffer = (void *)(pSocketRead + 1);			// �ƶ�һ��SocketReadLine
					}

					if (!pThis->OnSocketRead(&pSocketRead->NetMessageHead, pBuffer, size, pSocketRead->uAccessIP, pSocketRead->uIndex, pSocketRead->dwHandleID))
					{
						ERROR_LOG("OnSocketRead failed mainID=%d assistID=%d", pSocketRead->NetMessageHead.uMainID, pSocketRead->NetMessageHead.uAssistantID);
						pThis->m_TCPSocket.OnSocketClose(pSocketRead->uIndex);
					}
					break;
				}
				case HD_SOCKET_CLOSE:		// socket �ر�
				{
					SocketCloseLine * pSocketClose = (SocketCloseLine *)pDataLineHead;
					pThis->OnSocketClose(pSocketClose->uAccessIP, pSocketClose->uIndex, pSocketClose->uConnectTime);
					break;
				}
				case HD_ASYN_THREAD_RESULT://�첽�̴߳�����
				{
					AsynThreadResultLine * pDataRead = (AsynThreadResultLine *)pDataLineHead;
					void* pBuffer = NULL;
					unsigned int size = pDataRead->LineHead.uSize;

					if (size < sizeof(AsynThreadResultLine))
					{
						ERROR_LOG("AsynThreadResultLine data error !!!");
						break;
					}

					if (size > sizeof(AsynThreadResultLine))
					{
						pBuffer = (void *)(pDataRead + 1);			// �ƶ�һ��SocketReadLine
					}

					pThis->OnAsynThreadResult(pDataRead, pBuffer, size - sizeof(AsynThreadResultLine));

					break;
				}
				case HD_TIMER_MESSAGE:		// ��ʱ����Ϣ
				{
					WindowTimerLine * pTimerMessage = (WindowTimerLine *)pDataLineHead;
					pThis->OnTimerMessage(pTimerMessage->uTimerID);
					break;
				}
				case HD_PLATFORM_SOCKET_READ:	// ���ķ���Ϣ
				{
					PlatformDataLineHead* pPlaformMessageHead = (PlatformDataLineHead*)pDataLineHead;
					int size = pPlaformMessageHead->platformMessageHead.MainHead.uMessageSize - sizeof(PlatformMessageHead);
					UINT msgID = pPlaformMessageHead->platformMessageHead.AssHead.msgID;
					int userID = pPlaformMessageHead->platformMessageHead.AssHead.userID;
					void * pBuffer = NULL;
					if (size > 0)
					{
						pBuffer = (void*)(pPlaformMessageHead + 1);
					}

					if (!pThis->OnCenterServerMessage(msgID, &pPlaformMessageHead->platformMessageHead.MainHead, pBuffer, size, userID))
					{
						ERROR_LOG("�������ķ���������,msgID=%d", msgID);
					}

					break;
				}
				default:
					ERROR_LOG("HD_PLATFORM_SOCKET_READ invalid linedata type, type=%d", pDataLineHead->uDataKind);
					break;
				}

			}

			catch (CWin32Exception& Win32Ex)
			{
				PEXCEPTION_POINTERS pEx = Win32Ex.ExceptionInformation();
				if (pEx != NULL && pEx->ExceptionRecord != NULL)
				{
					CWin32Exception::OutputWin32Exception("[ LogonServer ��ţ�0x%x ] [ ������%s] [ Դ����λ�ã�0x%x ]", pEx->ExceptionRecord->ExceptionCode,
						CWin32Exception::GetDescByCode(pEx->ExceptionRecord->ExceptionCode), ((pEx)->ExceptionRecord)->ExceptionAddress);
				}
				continue;
			}

			catch (int iCode)
			{
				CWin32Exception::OutputWin32Exception("[ LogonServer ��ţ�%d ] [ �����������dump�ļ�����鿴dump�ļ� ] [ Դ����λ�ã�δ֪ ]", iCode);
				continue;
			}

			catch (...)
			{
				CWin32Exception::OutputWin32Exception("#### δ֪������####");
				continue;
			}
		}
	}

	INFO_LOG("LineDataHandleThread exit��");

	return 0;
}

//WINDOW ��Ϣѭ���߳�
unsigned __stdcall CBaseLogonServer::WindowMsgThread(LPVOID pThreadData)
{
	WindowThreadStartStruct * pStartData = (WindowThreadStartStruct *)pThreadData;

	try
	{
		//ע�ᴰ����
		LOGBRUSH		LogBrush;
		WNDCLASS		WndClass;
		TCHAR			szClassName[] = TEXT("CMainManageWindow");

		LogBrush.lbColor = RGB(0, 0, 0);
		LogBrush.lbStyle = BS_SOLID;
		LogBrush.lbHatch = 0;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hCursor = NULL;
		WndClass.hIcon = NULL;
		WndClass.lpszMenuName = NULL;
		WndClass.lpfnWndProc = WindowProcFunc;
		WndClass.lpszClassName = szClassName;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		WndClass.hInstance = NULL;
		WndClass.hbrBackground = (HBRUSH)::CreateBrushIndirect(&LogBrush);
		::RegisterClass(&WndClass);

		//��������
		pStartData->pMainManage->m_hWindow = ::CreateWindow(szClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, pStartData->pMainManage);
		if (pStartData->pMainManage->m_hWindow == NULL) throw TEXT("���ڽ���ʧ��");
	}
	catch (...)
	{
		ERROR_LOG("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
		//��������
		pStartData->bSuccess = FALSE;
		::SetEvent(pStartData->hEvent);
		_endthreadex(0);
	}

	//�����ɹ�
	pStartData->bSuccess = TRUE;
	::SetEvent(pStartData->hEvent);

	//��Ϣѭ��
	MSG	Message;
	while (::GetMessage(&Message, NULL, 0, 0))
	{
		if (!::TranslateAccelerator(Message.hwnd, NULL, &Message))
		{
			::TranslateMessage(&Message);
			::DispatchMessage(&Message);
		}
	}

	return 0;
}

//���ڻص�����
LRESULT CALLBACK CBaseLogonServer::WindowProcFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:		//���ڽ�����Ϣ
	{
		DWORD iIndex = TlsAlloc();
		CBaseLogonServer * pMainManage = (CBaseLogonServer *)(((CREATESTRUCT *)lParam)->lpCreateParams);
		TlsSetValue(iIndex, pMainManage);
		::SetWindowLong(hWnd, GWL_USERDATA, iIndex);
		break;
	}
	case WM_TIMER:		//��ʱ����Ϣ
	{
		DWORD iIndex = ::GetWindowLong(hWnd, GWL_USERDATA);
		CBaseLogonServer * pMainManage = (CBaseLogonServer *)::TlsGetValue(iIndex);
		if ((pMainManage != NULL) && (pMainManage->WindowTimerMessage((UINT)wParam) == false)) ::KillTimer(hWnd, (UINT)wParam);
		break;
	}
	case WM_CLOSE:		//���ڹر���Ϣ
	{
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY:	//���ڹر���Ϣ
	{
		DWORD iIndex = ::GetWindowLong(hWnd, GWL_USERDATA);
		CBaseLogonServer * pMainManage = (CBaseLogonServer *)::TlsGetValue(iIndex);
		if (pMainManage != NULL) pMainManage->m_hWindow = NULL;
		::TlsFree(iIndex);
		PostQuitMessage(0);
		break;
	}
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

unsigned CBaseLogonServer::TcpConnectThread(LPVOID pThreadData)
{
	CBaseLogonServer* pThis = (CBaseLogonServer*)pThreadData;
	if (!pThis)
	{
		return -1;
	}

	CTcpConnect* pTcpConnect = (CTcpConnect*)pThis->m_pTcpConnect;
	if (!pTcpConnect)
	{
		return -2;
	}

	while (pThis->m_bRun && pTcpConnect)
	{
		pTcpConnect->EventLoop();
		pTcpConnect->CheckConnection();
	}

	return 0;
}