#pragma once
//��Ӫ�澭�������Ż����޷����Ե��������ٶȸ���
#pragma pack(1)
//�ļ����ֶ���
#define GAMENAME						TEXT("���Ƕ�����")
#define NAME_ID							36610103								//���� ID

//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				2				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0				//�ڲ������Ͱ汾��

//��Ϸ��Ϣ
#define PLAY_COUNT						3								//��Ϸ����
#define SKIN_CARD						TEXT(".\\image\\cardskin")		//����ͼƬ�ļ���
static TCHAR szTempStr[MAX_PATH] = { 0 };//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_CLIENT_BCF_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".bcf"))

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_SEND_CARD					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_PLAY_GAME					22				//��Ϸ��״̬.
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

#define GS_FLAG_NORMAL					0				//�������
#define GS_FLAG_CALL_SCORE				1				//�з�
#define GS_FLAG_ROB_NT					2				//������
#define GS_FLAG_ADD_DOUBLE				3				//�ӱ�
#define GS_FLAG_SHOW_CARD				4				//����
#define GS_FLAG_PLAY_GAME               5 

//-------------------------------------------------------------------------------
/********************************************************************************/
// ���ݰ���������ʶ
//warning:79��IDԤ���������ͻ���,����ʹ��
/********************************************************************************/
#define ASS_GAME_BEGIN					51				//��Ϸ��ʼ
#define ASS_SEND_CARD					53				//������Ϣ
#define ASS_SEND_CARD_MSG				54				//���ƹ����д�����Ϣ
#define ASS_SEND_ALL_CARD				55				//����������(һ�·���ȫ��)
#define ASS_SEND_FINISH					56				//�������
#define ASS_CALL_SCORE					57				//�з�
#define ASS_CALL_SCORE_RESULT			58				//�зֽ��
#define ASS_CALL_SCORE_FINISH			59				//�зֽ���

#define ASS_ROB_NT						61				//������
#define ASS_ROB_NT_RESULT				62				//���������
#define ASS_GAME_MULTIPLE				64				//��Ϸ����(���������ӱ�)
#define ASS_ROB_NT_FINISH				65				//���������

#define ASS_BACK_CARD					66				//��������
#define ASS_BACK_CARD_EX				67				//��չ��������(������ҿɼ�)
#define ASS_ADD_DOUBLE					68				//֪ͨ�ӱ�
#define ASS_ADD_DOUBLE_RESULT			69				//�ӱ����
#define ASS_ADD_DOUBLE_FINISH			71				//�ӱ�����
#define ASS_SHOW_CARD					72				//����
#define ASS_SHOW_CARD_RESULT			73				//���ƽ��
#define ASS_SHOW_CARD_FINISH			75				//���ƽ���
#define ASS_GAME_PLAY					76				//��ʼ��Ϸ
#define ASS_OUT_CARD					77				//�û�����
#define ASS_OUT_CARD_RESULT				78				//���ƽY��
#define	ASS_SUPER_USER					79				//������Ϣ
#define ASS_REPLACE_OUT_CARD			80				//�������(79���������ͻ��˷�����)
#define ASS_ONE_TURN_OVER				81				//һ�����(ʹ�ͻ�����һ�ֿ���)
#define ASS_NEW_TURN					82				//��һ�ֿ�ʼ
#define ASS_AWARD_POINT					83				//����(ը�����)
#define ASS_CONTINUE_END				84				//��Ϸ����
#define ASS_NO_CONTINUE_END				85				//��Ϸ����
#define ASS_NO_CALL_SCORE_END			86				//���˽з�
#define ASS_CUT_END						87				//�û�ǿ���뿪
#define ASS_SAFE_END					88				//��Ϸ��ȫ����
#define ASS_TERMINATE_END				89				//�������
#define ASS_AHEAD_END					90				//��ǰ����
#define ASS_AUTO						91				//�û��й�
#define ASS_HAVE_THING					92				//����
#define ASS_LEFT_RESULT					93				//�����뿪���
#define ASS_AI_STATION					95				//�������й�(���߻���)
#define ASS_USER_LEFTDESK               97              //����뿪���ӻ����

