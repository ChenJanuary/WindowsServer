#pragma once

#include "ComStruct.h"
#include "InterFace.h"
#include "Lock.h"
#include "DataLine.h"
#include "Exception.h"
#include "MysqlHelper.h"

#pragma warning(disable:4146)
#pragma warning(default:4146)

class CDataLine;
class CDataBaseManage;

#ifndef STRUCT_DATABASELINEHEAD
#define STRUCT_DATABASELINEHEAD

//���ݿ����ݰ�ͷ�ṹ
struct DataBaseLineHead
{
	DataLineHead					DataLineHead;							///����ͷ
	UINT							uHandleKind;							///��������
	UINT							uIndex;									///��������
	DWORD							dwHandleID;								///�����ʶ
};
#endif

// ���ݿ⴦����
class KERNEL_CLASS CDataBaseManage
{
public:
	KernelInfoStruct* m_pKernelInfo;	// �ں�����
	ManageInfoStruct* m_pInitInfo;		// ��ʼ������ָ��
	CDataLine	m_DataLine;				// ���ݶ���
	CMysqlHelper * m_pMysqlHelper;		// ���ݿ�ģ��
protected:
	HANDLE	m_hThread;			// �߳̾��
	HANDLE	m_hCompletePort;	// ��ɶ˿�
	bool	m_bInit;			// ��ʼ����־
	bool	m_bRun;				// ���б�־
	IDataBaseHandleService* m_pHandleService;	// ���ݴ���ӿ�

public:
	CDataBaseManage();
	virtual ~CDataBaseManage();

public:
	//��ʼ������
	bool Init(ManageInfoStruct* pInitInfo, KernelInfoStruct * pKernelInfo, IDataBaseHandleService * pHandleService, IAsynThreadResultService * pResultService);
	//ȡ����ʼ��
	bool UnInit();
	//��ʼ����
	bool Start();
	//ֹͣ����
	bool Stop();
	//���봦�����
	bool PushLine(DataBaseLineHead* pData, UINT uSize, UINT uHandleKind, UINT uIndex, DWORD dwHandleID);

public:
	//�����������
	bool CheckSQLConnect();
	//�������ݿ�
	bool SQLConnectReset();

	int		m_sqlClass;

	SHORT	m_nPort;
	BOOL    m_bsqlInit;

	char	m_host[128];
	char	m_passwd[48];
	char	m_user[48];
	char	m_name[48];

private:
	//���ݿ⴦���߳�
	static unsigned __stdcall DataServiceThread(LPVOID pThreadData);
};

///***********************************************************************************************///
//���ݿ⴦��ӿ���
class KERNEL_CLASS CDataBaseHandle : public IDataBaseHandleService
{
public:
	CDataBaseHandle();
	virtual ~CDataBaseHandle();

protected:
	KernelInfoStruct* m_pKernelInfo;			//�ں�����
	ManageInfoStruct* m_pInitInfo;				//��ʼ������ָ��
	IAsynThreadResultService* m_pRusultService;	//�������ӿ�
	CDataBaseManage*	m_pDataBaseManage;		//���ݿ����

public:
	//���ò���
	virtual bool SetParameter(IAsynThreadResultService * pRusultService, CDataBaseManage * pDataBaseManage, ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData);
};