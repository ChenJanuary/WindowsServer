// GameServer.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "NewServerManage.h"

#ifdef _MANAGED
#error ���Ķ� GameServer.cpp �е�˵����ʹ�� /clr ���б���
// ���Ҫ��������Ŀ����� /clr������ִ�����в���:
//	1. �Ƴ������� afxdllx.h �İ���
//	2. ��û��ʹ�� /clr ���ѽ���Ԥ����ͷ��
//	   ��Ŀ���һ�� .cpp �ļ������к��������ı�:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	return 1;   // ȷ��
}

//��ȡ��Ϸ�����Ϣ
extern "C" __declspec(dllexport) BOOL GetServiceInfo(ServerDllInfoStruct * pServiceInfo, UINT uVer)
{
	//Ч����Ϣ
	if ((DEV_LIB_VER != uVer) || (NULL == pServiceInfo))
	{
		return FALSE;
	}

	//д����Ϣ
	memset(pServiceInfo, 0, sizeof(ServerDllInfoStruct));
	pServiceInfo->uServiceVer = GAME_MAX_VER;
	pServiceInfo->uNameID = NAME_ID;
	pServiceInfo->uDeskPeople = PLAY_COUNT;
	pServiceInfo->uSupportType = SUPPER_TYPE;
	lstrcpy(pServiceInfo->szGameName, GAMENAME);
	lstrcpy(pServiceInfo->szWriter, GAME_DLL_AUTHOR);
	lstrcpy(pServiceInfo->szDllNote, GAME_DLL_NOTE);
	return TRUE;
}

//��ȡ��Ϸ����ӿں���
extern "C" __declspec(dllexport) IModuleManageService * CreateServiceInterface(UINT uVer)
{
	if (DEV_LIB_VER == uVer)
	{
		try
		{
			return new CGameModuleTemplate<CNewServerManage, 1, 10>;
		}
		catch (...)
		{
		}
	}
	return NULL;
}
#ifdef _MANAGED
#pragma managed(pop)
#endif

