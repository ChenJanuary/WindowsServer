#pragma once
#include "GameDesk.h"
#include "GameTaskLogic.h"
#include "json/json.h"
#include "fstream"
#pragma comment(lib,"json_vc71_libmt.lib")

//��ʱ�� ID
#define IDT_USER_CUT				1L										//���߶�ʱ�� ID

#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIME_SEND_ALL_CARD			31				//��������
#define TIME_SEND_CARD_ANI      	32				//�ȴ�����
#define TIME_SEND_CARD_FINISH       33              //���ƽ���
#define TIME_ROB_NT					34				//������
#define TIME_ADD_DOUBLE				35				//�ӱ�
#define TIME_OUT_CARD				36				//����
#define TIME_WAIT_NEWTURN			37				//��һ��ʱ������
#define TIME_GAME_FINISH			38				//��Ϸ������ʱ��
#define TIME_JIAO_FEN               39              //�зֶ�ʱ��
#define TIME_SHOW_CARD              40              //����
#define TIME_START_GAME				41				//������ʼʱ������
//��Ϸ������־����
#define GF_NO_CALL_SCORE			15				//���˽з�
#define GF_AHEAD_END				16				//��ǰ����

//*****************************************************************************************
//��Ϸ����
class CServerGameDesk : public CGameDesk
{
	//��̬����
private:
	bool  m_bHaveKing;      			//�Ƿ�����(DEF=1,0)
	BOOL  m_bKingCanReplace;			//���ɷ������(DEF=1,0)
	int	  m_iModel;						//���������ǽз�ģʽ(1:�з�ģʽ,0:������ģʽ)
	bool  m_bRobnt;         			//�Ƿ����������
	bool  m_bAdddouble;    				//�Ƿ���Լӱ�
	bool  m_bShowcard;     				//�Ƿ��������
	UINT  m_iPlayCard;     				//�����˿˸���(1,DEF=2,3
	UINT  m_iPlayCount;    				//ʹ���˿���(52,54,104,DEF=108)
	UINT  m_iSendCount;    				//������(48,51,DEF=100,108)
	UINT  m_iBackCount;    				//������(3,6,DEF=8,12)
	UINT  m_iUserCount;    				//�����������(12,13,DEF=25,27)
	DWORD m_iCardShape;	  				//����
	//=============��չ
	BYTE m_iThinkTime;               	//��Ϸ˼��ʱ��
	BYTE m_iBeginTime;               	//��Ϸ��ʼʱ��
	BYTE m_iSendCardTime;           	//����ʱ��
	BYTE m_iCallScoreTime;           	//�з�ʱ��
	BYTE m_iRobNTTime;              	//������ʱ��
	BYTE m_iAddDoubleTime;           	//�ӱ�ʱ��
	bool m_bTurnRule;					//��Ϸ˳��
	int	 m_iTimeOutNoCardOut;			//����ʱ���Ƿ���Բ����ƣ�1Ϊ���Բ����ƣ�
	bool m_bNoXiPai;								//�Ƿ�ϴ�ƣ���ϴ���ƺܺ�
protected:
	CGameTaskLogic          m_Logic;                           //��Ϸ�е��߼������������߼���
	GameMutipleStruct       m_GameMutiple;                     //��Ϸ�еı���

	int                     m_iBaseMult;					    //��Ϸ����
	int                     m_iAddDoubleLimit;                  //�ӱ�����
	int                     m_iGameMaxLimit;                   //���������Ӯ 
	int                     m_iLimitPoint;                     //��Ϸ�����
	int                     m_iGameBaseScore;                   //��Ϸ�׷֣������ݿ��л��

	int  	                m_iLimitPlayGame;					//���ٴ�����پ�
	BYTE					m_iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	BYTE					m_iLeaveArgee;						//�뿪ͬ���־
	//״̬��Ϣ
	BYTE					m_iUserCardCount[PLAY_COUNT];		//�û������˿���Ŀ
	BYTE					m_iUserCard[PLAY_COUNT][45];		//�û����ϵ��˿�
	BYTE					m_iBackCard[12];					//�����б�
	BYTE					m_iBaseOutCount;					//���Ƶ���Ŀ
	BYTE					m_iDeskCardCount[PLAY_COUNT];		//�����˿˵���Ŀ
	BYTE					m_iDeskCard[PLAY_COUNT][45];		//������˿�
	//��������
	BYTE					m_iSendCardPos;						//����λ��

	BYTE					m_iHaveThingPeople;					//����Ҫ�����
	BYTE					m_iGameFlag;						//��Ϸ״̬С�ֽ�

