#pragma once

#include <vector>

//��������
#define	UG_HUA_MASK					0xF0			//1111 0000
#define	UG_VALUE_MASK				0x0F			//0000 1111

//�˿˻�ɫ
#define UG_FANG_KUAI				0x00			//����	0000 0000
#define UG_MEI_HUA					0x10			//÷��	0001 0000
#define UG_HONG_TAO					0x20			//����	0010 0000
#define UG_HEI_TAO					0x30			//����	0011 0000
#define UG_NT_CARD					0x40			//����	0100 0000
#define UG_ERROR_HUA				0xF0			//����  1111 0000

//�˿˳�������
#define UG_ERROR_KIND							0				//����

#define UG_ONLY_ONE								1				//����
#define UG_DOUBLE								2				//����

#define UG_VARIATION_STRAIGHT					3				//����˳��(A2345)˳������С
#define UG_STRAIGHT								4               //˳��,5+��������
#define UG_FLUSH								5				//ͬ��(����)
#define UG_STRAIGHT_FLUSH						6               //ͬ��˳,��ɫ��ͬ��˳��

#define UG_THREE								7				//����
#define UG_THREE_ONE							8               //3 �� 1
#define UG_THREE_TWO							9               //3 �� 2
#define UG_THREE_DOUBLE							10				//3 ��1��

#define UG_VARIATION_DOUBLE_SEQUENCE			11				//����˫˳(AA22)��С
#define UG_DOUBLE_SEQUENCE						12				//����,2+�������Ķ���

#define UG_VARIATION_THREE_SEQUENCE				13				//������˳(AAA222��С)
#define UG_THREE_SEQUENCE						14				//�����ţ�2+������������

#define UG_VARIATION_THREE_ONE_SEQUENCE			15				//������˳��һ
#define UG_THREE_ONE_SEQUENCE					16              //2+������������һ

#define UG_VARIATION_THREE_TWO_SEQUENCE			17				//������˳����
#define UG_THREE_TWO_SEQUENCE					18				//2+��������������

#define UG_VARIATION_THREE_DOUBLE_SEQUENCE		19				//���������Ŵ���
#define UG_THREE_DOUBLE_SEQUENCE				20				//�����Ŵ���

#define UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE		21		//���ֺ���(��˳����˳)
#define UG_THREE_SEQUENCE_DOUBLE_SEQUENCE		22				//����(��˳����˳)

#define UG_FOUR_ONE								23				//�Ĵ�һ
#define UG_FOUR_TWO								24				//�Ĵ�����
#define UG_FOUR_ONE_DOUBLE						25				//�Ĵ�һ��
#define UG_FOUR_TWO_DOUBLE						26				//�Ĵ�����

#define UG_VARIATION_FOUR_SEQUENCE				27				//��˳
#define UG_FOUR_SEQUENCE						28				//��˳

#define UG_VARIATION_FOUR_ONE_SEQUENCE			29				//�Ĵ�һ˳
#define UG_FOUR_ONE_SEQUENCE					30				//�Ĵ�һ˳

#define UG_VARIATION_FOUR_TWO_SEQUENCE			31				//�Ĵ���˳
#define UG_FOUR_TWO_SEQUENCE					32				//�Ĵ���˳

#define UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE	33				//�Ĵ���˳
#define UG_FOUR_ONE_DOUBLE_SEQUENCE				34				//�Ĵ���˳

#define UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE	35				//�Ĵ�����˳
#define UG_FOUR_TWO_DOUBLE_SEQUENCE				36				//�Ĵ�����˳


#define UG_SLAVE_510K							37              //510Kը��,��ɫ��ͬ
#define UG_MASTER_510K							38              //510Kͬ��ը��

#define UG_BOMB									39              //ը��>=4��
#define UG_BOMB_SAME_HUA						40				//ͬ��ը��(���ĸ������ϵ����г���)
#define UG_KING_BOMB							41				//��ը(���ը��)


#define KING_COUNT								2				//�������ĸ���
//�˿˷����ṹ	��ֻ���ڷ������Ա��˿ˣ�
struct CardAnalyseStruct
{
	int		iSingleCount;	  //������Ŀ
	int		iDoubleCount;	  //������Ŀ
	int		iThreeCount;	  //������Ŀ
	int		iBombCount;		  //ը����
	int		i510KCount;		  //510K��Ŀ
	int		i510KMastCount;	  //��510K��Ŀ
//	BYTE	iBigsingleCard;	  //�����
//	BYTE	iLesssingleCard;  //��С����

	BYTE	iSingleArray[45]; //����
	BYTE	iDoubleArray[45]; //��������
	BYTE	iThreeArray[45];  //��������
	BYTE	iBombArray[45];	  //�������������� ��0 �ָ���ȣ����������ݣ�
	BYTE	i510KArray[45];	  //�������������� ��0 �ָ���ȣ����������ݣ�
};

