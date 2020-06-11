#include "main.h"
#include "CenterServerManage.h"
#include "tableDefine.h"
#include "RedisLogon.h"
#include "ConfigManage.h"
#include "Define.h"
#include "NewMessageDefine.h"
#include "ErrorCode.h"
#include "log.h"
#include "InternalMessageDefine.h"
#include "PlatformMessage.h"
#include "Util.h"
#include "BillManage.h"
#include "TcpConnect.h"
#include <array>
#include <math.h>

//////////////////////////////////////////////////////////////////////
CCenterServerManage::CCenterServerManage() : CBaseCenterServer()
{
	m_nPort = 0;
	m_uMaxPeople = 0;
	m_lastNormalTimerTime = 0;
}

//////////////////////////////////////////////////////////////////////
CCenterServerManage::~CCenterServerManage()
{
	m_serverToSocketMap.clear();
	m_logonGroupSocket.clear();
	m_centerUserInfoMap.clear();
	m_timeMatchInfoMap.clear();
}

//////////////////////////////////////////////////////////////////////
bool CCenterServerManage::OnSocketRead(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	if (!pNetHead)
	{
		return false;
	}

	AUTOCOST("���ķ�������Ϣ��ʱ msgID: %d mainID: %d assistID: %d uIndex: %d", pCenterHead->msgID, pNetHead->uMainID, pNetHead->uAssistantID, uIndex);

	if (pCenterHead->msgID == COMMON_VERIFY_MESSAGE)
	{
		return OnHandleCommonServerVerifyMessage(pData, uSize, uAccessIP, uIndex, dwHandleID);
	}
	else if (CENTER_MESSAGE_COMMON_BEGIN < pCenterHead->msgID && pCenterHead->msgID < CENTER_MESSAGE_COMMON_END)
	{
		return OnHandleCommonMessage(pNetHead, pCenterHead, pData, uSize, uAccessIP, uIndex, dwHandleID);
	}
	else if (CENTER_MESSAGE_LOGON_BEGIN < pCenterHead->msgID && pCenterHead->msgID < CENTER_MESSAGE_LOGON_END)
	{
		return OnHandleLogonMessage(pNetHead, pCenterHead, pData, uSize, uAccessIP, uIndex, dwHandleID);
	}
	else if (CENTER_MESSAGE_LOADER_BEGIN < pCenterHead->msgID && pCenterHead->msgID < CENTER_MESSAGE_LOADER_END)
	{
		return OnHandleLoaderMessage(pNetHead, pCenterHead, pData, uSize, uAccessIP, uIndex, dwHandleID);
	}
	else if (PLATFORM_MESSAGE_BEGIN < pCenterHead->msgID && pCenterHead->msgID < PLATFORM_MESSAGE_END)
	{
		return OnHandlePHPMessage(pNetHead, pCenterHead, pData, uSize, uAccessIP, uIndex, dwHandleID);
	}
	else
	{
		ERROR_LOG("unhandled message pCenterHead->msgID=%d", pCenterHead->msgID);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CCenterServerManage::OnStart()
{
	INFO_LOG("CenterServerManage OnStart begin...");

	//ΪsocketIdx���·����ڴ�
	if (m_socketToServerVec.size() != m_InitData.uMaxPeople)
	{
		m_socketToServerVec.resize(m_InitData.uMaxPeople);
	}

	//�������������û�
	if (m_pRedis)
	{
		m_pRedis->ClearOnlineUser();
	}

	//���ر�����Ϣ
	m_pRedisPHP->LoadAllMatchInfo(m_timeMatchInfoMap);

	m_lastNormalTimerTime = time(NULL);

	//// ����redis��д�ٶ�
	//m_pRedis->TestRedis("CenterServer-GameRedis-Speed");
	//m_pRedisPHP->TestRedis("CenterServer-PHPRedis-Speed");

	InitRounteCheckEvent();

	INFO_LOG("CenterServerManage OnStart end.");

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CCenterServerManage::OnTimerMessage(UINT uTimerID)
{
	AUTOCOST("��ʱ����ʱtimerID = %d", uTimerID);
	switch (uTimerID)
	{
	case CENTER_TIMER_CHECK_REDIS_CONNECTION:
	{
		CheckRedisConnection();
		return true;
	}
	case CENTER_TIMER_NORMAL:
	{
		OnNormalTimer();
		return true;
	}
	case CENTER_TIMER_ROUTINE_CHECK_UNBINDID_SOCKET:
	{
		RoutineCheckUnbindIDSocket();
		return true;
	}
	case CENTER_TIMER_LOAD_MATCH_INFO:
	{
		m_pRedisPHP->LoadAllMatchInfo(m_timeMatchInfoMap);
		return true;
	}
	default:
		break;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
bool CCenterServerManage::OnStop()
{
	m_socketToServerVec.clear();

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CCenterServerManage::PreInitParameter(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)
{
	if (!pInitData || !pKernelData)
	{
		return false;
	}

	const CenterServerConfig& config = ConfigManage()->GetCenterServerConfig();

	m_uMaxPeople = config.maxSocketCount;
	m_nPort = config.port;

	pInitData->uListenPort = m_nPort;
	pInitData->uMaxPeople = m_uMaxPeople;
	pInitData->iSocketSecretKey = SECRET_KEY;
	memcpy(pInitData->szCenterIP, config.ip, sizeof(pInitData->szCenterIP));

	return true;
}

//////////////////////////////////////////////////////////////////////
// �ߵ������һ���ǵײ�֪ͨҵ���ر�(����ͻ��˶��ߣ����������ر�socket֮��)
bool CCenterServerManage::OnSocketClose(ULONG uAccessIP, UINT socketIdx, UINT uConnectTime)
{
	AUTOCOST("������ϵͳ�˳���Ⱥ��ʱ��socketIdx = %d", socketIdx);

	// �ȹر��������
	if (!m_TCPSocket.CloseSocket(socketIdx))
	{
		return false;
	}

	// ���ǵ�PHP�����ﲻ������־
	if (socketIdx >= m_socketToServerVec.size() || m_socketToServerVec[socketIdx].serverType == SERVICE_TYPE_BEGIN)
	{
		return true;
	}

	ServerBaseInfo serverInfo = m_socketToServerVec[socketIdx];

	// ɾ���������
	m_socketToServerVec[socketIdx].serverType = SERVICE_TYPE_BEGIN;
	m_socketToServerVec[socketIdx].serverID = 0;

	auto itrServerToScoket = m_serverToSocketMap.find(serverInfo);
	if (itrServerToScoket == m_serverToSocketMap.end())
	{
		ERROR_LOG("��������Ϣ������ serverType=%d,serverID=%d", serverInfo.serverType, serverInfo.serverID);
		return false;
	}

	// ɾ�����������
	m_serverToSocketMap.erase(itrServerToScoket);

	if (!m_pRedis)
	{
		ERROR_LOG("m_pRedis = NULL");
		return false;
	}

	if (serverInfo.serverType == SERVICE_TYPE_LOGON) //��¼���˳���Ⱥ
	{
		auto itrVecLogonGroup = m_logonGroupSocket.begin();
		for (; itrVecLogonGroup != m_logonGroupSocket.end(); itrVecLogonGroup++)
		{
			if (*itrVecLogonGroup == socketIdx)
			{
				break;
			}
		}
		if (itrVecLogonGroup == m_logonGroupSocket.end())
		{
			ERROR_LOG("��¼��socket���������� socketIdx=%d", socketIdx);
			return false;
		}

		// ɾ������
		m_logonGroupSocket.erase(itrVecLogonGroup);

		//���·��ͼ�Ⱥ��Ϣ
		SendDistributedSystemInfo();

		//���˵�¼�������������Ƴ�
		for (auto iter = m_centerUserInfoMap.begin(); iter != m_centerUserInfoMap.end(); )
		{
			if (iter->second.socketIdx == socketIdx)
			{
				m_pRedis->RemoveOnlineUser(iter->first, iter->second.isVirtual);
				m_centerUserInfoMap.erase(iter++);
			}
			else
			{
				iter++;
			}
		}

		m_pRedis->SetLogonServerStatus(serverInfo.serverID, 0);
	}
	else if (serverInfo.serverType == SERVICE_TYPE_LOADER) //��Ϸ���˳���Ⱥ
	{
		m_pRedis->SetRoomServerStatus(serverInfo.serverID, 0);
	}

	INFO_LOG("====== serverType:%d,serverID:%d�˳���Ⱥ ��¼����Ⱥ����:%d=========", serverInfo.serverType, serverInfo.serverID, m_logonGroupSocket.size());

	return true;
}

//////////////////////////////��������(��½������)////////////////////////////////////////////
bool CCenterServerManage::SendData(int userID, UINT msgID, void * pData, int size, unsigned int mainID, unsigned int assistID, unsigned int handleCode)
{
	auto itr = m_centerUserInfoMap.find(userID);
	if (itr == m_centerUserInfoMap.end())
	{
		//WARNNING_LOG("===�û� userID:%d�������ķ�������û��===", userID);
		return true;
	}

	int ret = m_TCPSocket.CenterServerSendData(itr->second.socketIdx, msgID, pData, size, mainID, assistID, handleCode, userID);
	if (ret <= 0)
	{
		ERROR_LOG("##��������ʧ�ܣ�msgID=%d##", msgID);
		return false;
	}

	return true;
}

// ���������������
bool CCenterServerManage::SendData(ServerBaseInfo * pServer, UINT msgID, void* pData, UINT size, int userID /*= 0*/, UINT mainID/* = 0*/, UINT assistID /*= 0*/, UINT handleCode/* = 0*/)
{
	if (!pServer)
	{
		return false;
	}

	auto itr = m_serverToSocketMap.find(*pServer);
	if (itr == m_serverToSocketMap.end())
	{
		ERROR_LOG("������û�к����ķ�������������,serverType=%d,serverID=%d", pServer->serverType, pServer->serverID);
		return false;
	}

	int ret = m_TCPSocket.CenterServerSendData(itr->second, msgID, pData, size, mainID, assistID, handleCode, userID);
	if (ret <= 0)
	{
		ERROR_LOG("##��������ʧ�ܣ�msgID=%d,mainID=%d,assistID=%d##", msgID, mainID, assistID);
		return false;
	}

	return true;
}

// ���������������
bool CCenterServerManage::SendData(UINT uScoketIndex, UINT msgID, void* pData, UINT size, UINT handleCode/* = 0*/)
{
	int	ret = m_TCPSocket.CenterServerSendData(uScoketIndex, msgID, pData, size, 0, 0, handleCode, 0);
	if (ret <= 0)
	{
		ERROR_LOG("##��������ʧ�ܣ�uScoketIndex = %d msgID=%d ##", uScoketIndex, msgID);
		return false;
	}

	return true;
}

void CCenterServerManage::NotifyResourceChange(int userID, int resourceType, long long value, long long changeValue, int reason, int reserveData/* = 0*/, BYTE isNotifyRoom/* = 0*/)
{
	if (!IsUserOnline(userID))
	{
		return;
	}

	UserData userData;
	if (!m_pRedis->GetUserData(userID, userData))
	{
		ERROR_LOG("��Ҳ�����,userID=%d", userID);
		return;
	}

	LogonNotifyResourceChange msg;

	msg.resourceType = resourceType;
	msg.value = value;
	msg.reason = reason;
	msg.changeValue = changeValue;

	SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(LogonNotifyResourceChange), MSG_MAIN_LOGON_NOTIFY, MSG_NTF_LOGON_RESOURCE_CHANGE, 0);

	// ����������Ϸ�У�֪ͨ����Ϸ
	int roomID = userData.roomID;
	if (roomID == 0 || !isNotifyRoom)
	{
		// ��Ҳ�����Ϸ
		return;
	}

	PlatformResourceChange msgToLoader;

	msgToLoader.resourceType = resourceType;
	msgToLoader.value = value;
	msgToLoader.changeValue = changeValue;
	msgToLoader.reason = reason;
	msgToLoader.reserveData = reserveData;
	msgToLoader.isNotifyRoom = isNotifyRoom;

	ServerBaseInfo serverInfo;
	serverInfo.serverType = SERVICE_TYPE_LOADER;
	serverInfo.serverID = roomID;

	if (!SendData(&serverInfo, CENTER_MESSAGE_COMMON_RESOURCE_CHANGE, &msgToLoader, sizeof(msgToLoader), userID))
	{
		ERROR_LOG("����Ϸ����������ʧ�� roomID=%d", roomID);
	}
}

// ����¼����������Ⱥ���͵�ǰ��Ⱥ��Ϣ
void CCenterServerManage::SendDistributedSystemInfo()
{
	size_t iLogonGroupSocketSize = m_logonGroupSocket.size();
	if (iLogonGroupSocketSize <= 0)
	{
		return;
	}

	int iMainLogonServer = CUtil::GetRandNum() % iLogonGroupSocketSize;
	for (size_t i = 0; i < iLogonGroupSocketSize; i++)
	{
		PlatformDistributedSystemInfo msg;
		msg.logonGroupCount = iLogonGroupSocketSize;
		msg.logonGroupIndex = i;
		msg.mainLogonGroupIndex = iMainLogonServer;

		SendData(m_logonGroupSocket[i], CENTER_MESSAGE_COMMON_LOGON_GROUP_INFO, &msg, sizeof(msg));
	}
}

// ����������ʼ�������б�����ҷ�����Ϣ֪ͨ
void CCenterServerManage::SendNotifyMatchStart(const MatchInfo &matchInfo)
{
	//��ȡ��������
	std::vector<MatchUserInfo> vecPeople;
	if (!m_pRedis->GetTimeMatchPeople(matchInfo.matchID, vecPeople))
	{
		return;
	}

	LogonNotifyStartMatchRightAway msg;
	msg.matchID = matchInfo.matchID;
	msg.gameID = matchInfo.gameID;
	msg.startTime = MATCH_START_NOTIFY_TIME;

	for (size_t i = 0; i < vecPeople.size(); i++)
	{
		SendData(vecPeople[i].userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(msg), MSG_MAIN_MATCH_NOTIFY, MSG_NTF_LOGON_MATCH_NOTIFY_START_MATCH_RIGHTAWAY, 0);
	}
}

////////////////////////////////�ڲ�ϵͳ����//////////////////////////////////////////
void CCenterServerManage::InitRounteCheckEvent()
{
	// ���ü��redis���Ӷ�ʱ��
	SetTimer(CENTER_TIMER_CHECK_REDIS_CONNECTION, CHECK_REDIS_CONNECTION_SECS * 1000);

	// ����ͨ�ö�ʱ��
	SetTimer(CENTER_TIMER_NORMAL, NORMAL_TIMER_SECS * 1000);

	// ��������php���Ӷ�ʱ��
	SetTimer(CENTER_TIMER_ROUTINE_CHECK_UNBINDID_SOCKET, ROUTINE_CHECK_UNBINDID_SOCKET * 1000);

	// ���ö�ʱ���ر�����Ϣ
	SetTimer(CENTER_TIMER_LOAD_MATCH_INFO, CHECK_REDIS_LOAD_MATCH_INFO * 1000);
}

void CCenterServerManage::CheckRedisConnection()
{
	if (!m_pRedis)
	{
		ERROR_LOG("m_pRedis is NULL");
		return;
	}

	if (!m_pRedisPHP)
	{
		ERROR_LOG("m_pRedisPHP is NULL");
		return;
	}

	const RedisConfig& redisGameConfig = ConfigManage()->GetRedisConfig(REDIS_DATA_TYPE_GAME);

	m_pRedis->CheckConnection(redisGameConfig);

	const RedisConfig& redisPHPConfig = ConfigManage()->GetRedisConfig(REDIS_DATA_TYPE_PHP);

	m_pRedisPHP->CheckConnection(redisPHPConfig);
}

void CCenterServerManage::OnNormalTimer()
{
	//////////////////////////////���ķ��賿6��////////////////////////////////////////////
	// ����Ƿ����
	time_t currTime = time(NULL);
	int currHour = CUtil::GetHourTimeStamp(currTime);
	int lastHour = CUtil::GetHourTimeStamp(m_lastNormalTimerTime);
	if (currHour == 6 && currHour > lastHour)
	{
		OnServerCrossDay();

		int currWeekday = CUtil::GetWeekdayFromTimeStamp(currTime);

		if (currWeekday == 1)
		{
			OnServerCrossWeek();
		}
	}

	//����Ƿ��ж�ʱ�����Կ���
	CheckTimeMatch(currTime);

	m_lastNormalTimerTime = currTime;
}

void CCenterServerManage::RoutineCheckUnbindIDSocket()
{
	time_t currTime = time(NULL);

	for (size_t i = 0; i < m_InitData.uMaxPeople; i++)
	{
		bool bIsConnect = false;
		time_t llAcceptMsgTime = 0, llLastRecvMsgTime = 0;
		m_TCPSocket.GetTCPSocketInfo(i, bIsConnect, llAcceptMsgTime, llLastRecvMsgTime);

		// �����󶨵�socket
		if (bIsConnect && currTime - llLastRecvMsgTime > CONNECT_TIME_SECS && m_socketToServerVec[i].serverType == SERVICE_TYPE_BEGIN)
		{
			m_TCPSocket.CloseSocket(i);
		}
	}
}

void CCenterServerManage::OnServerCrossDay()
{
	AUTOCOST("OnServerCrossDay");
	INFO_LOG("OnServerCrossDay");

	// ÿ�췢��һ�μ�Ⱥ��Ϣ
	SendDistributedSystemInfo();
}

void CCenterServerManage::OnServerCrossWeek()
{
	AUTOCOST("OnServerCrossWeek");
	INFO_LOG("OnServerCrossWeek");

}

// ����Ƿ�����
bool CCenterServerManage::IsUserOnline(int userID)
{
	if (m_centerUserInfoMap.find(userID) != m_centerUserInfoMap.end())
	{
		return true;
	}

	return true;
}

// ��鶨ʱ��
void CCenterServerManage::CheckTimeMatch(const time_t &currTime)
{
	std::vector<int> matchRoomVec;

	for (auto itr = m_timeMatchInfoMap.begin(); itr != m_timeMatchInfoMap.end();)
	{
		if (currTime >= itr->second.startTime)
		{
			if (_abs64(currTime - itr->second.startTime) <= 120)
			{
				//ʱ�䵽�ˣ����Կ�ʼ������
				PlatformReqStartMatchTime msg;
				msg.gameID = itr->second.gameID;
				msg.matchID = itr->second.matchID;
				msg.minPeople = itr->second.minPeople;
				msg.costResType = itr->second.costResType;
				msg.costResNums = itr->second.costResNums;

				matchRoomVec.clear();
				ConfigManage()->GetBuyRoomInfo(msg.gameID, ROOM_TYPE_MATCH, matchRoomVec);
				if (matchRoomVec.size() <= 0)
				{
					ERROR_LOG("û�����ñ�������gameID=%d", msg.gameID);
					m_timeMatchInfoMap.erase(itr++);
					return;
				}

				//����һ��������
				int roomID = matchRoomVec[CUtil::GetRandNum() % matchRoomVec.size()];

				ServerBaseInfo serverInfo;
				serverInfo.serverType = SERVICE_TYPE_LOADER;
				serverInfo.serverID = roomID;

				if (!SendData(&serverInfo, CENTER_MESSAGE_COMMON_START_MATCH_TIME, &msg, sizeof(msg)))
				{
					ERROR_LOG("��ʱ������ʼ����ʧ�� roomID=%d", roomID);
				}
				else
				{
					INFO_LOG("��ʼ��ʱ����gameid=%d,starttime=%lld,matchID=%d", itr->second.gameID, itr->second.startTime, itr->second.matchID);
				}
			}

			m_timeMatchInfoMap.erase(itr++);
			continue;
		}
		else if (itr->second.startTime - currTime < MATCH_START_NOTIFY_TIME && itr->second.isNotified == 0)
		{
			// ������ʼǰ֪ͨһ��
			SendNotifyMatchStart(itr->second);

			itr->second.isNotified = 1;
			itr++;
		}
		else if (itr->second.startTime - currTime < MATCH_START_NOTIFY_TIME)
		{
			itr++;
		}
		else
		{
			break;
		}
	}
}

/////////////////////////////////PHP/////////////////////////////////////////
bool CCenterServerManage::OnHandlePHPMessage(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	switch (pCenterHead->msgID)
	{
	case PLATFORM_MESSAGE_NOTICE:
	{
		return OnHandlePHPNoticeMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_REQ_ALL_USER_MAIL:
	{
		return OnHandlePHPAllUserMailMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_CLOSE_SERVER:
	{
		return OnHandlePHPCloseServerMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_OPEN_SERVER:
	{
		return OnHandlePHPOpenServerMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_SEND_HORN:
	{
		return OnHandlePHPHornMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_MASTER_DISSMISS_DESK:
	case PLATFORM_MESSAGE_FG_DISSMISS_DESK:
	{
		return OnHandlePHPDissmissDeskMessage(uIndex, pCenterHead->msgID, pData, uSize);
	}
	case PLATFORM_MESSAGE_RESOURCE_CHANGE:
	{
		return OnHandlePHPNotifyResChangeMessage(uIndex, pCenterHead->userID, pData, uSize);
	}
	case PLATFORM_MESSAGE_NOTIFY_USERID:
	{
		return OnHandlePHPNotifyOneUserMessage(uIndex, pCenterHead->userID, pData, uSize, pNetHead->uMainID, pNetHead->uAssistantID);
	}
	case PLATFORM_MESSAGE_NOTIFY_FG:
	{
		return OnHandlePHPNotifyFGMessage(uIndex, pCenterHead->userID, pData, uSize, pNetHead->uMainID, pNetHead->uAssistantID);
	}
	case PLATFORM_MESSAGE_IDENTUSER:
	{
		return OnHandlePHPSetUserMessage(uIndex, pCenterHead->userID, pData, uSize);
	}
	case PLATFORM_MESSAGE_RED_SPOT:
	{
		return OnHandlePHPNotifyUserRedspotMessage(uIndex, pCenterHead->userID, pData, uSize);
	}
	case PLATFORM_MESSAGE_RED_FG_SPOT:
	{
		return OnHandlePHPNotifyUserRedFGspotMessage(uIndex, pCenterHead->userID, pData, uSize);
	}
	case PLATFORM_MESSAGE_RELOAD_GAME_CONFIG:
	{
		return OnHandlePHPReloadGameConfigMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_PHONE_INFO:
	{
		return OnHandlePHPPhoneInfoMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_SIGN_UP_MATCH_PEOPLE:
	{
		return OnHandlePHPSignUpMatchMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_START_MATCH_PEOPLE:
	{
		return OnHandlePHPReqStartMatchMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_MODIFY_TIME_MATCH:
	{
		return OnHandlePHPReqModifyTimeMatchMessage(uIndex, pData, uSize);
	}
	case PLATFORM_MESSAGE_SIGN_UP_MATCH_TIME:
	{
		return OnHandlePHPTimeMatchPeopleChangeMessage(uIndex, pData, uSize);
	}
	default:
		break;
	}

	return false;
}

// ��ӹ���
bool CCenterServerManage::OnHandlePHPNoticeMessage(UINT socketIdx, void* pData, int size)
{
	if (size != sizeof(PlatformNoticeMessage))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_NOTICE, NULL, 0, PF_SIZEERROR);
		return false;
	}

	PlatformNoticeMessage* pMessage = (PlatformNoticeMessage*)pData;
	if (!pMessage)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_NOTICE, NULL, 0, PF_DATA_NULL);
		return false;
	}

	//�����е�¼��������
	for (size_t i = 0; i < m_logonGroupSocket.size(); i++)
	{
		SendData(m_logonGroupSocket[i], PLATFORM_MESSAGE_NOTICE, pData, size);
	}

	SendData(socketIdx, PLATFORM_MESSAGE_NOTICE, NULL, 0, PF_SUCCESS);

	return true;
}

// ȫ���ʼ�֪ͨ
bool CCenterServerManage::OnHandlePHPAllUserMailMessage(UINT socketIdx, void* pData, int size)
{
	SendData(socketIdx, PLATFORM_MESSAGE_REQ_ALL_USER_MAIL, NULL, 0, PF_SUCCESS);

	for (size_t i = 0; i < m_logonGroupSocket.size(); i++)
	{
		SendData(m_logonGroupSocket[i], PLATFORM_MESSAGE_REQ_ALL_USER_MAIL, NULL, 0);
	}

	return true;
}

//��reids���ݱ���db��
bool CCenterServerManage::OnHandlePHPCloseServerMessage(UINT socketIdx, void*pData, int size)
{
	if (size != sizeof(PlatformCloseServerMessage))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_CLOSE_SERVER, NULL, 0, PF_SIZEERROR);
		return false;
	}

	PlatformCloseServerMessage* pMessage = (PlatformCloseServerMessage*)pData;
	if (!pMessage)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_CLOSE_SERVER, NULL, 0, PF_DATA_NULL);
		return false;
	}

	if (!m_pRedis)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_CLOSE_SERVER, NULL, 0, PF_REDIS_NULL);
		return false;
	}

	if (pMessage->status != SERVER_PLATFROM_STATUS_CLOSE && pMessage->status != SERVER_PLATFROM_STATUS_TEST)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_CLOSE_SERVER, NULL, 0, PF_CLOSE_STATUS_ERR);
		return false;
	}

	SendData(socketIdx, PLATFORM_MESSAGE_CLOSE_SERVER, NULL, 0, PF_SUCCESS);

	m_pRedis->SetServerStatus(pMessage->status);

	INFO_LOG("==========CenterServer ���͹ط�����===========");

	for (auto itr = m_serverToSocketMap.begin(); itr != m_serverToSocketMap.end(); itr++)
	{
		SendData(itr->second, PLATFORM_MESSAGE_CLOSE_SERVER, NULL, 0);
	}

	////���������������
	//m_centerUserInfoMap.clear();

	return true;
}

//��reids�ж�ȡ���ݻָ�
bool CCenterServerManage::OnHandlePHPOpenServerMessage(UINT socketIdx, void*pData, int size)
{
	if (size != 0)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_OPEN_SERVER, NULL, 0, PF_SIZEERROR);
		return false;
	}

	if (!m_pRedis)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_OPEN_SERVER, NULL, 0, PF_REDIS_NULL);
		return false;
	}

	INFO_LOG("==========CenterServer ���Ϳ�������===========");

	m_pRedis->SetServerStatus(SERVER_PLATFROM_STATUS_NORMAL);

	SendData(socketIdx, PLATFORM_MESSAGE_OPEN_SERVER, NULL, 0, PF_SUCCESS);

	for (size_t i = 0; i < m_logonGroupSocket.size(); i++)
	{
		SendData(m_logonGroupSocket[i], PLATFORM_MESSAGE_OPEN_SERVER, NULL, 0);
	}

	return true;
}

