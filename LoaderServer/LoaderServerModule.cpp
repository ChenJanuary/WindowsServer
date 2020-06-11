#include "main.h"
#include "LoaderServerModule.h"
#include "configManage.h"
#include "log.h"
#include "commonuse.h"
#include "Util.h"

//dll�ӿں�������
typedef IModuleManageService* (CreateServiceInterface)(UINT uVer);
typedef BOOL(GetServiceInfo)(ServerDllInfoStruct* pServiceInfo, UINT uVer);

CLoaderServerModule::CLoaderServerModule()
{
	m_szError[0] = 0;
}

CLoaderServerModule::~CLoaderServerModule()
{

}

int CLoaderServerModule::StartAllRoom()
{
	std::cout << "StartAllRoom Begin...\n";

	int iStartCount = 0;

	auto iter = ConfigManage()->m_roomBaseInfoMap.begin();
	for (; iter != ConfigManage()->m_roomBaseInfoMap.end(); iter++)
	{
		const RoomBaseInfo& roomBaseInfo = iter->second;

		// �����������ļ���ͬ���ֵ�
		if (strcmp(roomBaseInfo.serviceName, ConfigManage()->m_loaderServerConfig.serviceName))
		{
			continue;
		}
		
		GameBaseInfo* pGameBaseInfo = ConfigManage()->GetGameBaseInfo(roomBaseInfo.gameID);
		if (!pGameBaseInfo)
		{
			std::cout << "GameBaseInfo ����û�� gameID:" << roomBaseInfo.gameID << endl;
			continue;
		}

		GameRoomInitStruct info;
		int roomID = roomBaseInfo.roomID;

		memcpy(info.szRoomNote, roomBaseInfo.name, sizeof(roomBaseInfo.name));
		info.uServiceID = roomID;
		info.ServiceInfo.uNameID = roomBaseInfo.gameID;
		memcpy(info.ServiceInfo.szGameName, pGameBaseInfo->name, sizeof(roomBaseInfo.name));
		const char* dllFileName = ConfigManage()->GetDllFileName(roomBaseInfo.gameID);
		if (dllFileName)
		{
			memcpy(info.ServiceInfo.szDLLFileName, dllFileName, sizeof(info.ServiceInfo.szDLLFileName));
		}

		MakeInitData(&info, roomID);

		if (!LoadServiceInfo(&info))
		{
			std::cout << "������Ϸ����ʧ�� roomID:" << roomBaseInfo.roomID << endl;
			continue;
		}

		if (!StartGameRoom(&info))
		{
			std::cout << "������Ϸ����ʧ�� roomID:" << roomBaseInfo.roomID << endl;
			continue;
		}

		iStartCount++;
		m_allRoomInfoMap[roomID] = info;
		CON_INFO_LOG("�����ɹ���roomID=%d gameID=%d name=%s roomType=%d roomsort=%d deskCount=%d minPoint=%d maxPoint=%d basePoint=%d roomlevel=%d\n"
			, roomID, roomBaseInfo.gameID, roomBaseInfo.name, roomBaseInfo.type, roomBaseInfo.sort, roomBaseInfo.deskCount, roomBaseInfo.minPoint,
			roomBaseInfo.maxPoint, roomBaseInfo.basePoint, roomBaseInfo.level);
	}

	std::cout << "StartAllRoom End...\n";

	return iStartCount;
}

void CLoaderServerModule::StopAllRoom()
{
	std::cout << "StopAllRoom begin...\n";

	for (auto iter = m_allRoomInfoMap.begin(); iter != m_allRoomInfoMap.end(); iter++)
	{
		StopGameRoom(&(iter->second));
	}

	m_allRoomInfoMap.clear();

	std::cout << "StopAllRoom end...\n";
}

void CLoaderServerModule::StartRoomByRoomID(int roomID)
{



}

void CLoaderServerModule::StopRoomByRoomID(int roomID)
{

}

void CLoaderServerModule::UpdateAllRoom()
{
	for (auto iter = m_allRoomInfoMap.begin(); iter != m_allRoomInfoMap.end(); iter++)
	{
		if (iter->second.pIManageService)
		{
			iter->second.pIManageService->UpdateService();
		}
	}

	std::cout << "UpdateAllRoom Success\n";
}

