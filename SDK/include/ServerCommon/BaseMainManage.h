#pragma once

#include "Interface.h"
#include "ComStruct.h"
#include "DataLine.h"
#include "TCPSocket.h"
#include "DataBase.h"
#include "RedisLoader.h"
#include "RedisPHP.h"
#include "TcpConnect.h"
#include "GServerConnect.h"

//�������ݹ�����
class KERNEL_CLASS CBaseMainManage : public IServerSocketService, public IAsynThreadResultService
{
protected:
	bool									m_bRun;						//���б�־
	bool									m_bInit;					//��ʼ����־
	HWND									m_hWindow;					//���ھ��
	HANDLE									m_hWindowThread;			//�����߳�
	HANDLE									m_hHandleThread;			//�����߳�
	HANDLE									m_hCompletePort;			//������ɶ˿�
	CDataLine								m_DataLine;					//���ݶ���
	HANDLE									m_connectCServerHandle;		//�����ķ������߳̾��

public:
	ManageInfoStruct						m_InitData;					//��ʼ������
	KernelInfoStruct						m_KernelData;				//�ں�����
	ServerDllInfoStruct						m_DllInfo;					//DLL��Ϣ
	CDataBaseManage							m_SQLDataManage;			//���ݿ�ģ��
	CRedisLoader*							m_pRedis;					//redis
	CRedisPHP*								m_pRedisPHP;				//����php��redis server
	CTcpConnect*							m_pTcpConnect;				//�����ķ�����������
	CGServerConnect*						m_pGServerConnect;			//���¼������������

public:
	CBaseMainManage();
	virtual ~CBaseMainManage();

	CRedisLoader* GetRedis() { return m_pRedis; }
	CRedisPHP* GetRedisPHP() { return m_pRedisPHP; }

public:
	//��ʼ������
	virtual bool Init(ManageInfoStruct * pInitData, IDataBaseHandleService * pDataHandleService);
	//ȡ����ʼ������
	virtual bool UnInit();
	//��������
	virtual bool Start();
	//ֹͣ����
	virtual bool Stop();
	//ˢ�·���
	virtual bool Update();
	//�趨��ʱ��
	bool SetTimer(UINT uTimerID, UINT uElapse);
	//�����ʱ��
	bool KillTimer(UINT uTimerID);
	//����رմ��� 
	virtual bool OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, UINT uConnectTime);
	//������Ϣ����
	virtual bool OnSocketReadEvent(CTCPSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID);
	//�첽�߳̽������
	virtual bool OnAsynThreadResultEvent(UINT uHandleKind, UINT uHandleResult, void * pData, UINT uResultSize, UINT uDataType, UINT uHandleID);

	virtual CDataLine* GetDataLine() { return &m_DataLine; }

	//����ӿں���
private:
	//��ȡ��Ϣ����
	virtual bool PreInitParameter(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData) = 0;
	//���ݹ���ģ���ʼ��
	virtual bool OnInit(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData) { return true; }
	//���ݹ���ģ��ж��
	virtual bool OnUnInit() { return true; }
	//���ݹ���ģ������
	virtual bool OnStart() { return true; }
	//���ݹ���ģ��ر�
	virtual bool OnStop() { return true; }
	//���ݹ���ˢ��ģ��
	virtual bool OnUpdate() { return true; }
	//socket���ݶ�ȡ
	virtual bool OnSocketRead(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID) = 0;
	//socket�ر�
	virtual bool OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, UINT uConnectTime) = 0;
	//�첽�̴߳�����
	virtual bool OnAsynThreadResult(AsynThreadResultLine * pResultData, void * pData, UINT uSize) = 0;
	//��ʱ����Ϣ
	virtual bool OnTimerMessage(UINT uTimerID) = 0;
	//��������Ϊ�����ɶ�ʱ��
	bool CreateWindowsForTimer();
	//��ʱ��֪ͨ��Ϣ
	bool WindowTimerMessage(UINT uTimerID);
	//�������ݴ����߳�
	static unsigned __stdcall LineDataHandleThread(LPVOID pThreadData);
	//window��Ϣѭ���߳�
	static unsigned __stdcall WindowMsgThread(LPVOID pThreadData);
	//���ڻص�����
	static LRESULT CALLBACK WindowProcFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//�����ķ����������߳�
	static unsigned __stdcall TcpConnectThread(LPVOID pThreadData);

private:
	//�������ķ���Ϣ
	virtual bool OnCenterServerMessage(UINT msgID, NetMessageHead * pNetHead, void* pData, UINT size, int userID) = 0;
};