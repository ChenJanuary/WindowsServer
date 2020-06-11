// UpGradeDLL.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ServerManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ERROR_LOG("UpGradeDLL.DLL ���ڳ�ʼ����\n");

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		ERROR_LOG("UpGradeDLL.DLL ������ֹ��\n");
	}
	return 1;   // ȷ��
}

//��ȡ��Ϸ�����Ϣ
extern "C" __declspec(dllexport) BOOL GetServiceInfo(ServerDllInfoStruct * pServiceInfo, UINT uVer)
{
	//Ч����Ϣ
	if ((uVer!=DEV_LIB_VER)||(pServiceInfo==NULL)) return FALSE;

	//д����Ϣ
	memset(pServiceInfo,0,sizeof(ServerDllInfoStruct));
	pServiceInfo->uServiceVer=GAME_MAX_VER;
	pServiceInfo->uNameID=NAME_ID;
	pServiceInfo->uDeskPeople=PLAY_COUNT;
	pServiceInfo->uSupportType=SUPPER_TYPE;
	lstrcpy(pServiceInfo->szGameName,GAMENAME);
	///lstrcpy(pServiceInfo->szGameTable,GAME_TABLE_NAME);
	///lstrcpy(pServiceInfo->szDLLFileName,SERVER_DLL_NAME);
	lstrcpy(pServiceInfo->szWriter,TEXT("  "));//������"));
	lstrcpy(pServiceInfo->szDllNote,TEXT("���� -- ��������Ϸ���"));
	return TRUE;
}

//��ȡ��Ϸ����ӿں���
extern "C" __declspec(dllexport) IModuleManageService * CreateServiceInterface(UINT uVer)
{
	if (uVer==DEV_LIB_VER) 
	{
		try	{ return new CGameModuleTemplate<CServerGameDesk,1,10>; }
		catch (...) {}
	}
	return NULL;
}