void CLoaderServerModule::UpdateRoomByRoomID(int roomID)
{
	auto iter = m_allRoomInfoMap.find(roomID);
	if (iter == m_allRoomInfoMap.end())
	{
		std::cout << "UpdateRoomID Fail " << roomID << endl;
		return;
	}
	
	iter->second.pIManageService->UpdateService();

	std::cout << "UpdateRoomID Success " << roomID << endl;

}

bool CLoaderServerModule::LoadServiceInfo(GameRoomInitStruct* pGameRoomInfo)
{
	try
	{
		//�ж��ļ�
		struct stat FileStat;
		int ret = ::stat(pGameRoomInfo->ServiceInfo.szDLLFileName, &FileStat);
		if (ret != 0)
		{
			std::cout << "�Ҳ����ļ� " << pGameRoomInfo->ServiceInfo.szDLLFileName << endl;
			return false;
		}

		//�������
		pGameRoomInfo->hDllInstance = LoadLibrary(pGameRoomInfo->ServiceInfo.szDLLFileName);
		if (pGameRoomInfo->hDllInstance == NULL)
		{
			std::cout << "���ض�̬��ʧ�� " << pGameRoomInfo->ServiceInfo.szDLLFileName << endl;
			return false;
		}

		//��ȡ����ӿ�
		CreateServiceInterface* pCreateServiceInterface = (CreateServiceInterface*)GetProcAddress(pGameRoomInfo->hDllInstance, TEXT("CreateServiceInterface"));
		if (pCreateServiceInterface == NULL)
		{
			std::cout << "���ض�̬�⺯��ʧ�� " << "CreateServiceInterface" << endl;
			return false;
		}

		pGameRoomInfo->pIManageService = pCreateServiceInterface(DEV_LIB_VER);
		if (pGameRoomInfo->pIManageService == NULL)
		{
			std::cout << "���� CreateServiceInterface ʧ��" << endl;
			return false;
		}

		GetServiceInfo* pGetServiceInfo = (GetServiceInfo*)GetProcAddress(pGameRoomInfo->hDllInstance, TEXT("GetServiceInfo"));
		if (pGetServiceInfo == NULL)
		{
			std::cout << "���ض�̬�⺯��ʧ�� " << "GetServiceInfo" << endl;
			return false;
		}

		//ָ����Щ��Ϸ�ǿ��õ�
		ServerDllInfoStruct ServiceDllInfo;
		if (pGetServiceInfo(&ServiceDllInfo, DEV_LIB_VER) == false)
		{
			std::cout << "���� GetServiceInfo ʧ��" << endl;
			return false;
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

void CLoaderServerModule::MakeInitData(GameRoomInitStruct* pGameRoomInfo, int roomID)
{
	if (pGameRoomInfo == nullptr)
	{
		return;
	}

	RoomBaseInfo* pRoomBaseInfo = ConfigManage()->GetRoomBaseInfo(roomID);
	GameBaseInfo* pGameBaseInfo = ConfigManage()->GetGameBaseInfo(pRoomBaseInfo->gameID);
	if (pRoomBaseInfo == nullptr || pGameBaseInfo == nullptr)
	{
		return;
	}

	pGameRoomInfo->InitInfo.uRoomID = roomID;
	pGameRoomInfo->InitInfo.uNameID = pRoomBaseInfo->gameID;
	pGameRoomInfo->InitInfo.iSocketSecretKey = SECRET_KEY;
	pGameRoomInfo->InitInfo.uDeskCount = pRoomBaseInfo->deskCount;
	pGameRoomInfo->InitInfo.bPrivate = pRoomBaseInfo->type == 1 ? true : false;
	pGameRoomInfo->ServiceInfo.uDeskPeople = pGameBaseInfo->deskPeople;
	memcpy(pGameRoomInfo->InitInfo.szGameRoomName, pRoomBaseInfo->name, sizeof(pRoomBaseInfo->name));
	pGameRoomInfo->InitInfo.iRoomType = pRoomBaseInfo->type;
	pGameRoomInfo->InitInfo.iRoomSort = pRoomBaseInfo->sort;
	pGameRoomInfo->InitInfo.iRoomLevel = pRoomBaseInfo->level;
	pGameRoomInfo->InitInfo.uLessPoint = pRoomBaseInfo->minPoint;
	pGameRoomInfo->InitInfo.uBasePoint = pRoomBaseInfo->basePoint;
	pGameRoomInfo->InitInfo.uMaxPoint = pRoomBaseInfo->maxPoint;
	pGameRoomInfo->InitInfo.bCanCombineDesk = pGameBaseInfo->canCombineDesk;
}

//������Ϸ����
bool CLoaderServerModule::StartGameRoom(GameRoomInitStruct* pGameRoomInfo)
{
	try
	{
		if (pGameRoomInfo->pIManageService == NULL)
		{
			// ���ؿ�
			if (!LoadServiceInfo(pGameRoomInfo))
			{
				std::cout << "������Ϸ����ʧ�� roomID:" << pGameRoomInfo->uServiceID << endl;
				return false;
			}
		}

		UINT errCode = 0;
		//�������
		if (pGameRoomInfo->pIManageService->InitService(&pGameRoomInfo->InitInfo) == false)
		{
			throw TEXT("�����ʼ������");
		}

		if (pGameRoomInfo->pIManageService->StartService(errCode) == false)
		{
			throw TEXT("�����������");
		}

		return true;
	}
	catch (TCHAR* szError)
	{
		std::cout << szError << std::endl;
		ERROR_LOG("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
	}
	catch (CException* pException)
	{
		pException->GetErrorMessage(m_szError, sizeof(m_szError));
		pException->Delete();
		std::cout << m_szError << std::endl;
	}
	catch (...)
	{
		std::cout << "����δ֪�쳣����" << std::endl;
		ERROR_LOG("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
	}

	//������Դ
	if (pGameRoomInfo->pIManageService != NULL)
	{
		pGameRoomInfo->pIManageService->StoptService();
		pGameRoomInfo->pIManageService->UnInitService();
		pGameRoomInfo->pIManageService->DeleteService();
		pGameRoomInfo->pIManageService = NULL;
	}

	if (pGameRoomInfo->hDllInstance != NULL)
	{
		FreeLibrary(pGameRoomInfo->hDllInstance);
		pGameRoomInfo->hDllInstance = NULL;
	}

	return false;
}

//ֹͣ��Ϸ����
bool CLoaderServerModule::StopGameRoom(GameRoomInitStruct* pGameRoomInfo)
{
	if (!pGameRoomInfo)
	{
		return false;
	}

	try
	{
		if (pGameRoomInfo->hDllInstance == NULL || pGameRoomInfo->pIManageService == NULL)
		{
			return true;
		}

		//ֹͣ���
		if (pGameRoomInfo->pIManageService->StoptService() == false)
		{
			throw TEXT("���ֹͣ����");
		}

		if (pGameRoomInfo->pIManageService->UnInitService() == false)
		{
			throw TEXT("���ж�ش���");
		}

		if (pGameRoomInfo->pIManageService->DeleteService() == false)
		{
			throw TEXT("����������");
		}

		pGameRoomInfo->pIManageService = NULL;

		//ж�����
		FreeLibrary(pGameRoomInfo->hDllInstance);
		pGameRoomInfo->hDllInstance = NULL;

		return true;
	}
	catch (TCHAR* szError)
	{
		std::cout << szError << std::endl;
		ERROR_LOG("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
	}
	catch (CException* pException)
	{
		pException->GetErrorMessage(m_szError, sizeof(m_szError));
		pException->Delete();
		std::cout << m_szError << std::endl;
	}
	catch (...)
	{
		std::cout << "����δ֪�쳣����" << std::endl;
		ERROR_LOG("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
	}

	//������Դ
	if (pGameRoomInfo->pIManageService != NULL)
	{
		pGameRoomInfo->pIManageService->StoptService();
		pGameRoomInfo->pIManageService->UnInitService();
		pGameRoomInfo->pIManageService->DeleteService();
		pGameRoomInfo->pIManageService = NULL;
	}
	if (pGameRoomInfo->hDllInstance != NULL)
	{
		FreeLibrary(pGameRoomInfo->hDllInstance);
		pGameRoomInfo->hDllInstance = NULL;
	}

	return false;
}