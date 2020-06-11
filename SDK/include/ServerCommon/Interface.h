#pragma once

/********************************************************************************************/
///����ӿڶ���
/********************************************************************************************/

class CTCPSocket;					///<SOCKET ��
class CDataBaseManage;				///<���ݿ�ģ����
struct NetMessageHead;				///<�������ݰ�ͷ
struct ManageInfoStruct;			///<��ʼ����Ϣ�ṹ
struct KernelInfoStruct;			///<�ں���Ϣ�ṹ
struct DataLineHead;				///<����ͷ�ṹ
struct DataBaseLineHead;			///<���ݿ����ͷ�ṹ
struct AsynThreadResultLine;		///<�첽�߳̽��ͷ�ṹ
class CDataLine;

//�������������ӿ� 
class IServerSocketService
{
	///�ӿں���
public:
	//����رմ���
	virtual bool OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, UINT uConnectTime) = 0;
	//������Ϣ����
	virtual bool OnSocketReadEvent(CTCPSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID) = 0;
	// ��ȡdataline
	virtual CDataLine* GetDataLine() = 0;
};

///�첽�߳̽������ӿ�
class IAsynThreadResultService
{
	///�ӿں���
public:
	///�첽�߳̽������
	virtual bool OnAsynThreadResultEvent(UINT uHandleKind, UINT uHandleResult, void * pData, UINT uResultSize, UINT uDataType, UINT uHandleID) = 0;
};

///���ݿ⴦�����ӿ�
class IDataBaseHandleService
{
	///�ӿں���
public:
	///���ò���
	virtual bool SetParameter(IAsynThreadResultService * pRusultService, CDataBaseManage * pDataBaseManage, ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData) = 0;
	///���ݿ⴦��ӿ�
	virtual UINT HandleDataBase(DataBaseLineHead * pSourceData) = 0;
};

///�������ӿ�
class IModuleManageService
{
	///�ӿں���
public:
	///��ʼ������ 
	virtual bool InitService(ManageInfoStruct * pInitData) = 0;
	///ȡ����ʼ������ 
	virtual bool UnInitService() = 0;
	///��ʼ������ 
	virtual bool StartService(UINT &errCode) = 0;
	///��ʼ������ 
	virtual bool StoptService() = 0;
	///ɾ������
	virtual bool DeleteService() = 0;
	///���º���
	virtual bool UpdateService() = 0;
};
/********************************************************************************************/
