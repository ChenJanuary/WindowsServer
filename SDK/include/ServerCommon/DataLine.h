#pragma once

#include <list>
#include "Lock.h"

#ifndef STRUCT_DATALINEHEAD
#define STRUCT_DATALINEHEAD
//���ݶ�����Ϣͷ
struct DataLineHead
{
	UINT						uSize;								//���ݴ�С
	UINT						uDataKind;							//��������
};
#endif //STRUCT_DATALINEHEAD
#ifndef STRUCT_DATABASERESULTLINE
#define STRUCT_DATABASERESULTLINE
///�첽�߳̽����Ϣ�ṹ����
struct AsynThreadResultLine
{
	DataLineHead						LineHead;					///����ͷ
	UINT								uHandleResult;				///������
	UINT								uHandleKind;				///��������
	UINT								uHandleID;					///�����ʶ
};
#endif //STRUCT_DATABASERESULTLINE

/*
Struct		:ListItemData
Memo		:���������ݽṹ
Author		:Fred Huang
Add Data	:2008-3-4
Modify Data	:none
Parameter	:
	stDataHead	:���ݰ�ͷ
	pData		:ÿ�����������ݵ�ָ�룬ʹ����new��ʽ������ڴ棬ע�⣬�ڳ�����ʱ��Ҫ��ʽ��delete ���ڴ�
*/
struct ListItemData
{
	DataLineHead stDataHead;
	BYTE* pData;
};

//���ݶ�����
class KERNEL_CLASS CDataLine
{
private:
	std::list <ListItemData*> m_DataList;

	HANDLE				m_hCompletionPort;		//��ɶ˿�
	CSignedLock		m_csLock;				//ͬ����

public:
	CDataLine();
	virtual ~CDataLine();

public:
	//��ȡ��
	CSignedLock * GetLock() { return &m_csLock; }
	//������������
	bool CleanLineData();
	//������ɶ˿�
	void SetCompletionHandle(HANDLE hCompletionPort) { m_hCompletionPort = hCompletionPort; }
	//������Ϣ����
	virtual UINT AddData(DataLineHead * pDataInfo, UINT uAddSize, UINT uDataKind, void * pAppendData = NULL, UINT uAppendAddSize = 0);
	//��ȡ��Ϣ����
	virtual UINT GetData(DataLineHead * pDataBuffer, UINT uBufferSize);

public:
	INT_PTR GetDataCount();
};