// ��������
bool CCenterServerManage::OnHandlePHPHornMessage(UINT socketIdx, void*pData, int size)
{
	if (size != sizeof(PlatformHorn))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_SEND_HORN, NULL, 0, PF_SIZEERROR);
		return false;
	}

	PlatformHorn* pMessage = (PlatformHorn*)pData;
	if (!pMessage)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_SEND_HORN, NULL, 0, PF_DATA_NULL);
		return false;
	}

	for (size_t i = 0; i < m_logonGroupSocket.size(); i++)
	{
		SendData(m_logonGroupSocket[i], PLATFORM_MESSAGE_SEND_HORN, pData, size);
	}

	SendData(socketIdx, PLATFORM_MESSAGE_SEND_HORN, NULL, 0, PF_SUCCESS);

	return true;
}

// ��ɢ����
bool CCenterServerManage::OnHandlePHPDissmissDeskMessage(UINT socketIdx, UINT msgID, void*pData, int size)
{
	if (size != sizeof(PlatformDissmissDesk))
	{
		SendData(socketIdx, msgID, NULL, 0, PF_SIZEERROR);
		return false;
	}

	PlatformDissmissDesk* pMessage = (PlatformDissmissDesk*)pData;
	if (!pMessage)
	{
		SendData(socketIdx, msgID, NULL, 0, PF_DATA_NULL);
		return false;
	}

	if (pMessage->deskMixID <= 0 || pMessage->roomID <= 0)
	{
		SendData(socketIdx, msgID, NULL, 0, PF_DISSMISS_DESK_ERR);
		return false;
	}

	ServerBaseInfo serverInfo;
	serverInfo.serverType = SERVICE_TYPE_LOADER;
	serverInfo.serverID = pMessage->roomID;

	if (!SendData(&serverInfo, msgID, pData, size))
	{
		SendData(socketIdx, msgID, NULL, 0, PF_SEND_DATA_ERR);
		return false;
	}

	SendData(socketIdx, msgID, NULL, 0, PF_DISSMISS_DESK_ERR);

	return true;
}

