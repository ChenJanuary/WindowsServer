#include "pch.h"
#include "TcpConnect.h"
#include "InternalMessageDefine.h"
#include "log.h"

CTcpClient::CTcpClient()
{
	m_pDataLine = NULL;			//�����dataline����
	m_pTcpConnect = NULL;
	Clear();
}

CTcpClient::~CTcpClient()
{

}

bool CTcpClient::SetDataLine(CDataLine* pDataLine)
{
	if (!pDataLine)
	{
		return false;
	}

	m_pDataLine = pDataLine;

	return true;
}

bool CTcpClient::SetTcpConnect(CTcpConnect* pCTcpConnect)
{
	if (!pCTcpConnect)
	{
		return false;
	}

	m_pTcpConnect = pCTcpConnect;

	return true;
}

int CTcpClient::GetSocketIdx()
{
	return m_index;
}

inline bool CTcpClient::Enable()
{
	if (m_socket != INVALID_SOCKET && m_isConnected == true)
	{
		return true;
	}

	return false;
}

bool CTcpClient::OnRead()
{
	if (m_isConnected == false || m_socket == INVALID_SOCKET)
	{
		return false;
	}

	if (!m_pDataLine)
	{
		return false;
	}

	// ��������
	int recvBytes = recv(m_socket, m_recvBuf + m_remainRecvBytes, sizeof(m_recvBuf) - m_remainRecvBytes, 0);
	if (recvBytes <= 0)
	{
		return OnClose();
	}

	m_remainRecvBytes += recvBytes;

	PlatformMessageHead* pHead = (PlatformMessageHead*)m_recvBuf;
	if (!pHead)
	{
		// ���ﲻ̫����
		return false;
	}

	// ��������
	while (m_remainRecvBytes >= sizeof(PlatformMessageHead) && m_remainRecvBytes >= (int)pHead->MainHead.uMessageSize)
	{
		UINT messageSize = pHead->MainHead.uMessageSize;
		int realSize = messageSize - sizeof(PlatformMessageHead);
		void* pData = NULL;
		if (realSize < 0)
		{
			ERROR_LOG("��ͷ�ֽڴ�С�����Ѷ�����%d�ֽڣ��յ����ݣ�%d�ֽ�", m_remainRecvBytes, recvBytes);

			// ����
			m_remainRecvBytes = 0;
			return false;
		}

		if (realSize > 0)
		{
			// û���ݾ�ΪNULL
			pData = (void*)(m_recvBuf + sizeof(PlatformMessageHead));
		}

		if (pHead->MainHead.uMainID != MDM_CONNECT)	// ���˵����Ӳ�����Ϣ
		{
			PlatformDataLineHead dataLineHead;
			dataLineHead.platformMessageHead = *pHead;
			dataLineHead.socketIdx = m_index;
			m_pDataLine->AddData(&dataLineHead.lineHead, sizeof(PlatformDataLineHead), HD_PLATFORM_SOCKET_READ, pData, realSize);
		}

		MoveMemory(m_recvBuf, m_recvBuf + messageSize, m_remainRecvBytes - messageSize);
		m_remainRecvBytes -= messageSize;
	}

	return true;
}

bool CTcpClient::Send(const void* pData, int size)
{
	CSignedLockObject lock(&m_lock, true);

	if (!pData || size <= 0)
	{
		return false;
	}

	// ����Ƿ�����������
	int bytes = 0;
	while (m_remainSendBytes > 0)
	{
		bytes = send(m_socket, m_sendBuf, m_remainSendBytes, 0);
		if (bytes == 0)
		{
			return false;
		}

		if (bytes < 0)
		{
			break;
		}

		memmove(m_sendBuf, m_sendBuf + bytes, m_remainSendBytes - bytes);
		m_remainSendBytes -= bytes;
	}

	if (m_remainSendBytes + size >= sizeof(m_sendBuf))
	{
		return false;
	}

	memcpy(m_sendBuf + m_remainSendBytes, pData, size);
	m_remainSendBytes += size;

	// ��ֻ����һ�ΰ�
	bytes = send(m_socket, m_sendBuf, m_remainSendBytes, 0);
	if (bytes > 0)
	{
		memmove(m_sendBuf, m_sendBuf + bytes, m_remainSendBytes - bytes);
		m_remainSendBytes -= bytes;
	}

	return true;
}

bool CTcpClient::OnClose()
{
	CSignedLockObject lock(&m_lock, true);

	closesocket(m_socket);

	Clear();

	return true;
}

void CTcpClient::Clear()
{
	m_socket = INVALID_SOCKET;
	m_isConnected = false;

	memset(m_recvBuf, 0, sizeof(m_recvBuf));
	m_remainRecvBytes = 0;

	memset(m_sendBuf, 0, sizeof(m_sendBuf));
	m_remainSendBytes = 0;
}


