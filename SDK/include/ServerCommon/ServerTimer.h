#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

// ��ʱ�����ȣ���λ����
#define SERVER_TIME_ONCE	100

struct ServerTimerInfo
{
	unsigned int elapse;  // ת��֮���ʱ����
	long long startslice; // ��ʼʱ��
	ServerTimerInfo()
	{
		elapse = 10;
		startslice = 0;
	}
};

class CDataLine;
class CSignedLock;
class KERNEL_CLASS CServerTimer
{
public:
	CServerTimer();
	~CServerTimer();
	bool Start(CDataLine* pDataLine);
	bool Stop();
	bool SetTimer(unsigned int uTimerID, unsigned int uElapse); //uElapse�Ǻ��뵥λ
	bool KillTimer(unsigned int uTimerID);
	bool ExistsTimer(unsigned int uTimerID);

private:
	// ��ʱ���̺߳���
	static unsigned __stdcall ThreadCheckTimer(LPVOID pThreadData);

private:
	volatile bool m_bRun;
	volatile long long m_llTimeslice;// ʱ��Ƭ
	std::unordered_map<unsigned int, ServerTimerInfo> m_timerMap;
	CDataLine* m_pDataLine;	// �����dataline����
	CSignedLock* m_pLock; // �߳���
};