// ֪ͨĳ������Դ�仯
bool CCenterServerManage::OnHandlePHPNotifyResChangeMessage(UINT socketIdx, int userID, void*pData, int size)
{
	if (size != sizeof(PlatformResourceChange))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RESOURCE_CHANGE, NULL, 0, PF_SIZEERROR);
		return false;
	}

	PlatformResourceChange* pMessage = (PlatformResourceChange*)pData;
	if (!pMessage)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RESOURCE_CHANGE, NULL, 0, PF_DATA_NULL);
		return false;
	}

	NotifyResourceChange(userID, pMessage->resourceType, pMessage->value, pMessage->changeValue, pMessage->reason, pMessage->reserveData, pMessage->isNotifyRoom);

	return true;
}

// ��ĳ�����������Ϣ
bool CCenterServerManage::OnHandlePHPNotifyOneUserMessage(UINT socketIdx, int userID, void*pData, int size, unsigned int mainID, unsigned int assistID)
{
	return SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, pData, size, mainID, assistID, 0);
}

// ����ֲ����������Ϣ
bool CCenterServerManage::OnHandlePHPNotifyFGMessage(UINT socketIdx, int friendsGroupID, void*pData, int size, unsigned int mainID, unsigned int assistID)
{
	if (!m_pRedisPHP)
	{
		ERROR_LOG("m_pRedisPHP = NULL");
		return false;
	}

	m_pRedisPHP->GetFGMember(friendsGroupID, m_memberUserIDVec);

	if (m_memberUserIDVec.size() <= 0)
	{
		ERROR_LOG("���ֲ��޳�Ա friendsGroupID=%d", friendsGroupID);
		return false;
	}

	for (size_t i = 0; i < m_memberUserIDVec.size(); i++)
	{
		SendData(m_memberUserIDVec[i], PLATFORM_MESSAGE_NOTIFY_USERID, pData, size, mainID, assistID, 0);
	}

	return true;
}