struct CardType
{
	BYTE byCard[4];
	BYTE byNum;
	CardType()
	{
		memset(byCard,0,sizeof(byCard));
		byNum = 0;
	}
};

//510K�߼��� ֧�� 2 ���˿ˣ�
class CUpGradeGameLogic
{
	//��������
private:
//	int				m_iStation[5];	   //���λ�ã����飬÷�������ң����ң����ƣ�
	BYTE			m_bSortCardStyle;  //����ʽ
	int				m_iCondition;			//��������
	BOOL			m_bKingCanReplace;		//���Ƿ�ɵ�

	DWORD           m_iCardShape;	//֧������
	int             m_iLaiZiSize;   //��Ӵ�С
	CardType		m_cdType[18];
	//��������
public:
	//���캯��		
	CUpGradeGameLogic(void);
	//��������
	virtual ~CUpGradeGameLogic();

	//���ܺ���������������
public:
	//[�������]
	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK)+1; }
	//��ȡ�˿˻�ɫ(Ĭ��Ϊ��ʵ��ɫ)
	BYTE GetCardHuaKind(BYTE iCard, BOOL bTrueHua=TRUE);
	//��ȡ�˿���Դ�С(Ĭ��Ϊ�ƴ�С,�������С)
	int GetCardBulk(BYTE iCard, bool bExtVal=false);
	//��ȡ�˿���ͨ����Դ�С
	BYTE GetCardByValue(int iCardValue);
	//�O�������Ԯ���
	void SetKingCanReplace(BOOL bKingCanReplace=false){m_bKingCanReplace=bKingCanReplace;}
	//�Cȡ���Ƿ���Ԯ���
	BOOL GetKingCanReplace(){return m_bKingCanReplace;}
	//��������ʽ
	void SetSortCardStyle(BYTE SortCardStyle){m_bSortCardStyle=SortCardStyle;}
	//��ȡ����ʽ
	BYTE GetSortCardStyle(){return m_bSortCardStyle;}
	//�������
	void SetLaiZi(BYTE byCard){m_iLaiZiSize=GetCardBulk(byCard);}
	//[����]
public:
	//�����˿�,����С(����ϵͳ����)
	BOOL SortCard(BYTE iCardList[], BYTE bUp[], BYTE iCardCount,BOOL bSysSort = FALSE);
	//��ת��˳(�ӵ�->��)
	BOOL ReverseCard(BYTE iCardList[], BYTE bUp[], BYTE iCardCount);
	//����������
	BOOL SortCardByStyle(BYTE iCardList[],BYTE iCardCount);
	//����ɫ����
	BOOL SortCardByKind(BYTE iCardList[],BYTE iCardCount);
public:
	//�����˿�
	BYTE RandCard(BYTE iCard[], int iCardCount,int iRoomId,bool bHaveKing=false);		//bHaveKing��ʾ�Ƿ��д�Сè,false��,ture��
	//ɾ���˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);


	int GetCardCount(BYTE iCard[], int iCardCount);

	bool IsLegalCard(BYTE iCard);
private:
	//��� 0 λ�˿�
	int RemoveNummCard(BYTE iCardList[], int iCardCount);

