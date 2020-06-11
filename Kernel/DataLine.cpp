#pragma once

#include "pch.h"
#include "DataLine.h"
#include "Function.h"
#include "KernelDefine.h"
#include "log.h"

CDataLine::CDataLine()
{
}

CDataLine::~CDataLine()
{
	ListItemData *pListItem = NULL;
	while (m_DataList.size() > 0)
	{
		pListItem = m_DataList.front();
		m_DataList.pop_front();
		delete pListItem->pData;
		delete pListItem;
	}
}

//������Ϣ����
/*
Function		:AddData
Memo			:������ѹ�뵽���е���
Author			:Fred Huang
Add Data		:2008-3-4
Modify Data		:none
Parameter		:
[IN]		pDataInfo	:Ҫѹ����е�����ָ��
[IN]		uAddSize	:���ݴ�С
[IN]		uDataKind	:��������
[IN]		pAppendData	:�������ݣ������ǿյ�
[IN]		pAppendAddSize	:�������ݴ�С������Ϊ0����ʱʵ������Ϊ��
Return			:ָѹ����еĴ�С
*/
UINT CDataLine::AddData(DataLineHead * pDataInfo, UINT uAddSize, UINT uDataKind, void * pAppendData, UINT uAppendAddSize)
{
	if (!m_hCompletionPort)
	{
		return 0;
	}

	if (!pDataInfo)
	{
		return 0;
	}

	CSignedLockObject LockObject(&m_csLock, true);

	ListItemData *pListItem = new ListItemData;			//����һ��������

	pListItem->pData = NULL;							//����Ϊ0���Ա�֤����������
	pListItem->stDataHead.uSize = uAddSize;				//���ݴ�С
	pListItem->stDataHead.uDataKind = uDataKind;		//��������
	if (pAppendData)									//����и�������
	{
		pListItem->stDataHead.uSize += uAppendAddSize;
	}

	pListItem->pData = new BYTE[pListItem->stDataHead.uSize + 1];	//�����������ڴ�
	ZeroMemory(pListItem->pData, pListItem->stDataHead.uSize + 1);	//����ڴ�

	pDataInfo->uDataKind = uDataKind;
	pDataInfo->uSize = pListItem->stDataHead.uSize;

	memcpy(pListItem->pData, pDataInfo, uAddSize);					//����ʵ������
	if (pAppendData != NULL)										//����и������ݣ�������ʵ�����ݺ���
	{
		memcpy(pListItem->pData + uAddSize, pAppendData, uAppendAddSize);
	}

	m_DataList.push_back(pListItem);								//�ӵ�����β��

	BOOL ret = PostQueuedCompletionStatus(m_hCompletionPort, pListItem->stDataHead.uSize, NULL, NULL);	//֪ͨ��ɶ˿�
	if (ret == FALSE)
	{
		//ERROR_LOG("CDataLine::AddData PostQueuedCompletionStatus failed err=%d", GetLastError());
	}

	return pListItem->stDataHead.uSize;		//���ش�С
}

//��ȡ��Ϣ����
/*
Function		:GetData
Memo			:�Ӷ�����ȡ������
Author			:Fred Huang
Add Data		:2008-3-4
Modify Data		:none
Parameter		:
[OUT]		pDataBuffer	:ȡ�����ݵĻ���
[IN]		uBufferSize	:�����С��ȱʡΪ LD_MAX_PART = 3096
Return			:ȡ�����ݵ�ʵ�ʴ�С
*/
UINT CDataLine::GetData(DataLineHead * pDataBuffer, UINT uBufferSize)
{
	CSignedLockObject LockObject(&m_csLock, true);

	memset(pDataBuffer, 0, uBufferSize);
	//��������ǿյģ�ֱ�ӷ���
	if (m_DataList.size() <= 0)
	{
		return 0;
	}

	//ȡ����
	ListItemData *pListItem = m_DataList.front();
	m_DataList.pop_front();

	UINT uDataSize = pListItem->stDataHead.uSize;

	if (uDataSize <= LD_MAX_PART)
	{
		//Ͷ������
		memcpy((void*)pDataBuffer, pListItem->pData, uDataSize);
	}
	else
	{
		ERROR_LOG("### DataLine GetData fail uDataSize=%d max=%d ###", uDataSize, LD_MAX_PART);
		uDataSize = 0;
	}

	//ɾ�������е�����
	delete[] pListItem->pData;
	delete pListItem;

	return uDataSize;
}

//������������
bool CDataLine::CleanLineData()
{
	CSignedLockObject LockObject(&m_csLock, true);

	ListItemData *pListItem = NULL;

	while (m_DataList.size() > 0)
	{
		pListItem = m_DataList.front();
		m_DataList.pop_front();
		delete pListItem->pData;
		delete pListItem;
	}

	return true;
}

// ��ȡ������������
INT_PTR CDataLine::GetDataCount(void)
{
	CSignedLockObject LockObject(&m_csLock, true);
	return m_DataList.size();
}