// ����������
bool CCenterServerManage::OnHandlePHPSetUserMessage(UINT socketIdx, int userID, void* pData, int size)
{
	if (size != sizeof(PlatformIdentUser))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_IDENTUSER, NULL, 0, PF_SIZEERROR);
		return false;
	}

	PlatformIdentUser* pMessage = (PlatformIdentUser*)pData;
	if (!pMessage)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_IDENTUSER, NULL, 0, PF_DATA_NULL);
		return false;
	}

	if (!m_pRedis)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_IDENTUSER, NULL, 0, PF_REDIS_NULL);
		return false;
	}

	if (pMessage->statusValue <= 0)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_IDENTUSER, NULL, 0, PF_NOUSER);
		return false;
	}

	UserData userData;
	if (!m_pRedis->GetUserData(userID, userData))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_IDENTUSER, NULL, 0, PF_NOUSER);
		ERROR_LOG("GetUserData failed userID:%d", userID);
		return false;
	}

	int iStatus = 0;
	if (pMessage->type == 1) //����
	{
		iStatus = userData.status | pMessage->statusValue;
		if (pMessage->statusValue == USER_IDENTITY_TYPE_WIN && (userData.status&USER_IDENTITY_TYPE_FAIL) == USER_IDENTITY_TYPE_FAIL)
		{
			iStatus = iStatus ^ USER_IDENTITY_TYPE_FAIL;
		}
		else if (pMessage->statusValue == USER_IDENTITY_TYPE_FAIL && (userData.status&USER_IDENTITY_TYPE_WIN) == USER_IDENTITY_TYPE_WIN)
		{
			iStatus = iStatus ^ USER_IDENTITY_TYPE_WIN;
		}
	}
	else //ȡ��
	{
		if ((userData.status&pMessage->statusValue) == pMessage->statusValue)
		{
			iStatus = userData.status^pMessage->statusValue;
		}
		else
		{
			SendData(socketIdx, PLATFORM_MESSAGE_IDENTUSER, NULL, 0, PF_SUCCESS);
			return true;
		}
	}

	//���÷�Ź��ܣ���д�û�����ʱ��
	int iFinishTime = (int)time(NULL) + pMessage->otherValue;
	if (pMessage->statusValue == USER_IDENTITY_TYPE_SEAL)
	{
		if (pMessage->type == 1)
		{
			if (pMessage->otherValue <= 0)
			{
				iFinishTime = -1;
			}
			m_pRedis->SetUserSealTime(userID, iFinishTime);

			//�������Ҷ���
			SendData(userID, PLATFORM_MESSAGE_IDENTUSER, NULL, 0, 0, 0, 0);
		}
		else if (pMessage->type == 0)
		{
			m_pRedis->SetUserSealTime(userID, 0);
		}
	}

	std::unordered_map<std::string, std::string> umap;
	umap["status"] = CUtil::Tostring(iStatus);

	if (!m_pRedis->hmset(TBL_USER, userID, umap))
	{
		ERROR_LOG("����������ʧ��:userID=%d", userID);
	}

	SendData(socketIdx, PLATFORM_MESSAGE_IDENTUSER, NULL, 0, PF_SUCCESS);

	return true;
}

// ֪ͨС���
bool CCenterServerManage::OnHandlePHPNotifyUserRedspotMessage(UINT socketIdx, int userID, void* pData, int size)
{
	if (size != sizeof(PlatformPHPRedSpotNotify))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RED_SPOT, NULL, 0, PF_SIZEERROR);
		return false;
	}

	PlatformPHPRedSpotNotify* pMessage = (PlatformPHPRedSpotNotify*)pData;
	if (!pMessage)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RED_SPOT, NULL, 0, PF_DATA_NULL);
		return false;
	}

	UserRedSpot userRedspot;
	if (!m_pRedis->GetUserRedSpot(userID, userRedspot))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RED_SPOT, NULL, 0, PF_REDSPOT_NOT_EXIST);
		return true;
	}

	if (pMessage->redspotType == PlatformPHPRedSpotNotify::PHP_REDSPOT_TYPE_EMAIL)
	{
		LogonNotifyEmailRedSpot msg;

		msg.notReadCount = userRedspot.notEMRead;
		msg.notReceivedCount = userRedspot.notEMReceived;

		SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(msg), MSG_MAIN_LOGON_NOTIFY, MSG_NTF_LOGON_EMAIL_REDSPOT, 0);
	}
	else if (pMessage->redspotType == PlatformPHPRedSpotNotify::PHP_REDSPOT_TYPE_FIREND)
	{
		LogonNotifyFriendRedSpot msg;

		msg.friendListRedSpotCount = userRedspot.friendList;
		msg.notifyListRedSpotCount = userRedspot.friendNotifyList;

		SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(msg), MSG_MAIN_LOGON_NOTIFY, MSG_NTF_LOGON_FRIEND_REDSPOT, 0);
	}
	else if (pMessage->redspotType == PlatformPHPRedSpotNotify::PHP_REDSPOT_TYPE_FG)
	{
		LogonFriendsGroupPushRedSpot msg;

		msg.notifyListRedSpotCount = userRedspot.FGNotifyList;

		int iSendSize = 8 + sizeof(LogonFriendsGroupPushRedSpot::MsgRedSpot) * msg.friendsGroupCount;
		SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, iSendSize, MSG_MAIN_FRIENDSGROUP_NOTIFY, MSG_NTF_LOGON_FRIENDSGROUP_REDSPOT, 0);
	}
	else
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RED_SPOT, NULL, 0, PF_SEND_DATA_ERR);
		return true;
	}

	SendData(socketIdx, PLATFORM_MESSAGE_RED_SPOT, NULL, 0, PF_SUCCESS);

	return true;
}

