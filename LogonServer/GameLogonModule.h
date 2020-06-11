#pragma once

#include "GameLogonManage.h"
#include "GameLogonDataBase.h"

//��Ϸ��½ģ��
class CGameLogonModule : public IModuleManageService
{
public:
	CGameLogonManage		m_LogonManage;				//��½����
	CServiceDataBaseHandle	m_DataBaseHandle;			//���ݿ����ݴ���ģ��

public:
	CGameLogonModule();
	virtual ~CGameLogonModule();

public:
	virtual bool InitService(ManageInfoStruct * pInitData);
	virtual bool UnInitService();
	virtual bool StartService(UINT &errCode);
	virtual bool StoptService();
	virtual bool DeleteService();
	virtual bool UpdateService();
};