//////////////////////////////////////////////////////////////////////////
//CTcpConnect
CTcpConnect::CTcpConnect()
{
	m_ip[0] = '\0';
	m_port = 0;
	m_pDataLine = NULL;
	m_connectServerID = 0;
	m_connectServerType = 0;
	m_bStart = false;
}

CTcpConnect::~CTcpConnect()
{

}

bool CTcpConnect::Start(CDataLine* pDataLine, const char* ip, int port, int serverType, int serverID)
{
	if (!pDataLine || !ip || port <= 0)
	{
		return false;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	strcpy(m_ip, ip);
	m_port = port;

	m_tcpClient.SetSocket(sock);
	m_tcpClient.SetDataLine(pDataLine);
	m_tcpClient.SetTcpConnect(this);
	m_pDataLine = pDataLine;
	m_connectServerID = serverID;
	m_connectServerType = serverType;
	m_bStart = true;

	Connect();

	return true;
}

bool CTcpConnect::Stop()
{
	m_bStart = false;
	m_tcpClient.OnClose();

	return true;
}

bool CTcpConnect::Connect()
{
	if (!m_bStart)
	{
		return false;
	}
	SOCKET sock = m_tcpClient.GetSocket();
	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	sockaddr_in svrAddr;

	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.s_addr = inet_addr(m_ip);
	svrAddr.sin_port = htons(m_port);

	int ret = connect(sock, (sockaddr*)& svrAddr, sizeof(sockaddr_in));
	if (ret == 0)
	{
		m_tcpClient.SetConnection(true);

		//������֤��Ϣ
		PlatformCenterServerVerify msg;
		msg.serverType = m_connectServerType;
		msg.serverID = m_connectServerID;
		Send(COMMON_VERIFY_MESSAGE, &msg, sizeof(msg));

		return false;
	}

	ERROR_LOG("�������ķ�����ʧ�ܣ��Ժ��������ip=%s,port=%d", m_ip, m_port);

	return true;
}

bool CTcpConnect::Send(UINT msgID, const void* pData, UINT size, int userID/* = 0*/, NetMessageHead* pNetHead/* = NULL*/)
{
	if (size + sizeof(PlatformMessageHead) > MAX_TEMP_SENDBUF_SIZE)
	{
		return false;
	}

	if (m_tcpClient.Enable() == false)
	{
		return false;
	}

	char buf[MAX_TEMP_SENDBUF_SIZE] = "";

	// head
	PlatformMessageHead* pHead = (PlatformMessageHead*)buf;
	pHead->AssHead.msgID = msgID;
	pHead->AssHead.userID = userID;
	pHead->MainHead.uMessageSize = sizeof(PlatformMessageHead) + size;
	if (pNetHead)
	{
		pHead->MainHead.uMainID = pNetHead->uMainID;
		pHead->MainHead.uAssistantID = pNetHead->uAssistantID;
		pHead->MainHead.uHandleCode = pNetHead->uHandleCode;
	}

	// body
	if (pData && size > 0)
	{
		memcpy(buf + sizeof(PlatformMessageHead), pData, size);
	}

	return m_tcpClient.Send(buf, pHead->MainHead.uMessageSize);
}

bool CTcpConnect::EventLoop()
{
	if (m_tcpClient.Enable() == false)
	{
		return false;
	}

	SOCKET sock = m_tcpClient.GetSocket();

	fd_set fdRead;
	FD_ZERO(&fdRead);

	FD_SET(sock, &fdRead);

	int ret = select(0, &fdRead, NULL, NULL, NULL);
	if (ret == SOCKET_ERROR)
	{
		//���������Ϣ
		ERROR_LOG("##### CGServerConnect::ThreadRSSocket select error,thread Exit.WSAGetLastError =%d #####", WSAGetLastError());
		return false;
	}

	if (ret == 0)
	{
		// ����
		return true;
	}

	if (FD_ISSET(sock, &fdRead))
	{
		m_tcpClient.OnRead();
	}

	return true;
}

bool CTcpConnect::CheckConnection()
{
	if (m_tcpClient.Enable() == false)
	{
		Sleep(5000);
		ReStart();
		return Connect();
	}

	return false;
}

bool CTcpConnect::ReStart()
{
	if (!m_bStart)
	{
		return false;
	}

	if (m_tcpClient.GetSocket() != INVALID_SOCKET)
	{
		return false;
	}

	if (!m_pDataLine || !m_ip || m_port <= 0)
	{
		return false;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	m_tcpClient.SetSocket(sock);
	m_tcpClient.SetDataLine(m_pDataLine);
	m_tcpClient.SetTcpConnect(this);

	return true;
}