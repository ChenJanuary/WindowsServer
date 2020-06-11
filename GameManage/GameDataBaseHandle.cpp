#include "pch.h"
#include "GameDataBaseHandle.h"
#include "LoaderAsyncEvent.h"
#include "MyCurl.h"
#include "log.h"
#include "InternalMessageDefine.h"
#include "configManage.h"
#include "Define.h"

CGameDataBaseHandle::CGameDataBaseHandle()
{
	m_ErrorSQLCount = 0;
	m_lastCheckDBConnectionTime = 0;
}

CGameDataBaseHandle::~CGameDataBaseHandle()
{
}

UINT CGameDataBaseHandle::HandleDataBase(DataBaseLineHead* pSourceData)
{
	if (!pSourceData)
	{
		return 0;
	}

	time_t currTime = time(NULL);
	if (m_pDataBaseManage && currTime - m_lastCheckDBConnectionTime >= 60)		// 60 TODO
	{
		m_lastCheckDBConnectionTime = currTime;
		m_pDataBaseManage->CheckSQLConnect();
	}

	switch (pSourceData->uHandleKind)
	{
	case LOADER_ASYNC_EVENT_UPLOAD_VIDEO:
	{
		return OnUploadVideo(pSourceData);
	}
	case DTK_GP_SQL_STATEMENT:			// ����sql���
	{
		return OnHandleExecuteSQLStatement(pSourceData);
	}
	case LOADER_ASYNC_EVENT_HTTP:
	{
		return OnHandleHTTP(pSourceData);
	}
	default:
		break;
	}

	return 0;
}

int CGameDataBaseHandle::OnUploadVideo(DataBaseLineHead * pSourceData)
{
	if (pSourceData->DataLineHead.uSize != sizeof(LoaderAsyncUploadVideo))
	{
		ERROR_LOG("size is not match realsize=%d expect=%d", pSourceData->DataLineHead.uSize, sizeof(LoaderAsyncUploadVideo));
		return -1;
	}

	LoaderAsyncUploadVideo* pAsyncMessage = (LoaderAsyncUploadVideo*)pSourceData;
	if (!pAsyncMessage)
	{
		ERROR_LOG("asyncMessage is NULL");
		return -2;
	}

	AUTOCOST("�ϴ�¼��json��ʱ: %s", pAsyncMessage->videoCode);

	if (!m_pInitInfo)
	{
		ERROR_LOG("m_pInitInfo is NULL");
		return -3;
	}

	int gameID = m_pInitInfo->uNameID;

	MyCurl curl;
	curl.uploadUrl(pAsyncMessage->videoCode, gameID, 0, 3);

	return 0;
}

// ִ��sql���
int CGameDataBaseHandle::OnHandleExecuteSQLStatement(DataBaseLineHead * pSourceData)
{
	AUTOCOST("ִ��SQL����ʱ");

	InternalSqlStatement* pMessage = (InternalSqlStatement*)pSourceData;
	if (!pMessage)
	{
		ERROR_LOG("pMessage is NULL");
		return -1;
	}

	if (!m_pDataBaseManage || !m_pDataBaseManage->m_pMysqlHelper)
	{
		ERROR_LOG("invalid hDatabase");
		return -2;
	}

	try
	{
		m_pDataBaseManage->m_pMysqlHelper->sqlExec(pMessage->sql);
	}
	catch (MysqlHelper_Exception& excep)
	{
		ERROR_LOG("ִ��sql���ʧ��==>>%s", excep.errorInfo.c_str());
		return -3;
	}

	return 0;
}

// HTTP����
int CGameDataBaseHandle::OnHandleHTTP(DataBaseLineHead * pSourceData)
{
	if (pSourceData->DataLineHead.uSize != sizeof(LoaderAsyncHTTP))
	{
		ERROR_LOG("size is not match realsize=%d expect=%d", pSourceData->DataLineHead.uSize, sizeof(LoaderAsyncHTTP));
		return -1;
	}

	LoaderAsyncHTTP* pAsyncMessage = (LoaderAsyncHTTP*)pSourceData;
	if (!pAsyncMessage)
	{
		ERROR_LOG("asyncMessage is NULL");
		return -2;
	}

	AUTOCOST("HTTP����: userID=%d", pAsyncMessage->userID);

	//�����ʼ��ӿ�
	MyCurl curl;
	std::vector<std::string> vUrlHeader;
	std::string postFields = "";
	std::string result = "";

	//���̶�ͷ��
	//todo�����ݾ���ҵ����Ըģ�Ҳ���Բ����ͷ��
	if (pAsyncMessage->postType == HTTP_POST_TYPE_REQ_DATA)
	{
		vUrlHeader.push_back("Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		vUrlHeader.push_back("Accept-Encoding:gzip, deflate, br");
		vUrlHeader.push_back("Accept-Language:zh-CN,zh;q=0.8");
		vUrlHeader.push_back("Cache-Control:max-age=0");
		vUrlHeader.push_back("Connection:keep-alive");
		vUrlHeader.push_back("Cookie:ccsalt=436fa91b51b76de36af8a7dc7002679c");
		vUrlHeader.push_back("Host:www.1392p.com");
		vUrlHeader.push_back("Upgrade-Insecure-Requests:1");
		vUrlHeader.push_back("User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
	}

	//�������URL
	std::string url = pAsyncMessage->url;

	curl.postUrlHttps(url, vUrlHeader, postFields, result);
	if (result.size() <= 0)
	{
		ERROR_LOG("HTTP����ʧ�ܣ�url=[%s],userID=[%d],ret=[%s]", pAsyncMessage->url, pAsyncMessage->userID, result.c_str());
		return -2;
	}

	if (result.size() < LD_MAX_PART - 1)
	{
		//���ؽ��
		char szBuffer[LD_MAX_PART] = "";
		memcpy(szBuffer, result.c_str(), min(result.size(), LD_MAX_PART - 1));

		szBuffer[LD_MAX_PART - 1] = 0;

		m_pRusultService->OnAsynThreadResultEvent(ANSY_THREAD_RESULT_TYPE_HTTP, 0, szBuffer,
			result.size() + 1, pAsyncMessage->postType, pAsyncMessage->userID);
	}
	else
	{
		ERROR_LOG("http���󷵻����ݹ��� size=%d", result.size());
	}

	return 0;
}