#define S_C_UPDATE_CALCULATE_BOARD_SIG	98				//���½����
#define S_C_UPDATE_REMAIN_JUSHU_SIG		99				//����ʣ�����

#define S_C_ROBOT_CARD					100				//������ר����Ϣ
#define S_C_OUT_CARD_RESULT				109             //���ƽ��(��Ҫ����)

const  int  MAX_TASK_TYPE = 4;  ///����������� 
const  int  MAX_CARD_SHAPE = 8;  ///�����������
const  int  MAX_CARD_TYPE = 15; ///������

///��������
enum BackCardType
{
	TYPE_NONE = 0,//ʲô���Ͷ�����
	TYPE_DOUBLE_CARD = 10,
	TYPE_SAME_HUA = 11,
	TYPE_SMALL_KING = 12,
	TYPE_BIG_KING = 13, //����
	TYPE_TRIPLE_CARD = 14,
	TYPE_STRAIT = 15,
	TYPE_ROCKET = 16
};

///��������
enum LastTaskType
{
	TYPE_LAST_NONE = 0,    ///���κ����� 
	TYPE_HAVE_A_CARD = 100,  ///��ĳ����
	TYPE_SOME_SHAPE = 101,  ///��ĳ������
	TYPE_SINGLE_SOME_CARD = 102,  ///���ĳ����
	TYPE_DOUBLE_SOME_CARD = 103   ///���һ��ĳ����
};

///��Ϸ����ṹ
struct GameTaskStruct
{
	BYTE     byBackCardType;
	BYTE     byTaskType;
	BYTE     bySpecifyShape;
	BYTE     bySpecifyCard;
	BYTE     byBackCardMutiple;  ///���Ʊ���

	GameTaskStruct()
	{
		memset(this, 0, sizeof(GameTaskStruct));
	}
	void IniData(void)
	{
		memset(this, 0, sizeof(GameTaskStruct));
	}
};

///��Ϸ�еı���
struct  GameMutipleStruct
{
	int       sBaseMutiple;                 ///��Ϸ�еĻ�����ע
	int       sBackCardMutiple;             ///���Ʊ���
	int       sBombCount;                   ///ը������
	int       sSprintMutiple;               ///����
	int       sCardShapeMutiple;           ///���ͱ�����һ��ָ�����еı�����
	BYTE      sAddGameMutiple[PLAY_COUNT];  /// ��Ϸ�мӱ�
	BYTE      sRobNtMutiple[PLAY_COUNT];    ///����������
	BYTE      sMingPaiMutiple[PLAY_COUNT];  ///ÿ���˵����Ʊ���
	GameMutipleStruct()
	{
		sBaseMutiple = 1;
		sBackCardMutiple = 1;
		sBombCount = 0;
		sSprintMutiple = 1;
		sCardShapeMutiple = 100;

		memset(sAddGameMutiple, 0, sizeof(sAddGameMutiple));
		memset(sRobNtMutiple, 0, sizeof(sRobNtMutiple));
		memset(sMingPaiMutiple, 0, sizeof(sMingPaiMutiple));
	};
	void  IniData(int  iBaseMutiple)
	{
		sBaseMutiple = iBaseMutiple;
		sBombCount = 0;
		sBackCardMutiple = 1;
		sSprintMutiple = 1;
		sCardShapeMutiple = 100;

		memset(sAddGameMutiple, 0, sizeof(sAddGameMutiple));
		memset(sRobNtMutiple, 0, sizeof(sRobNtMutiple));
		memset(sMingPaiMutiple, 0, sizeof(sMingPaiMutiple));
	}
	///��ȡ���������
	int  GetMingMaxMutiple(void)
	{
		int iMingMutiple = max(max(sMingPaiMutiple[0], sMingPaiMutiple[1]), sMingPaiMutiple[2]);

		return (iMingMutiple > 0 ? iMingMutiple : 1);
	}
	///��ȡ������������
	int  GetRobNtMutiple(void)
	{
		int  iRobMutiple = 0;
		int  iRobCount = 0;

		for (int i = 0; i < PLAY_COUNT; i++)
		{
			iRobCount += sRobNtMutiple[i];
		}

		iRobMutiple = pow(2.0, iRobCount);

		return iRobMutiple;
	}
	///��ȡ��������
	int  GetPublicMutiple(void)
	{
		int  iBombMutiple = pow(2.0, sBombCount);
		sBackCardMutiple = 1;
		int  iGameMutiple = sBaseMutiple*sBackCardMutiple*iBombMutiple*sSprintMutiple*sCardShapeMutiple*GetRobNtMutiple()*GetMingMaxMutiple() / 100;
		return iGameMutiple;
	}
	//��ȡը������
	int GetBombMutiple()
	{
		int  iBombMutiple = pow(2.0, sBombCount);
		int  iGameMutiple = sBaseMutiple*iBombMutiple;
		return iGameMutiple;
	}
};

