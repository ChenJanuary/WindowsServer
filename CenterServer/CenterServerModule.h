#pragma once

#include "CenterServerManage.h"

//��Ϸ��½ģ��
class CCenterServerModule : public IModuleManageService
{
public:
	CCenterServerManage		m_CenterServerManage;				//��½����

public:
	CCenterServerModule();
	virtual ~CCenterServerModule();

public:
	virtual bool InitService(ManageInfoStruct* pInitData);
	virtual bool UnInitService();
	virtual bool StartService(UINT& errCode);
	virtual bool StoptService();
	virtual bool DeleteService();
	virtual bool UpdateService();
};