// ֪ͨ���ֲ�С���
bool CCenterServerManage::OnHandlePHPNotifyUserRedFGspotMessage(UINT socketIdx, int friendsGroupID, void* pData, int size)
{
	if (size != sizeof(PlatformPHPRedSpotNotify))
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RED_FG_SPOT, NULL, 0, PF_SIZEERROR);
		return false;
	}

	PlatformPHPRedSpotNotify* pMessage = (PlatformPHPRedSpotNotify*)pData;
	if (!pMessage)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RED_FG_SPOT, NULL, 0, PF_DATA_NULL);
		return false;
	}

	if (pMessage->redspotType != PlatformPHPRedSpotNotify::PHP_REDSPOT_TYPE_FG)
	{
		SendData(socketIdx, PLATFORM_MESSAGE_RED_FG_SPOT, NULL, 0, PF_DATA_NULL);
		return false;
	}

	m_pRedisPHP->GetFGMember(friendsGroupID, m_memberUserIDVec);

	if (m_memberUserIDVec.size() <= 0)
	{
		ERROR_LOG("���ֲ��޳�Ա friendsGroupID=%d", friendsGroupID);
		return false;
	}

	SendData(socketIdx, PLATFORM_MESSAGE_RED_FG_SPOT, NULL, 0, PF_SUCCESS);

	for (size_t i = 0; i < m_memberUserIDVec.size(); i++)
	{
		int userID = m_memberUserIDVec[i];
		if (!IsUserOnline(userID))
		{
			continue;
		}

		UserRedSpot userRedspot;
		if (!m_pRedis->GetUserRedSpot(userID, userRedspot))
		{
			continue;
		}

		LogonFriendsGroupPushRedSpot msg;

		msg.notifyListRedSpotCount = userRedspot.FGNotifyList;

		int iSendSize = 8 + sizeof(LogonFriendsGroupPushRedSpot::MsgRedSpot) * msg.friendsGroupCount;
		SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, iSendSize, MSG_MAIN_FRIENDSGROUP_NOTIFY, MSG_NTF_LOGON_FRIENDSGROUP_REDSPOT, 0);
	}

	return true;
}

// ������Ϸ����
bool CCenterServerManage::OnHandlePHPReloadGameConfigMessage(UINT socketIdx, void* pData, int size)
{
	SAFECHECK_MESSAGE(pMessage, PlatformPHPReloadGameConfig, pData, size);

	ServerBaseInfo serverInfo;
	serverInfo.serverType = SERVICE_TYPE_LOADER;
	serverInfo.serverID = pMessage->roomID;

	if (!SendData(&serverInfo, PLATFORM_MESSAGE_RELOAD_GAME_CONFIG, NULL, 0))
	{
		ERROR_LOG("����Ϸ����������ʧ�� roomID=%d", pMessage->roomID);
	}

	return true;
}

// �ֻ�ע����֤�ɹ���ע���˺�
bool CCenterServerManage::OnHandlePHPPhoneInfoMessage(UINT socketIdx, void* pData, int size)
{
	if (size != sizeof(PlatformPhoneInfo))
	{
		return false;
	}

	PlatformPhoneInfo* pMessage = (PlatformPhoneInfo*)pData;
	if (!pMessage)
	{
		return false;
	}

	INFO_LOG("phone=%s, passwd=%s, name=%s, headURL=%s", pMessage->phone, pMessage->passwd, pMessage->name, pMessage->headURL);

	if (strlen(pMessage->phone) >= sizeof(pMessage->phone) || strlen(pMessage->passwd) >= sizeof(pMessage->passwd)
		|| strlen(pMessage->name) >= sizeof(pMessage->name) || strlen(pMessage->headURL) >= sizeof(pMessage->headURL))
	{
		ERROR_LOG("�����쳣 phone=%s, passwd=%s, name=%s, headURL=%s", pMessage->phone, pMessage->passwd, pMessage->name, pMessage->headURL);
		return false;
	}

	UserData userData;

	memcpy(userData.phone, pMessage->phone, sizeof(userData.phone));
	memcpy(userData.name, pMessage->name, sizeof(userData.name));
	memcpy(userData.headURL, pMessage->headURL, sizeof(userData.headURL));
	userData.sex = pMessage->sex;
	strcpy(userData.passwd, pMessage->passwd);
	memcpy(userData.phonePasswd, userData.passwd, sizeof(userData.phonePasswd));

	OtherConfig otherConfig;
	if (!m_pRedis->GetOtherConfig(otherConfig))
	{
		otherConfig = ConfigManage()->GetOtherConfig();
	}
	userData.money = otherConfig.registerGiveMoney;
	userData.jewels = otherConfig.registerGiveJewels;
	userData.registerType = LOGON_TEL_PHONE;
	userData.registerTime = (int)time(NULL);

	SendData(m_logonGroupSocket[CUtil::GetRandNum() % m_logonGroupSocket.size()], PLATFORM_MESSAGE_PHONE_INFO, &userData, sizeof(userData));

	return true;
}

// ���˱��������˳�������ʵʱ����
bool CCenterServerManage::OnHandlePHPSignUpMatchMessage(UINT socketIdx, void*pData, int size)
{
	SAFECHECK_MESSAGE(pMessage, PlatformPHPSignUpMatchPeople, pData, size);

	for (size_t i = 0; i < m_logonGroupSocket.size(); i++)
	{
		SendData(m_logonGroupSocket[i], PLATFORM_MESSAGE_SIGN_UP_MATCH_PEOPLE, pData, size);
	}

	return true;
}

// ����ʼ����
bool CCenterServerManage::OnHandlePHPReqStartMatchMessage(UINT socketIdx, void*pData, int size)
{
	SAFECHECK_MESSAGE(pMessage, PlatformPHPReqStartMatchPeople, pData, size);

	GameBaseInfo * pGameBaseInfo = ConfigManage()->GetGameBaseInfo(pMessage->gameID);
	if (!pGameBaseInfo)
	{
		ERROR_LOG("û�����ø���Ϸ��gameID=%d", pMessage->gameID);
		return false;
	}

	if (pGameBaseInfo->deskPeople <= 1 || pMessage->peopleCount <= 1 || pMessage->peopleCount > MAX_MATCH_PEOPLE_COUNT)
	{
		ERROR_LOG("�������ô���,pGameBaseInfo->deskPeople=%d,pMessage->peopleCount=%d", pGameBaseInfo->deskPeople, pMessage->peopleCount);
		return false;
	}

	int iRound = 0;
	for (int i = 1; i <= MAX_MATCH_ROUND; i++)
	{
		if (pMessage->peopleCount == pGameBaseInfo->deskPeople * (int)pow(2.0, i - 1))
		{
			iRound = i;
			break;
		}
	}
	if (iRound == 0)
	{
		ERROR_LOG("������������ȷ,peopleCount=%d", pMessage->peopleCount);
		return false;
	}
	if (iRound != pMessage->matchRound)
	{
		ERROR_LOG("������������ȷ,pMessage->matchRound=%d,iRound=%d", pMessage->matchRound, iRound);
		return false;
	}

	std::vector<int> matchRoomVec;
	ConfigManage()->GetBuyRoomInfo(pMessage->gameID, ROOM_TYPE_MATCH, matchRoomVec);
	if (matchRoomVec.size() <= 0)
	{
		ERROR_LOG("û�����ñ�������gameID=%d", pMessage->gameID);
		return false;
	}

	//����һ��������
	int roomID = matchRoomVec[CUtil::GetRandNum() % matchRoomVec.size()];

	ServerBaseInfo serverInfo;
	serverInfo.serverType = SERVICE_TYPE_LOADER;
	serverInfo.serverID = roomID;

	if (!SendData(&serverInfo, PLATFORM_MESSAGE_START_MATCH_PEOPLE, pData, size))
	{
		ERROR_LOG("����Ϸ����������ʧ�� roomID=%d", roomID);
	}

	SendData(socketIdx, PLATFORM_MESSAGE_START_MATCH_PEOPLE, NULL, 0, PF_SUCCESS);

	return true;
}