/********************************************************************************/
//��Ϸ���ݰ�
/********************************************************************************/
//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	bool                bUserReady[PLAY_COUNT];        ///����Ƿ���׼��

	BYTE				iBeginTime;						//��ʼ׼��ʱ��
	BYTE				iThinkTime;						//����˼��ʱ��
	BYTE				iCallScoreTime;					//�зּ�ʱ
	BYTE                iRobNTTime;                    //������ʱ��
	BYTE				iAddDoubleTime;					//�ӱ�ʱ��

	int                 iGameMutiple;             ///��Ϸ�еı���
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit;              ///��Ϸ�����Ӯ

	DWORD				iCardShape;						//��������
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	__int64				iRunPublish;					//���ܿ۷�

	int					iFengDing;//�ⶥ������ը����
	int                 iRunTime;//��λ��
	int                 iRemainRunTime; //��λ��

	GameStation_2()
	{
		memset(this, 0, sizeof(GameStation_2));
	}
};

//��Ϸ״̬���ݰ�	�� �ȴ���Ѻ����״̬ ��
struct GameStation_3
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	BYTE				iBackCount;						//������
	BYTE				iBeginTime;						//��ʼ׼��ʱ��
	BYTE				iThinkTime;						//����˼��ʱ��
	BYTE                iRobNTTime;                    //������ʱ��
	BYTE				iCallScoreTime;					//�зּ�ʱ
	BYTE				iAddDoubleTime;					//�ӱ�ʱ��

	BYTE				iCallScorePeople;				//��ǰ�з���
	BYTE				iGameFlag;						//�зֱ��
	BYTE				iCallScoreResult;				//���еķ�

	int					iUpGradePeople;					//ׯ��λ��
	int                 iCurOperator;                 ///��ǰ��������

	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��

	DWORD				iCardShape;						//��������
	int                 iGameMutiple;             ///��Ϸ�еı���
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit;              ///��Ϸ�����Ӯ

	bool				bAuto[PLAY_COUNT];				//�й����
	bool				bCanleave[PLAY_COUNT];			//�ܷ���˳�
	int					iCallScore[PLAY_COUNT];			//���ҽз����
	int 				iRobNT[PLAY_COUNT];				//���������
	int                 iUserDoubleValue[PLAY_COUNT];  ///��Ҽӱ����
	BYTE				iUserCardCount[PLAY_COUNT];		//�û������˿���Ŀ
	BYTE				iUserCardList[195];				//�û����ϵ��˿�
	BYTE                iGameBackCard[12];                 ///����
	BYTE 				iBackCardCount;						//��������
	__int64				iRunPublish;					//���ܿ۷�

	GameMutipleStruct   gameMutiple;                      ///��Ϸ����  
	GameTaskStruct      gameTask;                         ///��Ϸ����

	int					iFengDing;//�ⶥ������ը����
	int                 iRunTime;//��λ��
	int                 iRemainRunTime; //��λ��

	BYTE                byJiaoFenSFTime;                //�з�ʣ��ʱ��
	bool				bIsCall[3];						//���1,2,3���Ƿ񱻽�
	GameStation_3()
	{
		memset(this, 0, sizeof(GameStation_3));
	}

};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4
{
	bool				bIsLastCard;						//�Ƿ�����������

	BYTE				iVersion;							//��Ϸ�汾��
	BYTE				iVersion2;							//��Ϸ�汾��
	BYTE				iBackCount;							//������

	BYTE				iBeginTime;							//��ʼ׼��ʱ��
	BYTE				iThinkTime;							//����˼��ʱ��
	BYTE                iRobNTTime;                    //������ʱ��
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iAddDoubleTime;						//�ӱ�ʱ��

	BYTE				bIsPass;							//�Ƿ񲻳�

	int					iRunPublish;						//���ܿ۷�	
	int					iBase;								//��ǰը������
	int					iUpGradePeople;						//ׯ��λ��

	int 				iCallScoreResult[PLAY_COUNT];		//�зֽ��
	int					iOutCardPeople;						//���ڳ����û�
	int					iFirstOutPeople;					//�ȳ��Ƶ��û�
	int					iBigOutPeople;						//�ȳ��Ƶ��û�

	UINT				iDeskBasePoint;						//���������
	UINT				iRoomBasePoint;						//���䱶��

	int                 iGameMutiple;             ///��Ϸ�еı���
	DWORD				iCardShape;							//��������
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit;              ///��Ϸ�����Ӯ

	BYTE				iAwardPoint[PLAY_COUNT];			//����
	BYTE				iPeopleBase[PLAY_COUNT];			//�ӱ�
	BYTE				iRobNT[PLAY_COUNT];					//������


	bool				bAuto[PLAY_COUNT];					//�й����
	bool				bCanleave[PLAY_COUNT];				//�ܷ���˳�

	BYTE				iUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
	BYTE				iUserCardList[195];					//�û����ϵ��˿�

	BYTE				iBaseOutCount;						//���Ƶ���Ŀ
	BYTE                iBaseCardList[45];                //�����ϵ���

	BYTE				iDeskCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
	BYTE                iDeskCardList[PLAY_COUNT][45];    ///�����ϵ���

	BYTE				iLastCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
	BYTE				iLastOutCard[PLAY_COUNT][45];		//���ֵ��˿�

	bool                bPass[PLAY_COUNT];                //����
	bool                bLastTurnPass[PLAY_COUNT];         //��һ�ֲ���

	BYTE                iGameBackCard[12];                 ///����
	BYTE 				iBackCardCount;						//��������

	GameMutipleStruct   gameMutiple;                      ///��Ϸ����  
	GameTaskStruct      gameTask;                         ///��Ϸ����

	int					iFengDing;//�ⶥ������ը����
	int                 iRunTime;//��λ��
	int                 iRemainRunTime; //��λ��

	BYTE                byOutCardSFTime;                //����ʣ��ʱ��
	GameStation_4()
	{
		memset(this, 0, sizeof(GameStation_4));
	}
};

