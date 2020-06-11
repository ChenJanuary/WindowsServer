#pragma once

#include <list>

// ������Ϸ�е�һЩ����
const int AREA_CARD_COUNT = 5;					// ÿ��λ���Ƶ�����
const int MAX_AREA_COUNT = 4;					// ������
const int MAX_SHANGZHUANG_COUNT = 5;			// ��ׯ�б��������
const int MAX_NOTE_SORT = 6;					// ����������
const int MAX_AREA_TREND_COUNT = 10;			// ���������������
const int MAX_AREA_CARD_TYPE = 5;				// ������������
const int MAX_GOLD_ROOM_COUNT = 5;				// ��ҳ��������
const int MAX_SHAPE_RATE = 4;					// ������ͷ���
const int MAX_DESK_USER = 6;					// �����λ��
const int MAX_CHOU_MA_COUNT = 100;				// ���ĳ�������

// ��Ϸ���ýṹ
struct GameConfig
{
	bool randBankerCount;
	BYTE randCount;						//�����ׯ����
	BYTE CanSitCount;					//��������������¶�����
	BYTE GamePlayCount_1;			//������Ϸ�������жϻ�������������
	BYTE GamePlayCount_2;			//������Ϸ�������жϻ�������������
	BYTE GamePlayCount_3;			//������Ϸ�������жϻ�������������

	int waitBeginTime;					// �ȴ���ʼʱ��
	int noteKeepTime;					// ��ע�׶γ���ʱ��
	int sendCardKeepTime;				// ���ƽ׶γ���ʱ��
	int compareKeepTime;				// ���ƽ׶γ���ʱ��
	int waitSettleTime;					// �ȴ�����ʱ��
	int maxZhuangGameCount;				// ׯ�������еľ���
	int taxRate;						// ��ˮ
	int maxDeskCount;					// �����λ��
	long long shangZhuangLimitMoney;
	long long sitLimitMoney;			// ����������Ҫ���
	long long noteList[MAX_GOLD_ROOM_COUNT][MAX_NOTE_SORT];
	long long allUserMaxNote;			// �������(����������)���ܹ�����ע����

	GameConfig()
	{
		noteList[0][0] = 10;
		noteList[0][1] = 20;
		noteList[0][2] = 50;
		noteList[0][3] = 100;
		noteList[0][4] = 200;
		noteList[0][5] = 400;

		noteList[1][0] = 100;
		noteList[1][1] = 500;
		noteList[1][2] = 1000;
		noteList[1][3] = 2000;
		noteList[1][4] = 5000;
		noteList[1][5] = 10000;

		noteList[2][0] = 500;
		noteList[2][1] = 1000;
		noteList[2][2] = 10000;
		noteList[2][3] = 50000;
		noteList[2][4] = 100000;
		noteList[2][5] = 200000;

		randBankerCount = true;
		randCount = 1;
		CanSitCount = 4;
		shangZhuangLimitMoney = 1000000;
		waitBeginTime = 10;
		noteKeepTime = 30;
		sendCardKeepTime = 8;
		compareKeepTime = 5;
		waitSettleTime = 3;
		maxZhuangGameCount = 1;
		taxRate = 5;
		maxDeskCount = 0;
		sitLimitMoney = 5000;
		allUserMaxNote = 0;
	}
};

// �����Ϣ
struct BrnnUserInfo
{
	long userID;
	bool isNote;//����Ƿ���ע
	bool isContinueNote;//�Ƿ���ѹ
	long long noteList[MAX_AREA_COUNT];
	long long lastNoteList[MAX_AREA_COUNT];

	BrnnUserInfo()
	{
		clear();
	}

	void clearNoteInfo()
	{
		isContinueNote = false;
		isNote = false;
		memset(noteList, 0, sizeof(noteList));
	}

	void clear()
	{
		memset(this, 0, sizeof(BrnnUserInfo));
	}
};

// ׯ����Ϣ
struct DealerInfo
{
	bool isSystem;		// �Ƿ�ϵͳ��ׯ	
	long userID;
	BYTE cardList[AREA_CARD_COUNT];

	DealerInfo()
	{
		memset(this, 0, sizeof(DealerInfo));
	}

	void Clear()
	{
		memset(this, 0, sizeof(DealerInfo));
	}
};

// ������Ϣ
struct AreaInfo
{
	long long areaNote;
	long long areaRealPeopleNote;	//������עֵ
	BYTE cardList[AREA_CARD_COUNT];
	int currWinRate;				// ��ǰ��õı��ʣ�������Ϊ����
	std::list<int> resultList;		// ��������ֻ���������10��
	AreaInfo()
	{
		Clear();
	}

	void ClearInfo()
	{
		areaNote = 0;
		areaRealPeopleNote = 0;
		memset(cardList, 255, sizeof(cardList));
		currWinRate = 0;
	}

	void Clear()
	{
		areaNote = 0;
		areaRealPeopleNote = 0;
		memset(cardList, 255, sizeof(cardList));
		currWinRate = 0;
	}

	void SetResult(bool isWin)
	{
		if (resultList.size() >= MAX_AREA_TREND_COUNT)
		{
			resultList.pop_back();
		}

		resultList.push_front(isWin);
	}
};

struct ShangZhuangUser
{
	int userID;
	long long money;
	BYTE deskStation;
	ShangZhuangUser()
	{
		memset(this, 0, sizeof(ShangZhuangUser));
		deskStation = 255;
	}
	ShangZhuangUser(int userID, long long money, BYTE deskStation)
	{
		this->userID = userID;
		this->money = money;
		this->deskStation = deskStation;
	}
};

// ��Ӯ�������
struct CtrlSystemParam 
{
	int winCount;
	int allCount;
	CtrlSystemParam()
	{
		winCount = 5;
		allCount = 10;
	}
};

// ��Ϸ״̬
enum GameStatus
{
	GAME_STATUS_WAITNEXT = 1,		// �ȴ��¾ֿ�ʼ
	GAME_STATUS_NOTE,				// ��ע�׶�
	GAME_STATUS_SENDCARD,			// ���ƽ׶�
	GAME_STATUS_COMPARE,			// ���ƽ׶�
	GAME_STATUS_SETTLE,				// ����׶�
};

// ��ʱ��
enum GameTimer
{
	GAME_TIMER_DEFAULT = 40,
	GAME_TIMER_WAITNEXT,
	GAME_TIMER_NOTE,				// ��ע�׶ζ�ʱ��
	GAME_TIMER_NOTEINFO,			// ������ע��Ϣ��ʱ��
	GAME_TIMER_SENDCARD,			// ���ƶ�ʱ��
	GAME_TIMER_COMPARE,				// ���ƽ׶ζ�ʱ��
	GAME_TIMER_SETTLE,				// ����׶ζ�ʱ��
	GAME_TIMER_END,
};

#define SYSTEM_DEALER_DESKSTATION	254		// ϵͳ��ׯ