// �������޸ġ�ɾ����һ����ʱ��
bool CCenterServerManage::OnHandlePHPReqModifyTimeMatchMessage(UINT socketIdx, void*pData, int size)
{
	SAFECHECK_MESSAGE(pMessage, PlatformPHPModifyTimeMatch, pData, size);

	INFO_LOG("������Ϣ��matchID=%d,gameID=%d,startTime=%lld,type=%d",
		pMessage->matchID, pMessage->gameID, pMessage->startTime, pMessage->type);

	if (pMessage->type == 0)
	{
		MatchInfo matchInfo;
		matchInfo.matchID = pMessage->matchID;
		matchInfo.gameID = pMessage->gameID;
		matchInfo.minPeople = pMessage->minPeople;
		matchInfo.costResType = pMessage->costResType;
		matchInfo.costResNums = pMessage->costResNums;
		matchInfo.startTime = pMessage->startTime;

		if (m_timeMatchInfoMap.count(matchInfo.startTime) > 0)
		{
			ERROR_LOG("����ʱ���ظ���time=%lld,matchID=%d,gameID=%d", matchInfo.startTime, matchInfo.matchID, matchInfo.gameID);
		}

		m_timeMatchInfoMap[matchInfo.startTime] = matchInfo;
	}
	else if (pMessage->type == 1)
	{
		auto itr = m_timeMatchInfoMap.find(pMessage->startTime);
		if (itr == m_timeMatchInfoMap.end())
		{
			ERROR_LOG("ɾ���ı�����Ϣ�����ڣ�time=%lld,matchID=%d,gameID=%d", pMessage->startTime, pMessage->matchID, pMessage->gameID);
			return false;
		}
		if (itr->second.matchID == pMessage->matchID)
		{
			m_timeMatchInfoMap.erase(itr);
		}
		else
		{
			ERROR_LOG("ɾ���ı�����Ϣ�����ڣ�time=%lld,matchID=%d,gameID=%d", itr->second.startTime, itr->second.matchID, itr->second.gameID);
		}
	}
	else if (pMessage->type == 2)
	{
		auto itr = m_timeMatchInfoMap.begin();
		for (; itr != m_timeMatchInfoMap.end(); itr++)
		{
			if (itr->second.matchID == pMessage->matchID)
			{
				break;
			}
		}

		if (itr == m_timeMatchInfoMap.end())
		{
			ERROR_LOG("�޸ĵı�����Ϣ�����ڣ�time=%lld,matchID=%d,gameID=%d", pMessage->startTime, pMessage->matchID, pMessage->gameID);
			return false;
		}

		//��ɾ��
		m_timeMatchInfoMap.erase(itr);

		//�����
		MatchInfo matchInfo;
		matchInfo.matchID = pMessage->matchID;
		matchInfo.gameID = pMessage->gameID;
		matchInfo.minPeople = pMessage->minPeople;
		matchInfo.costResType = pMessage->costResType;
		matchInfo.costResNums = pMessage->costResNums;
		matchInfo.startTime = pMessage->startTime;

		if (m_timeMatchInfoMap.count(matchInfo.startTime) > 0)
		{
			ERROR_LOG("����ʱ���ظ���time=%lld,matchID=%d,gameID=%d", pMessage->startTime, pMessage->matchID, pMessage->gameID);
		}

		m_timeMatchInfoMap[matchInfo.startTime] = matchInfo;
	}

	return true;
}

// ��ұ��������˳���������ʱ����
bool CCenterServerManage::OnHandlePHPTimeMatchPeopleChangeMessage(UINT socketIdx, void*pData, int size)
{
	SAFECHECK_MESSAGE(pMessage, PlatformPHPSignUpMatchTime, pData, size);

	for (size_t i = 0; i < m_logonGroupSocket.size(); i++)
	{
		SendData(m_logonGroupSocket[i], PLATFORM_MESSAGE_SIGN_UP_MATCH_TIME, pData, size);
	}

	return true;
}

////////////////////////////////����ͨ����Ϣ//////////////////////////////////////////
bool CCenterServerManage::OnHandleCommonMessage(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	return true;
}