//�û��������ݰ� �������������
struct CMD_C_OutCard
{
	int					iCardCount;						//�˿���Ŀ
	BYTE				iCardList[45];					//�˿���Ϣ
	CMD_C_OutCard()
	{
		memset(this, 0, sizeof(CMD_C_OutCard));
	}
};

//���ƽ��
struct CMD_S_OutCard
{
	BYTE byCurOutCardStation;   //��ǰ�������
	BYTE byCardCount;			//���˿���Ŀ
	BYTE byCardList[45];		//�˿���Ϣ
	BYTE byHandCardCount;       //��������
	BYTE byHandCard[45];		//����֮���������
	CMD_S_OutCard()
	{
		byCurOutCardStation = 255;
		byCardCount = 0;
		byHandCardCount = 0;
		memset(byCardList, 0, sizeof(byCardList));
		memset(byHandCard, 0, sizeof(byHandCard));
	}
};

//֪ͨ����
struct CMD_S_NotifyOutCard
{
	BYTE byNextDeskStation;//��һ������
	CMD_S_NotifyOutCard()
	{
		byNextDeskStation = 255;
	}
};

//��һ��
struct NewTurnStruct
{
	BYTE				bDeskStation;					//����
	BYTE				bReserve;						//����
};

///���ƿ�ʼ
struct  UserMingStruct
{
	bool    bStart;       ///�Ƿ�Ϊ��ʼ
	bool    bMing;         ///����Ƿ�����
	BYTE    bDeskStaion;   ///��ҵ�λ��
	BYTE    byCardCount;   ///����ʱ����˿˵�����
};

