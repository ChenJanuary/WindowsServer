#pragma once

#include "DataBase.h"

//���ݿ⴦����
class CServiceDataBaseHandle : public CDataBaseHandle
{
public:
	CServiceDataBaseHandle();
	virtual ~CServiceDataBaseHandle();

private:
	virtual UINT HandleDataBase(DataBaseLineHead * pSourceData);

private:
	// ִ��sql���
	int OnHandleExecuteSQLStatement(DataBaseLineHead * pSourceData);
	// HTTP����
	int OnHandleHTTP(DataBaseLineHead * pSourceData);

private:
	time_t m_lastCheckDBConnectionTime;
};