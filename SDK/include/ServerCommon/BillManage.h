#pragma once

#include "DataBase.h"
#include "NewMessageDefine.h"

// ���Ӽ�¼��
#define TBL_STATI_LOGON_LOGOUT			"statistics_logonandlogout"
#define TBL_STATI_USER_MONEY_CHANGE		"statistics_moneychange"
#define TBL_STATI_USER_JEWELS_CHANGE	"statistics_jewelschange"
#define TBL_STATI_GAME_RECORD_INFO		"statistics_gamerecordinfo"
#define TBL_STATI_REWARDS_POOL			"statistics_rewardspool"
#define TBL_FG_RECORD_ACCOUNTS			"friendsGroupAccounts"
#define TBL_FG_COST_ACCOUNTS			"friendsGroupDeskListCost"
#define TBL_STATI_USER_FIRECOIN_CHANGE	"statistics_firecoinchange"
#define TBL_STATI_ROOM_WIN_COUNT		"statistics_roomwincount"

// ҵ����¼
#define TBL_WEB_AGENT_PUMP_JEWELS		"web_agent_pump_jewels"
#define TBL_WEB_AGENT_PUMP_MONEY		"web_agent_pump_money"

// �˵�������
class KERNEL_CLASS CBillManage
{
private:
	CBillManage();
	~CBillManage();

public:
	static CBillManage* Instance();
	void Release();

	void WriteUserBill(int userID, int billType, const char* pFormat, ...);

	void WriteCommonBill(int billType, const char* pFormat, ...);

	void WriteBill(CDataBaseManage* m_pDBManage, const char* pFormat, ...);
};

#define BillManage()	CBillManage::Instance()