//��Ϸ��ʼ
struct	GameBeginStruct
{
	BYTE				iPlayLimit;							//��Ϸ�ܾ���
	BYTE				iBeenPlayGame;						//�Ѿ����˶��پ�
	BYTE                byUserMingBase[PLAY_COUNT];        ///������Ʊ������
	DWORD				iCardShape;							//��������

	GameBeginStruct()
	{
		memset(this, 0, sizeof(GameBeginStruct));
	}
};

//�������ݰ�
struct	SendCardStruct
{
	BYTE				bDeskStation;							//���
	BYTE				bCard;									//�Ʊ��
};

//��������������
struct	SendAllStruct
{
	BYTE             iUserCardCount[PLAY_COUNT];		//��������
	BYTE             iUserCardList[108];				//���ƶ���

	SendAllStruct()
	{
		memset(this, 0, sizeof(SendAllStruct));
	}
};

//���ƽ���
struct	SendCardFinishStruct
{
	BYTE		bReserve;
	BYTE		byBackCardList[12];						//��������
	BYTE		byUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
	BYTE		byUserCard[PLAY_COUNT][45];				//�û����ϵ��˿�
	SendCardFinishStruct()
	{
		memset(this, 0, sizeof(SendCardFinishStruct));
	}
};

//�������ݰ�
struct BackCardExStruct
{
	BYTE				iGiveBackPeople;				//�������
	BYTE				iBackCardCount;					//�˿���Ŀ
	BYTE				iBackCard[12];					//��������

	GameTaskStruct      gameTask;                      //��Ϸ����  
	BackCardExStruct()
	{
		memset(this, 0, sizeof(BackCardExStruct));
	}
};

//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;				//���Ƶ�λ��
};

//�з����ݰ�
struct CallScoreStruct
{
	BYTE				bDeskStation;		//��ǰ�з���
	int					iValue;				//�з�����
	bool 				bCallScoreflag;		//�зֱ��			
	BYTE                byFirstCard;        //�з���
	bool                bIsCall[3];			//���1,2,3���Ƿ񱻽�
	CallScoreStruct()
	{
		bDeskStation = 255;
		iValue = -1;
		bCallScoreflag = false;
		byFirstCard = 0;
		memset(bIsCall, 0, sizeof(bIsCall));
	}
};

//������
struct	RobNTStruct
{
	BYTE   byDeskStation;		//����������
	BYTE   byRobCount;         //�������������
	int    iValue;				//���������(0-�е���״̬ 1-������״̬)
	BYTE   byFirstCard;         //�з���
	RobNTStruct()
	{
		byDeskStation = 255;
		byRobCount = 0;
		iValue = 0;
		byFirstCard = 0;
	}
};

//����
struct ShowCardStruct
{
	BYTE bDeskStation;										//����
	BYTE iCardList[54];					                   //�˿���Ϣ
	BYTE iCardCount;						              //�˿���Ŀ
	int iValue;											  //����
	int iBase;											  //���� 20081204
};

//�ӱ�
struct AddDoubleStruct
{
	BYTE bDeskStation;										//�ӱ�λ��
	int iValue;												//�ӱ����
};

