#include "pch.h"
#include "BaseMainManage.h"
#include "Function.h"
#include "Exception.h"
#include "commonuse.h"
#include "GameLogManage.h"
#include "InternalMessageDefine.h"
#include "log.h"
#include "PlatformMessage.h"
#include "ConfigManage.h"
#include "Util.h"

#pragma warning (disable: 4355)

//�����߳������ṹ
struct WindowThreadStartStruct
{
	//��������
	HANDLE								hEvent;						//�����¼�
	BOOL								bSuccess;					//�����ɹ���־
	CBaseMainManage						* pMainManage;				//���ݹ���ָ��
};

//�����߳������ṹ
struct HandleThreadStartStruct
{
	//��������
	HANDLE								hEvent;						//�����¼�
	HANDLE								hCompletionPort;			//��ɶ˿�
	CBaseMainManage						* pMainManage;				//���ݹ���ָ��
};

//���캯��
CBaseMainManage::CBaseMainManage()
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
	m_pGServerConnect = NULL;
}

CBaseMainManage::~CBaseMainManage()
{
	SAFE_DELETE(m_pRedis);
	SAFE_DELETE(m_pRedisPHP);
	SAFE_DELETE(m_pTcpConnect);
	SAFE_DELETE(m_pGServerConnect);
}

bool CBaseMainManage::Init(ManageInfoStruct * pInitData, IDataBaseHandleService * pDataHandleService)
{
	if (!pInitData || !pDataHandleService)
	{
		ERROR_LOG("invalid input params pInitData=%p pDataHandleService=%p", pInitData, pDataHandleService);
		return false;
	}

	if (m_bInit)
	{
		ERROR_LOG("already inited");
		return false;
	}

	m_InitData = *pInitData;

	bool ret = false;

	ret = PreInitParameter(&m_InitData, &m_KernelData);
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::Init PreInitParameter �������ڴ���"), 0x41A);
	}

	m_pRedis = new CRedisLoader;
	if (!m_pRedis)
	{
		return false;
	}

	ret = m_pRedis->Init();
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::Init redis��ʼ��ʧ�ܣ�������redis������δ������"), 0x401);
	}

	//vip�������Ҫ����php redis
	if (m_InitData.iRoomType != ROOM_TYPE_GOLD)
	{
		m_pRedisPHP = new CRedisPHP;
		if (!m_pRedisPHP)
		{
			return false;
		}

		ret = m_pRedisPHP->Init();
		if (!ret)
		{
			throw new CException(TEXT("CBaseMainManage::Init redisPHP��ʼ��ʧ�ܣ�������redis PHP������δ������"), 0x402);
		}
	}

	ret = pDataHandleService->SetParameter(this, &m_SQLDataManage, &m_InitData, &m_KernelData);
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::Init pDataHandleService->SetParameterʧ��"), 0x41C);
	}

	ret = m_SQLDataManage.Init(&m_InitData, &m_KernelData, pDataHandleService, this);
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::Init m_SQLDataManage ��ʼ��ʧ��"), 0x41D);
	}

	m_pTcpConnect = new CTcpConnect;
	if (!m_pTcpConnect)
	{
		throw new CException(TEXT("CBaseMainManage::Init new CTcpConnect failed"), 0x43A);
	}

	m_pGServerConnect = new CGServerConnect;
	if (!m_pGServerConnect)
	{
		throw new CException(TEXT("CBaseMainManage::Init new CGServerConnect failed"), 0x43A);
	}

	ret = OnInit(&m_InitData, &m_KernelData);
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::Init OnInit ��������"), 0x41B);
	}

	m_bInit = true;

	return true;
}

//ȡ����ʼ������ 
bool CBaseMainManage::UnInit()
{
	//ֹͣ����
	if (m_bRun)
	{
		Stop();
	}

	//���ýӿ�
	OnUnInit();

	//ȡ����ʼ��
	m_bInit = false;
	m_SQLDataManage.UnInit();

	//��������
	memset(&m_DllInfo, 0, sizeof(m_DllInfo));
	memset(&m_InitData, 0, sizeof(m_InitData));
	memset(&m_KernelData, 0, sizeof(m_KernelData));

	//�ͷ�redis
	m_pRedis->Stop();
	SAFE_DELETE(m_pRedis);
	if (m_pRedisPHP)
	{
		m_pRedisPHP->Stop();
		SAFE_DELETE(m_pRedisPHP);
	}

	SAFE_DELETE(m_pTcpConnect);
	SAFE_DELETE(m_pGServerConnect);

	return true;
}

