#pragma once

#include "DataBase.h"

//��Ϸ���ݿ⴦��
class KERNEL_CLASS CGameDataBaseHandle : public CDataBaseHandle
{
public:
	CGameDataBaseHandle();
	virtual ~CGameDataBaseHandle();

private:
	virtual UINT HandleDataBase(DataBaseLineHead * pSourceData);

private:
	// �ϴ�¼��
	int OnUploadVideo(DataBaseLineHead* pSourceData);
	// ִ��sql���
	int OnHandleExecuteSQLStatement(DataBaseLineHead * pSourceData);
	// HTTP����
	int OnHandleHTTP(DataBaseLineHead * pSourceData);

public:
	int m_ErrorSQLCount;
	time_t m_lastCheckDBConnectionTime;
};