public://[��������]
	//�Աȵ���
	BOOL CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard);
	//���ҷ���
	int FindPoint(BYTE iCardList[], int iCardCount);
	//�Ƿ�Ϊͬһ������
	BOOL IsSameNumCard(BYTE iCardList[],int iCardCount,bool bExtVal=false);
	//�Ƿ�Ϊͬһ��ɫ
	BOOL IsSameHuaKind(BYTE iCardList[],int iCardCount,bool bExtVal=false);
	//���� >=4 ը��������ը������
	BYTE GetBombCount(BYTE iCardList[], int iCardCount,int iNumCount = 4, bool bExtVal=false);
	//��ȡָ����С�Ƹ���
	BYTE GetCountBySpecifyNumCount(BYTE iCardList[],int iCardCount,int Num);
	//��ȡָ���Ƹ���
	BYTE GetCountBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE bCard);
	//��ȡָ���������ƴ�С(������ֻ�ܹ���һ���Ƶ�����ΪiCount,��Ȼ����ȥ�Ľ��ǵ�һ��ָ��������ֵ)
	BYTE GetBulkBySpecifyCardCount(BYTE iCardList[],int iCardCount,int iCount);
	//�Ƿ�Ϊĳָ����˳��(����˳��)
	BOOL IsVariationSequence(BYTE iCardList[], int iCardCount, int iCount);
	//�Ƿ�Ϊĳָ����˳��
	BOOL IsSequence(BYTE iCardList[], int iCardCount, int iCount);
	//��ȡָ������
	BYTE TackOutBySpecifyCard(BYTE iCardList[], int iCardCount,BYTE bCardBuffer[],int &iResultCardCount,BYTE bCard);
	//��ȡĳ��ָ�����ֵ���
	BOOL TackOutCardBySpecifyCardNum(BYTE iCardList[],int iCardCount,BYTE iBuffer[],int &iBufferCardCount,BYTE iCard,BOOL bExtVal=false);
	//��ȡ���з�����������,����,����,����,4ը������
	int TackOutBySepcifyCardNumCount(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[],BYTE bCardNum, bool bExtVal=false);
	//��ȡָ����ɫ��
	int TackOutByCardKind(BYTE iCardList[],int iCardCount,BYTE iDoubleBuffer[],BYTE iCardKind);
	//���(�������ƶ�Ĳ���ٵ�)
	int TackOutMuchToFew(BYTE iCardList[],int iCardCount,BYTE iDoubleBuffer[],int &iBufferCardCount,BYTE iCardMuch,BYTE iCardFew);
	//���Ҵ���iCard�ĵ�������iCardList�е����
	BYTE GetSerialByMoreThanSpecifyCard(BYTE iCardList[],int iCardCount,BYTE iCard,BYTE iBaseCardCount, bool bExtValue=false);
	//����==iCard�ĵ�������iCardList�е����(��ʼλ��,���K�cλ��)
	int GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard);
	//��ȡָ��˳�����Ƶ���Сֵ(iSequence ����˳�ӵ��������Ϊ
	BYTE GetBulkBySpecifySequence(BYTE iCardList[],int iCardCount ,int iSequence = 3);
	//��ȡָ��˳�����Ƶ����ֵ����˳��
	BYTE GetBulkBySpecifyVariationSequence(BYTE iCardList[],int iCardCount ,int iSequence = 3);
	//������С (1) or ��� (255) ��
	int	GetBulkBySepcifyMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax, bool bExtVal=false);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//[�������]
public:
    //��ȡ����
	BYTE GetCardShape(BYTE iCardList[], int iCardCount,bool bExlVal=false);
	//�Ƿ���
	inline BOOL IsOnlyOne(BYTE iCardList[], int iCardCount) { return iCardCount==1; };
	//�Ƿ����
	BOOL IsDouble(BYTE iCardList[], int iCardCount,bool bExtVal=false);
	//3 �� 1or2(��һ�Դ������Ż��һ����
	BOOL IsThreeX(BYTE iCardList[], int iCardCount, int iX/*1or2*/, bool bExtVal=false);
	//��ը
	BOOL IsKingBomb(BYTE iCardList[],int iCardCount);
	//4+���� ը��
	BOOL IsBomb(BYTE iCardList[], int iCardCount, bool bExtVal=false);
	//ͬ��ը��
	BOOL IsBombSameHua(BYTE iCardList[],int iCardCount);
	//ͬ��(��˳��)
	BOOL IsFlush(BYTE iCardList[],int iCardCount);
	//�Ƿ���ͬ��˳
	BOOL IsStraightFlush(BYTE iCardList[], int iCardCount, bool bExtVal=false);
	//���ֵ�˦
	BOOL IsVariationStraight(BYTE iCardList[],int iCardCount,bool bExtVal=false);
	//��˦
	BOOL IsStraight(BYTE iCardList[],int iCardCount,bool bExtVal=false);
	//���ֶ�˦
	BOOL IsVariationDoubleSequence(BYTE iCardList[],int iCardCount,bool bExtVal=false);
	//��˦ //����?
	BOOL IsDoubleSequence(BYTE iCardList[],int iCardCount,bool bExtVal=false);

	//�Ƿ����������������X(0,1,2,3)
	BOOL IsVariationThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX/*0,1or2*/, bool bExtVal=false);

	//�Ƿ�������������X(0,1,2,3)
	BOOL IsThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX/*0,1or2*/, bool bExtVal=false);
	//�Ƿ���˳����˳(����)
	BOOL IsThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal=false);
	//�Ƿ���˳����˳(����)
	BOOL IsVariationThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal=false);
	//510K ը��
	BOOL IsSlave510K(BYTE iCardList[], int iCardCount, bool bExtVal=false);
	//510K ͬ��ը��
	BOOL IsMaster510K(BYTE iCardList[], int iCardCount, bool bExtVal=false);
	//�Ĵ�һ�����Ĵ���
	BOOL IsFourX(BYTE iCardList[],int iCardCount,int iX/*1or 2*/);//����1,������2,һ����,2��4
	//�Ƿ�����Ĵ�X˳
	BOOL IsVariationFourXSequence(BYTE iCardList[],int iCardCount,int iSeqX);
	//�Ĵ�һ�����Ĵ�����˳��
	BOOL IsFourXSequence(BYTE iCardList[],int iCardCount,int iSeqX);
	//�������3��1
	bool IsLianSanDaiYi(BYTE iCardList[],int iCardCount);
	//[�������]