//��������
bool CBaseMainManage::Start()
{
	if (!m_bInit || m_bRun)
	{
		return false;
	}

	m_bRun = true;

	//�����¼�
	HANDLE StartEvent = CreateEvent(FALSE, TRUE, NULL, NULL);

	//������ɶ˿�
	m_hCompletePort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (m_hCompletePort == NULL)
	{
		throw new CException(TEXT("CBaseMainManage::Start m_hCompletePort ����ʧ��"), 0x41D);
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
		throw new CException(TEXT("CBaseMainManage::Start LineDataHandleThread �߳�����ʧ��"), 0x41E);
	}
	// ������Ϸҵ���߼��߳����Ӧ��־�ļ�
	GameLogManage()->AddLogFile(uThreadID, THREAD_TYPE_LOGIC, m_InitData.uRoomID);

	WaitForSingleObject(StartEvent, INFINITE);

	//��������Ϊ�����ɶ�ʱ��
	bool ret = CreateWindowsForTimer();
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::CreateWindowsForTimer �������ʧ��"), 0x41F);
	}

	ret = m_SQLDataManage.Start();
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::m_SQLDataManage.Start ���ݿ�ģ������ʧ��"), 0x420);
	}

	//////////////////////////////////���������ķ�������////////////////////////////////////////
	const CenterServerConfig & centerServerConfig = ConfigManage()->GetCenterServerConfig();
	ret = m_pTcpConnect->Start(&m_DataLine, centerServerConfig.ip, centerServerConfig.port, SERVICE_TYPE_LOADER, m_InitData.uRoomID);
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::m_pTcpConnect.Start ����ģ������ʧ��"), 0x433);
	}

	m_connectCServerHandle = (HANDLE)_beginthreadex(NULL, 0, TcpConnectThread, this, 0, NULL);
	if (!m_connectCServerHandle)
	{
		throw new CException(TEXT("CBaseMainManage::m_pTcpConnect.Start �����̺߳�������ʧ��"), 0x434);
	}

	//////////////////////////////////�������¼��������////////////////////////////////////////
	ret = m_pGServerConnect->Start(&m_DataLine, m_InitData.uRoomID);
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::m_pGServerConnect.Start ����ģ������ʧ��"), 0x433);
	}

	//////////////////////////////////////////////////////////////////////////

	ret = OnStart();
	if (!ret)
	{
		throw new CException(TEXT("CBaseMainManage::Start OnStart ��������"), 0x422);
	}

	return true;
}

//ֹͣ����
bool CBaseMainManage::Stop()
{
	if (m_bRun == false)
	{
		return true;
	}

	OnStop();

	m_bRun = false;

	m_DataLine.SetCompletionHandle(NULL);

	m_SQLDataManage.Stop();

	if (m_pTcpConnect)
	{
		m_pTcpConnect->Stop();
	}

	if (m_pGServerConnect)
	{
		m_pGServerConnect->Stop();
	}

	//�رմ���
	if ((m_hWindow != NULL) && (::IsWindow(m_hWindow) == TRUE))
	{
		::SendMessage(m_hWindow, WM_CLOSE, 0, 0);
	}

	//�ر���ɶ˿�
	if (m_hCompletePort != NULL)
	{
		::PostQueuedCompletionStatus(m_hCompletePort, 0, NULL, NULL);
		::CloseHandle(m_hCompletePort);
		m_hCompletePort = NULL;
	}

	// �ر����ķ������߳�
	if ((m_connectCServerHandle != NULL) && (::WaitForSingleObject(m_connectCServerHandle, 50) == WAIT_TIMEOUT))
	{
		TerminateThread(m_connectCServerHandle, 0);
		CloseHandle(m_connectCServerHandle);
		m_connectCServerHandle = NULL;
	}

	//�˳������߳�
	if ((m_hWindowThread != NULL) && (::WaitForSingleObject(m_hWindowThread, 3000) == WAIT_TIMEOUT))
	{
		TerminateThread(m_hWindowThread, 0);
		CloseHandle(m_hWindowThread);
		m_hWindowThread = NULL;
	}

	//�˳������߳�
	if ((m_hHandleThread != NULL) && (::WaitForSingleObject(m_hHandleThread, 3000) == WAIT_TIMEOUT))
	{
		TerminateThread(m_hHandleThread, 0);
		CloseHandle(m_hHandleThread);
		m_hHandleThread = NULL;
	}

	//�����������
	m_DataLine.CleanLineData();

	return true;
}

//ˢ�·���
bool CBaseMainManage::Update()
{
	return OnUpdate();
}

//����رմ���
bool CBaseMainManage::OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, UINT uConnectTime)
{
	SocketCloseLine SocketClose;
	SocketClose.uConnectTime = uConnectTime;
	SocketClose.uIndex = uIndex;
	SocketClose.uAccessIP = uAccessIP;
	return (m_DataLine.AddData(&SocketClose.LineHead, sizeof(SocketClose), HD_SOCKET_CLOSE) != 0);
}

