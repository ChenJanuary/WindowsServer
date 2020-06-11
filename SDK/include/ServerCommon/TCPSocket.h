#pragma once

#include "Lock.h"
#include "Interface.h"
#include "BaseMessage.h"
#include <CommCtrl.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

// ��������
const int SOCKET_SEND_BUF_SIZE = 128 * 1024;	// TCP���ͻ�������С
const int SOCKET_RECV_BUF_SIZE = 64 * 1024;		// TCP���ջ�������С

// socket�¼�����
enum SocketEventType
{
	SOCKET_EVENT_TYPE_RECV = 1,		// �����¼�
	SOCKET_EVENT_TYPE_SEND = 2,		// �����¼�
};

//�Զ����ص��ṹ
struct OverLappedStruct
{
	OVERLAPPED	overLapped;		//�ص��ṹ
	WSABUF		WSABuffer;		//���ݻ���
	int			type;			//����
	int			handleID;		//����ID

	OverLappedStruct()
	{
		Clear();
	}

	void Clear()
	{
		memset(this, 0, sizeof(OverLappedStruct));
	}
};

//////////////////////////////////////////////////////////////////////
class KERNEL_CLASS CListenSocket
{
public:
	CListenSocket();
	~CListenSocket();

public:
	bool Init(int port, const char *ip = NULL);
	bool CloseSocket();
	SOCKET GetSocket() { return m_hSocket; }

protected:
	SOCKET	m_hSocket;
};

//////////////////////////////////////////////////////////////////////
class CTCPSocketManage;
class CDataLine;
class KERNEL_CLASS CTCPSocket
{
public:
	CTCPSocket();
	~CTCPSocket();

public:
	// ��ʼ��(��������ʱ���̵߳���)
	bool Init(int index, CTCPSocketManage* pManage);
	// ��շ���������(��ʼ��������������һ���������)
	void Clear();
	// ����������(���������̵߳���)
	bool AcceptNewConnection(SOCKET sock, unsigned int handleID, HANDLE hCompletePort, unsigned long ip);
	// Ͷ��recv����
	bool PostRecv();
	// Ͷ��send����
	bool PostSend();

public:
	// ��������(ҵ���߼��̵߳���)
	bool SendData(const char* pData, int size);
	// �Ƿ�����
	inline bool IsConnect();
	//������ɺ���(�շ��̵߳���)
	bool OnSendCompleted(unsigned int handleID, int sendedBytes);
	//������ɺ���(�շ��̵߳���)
	bool OnRecvCompleted(unsigned int handleID, int recvedBytes);
	// �ر�����(ֻ��ҵ���߼��̵߳���)
	bool CloseSocket();

private:
	// �ɷ����ݰ�(�շ��̵߳���)
	bool DispatchPacket(NetMessageHead* pHead, void* pData, int size);

public:
	// ��ȡ�����ʶ
	inline unsigned int GetHandleID() { return m_handleID; }
	// ��ȡ������������
	inline int GetIndex() { return m_index; }
	// ��ȡ�ϴη�����Ϣʱ��
	inline time_t GetLastRecvMsgTime() { return m_lastRecvMsgTime; }
	// ��ȡ�������ӵ�ʱ��
	inline time_t GetAcceptMsgTime() { return m_acceptMsgTime; }
	// ��ȡ��
	inline CSignedLock* GetLock() { return &m_lock; }
	// ��ȡIP
	inline const char* GetIP() { return m_ip; }

private:
	// ��������(�������ʱ��ȷ��)
	CTCPSocketManage*	m_pManage;			// �������
	int					m_index;			// ������������

	// ����������
	SOCKET				m_socket;
	unsigned int		m_handleID;			// ����ID
	char				m_ip[24];			// �ͻ����Լ���ip

	char				m_recvBuf[SOCKET_RECV_BUF_SIZE];
	int					m_recvBufLen;

	char				m_sendBuf[SOCKET_SEND_BUF_SIZE];
	int					m_sendBufLen;
	bool				m_bSending;							//�Ƿ�����

	OverLappedStruct	m_recvOverLapped;	// �����ص��ṹ
	OverLappedStruct	m_sendOverLapped;	// �����ص��ṹ

	volatile time_t		m_lastRecvMsgTime;	// �ϴη�����Ϣ��ʱ��(����������)
	volatile time_t		m_acceptMsgTime;	// �������ӵ�ʱ��

	CSignedLock		m_lock;
};


/**************************************************************************************************************/
// TCP SOCKET ���粿�� 
/**************************************************************************************************************/
//��˵��
class CTCPSocket;
class CTCPSocketManage;
/**************************************************************************************************************/
#define NO_CONNECT				0								//û������
#define CONNECTING				1								//��������
#define CONNECTED				2								//�ɹ�����

/**************************************************************************************************************/
//CloseSocket��OnSocketClose����
//1������CloseSocket�޷�ִ�е�CCenterServerManage::OnSocketClose�߼���
//2��OnSocketClose���ܻ�ִ������CCenterServerManage::OnSocketClose�߼������ǻᱣ֤�ر�socket֮ǰ����Ϣ���ͳ�ȥ
class KERNEL_CLASS CTCPSocketManage
{
public:
	CTCPSocketManage();
	~CTCPSocketManage();

public:
	// ��ʼ��
	bool Init(IServerSocketService * pService, int maxCount, int port, int secretKey, const char *ip = NULL);
	// ȡ����ʼ��
	virtual bool UnInit();
	// ��ʼ����
	virtual bool Start(int serverType);
	// ֹͣ����
	virtual bool Stop();

public:
	// �������ݺ���
	bool SendData(int idx, void* pData, int size, int mainID, int assistID, int handleCode, unsigned int handleID, unsigned int uIdentification = 0);
	// ���ķ�������������
	bool CenterServerSendData(int idx, UINT msgID, void* pData, int size, int mainID, int assistID, int handleCode, int userID);
	// �������ͺ���
	int SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT bAssistantID, UINT uHandleCode);
	// �ر�����(ҵ���߼��̵߳���)
	bool CloseSocket(int index);
	// ���չر������¼���
	bool OnSocketClose(int index);
	// �������
	void CheckHeartBeat(time_t llLastSendHeartBeatTime, int iHeartBeatTime);
	// ��ȡdataline
	CDataLine* GetDataLine();
	// ��ȡ����ip
	const char* GetSocketIP(int socketIdx);
	// ��ȡCTCPSocket����
	void GetTCPSocketInfo(UINT uIndex, bool &bIsConnect, time_t &llAcceptMsgTime, time_t &llLastRecvMsgTime);
private:
	// ����һ�����õ�socket(�̻߳�����ThreadAccept)
	CTCPSocket * TCPSocketNew();
	// ��ȡ����ID(�̻߳�����ThreadAccept)
	static unsigned int GetHandleID();

	// �̺߳���
private:
	// SOCKET ����Ӧ���߳�
	static unsigned __stdcall ThreadAccept(LPVOID pThreadData);
	// SOCKET ���ݽ����߳�
	static unsigned __stdcall ThreadRSSocket(LPVOID pThreadData);

private:
	CListenSocket				m_listenSocket;
	IServerSocketService*		m_pService;
	std::vector<CTCPSocket*>	m_socketVec;
	UINT	m_uRSThreadCount;
	HANDLE	m_hEventThread;
	HANDLE	m_hCompletionPortRS;
	HANDLE	m_hThreadAccept;
	volatile bool	m_running;
	UINT	m_uMaxSocket;

public:
	unsigned int	m_iServiceType;
};