// Ϊÿ����������socketIdx
bool CCenterServerManage::OnHandleCommonServerVerifyMessage(void* pData, UINT size, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	if (size != sizeof(PlatformCenterServerVerify))
	{
		return false;
	}

	PlatformCenterServerVerify* pMessage = (PlatformCenterServerVerify*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (!m_pRedis)
	{
		ERROR_LOG("m_pRedis is NULL");
		return false;
	}

	ServerBaseInfo serverInfo;
	serverInfo.serverType = pMessage->serverType;
	serverInfo.serverID = pMessage->serverID;

	if (m_serverToSocketMap.find(serverInfo) != m_serverToSocketMap.end())
	{
		ERROR_LOG("####��¼��ID�ظ�,pMessage->serverID = %d####", pMessage->serverID);
		SendData(uIndex, CENTER_MESSAGE_COMMON_REPEAT_ID, NULL, 0);
		return false;
	}

	if (m_socketToServerVec.size() != m_InitData.uMaxPeople)
	{
		WARNNING_LOG("#### �������������죬�ڴ滹û�з���,serverType=%d,serverID=%d,uIndex=%d,size=%d  ####",
			pMessage->serverType, pMessage->serverID, uIndex, m_socketToServerVec.size());
		m_socketToServerVec.resize(m_InitData.uMaxPeople);
	}

	m_socketToServerVec[uIndex] = serverInfo;
	m_serverToSocketMap.insert(std::make_pair(serverInfo, uIndex));

	//���͵�ǰ��������Ⱥ�����ͱ�ź��������Լ���Ҫ���������
	if (pMessage->serverType == SERVICE_TYPE_LOGON)   //��¼��
	{
		m_pRedis->SetLogonServerStatus(serverInfo.serverID, 1);

		m_logonGroupSocket.push_back(uIndex);

		SendDistributedSystemInfo();
	}
	else if (pMessage->serverType == SERVICE_TYPE_LOADER)//��Ϸ��
	{
		m_pRedis->SetRoomServerStatus(pMessage->serverID, 1);
	}

	INFO_LOG("====== serverType:%d,serverID:%d ���뼯Ⱥϵͳ ��¼����Ⱥ����:%d=========", serverInfo.serverType, serverInfo.serverID, m_logonGroupSocket.size());

	return true;
}

////////////////////////////////�����������Ϣ//////////////////////////////////////////
bool CCenterServerManage::OnHandleLogonMessage(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	switch (pCenterHead->msgID)
	{
	case CENTER_MESSAGE_LOGON_RESOURCE_CHANGE:
	{
		return OnHandleLogonResChangeMessage(pCenterHead->userID, pData, uSize);
	}
	case CENTER_MESSAGE_LOGON_USER_LOGON_OUT:
	{
		return OnHandleLogonUserStatusMessage(pData, uSize, uAccessIP, uIndex, dwHandleID);
	}
	case CENTER_MESSAGE_LOGON_REPEAT_USER_LOGON:
	{
		return OnHandleLogonRepeatUserMessage(pCenterHead, pData, uSize, uAccessIP, uIndex, dwHandleID);
	}
	case CENTER_MESSAGE_LOGON_RELAY_USER_MSG:
	{
		return OnHandleLogonRelayUserMessage(pNetHead, pData, uSize, pCenterHead->userID);
	}
	case CENTER_MESSAGE_LOGON_RELAY_FG_MSG:
	{
		return OnHandleLogonRelayFGMessage(pNetHead, pData, uSize, pCenterHead->userID);
	}
	case CENTER_MESSAGE_LOGON_REQ_FG_REDSPOT_MSG:
	{
		return OnHandleLogonReqFGRedSpotMessage(pData, uSize, pCenterHead->userID);
	}
	case CENTER_MESSAGE_LOGON_MASTER_DISSMISS_DESK:
	{
		return OnHandleLogonReqDissmissDeskessage(pData, uSize);
	}
	default:
	{
		WARNNING_LOG("���ķ���Ϣ������:pCenterHead->msgID = %d", pCenterHead->msgID);
		break;
	}
	}

	return true;
}

// ��½����Դ�仯
bool CCenterServerManage::OnHandleLogonResChangeMessage(int userID, void* pData, UINT size)
{
	if (size != sizeof(PlatformResourceChange))
	{
		return false;
	}

	PlatformResourceChange* pMessage = (PlatformResourceChange*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (!IsUserOnline(userID))
	{
		return false;
	}

	LogonNotifyResourceChange msg;

	msg.resourceType = pMessage->resourceType;
	msg.value = pMessage->value;
	msg.reason = pMessage->reason;
	msg.changeValue = pMessage->changeValue;

	SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(LogonNotifyResourceChange), MSG_MAIN_LOGON_NOTIFY, MSG_NTF_LOGON_RESOURCE_CHANGE, 0);

	return true;
}

// ���������
bool CCenterServerManage::OnHandleLogonUserStatusMessage(void* pData, UINT size, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	if (size != sizeof(PlatformUserLogonLogout))
	{
		return false;
	}

	PlatformUserLogonLogout* pMessage = (PlatformUserLogonLogout*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (uIndex >= m_socketToServerVec.size())
	{
		ERROR_LOG("#### redis�Ƴ��������ʧ�� m_socketToServerVec�е�½������������ uIndex = %d ####", uIndex);
		return false;
	}
	if (m_socketToServerVec[uIndex].serverType != SERVICE_TYPE_LOGON)
	{
		ERROR_LOG("#### redis�Ƴ��������ʧ�� serverType=%d ����####", m_socketToServerVec[uIndex].serverType);
		return false;
	}

	if (pMessage->type == 0) //����
	{
		CenterUserInfo userInfo;
		userInfo.userID = pMessage->userID;
		userInfo.socketIdx = uIndex;
		userInfo.isVirtual = pMessage->isVirtual;

		m_centerUserInfoMap[pMessage->userID] = userInfo;
	}
	else  //ɾ��
	{
		auto itr = m_centerUserInfoMap.find(pMessage->userID);
		if (itr == m_centerUserInfoMap.end())
		{
			ERROR_LOG("ɾ����Ҳ����� pMessage->userID = %d", pMessage->userID);
			return true;
		}
		if (itr->second.socketIdx == uIndex)
		{
			m_centerUserInfoMap.erase(itr);
			m_pRedis->RemoveOnlineUser(pMessage->userID, pMessage->isVirtual);
		}
		else
		{
			WARNNING_LOG("�¾ɷ�������һ��:��socketIdx=%d, ��uIndex=%d", itr->second.socketIdx, uIndex);
		}
	}

	return true;
}

// ��������¼���������ݣ��ߵ��ɷ��������
bool CCenterServerManage::OnHandleLogonRepeatUserMessage(CenterServerMessageHead * pCenterHead, void* pData, UINT size, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	if (size != 0)
	{
		ERROR_LOG("Э�鲻��");
		return false;
	}

	auto itr = m_centerUserInfoMap.find(pCenterHead->userID);
	if (itr == m_centerUserInfoMap.end())
	{
		ERROR_LOG("�߾����ʧ�ܣ�userID = %d", pCenterHead->userID);
		return false;
	}

	if (itr->second.socketIdx == uIndex)
	{
		ERROR_LOG("�߾����ʧ�ܣ��¾ɷ�����һ�£�userID = %d", pCenterHead->userID);
		return false;
	}

	PlatformRepeatUserLogon msg;
	msg.userID = pCenterHead->userID;

	SendData(itr->second.socketIdx, CENTER_MESSAGE_LOGON_REPEAT_USER_LOGON, &msg, sizeof(msg));

	return true;
}

// ת����Ϣ��ĳ����
bool CCenterServerManage::OnHandleLogonRelayUserMessage(NetMessageHead * pNetHead, void* pData, UINT size, int userID)
{
	return SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, pData, size, pNetHead->uMainID, pNetHead->uAssistantID, pNetHead->uHandleCode);
}

// ת����Ϣ�����ֲ�
bool CCenterServerManage::OnHandleLogonRelayFGMessage(NetMessageHead * pNetHead, void* pData, UINT size, int friendsGroupID)
{
	if (!m_pRedisPHP)
	{
		ERROR_LOG("m_pRedisPHP = NULL");
		return false;
	}

	m_pRedisPHP->GetFGMember(friendsGroupID, m_memberUserIDVec);

	if (m_memberUserIDVec.size() <= 0)
	{
		ERROR_LOG("���ֲ��޳�Ա friendsGroupID=%d", friendsGroupID);
		return false;
	}

	for (size_t i = 0; i < m_memberUserIDVec.size(); i++)
	{
		SendData(m_memberUserIDVec[i], PLATFORM_MESSAGE_NOTIFY_USERID, pData, size, pNetHead->uMainID, pNetHead->uAssistantID, pNetHead->uHandleCode);
	}

	return true;
}

// ��������ֲ�������ҷ���С���
bool CCenterServerManage::OnHandleLogonReqFGRedSpotMessage(void* pData, UINT size, int friendsGroupID)
{
	if (size != sizeof(PlatformFriendsGroupPushRedSpot))
	{
		return false;
	}

	PlatformFriendsGroupPushRedSpot* pMessage = (PlatformFriendsGroupPushRedSpot*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (!m_pRedisPHP)
	{
		ERROR_LOG("m_pRedisPHP = NULL");
		return false;
	}

	m_pRedisPHP->GetFGMember(friendsGroupID, m_memberUserIDVec);

	if (m_memberUserIDVec.size() <= 0)
	{
		ERROR_LOG("���ֲ��޳�Ա friendsGroupID=%d", friendsGroupID);
		return false;
	}

	LogonFriendsGroupPushRedSpot msg;
	msg.friendsGroupCount = 1;
	msg.msgRedSpot[0].friendsGroupID = pMessage->friendsGroupID;
	msg.msgRedSpot[0].deskMsgRedSpotCount = pMessage->deskMsgRedSpotCount;
	msg.msgRedSpot[0].VIPRoomMsgRedSpotCount = pMessage->VIPRoomMsgRedSpotCount;
	int iSendSize = 8 + sizeof(LogonFriendsGroupPushRedSpot::MsgRedSpot) * msg.friendsGroupCount;

	for (size_t i = 0; i < m_memberUserIDVec.size(); i++)
	{
		SendData(m_memberUserIDVec[i], PLATFORM_MESSAGE_NOTIFY_USERID, &msg, iSendSize, MSG_MAIN_FRIENDSGROUP_NOTIFY, MSG_NTF_LOGON_FRIENDSGROUP_REDSPOT, 0);
	}

	return true;
}

// �����ɢ����
bool CCenterServerManage::OnHandleLogonReqDissmissDeskessage(void* pData, UINT size)
{
	if (size != sizeof(PlatformDissmissDesk))
	{
		return false;
	}

	PlatformDissmissDesk* pMessage = (PlatformDissmissDesk*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (pMessage->deskMixID <= 0 || pMessage->roomID <= 0)
	{
		ERROR_LOG("���䲻����,deskMixID=%d,roomID=%d", pMessage->deskMixID, pMessage->roomID);
		return false;
	}

	ServerBaseInfo serverInfo;
	serverInfo.serverType = SERVICE_TYPE_LOADER;
	serverInfo.serverID = pMessage->roomID;

	return SendData(&serverInfo, PLATFORM_MESSAGE_MASTER_DISSMISS_DESK, pData, size);
}

////////////////////////////////������Ϸ����Ϣ//////////////////////////////////////////
bool CCenterServerManage::OnHandleLoaderMessage(NetMessageHead * pNetHead, CenterServerMessageHead * pCenterHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	switch (pCenterHead->msgID)
	{
	case CENTER_MESSAGE_LOADER_RESOURCE_CHANGE:
	{
		return OnHandleLoaderResChangeMessage(pCenterHead->userID, pData, uSize);
	}
	case CENTER_MESSAGE_LOADER_BUYDESKINFO_CHANGE:
	{
		return OnHandleLoaderBuyDeskInfoChangeMessage(pData, uSize);
	}
	case CENTER_MESSAGE_LOADER_DESK_DISSMISS:
	{
		return OnHandleLoaderDeskDissmissMessage(pData, uSize);
	}
	case CENTER_MESSAGE_LOADER_DESK_STATUS_CHANGE:
	{
		return OnHandleLoaderDeskStatusChangeMessage(pData, uSize);
	}
	case CENTER_MESSAGE_LOADER_FIRECOIN_CHANGE:
	{
		return OnHandleLoaderFireCoinChangeMessage(pCenterHead->userID, pData, uSize);
	}
	case CENTER_MESSAGE_LOADER_REWARD_ACTIVITY:
	{
		return OnHandleLoaderRewardMessage(pData, uSize);
	}
	case CENTER_MESSAGE_LOADER_NOTIFY_START_MATCH:
	{
		return OnHandleLoaderNotifyStartMatchMessage(pData, uSize);
	}
	case CENTER_MESSAGE_LOADER_NOTIFY_START_MATCH_FAIL:
	{
		return OnHandleLoaderNotifyStartMatchFailMessage(pCenterHead->userID, pData, uSize);
	}
	default:
		WARNNING_LOG("���ķ���Ϣ������:pCenterHead->msgID = %d", pCenterHead->msgID);
		break;
	}

	return true;
}

// ��Ϸ����Դ�仯
bool CCenterServerManage::OnHandleLoaderResChangeMessage(int userID, void* pData, UINT size)
{
	if (size != sizeof(PlatformResourceChange))
	{
		return false;
	}

	PlatformResourceChange* pMessage = (PlatformResourceChange*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (!IsUserOnline(userID))
	{
		return true;
	}

	LogonNotifyResourceChange msg;

	msg.resourceType = pMessage->resourceType;
	msg.value = pMessage->value;
	msg.reason = pMessage->reason;
	msg.changeValue = pMessage->changeValue;

	SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(LogonNotifyResourceChange), MSG_MAIN_LOGON_NOTIFY, MSG_NTF_LOGON_RESOURCE_CHANGE, 0);

	return true;
}

// �����б���Ϣ�仯(�����仯)
bool CCenterServerManage::OnHandleLoaderBuyDeskInfoChangeMessage(void* pData, UINT size)
{
	if (size != sizeof(PlatformDeskPeopleCountChange))
	{
		return false;
	}

	PlatformDeskPeopleCountChange* pMessage = (PlatformDeskPeopleCountChange*)pData;
	if (!pMessage)
	{
		return false;
	}

	int masterID = pMessage->masterID;
	LogonNotifyBuyDeskInfoChange msg;

	msg.changeType = LogonNotifyBuyDeskInfoChange::CHANGE_REASON_USERCOUNT_CHANGE;
	msg.currUserCount = pMessage->currUserCount;
	memcpy(msg.passwd, pMessage->roomPasswd, sizeof(msg.passwd));

	SendData(masterID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(msg), MSG_MAIN_LOGON_NOTIFY, MSG_NTF_LOGON_BUYDESKINFO_CHANGE, 0);

	if (!m_pRedisPHP)
	{
		ERROR_LOG("m_pRedisPHP = NULL");
		return false;
	}

	if (pMessage->friendsGroupID <= 0)
	{
		return true;
	}

	m_pRedisPHP->GetFGMember(pMessage->friendsGroupID, m_memberUserIDVec);

	if (m_memberUserIDVec.size() <= 0)
	{
		ERROR_LOG("���ֲ��޳�Ա friendsGroupID=%d", pMessage->friendsGroupID);
		return false;
	}

	// ����ֲ����г�Ա������Ϣ
	LogonFriendsGroupDeskInfoChange msgChange;
	msgChange.friendsGroupID = pMessage->friendsGroupID;
	msgChange.operType = 0;
	msgChange.friendsGroupDeskNumber = pMessage->friendsGroupDeskNumber;
	msgChange.curPeopleCount = pMessage->currUserCount;
	msgChange.updateType = pMessage->updateType;
	msgChange.userID = pMessage->userID;
	UINT iMessageID = pMessage->friendsGroupDeskNumber > MAX_FRIENDSGROUP_DESK_LIST_COUNT
		? MSG_NTF_LOGON_FRIENDSGROUP_VIPROOM_CHANGE : MSG_NTF_LOGON_FRIENDSGROUP_DESK_INFO_CHANGE;

	for (size_t i = 0; i < m_memberUserIDVec.size(); i++)
	{
		SendData(m_memberUserIDVec[i], PLATFORM_MESSAGE_NOTIFY_USERID, &msgChange, sizeof(msgChange), MSG_MAIN_FRIENDSGROUP_NOTIFY, iMessageID, 0);
	}

	return true;
}

// ��Ϸ�����
bool CCenterServerManage::OnHandleLoaderDeskDissmissMessage(void* pData, UINT size)
{
	if (size != sizeof(PlatformDeskDismiss))
	{
		return false;
	}

	PlatformDeskDismiss* pMessage = (PlatformDeskDismiss*)pData;
	if (!pMessage)
	{
		return false;
	}

	// �򿪷��б�����������
	int masterID = pMessage->masterID;
	LogonNotifyBuyDeskInfoChange msg;

	msg.changeType = LogonNotifyBuyDeskInfoChange::CHANGE_REASON_DEL;
	memcpy(msg.passwd, pMessage->passwd, sizeof(msg.passwd));

	SendData(masterID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(msg), MSG_MAIN_LOGON_NOTIFY, MSG_NTF_LOGON_BUYDESKINFO_CHANGE, 0);

	if (pMessage->friendsGroupID <= 0)
	{
		return true;
	}

	//���¾��ֲ���Ϣ
	int friendsGroupID = pMessage->friendsGroupID;       //���ֲ�id
	int friendsGroupDeskNumber = pMessage->friendsGroupDeskNumber; //���ֲ����Ӻ�

	if (!m_pRedisPHP)
	{
		ERROR_LOG("m_pRedisPHP = NULL");
		return false;
	}

	m_pRedisPHP->GetFGMember(friendsGroupID, m_memberUserIDVec);

	if (m_memberUserIDVec.size() <= 0)
	{
		ERROR_LOG("���ֲ��޳�Ա friendsGroupID=%d", friendsGroupID);
		return false;
	}

	// ������������
	LogonFriendsGroupDeskInfoChange msgChange;
	msgChange.friendsGroupID = friendsGroupID;
	msgChange.operType = 1;
	msgChange.friendsGroupDeskNumber = friendsGroupDeskNumber;

	//���͸��������߳�Ա
	UINT iMessageID = friendsGroupDeskNumber > MAX_FRIENDSGROUP_DESK_LIST_COUNT
		? MSG_NTF_LOGON_FRIENDSGROUP_VIPROOM_CHANGE : MSG_NTF_LOGON_FRIENDSGROUP_DESK_INFO_CHANGE;
	for (size_t i = 0; i < m_memberUserIDVec.size(); i++)
	{
		SendData(m_memberUserIDVec[i], PLATFORM_MESSAGE_NOTIFY_USERID, &msgChange, sizeof(msgChange), MSG_MAIN_FRIENDSGROUP_NOTIFY, iMessageID, 0);
	}

	//��Ҫ�Զ�������������½�������Զ�����
	if (!pMessage->bDeleteDesk)
	{
		if (m_logonGroupSocket.size() <= 0)
		{
			ERROR_LOG("�Զ�����ʧ�ܣ�û�е�¼������");
			return false;
		}

		PlatformAutoCreateRoom autoCreateMsg;

		autoCreateMsg.masterID = masterID;
		autoCreateMsg.friendsGroupID = friendsGroupID;
		autoCreateMsg.friendsGroupDeskNumber = friendsGroupDeskNumber;
		autoCreateMsg.roomType = pMessage->roomType;
		autoCreateMsg.gameID = pMessage->gameID;
		autoCreateMsg.maxCount = pMessage->maxCount;
		memcpy(autoCreateMsg.gameRules, pMessage->gameRules, sizeof(autoCreateMsg.gameRules));

		UINT uSendSocketIdx = 0;
		auto itr = m_centerUserInfoMap.find(masterID);
		if (itr == m_centerUserInfoMap.end()) // �������ڣ����ѡ�������
		{
			uSendSocketIdx = m_logonGroupSocket[CUtil::GetRandNum() % m_logonGroupSocket.size()];
		}
		else // �������ڣ��Զ��������񽻸������ĵ�½��
		{
			uSendSocketIdx = itr->second.socketIdx;
		}

		SendData(uSendSocketIdx, CENTER_MESSAGE_COMMON_AUTO_CREATEROOM, &autoCreateMsg, sizeof(autoCreateMsg));
	}

	return true;
}

// �����仯
bool CCenterServerManage::OnHandleLoaderDeskStatusChangeMessage(void* pData, UINT size)
{
	if (size != sizeof(PlatformDeskGameCountChange))
	{
		return false;
	}

	PlatformDeskGameCountChange* pMessage = (PlatformDeskGameCountChange*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (!m_pRedisPHP)
	{
		ERROR_LOG("m_pRedisPHP = NULL");
		return false;
	}

	if (pMessage->friendsGroupID <= 0)
	{
		return true;
	}

	m_pRedisPHP->GetFGMember(pMessage->friendsGroupID, m_memberUserIDVec);

	if (m_memberUserIDVec.size() <= 0)
	{
		ERROR_LOG("���ֲ��޳�Ա friendsGroupID=%d", pMessage->friendsGroupID);
		return false;
	}

	//��������
	LogonFriendsGroupDeskInfoChange msg;
	msg.friendsGroupID = pMessage->friendsGroupID;
	msg.operType = 0;
	msg.friendsGroupDeskNumber = pMessage->friendsGroupDeskNumber;
	msg.gameStatus = pMessage->gameStatus;
	msg.updateType = 2;

	//���͸��������߳�Ա
	UINT iMessageID = pMessage->friendsGroupDeskNumber > MAX_FRIENDSGROUP_DESK_LIST_COUNT
		? MSG_NTF_LOGON_FRIENDSGROUP_VIPROOM_CHANGE : MSG_NTF_LOGON_FRIENDSGROUP_DESK_INFO_CHANGE;

	for (size_t i = 0; i < m_memberUserIDVec.size(); i++)
	{
		SendData(m_memberUserIDVec[i], PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(msg), MSG_MAIN_FRIENDSGROUP_NOTIFY, iMessageID, 0);
	}

	return true;
}

// ��ұ仯֪ͨ
bool CCenterServerManage::OnHandleLoaderFireCoinChangeMessage(int userID, void* pData, UINT size)
{
	if (size != sizeof(PlatformResourceChange))
	{
		return false;
	}

	PlatformResourceChange* pMessage = (PlatformResourceChange*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (!IsUserOnline(userID))
	{
		return true;
	}

	LogonFriendsGroupUserFireCoinChange msg;

	msg.friendsGroupID = pMessage->reserveData;
	msg.user[0].userID = userID;
	msg.user[0].newFireCoin = (int)pMessage->value;

	SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(msg), MSG_MAIN_FRIENDSGROUP_NOTIFY, MSG_NTF_LOGON_FRIENDSGROUP_FIRECOIN_CHANGE, 0);

	return true;
}

// ��֪ͨ
bool CCenterServerManage::OnHandleLoaderRewardMessage(void* pData, UINT size)
{
	SAFECHECK_MESSAGE(pMessage, LogonNotifyActivity, pData, size);

	for (size_t i = 0; i < m_logonGroupSocket.size(); i++)
	{
		SendData(m_logonGroupSocket[i], CENTER_MESSAGE_LOADER_REWARD_ACTIVITY, pData, size);
	}

	return true;
}

// ��ʼ����
bool CCenterServerManage::OnHandleLoaderNotifyStartMatchMessage(void* pData, UINT size)
{
	SAFECHECK_MESSAGE(pMessage, PlatformLoaderNotifyStartMatch, pData, size);

	for (size_t i = 0; i < m_logonGroupSocket.size(); i++)
	{
		SendData(m_logonGroupSocket[i], CENTER_MESSAGE_LOADER_NOTIFY_START_MATCH, pData, size);
	}

	return true;
}

// ��ʼ����ʧ��
bool CCenterServerManage::OnHandleLoaderNotifyStartMatchFailMessage(int userID, void* pData, UINT size)
{
	SAFECHECK_MESSAGE(pMessage, PlatformLoaderNotifyStartMatchFail, pData, size);

	LogonStartMatchFail msg;

	msg.gameID = pMessage->gameID;
	msg.matchID = pMessage->matchID;
	msg.matchType = pMessage->matchType;
	msg.reason = pMessage->reason;

	SendData(userID, PLATFORM_MESSAGE_NOTIFY_USERID, &msg, sizeof(msg), MSG_MAIN_MATCH_NOTIFY, MSG_NTF_LOGON_MATCH_NOTIFY_START_MATCH_FAIL, 0);

	return true;
}