//������Ϣ����
bool CBaseMainManage::OnSocketReadEvent(CTCPSocket* pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID)
{
	SocketReadLine SocketRead;
	SocketRead.uHandleSize = uSize;
	SocketRead.uIndex = uIndex;
	SocketRead.dwHandleID = dwHandleID;
	SocketRead.uAccessIP = 0;		//todo
	SocketRead.NetMessageHead = *pNetHead;
	return (m_DataLine.AddData(&SocketRead.LineHead, sizeof(SocketRead), HD_SOCKET_READ, pData, uSize) != 0);
}

//��ʱ��֪ͨ��Ϣ
bool CBaseMainManage::WindowTimerMessage(UINT uTimerID)
{
	WindowTimerLine WindowTimer;
	WindowTimer.uTimerID = uTimerID;
	return (m_DataLine.AddData(&WindowTimer.LineHead, sizeof(WindowTimer), HD_TIMER_MESSAGE) != 0);
}

//�첽�߳̽������
bool CBaseMainManage::OnAsynThreadResultEvent(UINT uHandleKind, UINT uHandleResult, void * pData, UINT uResultSize, UINT uDataType, UINT uHandleID)
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
bool CBaseMainManage::SetTimer(UINT uTimerID, UINT uElapse)
{
	if ((m_hWindow != NULL) && (IsWindow(m_hWindow) == TRUE))
	{
		::SetTimer(m_hWindow, uTimerID, uElapse, NULL);
		return true;
	}

	return false;
}

//�����ʱ��
bool CBaseMainManage::KillTimer(UINT uTimerID)
{
	if ((m_hWindow != NULL) && (::IsWindow(m_hWindow) == TRUE))
	{
		::KillTimer(m_hWindow, uTimerID);
		return true;
	}
	return false;
}

//��������Ϊ�����ɶ�ʱ��
bool CBaseMainManage::CreateWindowsForTimer()
{
	if ((m_hWindow != NULL) && (::IsWindow(m_hWindow) == TRUE)) return false;
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
		throw new CException(TEXT("CBaseMainManage::CreateWindowsForTimer WindowMsgThread �߳̽���ʧ��"), 0x421);
	}

	::WaitForSingleObject(ThreadData.hEvent, INFINITE);
	if (ThreadData.bSuccess == FALSE)
	{
		throw new CException(TEXT("CBaseMainManage::CreateWindowsForTimer WindowMsgThread �߳̽���ʧ��"), 0x422);
	}

	return true;
}

