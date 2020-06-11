#pragma once

//��������
#define	UG_HUA_MASK					0xF0			//1111 0000
#define	UG_VALUE_MASK				0x0F			//0000 1111

#include <vector>

#define MAKE_CARD_SHAPE(x)	CARD_SHAPE_CATTLE_##x

const int POKER_CARD_NUM = 54;       // �˿��Ƶ����� 
const int POKER_CARD_NUM_NOKING = 52;		// �˿��Ƶ�����(û�д�С��)

static const BYTE g_cardArray[54] =
{
	//2     3     4     5     6     7     8     9     10   J      Q    K     A
	0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
	0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
	0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
	0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
	0x4E, 0x4F
};

static	int g_iRandNum = 0;

enum CardShape
{
	CARD_SHAPE_CATTLE_NONE = 0,		// ��ţ
	CARD_SHAPE_CATTLE_1,			// ţ1
	CARD_SHAPE_CATTLE_2,			// ţ2
	CARD_SHAPE_CATTLE_3,			// ţ3
	CARD_SHAPE_CATTLE_4,			// ţ4
	CARD_SHAPE_CATTLE_5,			// ţ5
	CARD_SHAPE_CATTLE_6,			// ţ6
	CARD_SHAPE_CATTLE_7,			// ţ7
	CARD_SHAPE_CATTLE_8,			// ţ8
	CARD_SHAPE_CATTLE_9,			// ţ9
	CARD_SHAPE_CATTLE_CATTLE,		// ţţ
	CARD_SHAPE_CATTLE_GOLD,			// �廨ţ
	CARD_SHAPE_CATTLE_BOMB,			// ը��ţ
	CARD_SHAPE_CATTLE_SMALL,		// ��Сţ
};

class CPokerGameLogic
{
public:
	CPokerGameLogic();
	~CPokerGameLogic();

public:
	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK) + 1; }
	// ��ȡ�ۼӵĵ���
	int GetSumPoints(const BYTE* pCardList, int cardCount);
	// ��ȡ����
	int GetCardPoint(BYTE card);
	//��ȡ�˿���Դ�С
	int GetCardBulk(BYTE iCard, int bExtVol = false);
	//��ȡ�˿˻�ɫ
	BYTE GetCardHuaKind(BYTE iCard);
	// �Ƿ���ţ
	bool IsHaveCattle(const BYTE* pCardList, int cardCount);
	// �Ƿ�ţţ
	bool IsCattleCattle(const BYTE* pCardList, int cardCount);
	// ��ȡ����
	int GetCardShape(const BYTE* pCardList, int cardCount);
	// ϴ��
	bool RandCard(std::vector<BYTE>& cardVec, int cardPairs);
	// ��������ȵ�����£��Ƚ����б�A��B�������� true=A>B false=A<B
	bool CompareCardValue(int iCardShape, const BYTE* pCardListA, int cardCountA, const BYTE* pCardListB, int cardCountB);
	// ��ȡ������
	BYTE GetMaxCardByNumber(const BYTE* pCardList, int cardCount);
	// ��ȡ���ͱ���
	int GetCardShapeRate(int cardShape);
	/// ��ţ
	bool IsGoldBull(const BYTE  byCardList[], int iCardCount = 5);
	/// ը��ţ
	bool IsBombBull(const BYTE  byCardList[], int iCardCount = 5);
	/// ��С
	bool IsFiveSmall(const BYTE  byCardList[], int iCardCount = 5);
	/// �õ���������С����
	int GetMinCard(const BYTE  byCardList[], int iCardCount = 5);
	// ��«ţ��ը��ţ����Сţ�Ƚϴ�С   true��byFirstCard > bySecondCard;
	bool CompareCardToSpecial(const BYTE byFirstCard[], const BYTE bySecondCard[], int iSpecial);
	// �Ƚ����ʹ�С,  1:A>B   -1:A<B   0:A=B
	int CompareCardShape(const BYTE* pCardListA, int cardCountA, const BYTE* pCardListB, int cardCountB);
};