//����
struct AwardPointStruct
{
	BYTE	iAwardPoint;									//����
	BYTE	bDeskStation;									//����
	int		iBase;											//����
};

//�й����ݽṹ
struct AutoStruct
{
	BYTE bDeskStation;
	bool bAuto;
};

//�������й�
struct UseAIStation
{
	BYTE bDeskStation;					//����
	BOOL bState;						//״̬
};

//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	bool bChunTian;	//�Ƿ���
	int iBaseFen;	//�׷�
	int iBombCount;	//ը������
	int iAllDouble[PLAY_COUNT];//ÿ����ҵ��ܱ���
	bool bAddDouble[PLAY_COUNT];//ÿ������Ƿ�ӱ�
	int iZongFen;				//�ܷ�
	BYTE	iUserCard[PLAY_COUNT][45];			//�û����ϵ��˿�
	BYTE	iUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
	int		iUserScore[PLAY_COUNT];				//������ҵĵ÷�
	int     iVipGameCount;                      //����
	GameEndStruct()
	{
		memset(this, 0, sizeof(GameEndStruct));
	}
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					iRoomBasePoint;					//����
	int					iDeskBasePoint;					//���汶��
	int					iHumanBasePoint;				//��ͷ����
	int					bDeskStation;					//�˳�λ��
	__int64				iChangeMoney[8];				//��ҽ��
	__int64				iTurePoint[PLAY_COUNT];			//ׯ�ҵ÷�

};

struct HaveThingStruct
{
	BYTE pos;
	char szMessage[61];
};

struct LeaveResultStruct
{
	BYTE bDeskStation;
	int bArgeeLeave;
};

struct UserleftDesk
{
	BYTE bDeskStation;                //�������
};

//������Ϣ�ṹ��
struct	SuperUserMsg
{
	BYTE	byDeskStation;
	bool	bIsSuper;
	SuperUserMsg()
	{
		memset(this, 0, sizeof(SuperUserMsg));
	}
};

//�����
struct	TZongResult
{
	bool	bWinner[PLAY_COUNT];				//��Ӯ��
	int		iBombCount[PLAY_COUNT];				//ը������
	int		iMaxWinMoney[PLAY_COUNT];			//���ӮǮ��Ŀ
	int		iMaxContinueCount[PLAY_COUNT];		//�����ʤ
	int		iWinCount[PLAY_COUNT];				//ʤ������
	__int64	i64WinMoney[PLAY_COUNT];			//��Ӯ���
	__int64	i64UserMoney[PLAY_COUNT];			//��ҽ��
	BYTE    byGameEndType[PLAY_COUNT];          //�����������ͣ�0�����һ��������1�����һ�����˽з�
	TZongResult()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0, sizeof(TZongResult));
		memset(bWinner, false, sizeof(bWinner));
		memset(iBombCount, 0, sizeof(iBombCount));
		memset(iMaxWinMoney, 0, sizeof(iMaxWinMoney));
		memset(iMaxContinueCount, 0, sizeof(iMaxContinueCount));
		memset(iWinCount, 0, sizeof(iWinCount));
		memset(i64WinMoney, 0, sizeof(i64WinMoney));
		memset(i64UserMoney, 0, sizeof(i64UserMoney));
		memset(byGameEndType, 0, sizeof(byGameEndType));
	}
};

//���ƽ��
struct CMD_S_OutCard_Result
{
	bool bResult;        //���ƽ�����ɹ�true��ʧ��false
	//�����룬1��Э���С���ԣ�2������Ϸ״̬��3:���ǳ�����ң�4��û���������ƣ�5������������ ,6�����ʹ���
	BYTE byErrorCode;

	BYTE byHandCardCount;	//��������
	BYTE byHandCard[45];	//�������
	CMD_S_OutCard_Result()
	{
		bResult = false;
		byErrorCode = 0;
		byHandCardCount = 0;
		memset(byHandCard, 0, sizeof(byHandCard));
	}
};

#pragma pack()