//�������ݴ����߳�
unsigned __stdcall CBaseMainManage::LineDataHandleThread(LPVOID pThreadData)
{
	// ��ʼ�����������
	srand((unsigned)time(NULL));

	//���ݶ���
	HandleThreadStartStruct		* pData = (HandleThreadStartStruct *)pThreadData;		//�߳���������ָ��
	CBaseMainManage				* pMainManage = pData->pMainManage;						//���ݹ���ָ��
	CDataLine					* m_pDataLine = &pMainManage->m_DataLine;				//���ݶ���ָ��
	HANDLE						hCompletionPort = pData->hCompletionPort;				//��ɶ˿�
	bool						& bRun = pMainManage->m_bRun;							//���б�־

	//�߳����ݶ�ȡ���
	::SetEvent(pData->hEvent);

	//�ص�����
	void						* pIOData = NULL;										//����
	DWORD						dwThancferred = 0;										//��������
	ULONG						dwCompleteKey = 0L;										//�ص� IO ��ʱ����
	LPOVERLAPPED				OverData;												//�ص� IO ��ʱ����
	//���ݻ���
	BOOL						bSuccess = FALSE;
	BYTE						szBuffer[LD_MAX_PART];
	DataLineHead				* pDataLineHead = (DataLineHead *)szBuffer;

	// ��ʼ���쳣����
	CWin32Exception::SetWin32ExceptionFunc();

	while (pMainManage->m_bRun == true)
	{
		//�ȴ���ɶ˿�
		bSuccess = ::GetQueuedCompletionStatus(hCompletionPort, &dwThancferred, &dwCompleteKey, (LPOVERLAPPED *)&OverData, INFINITE);
		if (bSuccess == FALSE || dwThancferred == 0)
		{
			continue;
		}

		while (m_pDataLine->GetDataCount())
		{
			try
			{
				unsigned int size = m_pDataLine->GetData(pDataLineHead, sizeof(szBuffer));
				if (size == 0)
				{
					continue;
				}

				switch (pDataLineHead->uDataKind)
				{
				case HD_SOCKET_READ://SOCKET���ݶ�ȡ
				{
					SocketReadLine * pSocketRead = (SocketReadLine *)pDataLineHead;
					if (pMainManage->OnSocketRead(&pSocketRead->NetMessageHead,
						pSocketRead->uHandleSize ? pSocketRead + 1 : NULL,
						pSocketRead->uHandleSize, pSocketRead->uAccessIP,
						pSocketRead->uIndex, pSocketRead->dwHandleID) == false)
					{
						//ERROR_LOG("OnSocketRead failed mainID=%d assistID=%d", pSocketRead->NetMessageHead.uMainID, pSocketRead->NetMessageHead.uAssistantID);
					}
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

					pMainManage->OnAsynThreadResult(pDataRead, pBuffer, size - sizeof(AsynThreadResultLine));

					break;
				}
				case HD_TIMER_MESSAGE://��ʱ����Ϣ
				{
					WindowTimerLine * pTimerMessage = (WindowTimerLine *)pDataLineHead;
					pMainManage->OnTimerMessage(pTimerMessage->uTimerID);
					break;
				}
				case HD_PLATFORM_SOCKET_READ:	// ���ķ���������������Ϣ
				{
					PlatformDataLineHead* pPlaformMessageHead = (PlatformDataLineHead*)pDataLineHead;
					int sizeCenterMsg = pPlaformMessageHead->platformMessageHead.MainHead.uMessageSize - sizeof(PlatformMessageHead);
					UINT msgID = pPlaformMessageHead->platformMessageHead.AssHead.msgID;
					int userID = pPlaformMessageHead->platformMessageHead.AssHead.userID;
					void * pBuffer = NULL;
					if (sizeCenterMsg > 0)
					{
						pBuffer = (void*)(pPlaformMessageHead + 1);
					}

					pMainManage->OnCenterServerMessage(msgID, &pPlaformMessageHead->platformMessageHead.MainHead, pBuffer, sizeCenterMsg, userID);
					break;
				}
				default:
					break;
				}
			}

			catch (CWin32Exception& Win32Ex)
			{
				PEXCEPTION_POINTERS pEx = Win32Ex.ExceptionInformation();
				if (pEx != NULL && pEx->ExceptionRecord != NULL)
				{
					CWin32Exception::OutputWin32Exception("[ LoaderServer ��ţ�0x%x ] [ ������%s] [ Դ����λ�ã�0x%x ]", pEx->ExceptionRecord->ExceptionCode,
						CWin32Exception::GetDescByCode(pEx->ExceptionRecord->ExceptionCode), ((pEx)->ExceptionRecord)->ExceptionAddress);
				}
				continue;
			}

			catch (int iCode)
			{
				CWin32Exception::OutputWin32Exception("[ LoaderServer ��ţ�%d ] [ �����������dump�ļ�����鿴dump�ļ� ] [ Դ����λ�ã�δ֪ ]", iCode);
				continue;
			}

			catch (...)
			{
				CWin32Exception::OutputWin32Exception("#### δ֪������####");
				continue;
			}
		}
	}

	//INFO_LOG("THREAD::thread LineDataHandleThread exit!!!");

	return 0;
}

//WINDOW��Ϣѭ���߳�
unsigned __stdcall CBaseMainManage::WindowMsgThread(LPVOID pThreadData)
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
	while (GetMessage(&Message, NULL, 0, 0))
	{
		if (!TranslateAccelerator(Message.hwnd, NULL, &Message))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	//INFO_LOG("THREAD::thread WindowMsgThread exit!!!");

	return 0;
}

//���ڻص�����
LRESULT CALLBACK CBaseMainManage::WindowProcFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:		//���ڽ�����Ϣ
	{
		DWORD iIndex = TlsAlloc();
		CBaseMainManage * pMainManage = (CBaseMainManage *)(((CREATESTRUCT *)lParam)->lpCreateParams);
		TlsSetValue(iIndex, pMainManage);
		::SetWindowLong(hWnd, GWL_USERDATA, iIndex);
		break;
	}
	case WM_TIMER:		//��ʱ����Ϣ
	{
		DWORD iIndex = ::GetWindowLong(hWnd, GWL_USERDATA);
		CBaseMainManage * pMainManage = (CBaseMainManage *)::TlsGetValue(iIndex);
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
		CBaseMainManage * pMainManage = (CBaseMainManage *)::TlsGetValue(iIndex);
		if (pMainManage != NULL) pMainManage->m_hWindow = NULL;
		::TlsFree(iIndex);
		PostQuitMessage(0);
		break;
	}
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// ���ķ������߳�
unsigned CBaseMainManage::TcpConnectThread(LPVOID pThreadData)
{
	CBaseMainManage* pThis = (CBaseMainManage*)pThreadData;
	if (!pThis)
	{
		return -1;
	}

	CTcpConnect* pTcpConnect = (CTcpConnect*)pThis->m_pTcpConnect;
	if (!pTcpConnect)
	{
		return -2;
	}

	while (pThis->m_bRun && pThis->m_pTcpConnect)
	{
		pTcpConnect->EventLoop();
		pTcpConnect->CheckConnection();
	}

	return 0;
}