	bool					m_bIsLastCard;						//�Ƿ�����������
	BYTE					m_iLastCardCount[PLAY_COUNT];		//�����˿˵���Ŀ
	BYTE					m_iLastOutCard[PLAY_COUNT][45];		//���ֵ��˿�
	BYTE					m_byteHitPass;						//��¼����
	bool                    m_byPass[PLAY_COUNT];				//���ֲ���
	bool                    m_byLastTurnPass[PLAY_COUNT];     //���ֲ���
	BYTE                    m_bySendFinishCount;               //�����Ƶ���

	BYTE					m_bThrowoutCard;						//����
	BYTE					m_bFirstCallScore;						//��һ���е�����
	BYTE					m_bCurrentCallScore;					//��ǰ�з���

	int						m_iUpGradePeople;						//ׯ��λ��
	int						m_iNtFirstCount;						//�������ĵ�һ������

	int						m_lastCardListIndex;
	BYTE					m_lastCardList[128];					//�ϴγ����б�

	//������Ϣ
	int						m_iOutCardPeople;						//���ڳ����û�
	int						m_iFirstOutPeople;						//�ȳ��Ƶ��û�
	int						m_iNowBigPeople;						//��ǰ��������������
	int						m_iRecvMsg;								//�յ���Ϣ����

	bool					m_bIsCall[3];			//���1,2,3���Ƿ񱻽�
	int						m_iCallScore[PLAY_COUNT];
	int						m_iAwardPoint[PLAY_COUNT];				//���μӷ�
	bool					m_bAuto[PLAY_COUNT];					//�й�����
	//������ʹ�ñ���
	int						m_iWinPoint[PLAY_COUNT];				//ʤ��
	int						m_iDealPeople;							//�������
	bool					m_bHaveSendBack;						//�����Ƿ񷢹�����
	bool					m_bCanleave[PLAY_COUNT];				//�ܷ��뿪

	BYTE					m_iPrepareNT;							//Ԥ��ׯ��
	BYTE                    m_iFirstRobNt;                        //��һ���е�������
	BYTE                    m_iFirstShow;                         ///��һ�����Ƶ���
	BYTE                    m_iCurrentRobPeople;                  ///��ǰ�е�������

	BYTE                    m_iRobStation[PLAY_COUNT];            ///���������״̬(0-δ���� 255-���е��� ����-�е���)
	BYTE                    m_iAddStation[PLAY_COUNT];            ///��Ҽӱ�״̬                     

	//�ྀ���
	BYTE					m_iFirstCutPeople;						//��һ���������
	int                     m_icountleave;                           //���뿪����ͳ��
	///���׼��״̬
	bool                    m_bUserReady[PLAY_COUNT];
	bool					m_bUserNetCut[PLAY_COUNT];	//��ʶ����Ƿ����
	///��ҳ��Ƴ�������
	vector<bool>			m_vecWinInfo[PLAY_COUNT];
	TZongResult				m_tZongResult;
	bool					m_bRetryStart;//�Ƿ��ٴο�ʼ
	bool                    m_byGameEndType;

	//�Զ������
	int                     m_iRunTime; //��λ��
	long long               m_iGameBeginTime; //��Ϸ��ʼʱ��
	long long               m_iGameEndTime;//��Ϸ����ʱ��
	int						m_iFengDing;//�ⶥ������ը����
	BYTE					m_byMaxScoreStation;
	bool                    m_bPoChan[PLAY_COUNT];//�Ƿ��Ʋ�
	long long               m_iJiaoFenBeginTime; //��Ϸ��ʼʱ��
	long long               m_iOutCardnBeginTime;//���ƿ�ʼʱ��
	int						m_iRobotCount;       //����������
	BYTE					m_byRobotDesk1, m_byRobotDesk2; //�����˵�λ��
	bool					m_bIsRobot[PLAY_COUNT];
	//��������
public:
	//���캯��
	CServerGameDesk();
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, unsigned int assistID, void* pData, int size, bool bWatchUser = false);
	//��Ҷ��ߴ���
	virtual bool UserNetCut(GameUserInfo *pUser);
	/// �û��뿪��Ϸ��
	virtual bool UserLeftDesk(GameUserInfo* pUser);
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//����INI�ļ�
	virtual bool InitDeskGameStation();
	//���ӳɹ���ɢ
	virtual void OnDeskSuccessfulDissmiss(bool isDismissMidway = true);
	//��ʼ�����ӽ�ɢ����
	virtual void InitDeskGameData();
	// ��������й�
	virtual bool OnHandleUserRequestAuto(BYTE deskStation);
	// ���ȡ���й�
	virtual bool OnHandleUserRequestCancelAuto(BYTE deskStation);
	// ��̬���������ļ�����
	virtual void LoadDynamicConfig();
