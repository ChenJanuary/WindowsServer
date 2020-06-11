#pragma once

#include "GameMainManage.h"

//����������Ϣ�ṹ
struct GameRoomInitStruct
{
	//����ģʽ
	UINT				  uRunStation;					//����״̬
	HANDLE				  hProcessHandle;				//���̾��

	//DLL ģʽ
	HINSTANCE			  hDllInstance;					//������
	IModuleManageService* pIManageService;				//����ӿ�

	//������Ϣ
	bool				  bEnable;						//�Ƿ�����
	UINT				  uKindID;						//���� ID
	UINT				  uSortID;						//���� ID
	UINT				  uServiceID;					//��� ID
	TCHAR				  szRoomNote[50];				//���䱸ע
	ManageInfoStruct	  InitInfo;						//��ʼ����Ϣ
	ServerDllInfoStruct	  ServiceInfo;					//�����Ϣ

	GameRoomInitStruct()
	{
		memset(this, 0, sizeof(GameRoomInitStruct));
	}
};

// �������ģ��
class CLoaderServerModule
{
public:
	CLoaderServerModule();
	~CLoaderServerModule();

	int StartAllRoom();
	void StopAllRoom();
	void StartRoomByRoomID(int roomID);
	void StopRoomByRoomID(int roomID);
	void UpdateAllRoom();
	void UpdateRoomByRoomID(int roomID);

private:
	bool LoadServiceInfo(GameRoomInitStruct* pGameRoomInfo); // ��ȡ��̬����Ϣ
	void MakeInitData(GameRoomInitStruct* pGameRoomInfo, int roomID); //��ʼ������
	bool StartGameRoom(GameRoomInitStruct* pGameRoomInfo); //������Ϸ����
	bool StopGameRoom(GameRoomInitStruct* pGameRoomInfo); //ֹͣ��Ϸ����
private:
	std::map<int, GameRoomInitStruct> m_allRoomInfoMap;
	char m_szError[255]; //������Ϣ
};