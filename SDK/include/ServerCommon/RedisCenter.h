#pragma once

#include "RedisCommon.h"

class KERNEL_CLASS CRedisCenter : public CRedisCommon
{
public:
	CRedisCenter();
	virtual ~CRedisCenter();

public:
	// ��ʼ��
	virtual bool Init();
	// �ر�
	virtual bool Stop();

public:
	// bUnBind��ʾ�Ƿ���
	bool SetUserPhone(int userID, const char* phone, bool bUnBind = false);

	// ���İ�
	bool SetUserXianLiao(int userID, const char* xianliao);

	// ������Ϸ��״̬
	bool SetRoomServerStatus(int roomID, int status);

	// ���õ�½��״̬
	bool SetLogonServerStatus(int logonID, int status);

	// ���÷��ʱ��
	bool SetUserSealTime(int userID, int time);

private:
	int NeedLoadAllUserData();		//�ж��Ƿ���Ҫ�������ݿ��������û����ݵ�redis��0������Ҫ��1����Ҫ��>1���쳣����
	bool LoadAllUserData();			//���������û�
	bool LoadAllRewardsPoolData();	//����������Ϸ����
	bool LoadAllAgentUser();		//�������д���
	bool LoadAllConfig();			//����ϵͳ�������õ�redis
	bool LoadAllUserBag();			//�����û���������

};