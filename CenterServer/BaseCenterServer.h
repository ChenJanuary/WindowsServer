#pragma once

#include "Interface.h"
#include "ComStruct.h"
#include "DataLine.h"
#include "TCPSocket.h"
#include "RedisCenter.h"
#include "RedisPHP.h"
#include "Exception.h"
#include "GameLogManage.h"

//�������ݹ�����
class CBaseCenterServer : public IServerSocketService
{
protected:
	bool			m_bRun;					//���б�־
	bool			m_bInit;				//��ʼ����־
	HWND			m_hWindow;				//���ھ��
	HANDLE			m_hWindowThread;		//�����߳�
	HANDLE			m_hHandleThread;		//�����߳�
	HANDLE			m_hCompletePort;		//������ɶ˿�
	CDataLine		m_DataLine;				//���ݶ���

public:
	ManageInfoStruct						m_InitData;					//��ʼ������
	KernelInfoStruct						m_KernelData;				//�ں�����
	ServerDllInfoStruct						m_DllInfo;					//DLL��Ϣ
	CTCPSocketManage						m_TCPSocket;				//����ģ��
	CRedisCenter*							m_pRedis;					//redis
	CRedisPHP*								m_pRedisPHP;				//PHP������redis

public:
	CBaseCenterServer();
	virtual ~CBaseCenterServer();

	//����ʹ�ù��ܺ��� ���������̵߳��ã�
public:
	bool SetTimer(UINT uTimerID, UINT uElapse);
	bool KillTimer(UINT uTimerID);

	//����ӿں��� �����̵߳��ã�
public:
	virtual bool Init(ManageInfoStruct * pInitData);
	virtual bool UnInit();
	virtual bool Start();
	virtual bool Stop();

	//����ӿں��� �����̵߳��ã�
private:
	//��ȡ��Ϣ���� ���������أ�
	virtual bool PreInitParameter(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData) = 0;
	//���ݹ���ģ���ʼ��
	virtual bool OnInit(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData) { return true; };
	//���ݹ���ģ��ж��
	virtual bool OnUnInit() { return true; }
	//���ݹ���ģ������
	virtual bool OnStart() { return true; }
	//���ݹ���ģ��ر�
	virtual bool OnStop() { return true; }

	//������չ�ӿں��� ���������̵߳��ã�
private:
	//SOCKET ���ݶ�ȡ ���������أ�
	virtual bool OnSocketRead(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID) = 0;
	//SOCKET �ر� ���������أ�
	virtual bool OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, UINT uConnectTime) = 0;
	//��ʱ����Ϣ ���������أ�
	virtual bool OnTimerMessage(UINT uTimerID) = 0;

	//����ӿں��� �����������̵߳��ã�
public:
	//����رմ��� ������Ҫ���أ�
	virtual bool OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, UINT uConnectTime);
	//������Ϣ���� ������Ҫ���أ�ΪZ������׼��
	virtual bool OnSocketReadEvent(CTCPSocket* pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID);

	virtual CDataLine* GetDataLine() { return &m_DataLine; }

	//�ڲ����������̵߳��ã�
private:
	//��������Ϊ�����ɶ�ʱ��
	bool CreateWindowsForTimer();
	//��ʱ��֪ͨ��Ϣ
	bool WindowTimerMessage(UINT uTimerID);
	//�������ݴ����߳�
	static unsigned __stdcall LineDataHandleThread(LPVOID pThreadData);
	//WINDOW ��Ϣѭ���߳�
	static unsigned __stdcall WindowMsgThread(LPVOID pThreadData);
	//���ڻص�����
	static LRESULT CALLBACK WindowProcFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};