public:
	//������Զ���ʼ���ɷ���˿���
	BOOL StartGame();
	//��Ϸ׼������
	BOOL GamePrepare();
	//���ƹ�����
	BOOL SendCardMsg(BYTE bDeskStation, BYTE bCard);
	//��ʼ��һϵ�й���(���¿�ʼ��Ϸ�Ž��д��ֳ�ʼ��)
	BOOL InitThisGame();
	//��������
	BOOL LoadExtIni();
	//���ݷ���ID��������
	BOOL LoadExtIni(int iRoomID);
	//�����˿˸��û�
	BOOL SendCard();
	//һ�ν�������ȫ������
	BOOL	SendAllCard();
	//�l�ͽY��
	BOOL	SendCardFinish();
	//���ͽз�
	BOOL SendCallScore(BYTE bDeskStation, BYTE byCard = 0);
	//�û��Ͻ��з�
	BOOL UserCallScore(BYTE bDeskStation, int iVal);
	//�зֽ���
	BOOL CallScoreFinish();
	//������������Ϣ
	BOOL	SendRobNT(BYTE bDeskStation, BYTE byCard = 0);
	//�����������Ϣ
	BOOL	UserRobNT(BYTE bDeskStation, int iVal);
	//����������
	BOOL	RobNTFinish();
	//���͵���
	BOOL	SendBackCard();
	//���ͼӱ���Ϣ
	BOOL	SendAddDouble();
	//��Ҽӱ�
	BOOL	UserAddDouble(BYTE bDeskStation, int iVal);
	//�ӱ����
	BOOL	AddDoubleResult();
	//�ӱ�����
	BOOL	AddDoubleFinish();
	//����
	BOOL	SendShowCard();
	//����
	BOOL	UserShowCard(BYTE bDeskStation, int iValue);
	//���ƽ���
	BOOL	ShowCardFinish();
	//��Ϸ��ʼ
	BOOL	BeginPlay();
	//�û�����
	BOOL	UserOutCard(BYTE bDeskStation, BYTE iOutCard[], int iCardCount);
	//��һ�ֿ�ʼ
	BOOL NewPlayTurn(BYTE bDeskStation);
	//�û������뿪
	BOOL UserHaveThing(BYTE bDeskStation, char * szMessage);
	//ͬ���û��뿪
	BOOL ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee);
	//�û��й�
	bool UserSetAuto(BYTE bDeskStation, bool bAutoCard);
	//�����־��
	//	bool FindAllInfo(BYTE iCardList[],int iCardCount,GamePlayInfo  &iWriteInfo);
	//�Ƿ����һ݆
	BOOL IsNewTurn();
	//������Ӌ�r��
	void KillAllTimer();
	//�ӷ��ж�
	BOOL IsAwardPoin(BYTE iOutCard[], int iCardCount, BYTE bDeskStation);
	//���汶��
	int GetDeskBasePoint();
	//���䱶��
	int GetRoomMul();
	//���ܿ۷�
	int GetRunPublish();
	//���ܿ۷�
	int GetRunPublish(BYTE bDeskStation);
	//������һ��ׯ��
	BOOL SetNextBanker(BYTE bGameFinishState = 0);
	//��ȡ��һ��ׯ��
	BYTE GetNextBanker();
	//�������й�
	BOOL UseAI(BYTE bDeskStation);
	//��ҳ�ʱ����(ȫ���ɷ���˽ӹ�)
	BOOL UserAutoOutCard(BYTE bDeskStation);
	//��ȡ��һ�����λ��
	BYTE GetNextDeskStation(BYTE bDeskStation);
	//��ȡһ��������λ��
	BYTE GetRobNtDeskStation(BYTE bDeskStation);
	//�Ƿ�Ϊδ������
	BYTE GetNoOutCard();

	//����Ƿ񳬶����
	bool	IsSuperUser(BYTE byDeskStation);
	//��֤�Ƿ񳬶�
	void	SpuerExamine(BYTE byDeskStation);
	//�����
	void	UpdateCalculateBoard();
	//����������
	void GetSpecialRule();
	//��ȡ��ǰʱ�䣬������
	long long GetCurTime();
	//���ͳ��Ƴ�����Ϣ
	void SendOutCardError(BYTE byDeskStation, int iErrorCode);
};

/******************************************************************************************************/
