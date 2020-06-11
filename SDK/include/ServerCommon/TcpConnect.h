#pragma once

#include "TCPSocket.h"
#include <vector>

#define CLIENT_SOCKET_SEND_BUF	(64 * 1024)		// ���ͻ�������С
#define CLIENT_SOCKET_RECV_BUF  (32 * 1024)		// ���ջ�������С

class CDataLine;
class CTcpConnect;
class  KERNEL_CLASS CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

public:
	bool SetDataLine(CDataLine* pDataLine);
	bool SetTcpConnect(CTcpConnect* pCTcpConnect);
	void SetIndex(int index) { m_index = index; }
	int GetSocketIdx();

	// �Ƿ���Ч
	inline bool Enable();
	bool OnRead();

	bool Send(const void* pData, int size);

	bool OnClose();

	void SetSocket(SOCKET sock) { m_socket = sock; }
	void SetConnection(bool isConneted) { m_isConnected = true; }

	bool IsConnected() { return m_isConnected; }

	SOCKET GetSocket() { return m_socket; }
	int GetRemainSendBufSize() { return m_remainSendBytes; }

	// ��շ���������(��ʼ��������������һ���������)
	void Clear();
private:
	int m_index;
	SOCKET m_socket;
	bool m_isConnected;

	char m_recvBuf[CLIENT_SOCKET_RECV_BUF];
	int  m_remainRecvBytes;

	char m_sendBuf[CLIENT_SOCKET_SEND_BUF];
	int m_remainSendBytes;

	CDataLine* m_pDataLine;			//�����dataline����
	CSignedLock m_lock;
	CTcpConnect* m_pTcpConnect;		//���������
};

class KERNEL_CLASS CTcpConnect
{
public:
	CTcpConnect();
	~CTcpConnect();

	bool Start(CDataLine* pDataLine, const char* ip, int port, int serverType, int serverID);
	bool Stop();

	bool Connect();
	bool Send(UINT msgID, const void* pData, UINT size, int userID = 0, NetMessageHead * pNetHead = NULL);

	bool EventLoop();
	bool CheckConnection();

	bool ReStart();
private:
	char m_ip[24];
	int m_port;
	CTcpClient m_tcpClient;
	CDataLine* m_pDataLine;
	int m_connectServerID;
	int m_connectServerType;
	volatile bool m_bStart;
};