public:
	//�Զ����ƺ���
	BOOL AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut);
	//�Ƿ���Գ���
	BOOL CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], int iBaseCount, BYTE iHandCard[], int iHandCount,bool bFirstOut = false);
	//���ұȵ�ǰ���ƴ��
	BOOL TackOutCardMoreThanLast(BYTE iHandCard[], int iHandCardCount,BYTE iBaseCard[], int iBaseCardCount,
									BYTE iResultCard[], int & iResultCardCount,bool bExtVal=false);
	
	//[��ȡ��]
public:	
	//��ȡ����������1 or 2or 3(��,һ��,�������)
	BYTE TackOutThreeX(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount,int  xValue);
	//��ȡ2����������������1,2
	BOOL TrackOut3XSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int xValue);
	//��ȡ2����������������1,2
	BOOL TrackOut3Sequence2Sequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount);
	//��ȡ2����������������1,2
	//BYTE TrackOut3XSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, bool bExtVal=false);
	//��ȡ���ŵ�˳��,���� or ����
	//BYTE TackOutSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount);
	//��ȡ˳������Сλ��ֵ(xSequence��ʾĬ�ϵ�˳)
	int GetSequenceStartPostion(BYTE iCardList[],int iCardCount,int xSequence=1);
	//��ȡ���ŵ�˳��,����˳��,����˳��
	BOOL TackOutSequence(BYTE iCardList[],int iCardCount,BYTE iBaseCard[],int iBaseCount,BYTE iResultCard[],int &iResultCardCount,int xSequence,BOOL bNoComp = false);
	//��ȡͬ��˳
	BOOL TackOutStraightFlush(BYTE iCardList[],int iCardCount,BYTE iBaseCard[],int iBaseCount,BYTE iResultCard[], int &iResultCardCount);
	//BYTE TackOutStraightFlush(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int bExtVal=false);
	//��ȡ����ը��
	BOOL TackOutAllBomb(BYTE iCardList[],int iCardCount,
									   BYTE iResultCard[],int &iResultCardCount,int iNumCount=4);
	//��ȡը��(����Ĭ��Ϊ4)
	BOOL TackOutBomb(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,int iNumCount=4);
	//��ȡ��ը
	BOOL TackOutKingBomb(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount);
	//��ȡ510K
	BOOL TrackOut510K(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount, bool bExtVal=false);
	//����510K
	BOOL Test510K(BYTE iCardList[],int iCardCount,bool bExtVal=false);
	//����
	BOOL Copy510K(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount);
	//���������
	BOOL TackOutCardByNoSameShape(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BYTE iBaseCard[],int iBaseCardCount );
   ///��ȡָ������
	BOOL TackOutCardBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BYTE iBaseCard[],int iBaseCardCount ,BYTE iSepcifyCard);
	//�ô�����ƴ������ϵ���
	BOOL TackOutMoreThanLastShape(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BYTE iBaseCard[],int iBaseCardCount);
	//��������
	//BOOL AnalyseCard(BYTE iCardList[], int iCardCount, CardAnalyseStruct & Analyse);

public:

	//�����������м������ŵ���
	//int SearchCountOfThree(BYTE iCardList[],int iCardCount,bool bExtVal=false);
	//�������ŵ��Ƶ�
	//int SearchThreeCard(BYTE iCardList[],int iCardCount,bool bExtVal=false);
	//��������������С���Ǹ����ŵ��Ƶ�
	int SearchMinThreeSeq(BYTE iCardList[], int iCardCount, bool bExtVal=false);
	//������������
	void ResetCondition();

	///������Ϸ����
	void SetCardShape(DWORD iCardShape){m_iCardShape=iCardShape;}

	/// �����Զ����
	void DragCardAutoSetValidCard(BYTE iUpCardList[], int iUpCardCount, BYTE bResult[], int & bResultCount);

	///�û������֮��������ȡ��
	void AITrackOutCard(BYTE iCardList[] ,int iCardCount,BYTE iUpCardList[], int iUpCardCount,BYTE iBaseCardList[] ,int iBaseCardCount, BYTE bResult[], int & bResultCount);
	///������С (1) or ��� (255) ��ֵ
	///
	/// [@param in bExtVal] �棬������2����
	BYTE GetCardMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax, bool bExtVal=false);
	
	//����������
	void MatchDDZ(BYTE byCardArray[], int iCount);

	//�ϲ�������������㷨
	void MatchMergeDDZ(BYTE byCardArray1[], int iCount1,BYTE byCardArray2[], int iCount2,bool bNoXiPai);
};