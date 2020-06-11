#include "StdAfx.h"
#include "UpGradeLogic.h"
#include "Util.h"

//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	m_bSortCardStyle = 0; //0 �ư���С����;1 ����������
	m_iCondition = 0;			//�޳�����������
	m_bKingCanReplace = false;
	m_iLaiZiSize = 0;
	//	m_iStation[4] = 500;
	//	for (int i=0; i<4; i++)
	//		m_iStation[i] = 100*i;
}
//���캯��
CUpGradeGameLogic::~CUpGradeGameLogic()
{
}
//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard, BOOL bTrueHua)
{
	int iHuaKind = (iCard&UG_HUA_MASK);
	if (!bTrueHua)
	{
		return iHuaKind = UG_NT_CARD;
	}
	return iHuaKind;
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard, bool bExtVal)
{
	if ((iCard == 0x4E) || (iCard == 0x4F))
	{
		return bExtVal ? (iCard - 14) : (iCard - 62); //��С��64+14-62=16	ֻ���ش�Сè��ֵ
	}

	int iCardNum = GetCardNum(iCard);
	int iHuaKind = GetCardHuaKind(iCard, TRUE);

	if (iCardNum == 2) //2��
	{
		if (bExtVal) //�й�
		{
			return ((iHuaKind >> 4) + (15 * 4));
		}
		else //û�й�����2��
		{
			return 15;
		}
	}

	return ((bExtVal) ? ((iHuaKind >> 4) + (iCardNum * 4)) : (iCardNum));
}

//��ֵ�õ���
BYTE CUpGradeGameLogic::GetCardByValue(int Value)
{
	BYTE CardArray[55] = {
		0x00,
		0x01,0x11, 0x21,0x31,
		0x02, 0x12 ,0x22 , 0x32 ,
		0x03,0x13, 0x23,0x33,
		0x04,0x14, 0x24, 0x34,
		0x05, 0x15,0x25, 0x35,
		0x06, 0x16, 0x26,0x36,
		0x07, 0x17,0x27, 0x37,
		0x08, 0x18, 0x28,  0x38,
		0x09, 0x19, 0x29,0x39,
		0x0A,  0x1A, 0x2A, 0x3A,
		0x0B,0x1B,  0x2B,0x3B,
		0x0C, 0x1C,0x2C, 0x3C,
		0x0D,  0x1D, 0x2D, 0x3D,
		0x4E, 0x4F };

	return CardArray[Value];
}


//���������ִӴ�С�����˿�
BOOL CUpGradeGameLogic::SortCard(BYTE iCardList[], BYTE bUp[], BYTE iCardCount, BOOL bSysSort)
{
	BOOL bSorted = TRUE, bTempUp;
	int iTemp, iLast = 0, iStationVol[45];
	memset(iStationVol, 0, sizeof(iStationVol));
	if (iCardCount > 45)
	{
		iCardCount = 45;
	}
	iLast = iCardCount - 1;
	//��ȡλ����ֵ
	for (int i = 0;i < iCardCount;i++)
	{
		iStationVol[i] = GetCardBulk(iCardList[i], true);
		//if (iStationVol[i]>=15) iStationVol[i]+=m_iStation[4];
		//else iStationVol[i]+=m_iStation[GetCardHuaKind(iCardList[i],FALSE)>>4];
	}

	//�������(���Ӵ�С����)
	do
	{
		bSorted = TRUE;
		for (int i = 0;i < iLast;i++)
		{
			if (iStationVol[i] < iStationVol[i + 1])
			{
				//����λ��				//==ð������
				iTemp = iCardList[i];
				iCardList[i] = iCardList[i + 1];
				iCardList[i + 1] = iTemp;

				iTemp = iStationVol[i];
				iStationVol[i] = iStationVol[i + 1];
				iStationVol[i + 1] = iTemp;

				if (bUp != NULL)
				{
					bTempUp = bUp[i];
					bUp[i] = bUp[i + 1];
					bUp[i + 1] = bTempUp;
				}
				bSorted = FALSE;
			}
		}
		iLast--;
	} while (!bSorted);

	//ϵͳ���в����ǻ�ɫ��������
	if (bSysSort)
	{
		ReverseCard(iCardList, bUp, iCardCount);
		return TRUE;
	}
	if (GetSortCardStyle() == 1) //����������
		SortCardByStyle(iCardList, iCardCount);

	if (GetSortCardStyle() == 2)
		SortCardByKind(iCardList, iCardCount);


	return TRUE;
}

BOOL CUpGradeGameLogic::ReverseCard(BYTE iCardList[], BYTE bUp[], BYTE iCardCount)
{
	BYTE iTemp;
	for (int i = 0;i < iCardCount / 2;i++)
	{
		iTemp = iCardList[i];
		iCardList[i] = iCardList[iCardCount - 1 - i];
		iCardList[iCardCount - 1 - i] = iTemp;
	}
	return TRUE;
}
//����������
BOOL CUpGradeGameLogic::SortCardByStyle(BYTE iCardList[], BYTE iCardCount)
{
	//�������������Ҫ�󰴴�С����
	if (m_bSortCardStyle == 0)
	{
		SortCard(iCardList, NULL, iCardCount);

		return TRUE;
	}

	//����Ĵ���==�������Ŵ�С
	int iStationVol[45];
	for (int i = 0;i < iCardCount;i++)
	{
		iStationVol[i] = GetCardBulk(iCardList[i], false);
	}

	int Start = 0;
	int j, step;
	BYTE CardTemp[8];					//��������Ҫ��λ������
	int CardTempVal[8];					//����������λ������ֵ
	for (int i = 8;i > 1;i--)				//����������һ������i����ͬ��ֵ
	{
		for (j = Start;j < iCardCount;j++)
		{
			CardTemp[0] = iCardList[j];			//���浱ǰi��������ͬ��ֵ
			CardTempVal[0] = iStationVol[j];
			for (step = 1;step < i&&j + step < iCardCount;)			//��һ������i��ֵ��ȵ�����(����������ʱ������)
			{
				if (iStationVol[j] == iStationVol[j + step])
				{
					CardTemp[step] = iCardList[j + step];			//������������
					CardTempVal[step] = iStationVol[j + step];		//��ֵ
					step++;
				}
				else
					break;
			}

			if (step >= i)	//�ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj,����λ��Ϊj+setp-1
			{			//����Start��ʼ��j���������setp��
				if (j != Start) //�ų���ʼ��������
				{
					for (;j >= Start;j--) //��Start����j�ź��ƶ�i��
					{
						iCardList[j + i - 1] = iCardList[j - 1];
						iStationVol[j + i - 1] = iStationVol[j - 1];
					}
					for (int k = 0;k < i;k++)
					{
						iCardList[Start + k] = CardTemp[k];	//��Start��ʼ���ó�CardSave
						iStationVol[Start + k] = CardTempVal[k];
					}
				}
				Start = Start + i;
			}
			j = j + step - 1;
		}
	}
	return TRUE;
}

//����ɫ����
BOOL CUpGradeGameLogic::SortCardByKind(BYTE iCardList[], BYTE iCardCount)
{
	return TRUE;
}

//�����˿�,������ʹ��
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount, int iRoomId, bool bHaveKing)
{
	static const BYTE gCardArray[54] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
		0x4E, 0x4F //С�����
	};

	BYTE iSend = 0, iStation = 0, iCardList[216], step = (bHaveKing ? 54 : 52);
	memset(iCardList, 0, sizeof(iCardList));
	for (int i = 0;i < iCardCount;i += step)
		::CopyMemory(&iCardList[i], gCardArray, sizeof(gCardArray));

	BYTE temp = 0, data_ = 0;
	int iCount = 3;
	while (--iCount >= 0)
	{
		for (int i = 0;i < iCardCount;i++)
		{
			temp = CUtil::GetRandNum() % (iCardCount - i) + i;
			data_ = iCardList[temp];
			iCardList[temp] = iCardList[i];
			iCardList[i] = data_;
		}
	}
	int iIndex = CUtil::GetRandNum() % (iCardCount - 20) + 20;
	int iCardIndex = 0;
	for (int i = iIndex; i < iCardCount; i++)
	{
		iCard[iCardIndex++] = iCardList[i];
	}
	for (int i = 0; i < iIndex; i++)
	{
		iCard[iCardIndex++] = iCardList[i];
	}

	return iCardCount;
}

bool CUpGradeGameLogic::IsLegalCard(BYTE iCard)
{
	static const BYTE m_Cards[54] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
		0x4E, 0x4F //С�����
	};
	for (int i = 0;i < 54;i++)
	{
		if (m_Cards[i] == iCard)
		{
			return true;
		}
	}
	return false;
}

int CUpGradeGameLogic::GetCardCount(BYTE iCard[], int iCardCount)
{
	int iCount = 0;
	for (int i = 0;i < iCardCount;i++)
	{
		if (IsLegalCard(iCard[i]))
		{
			iCount++;
		}
	}
	return iCount;
}

//ɾ���˿�
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[],   //Ҫɾ��������
	int iRemoveCount,		//Ҫɾ����������
	BYTE iCardList[],		//Ҫ���������
	int iCardCount)		//�������������
{
	//��������
	if (iRemoveCount > iCardCount) return 0;

	int iRecount;
	int iDeleteCount = 0; //��Ҫɾ����������

	for (int i = 0; i < iRemoveCount; i++)
	{
		for (int j = 0; j < iCardCount; j++)
		{
			if (iRemoveCard[i] == iCardList[j])
			{
				iDeleteCount++;
				iCardList[j] = 0;
				break;
			}
		}
	}
	iRecount = RemoveNummCard(iCardList, iCardCount); //ɾ�����˱�ǵ���

	if (iDeleteCount != iRecount)
		return 0;

	return iDeleteCount;
}


//��� 0 λ�˿�
int CUpGradeGameLogic::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount = 0;
	BYTE iCards[45];
	memset(iCards, 0, sizeof(iCards));
	memcpy(iCards, iCardList, sizeof(BYTE)*iCardCount);
	memset(iCardList, 0, sizeof(BYTE)*iCardCount);
	int index = 0;
	for (int i = 0;i < iCardCount;i++)
	{
		if (iCards[i] != 0)
		{
			iCardList[index++] = iCards[i];
		}
		else
		{
			iRemoveCount++;
		}
	}
	//for (int i=0;i<iCardCount;i++)
	//{
	//	if (iCardList[i]!=0)
	//		iCardList[i-iRemoveCount]=iCardList[i];
	//	else 
	//		iRemoveCount++;
	//}

	return iRemoveCount;
}



//��������

//�Ƚϵ���
BOOL CUpGradeGameLogic::CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard)
{
	//��һ����ʾ�����������, �ڶ�����ʾҪ������
	return GetCardBulk(iFirstCard) < GetCardBulk(iNextCard);
}

//��������ķ���(5,10,K)
int CUpGradeGameLogic::FindPoint(BYTE iCardList[], int iCardCount)
{
	int iPoint = 0; //����
	for (int i = 0; i < iCardCount; i++)
	{
		int iNum = GetCardNum(iCardList[i]); //�������
		switch (iNum)
		{
		case 5:
			iPoint += 5;
			break;
		case 10:
		case 13:
			iPoint += 10;
			break;
		}
	}
	return iPoint;
}

//�������Ƿ�����ͬ����
BOOL CUpGradeGameLogic::IsSameNumCard(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	int i, temp[18] = { 0 };
	for (i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i], false)]++;
	}

	for (i = 0; i < 18; i++)
	{
		if (temp[i] != 0)
			break;
	}
	if (m_bKingCanReplace)
	{
		if (i < 16)//����������
			return (temp[i] + temp[16] + temp[17] == iCardCount);
		//else//ֻ����
		if (i < 17)
			return (temp[16] + temp[17] == iCardCount);
	}
	else
		return (temp[i] == iCardCount);
	return 0;
}

//�Ƿ�Ϊͬ��
BOOL CUpGradeGameLogic::IsSameHuaKind(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount <= 0) return FALSE;

	BYTE iFirstHua = GetCardHuaKind(iCardList[0], TRUE); //ȡ�õ�һ���ƵĻ�ɫ

	for (int i = 1; i < iCardCount; i++) //����Ķ��͵�һ�ŵĻ�ɫ��
	{
		if (GetCardHuaKind(iCardList[i], TRUE) != iFirstHua)
		{
			return FALSE;
		}
	}

	return TRUE;
	/*
	BYTE Kind[5] = {0};

	for(int i = 0; i < iCardCount; i++)
	{
	Kind[GetCardHuaKind(iCardList[i], TRUE)/16] = 1; //��Ӧ��ɫ��ֵ��1
	}

	BYTE iPos = 0;
	for(int i = 0; i < 4; i++)
	{
	if(Kind[i] == 1) iPos++; //�����м�����ɫ
	}

	if(iPos != 1) return FALSE;

	return TRUE;
	//*/
}

//�����û�����ը����
BYTE CUpGradeGameLogic::GetBombCount(BYTE iCardList[], int iCardCount, int iNumCount, bool bExtVal)
{
	int iCount = 0,
		temp[18] = { 0 };
	for (int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}
	for (int i = 0;i < 16;i++)
	{
		if (temp[i] >= iNumCount)
			iCount++;
	}
	return iCount;
}

//��ȡָ�������Ƹ���
BYTE CUpGradeGameLogic::GetCountBySpecifyNumCount(BYTE iCardList[], int iCardCount, int Num)
{
	BYTE temp[18] = { 0 };
	int count = 0;
	for (int i = 0;i < iCardCount;i++)
		temp[GetCardBulk(iCardList[i])]++;

	for (int i = 0;i < 18;i++)
		if (temp[i] == Num)
			count++;

	return count;
}

//��ȡָ���Ƹ���
BYTE CUpGradeGameLogic::GetCountBySpecifyCard(BYTE iCardList[], int iCardCount, BYTE bCard)
{
	int count = 0;
	for (int i = 0;i < iCardCount;i++)
		if (iCardList[i] == bCard)
			count++;

	return count;
}
//��ȡָ���������ƴ�С
BYTE CUpGradeGameLogic::GetBulkBySpecifyCardCount(BYTE iCardList[], int iCardCount, int iCount)
{
	BYTE temp[18] = { 0 };
	for (int i = 0;i < iCardCount;i++)
		temp[GetCardBulk(iCardList[i])]++;

	for (int i = 17;i > 0;i--)
		if (temp[i] == iCount)
			return i;

	return 0;
}

//�Ƿ�Ϊ����˳��
BOOL CUpGradeGameLogic::IsVariationSequence(BYTE iCardList[], int iCardCount, int iCount)
{
	int iValue = iCardCount / iCount;
	if (iCardCount != iCount *iValue)						 //����������
		return FALSE;

	int iFirstMax = 0, iSecondMax = 0, iThirdMax = 0, iMin = 18;//�ҳ���һ��,�ڶ���,���������,����С��
	BYTE temp[18] = { 0 };
	for (int i = 0;i < iCardCount;i++)						//�ƶ���
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for (int i = 0;i < 18;i++)
	{
		if (temp[i] != 0 && temp[i] != iCount)	//���Ҷ�˳��
			return false;
	}

	for (int i = 0;i < 18;i++)						//��С�������ܵ�A
	{
		if (temp[i] != 0)
			iMin = i;
	}

	for (int i = 17;i > 0;i--)
	{
		if (temp[i] != 0)
		{
			iFirstMax = i;						//������2Ҳ������A
			for (int j = i - 1;j > 0;j--)
			{
				if (temp[j] != 0)//�ҵ��ڶ�����˳�ѭ��(�޵������)//������AҲ���Է�A
				{
					iSecondMax = j;
					for (int k = j - 1;j > 0;j--)
					{
						if (temp[k] != 0)//��ڵ�������˳�ѭ��	//���Ǵ���Ҳ���Բ�����
						{
							iThirdMax = k;
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	if (iFirstMax < 15)	//������2�����,���������
	{
		return (iFirstMax - iMin + 1 == iValue);
	}

	if (iFirstMax == 15)	//����2,�ٿ��Ƿ����A
	{
		if (iSecondMax == 14)		//����A
		{
			if (iThirdMax == 0)		//�����ڵ�����,Ҳֻ��A2������
				return true;

			return (iThirdMax - iMin + 1 == iValue - 2);		//���� A2�����������AA2233
		}
		return (iSecondMax - iMin + 1 == iValue - 1);
	}

	return false;
}

//�Ƿ�����
BOOL CUpGradeGameLogic::IsSequence(BYTE iCardList[], int iCardCount, int iCount)
{
	BYTE temp[18] = { 0 };
	for (int i = 0;i < iCardCount;i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for (int i = 0; i < 15; i++)
	{
		if (temp[i] != 0 && temp[i] != iCount)	//��ָ��˳
			return false;
	}

	int len = iCardCount / iCount;
	//TCHAR sz[200];
	//wsprintf(sz,"iCardCount=%d,iCount=%d,len=%d",iCardCount,iCount,len);
	//WriteStr(sz);
	for (int i = 0;i < 15;i++)
	{
		if (temp[i] != 0)//��ֵ
		{
			//if(temp[i] == iCount )
			//{	
			for (int j = i;j < i + len;j++)
			{
				if (temp[j] != iCount || j >= 15)
					return false;
			}
			return true;
			//}else 
			//	return false;
		}
	}
	return false;
}

//��ȡָ���Ʒ����ҵ��Ƃ���
BYTE  CUpGradeGameLogic::TackOutBySpecifyCard(BYTE iCardList[], int iCardCount, BYTE bCardBuffer[], int &iResultCardCount, BYTE bCard)
{
	iResultCardCount = 0;
	for (int i = 0;i < iCardCount;i++)
	{
		if (iCardList[i] == bCard)
			bCardBuffer[iResultCardCount++] = iCardList[i];
	}
	return iResultCardCount;
}


//��ȡ1,2,3 or 4����ͬ���ֵ���
int CUpGradeGameLogic::TackOutBySepcifyCardNumCount(BYTE iCardList[], int iCardCount,
	BYTE iDoubleBuffer[], BYTE bCardNum,
	bool bExtVal)
{
	int iCount = 0, temp[18] = { 0 };
	for (int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for (int i = 0; i < 18; i++)
	{
		if (temp[i] == bCardNum) //����Ҫ���ҵ�����:one?double?three?four_bomb?
		{
			for (int j = 0; j < iCardCount; j++)
			{
				if (i == GetCardBulk(iCardList[j]))
					iDoubleBuffer[iCount++] = iCardList[j];
			}
		}
	}
	return iCount;
}

//��ȡָ����ɫ��
int CUpGradeGameLogic::TackOutByCardKind(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], BYTE iCardKind)
{
	int count = 0;

	for (int i = 0;i < iCardCount; i++)
	{
		//TCHAR sz[200];
		//wsprintf(sz,"i=%d,iCardKind = %d %d",i,iCardKind,GetCardHuaKind(iCardList[i]));
		//WriteStr(sz,2,2);
		if (GetCardHuaKind(iCardList[i]) == iCardKind)
		{
			iDoubleBuffer[count++] = iCardList[i];
		}
	}
	return count;
}

//���(�������ƶ�Ĳ���ٵ�)
int CUpGradeGameLogic::TackOutMuchToFew(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], int &iBufferCardCount, BYTE iCardMuch, BYTE iCardFew)
{
	iBufferCardCount = 0;
	int count = 0;
	BYTE iBuffer[45];
	int iCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, iCardMuch);
	if (iCount <= 0)
		return count;
	for (int i = 0;i < iCount; i += iCardMuch)
	{
		::CopyMemory(&iDoubleBuffer[iBufferCardCount], &iBuffer[i], sizeof(BYTE)*iCardFew);
		iBufferCardCount += iCardFew;
		count++;
	}
	return count;
}

//��ȡĳ��ָ����С����
BOOL CUpGradeGameLogic::TackOutCardBySpecifyCardNum(BYTE iCardList[], int iCardCount, BYTE iBuffer[], int &iBufferCardCount, BYTE iCard, BOOL bExtVal)
{
	iBufferCardCount = 0;
	BYTE iCardNum = GetCardBulk(iCard); //�õ��������
	for (int i = 0; i < iCardCount; i++)
	{
		if (GetCardBulk(iCardList[i]) == iCardNum) //����Ҫ���ҵ��Ƶ�����
		{
			iBuffer[iBufferCardCount++] = iCardList[i];
		}
	}

	return iBufferCardCount;
}

//���Ҵ���iCard�ĵ�������iCardList�е����
BYTE  CUpGradeGameLogic::GetSerialByMoreThanSpecifyCard(BYTE iCardList[], int iCardCount,
	BYTE iCard, BYTE iBaseCardCount,
	bool bExtValue)
{
	BYTE MaxCard = 0;
	BYTE Serial = 0;
	BYTE MaxCardNum = 255;

	int BaseCardNum = GetCardBulk(iCard);	//��ǰ�Ƚ�ֵ

	for (BYTE i = 0; i < iCardCount; i += iBaseCardCount)
	{
		int temp = GetCardBulk(iCardList[i]);

		if (temp<MaxCardNum && temp>BaseCardNum)
		{
			MaxCardNum = temp;
			Serial = i; //�õ����
			break;
		}
	}

	return Serial;
}


//����==iCard�ĵ�������iCardList�е����
int  CUpGradeGameLogic::GetSerialBySpecifyCard(BYTE iCardList[], int iStart, int iCardCount, BYTE iCard)
{
	for (int i = iStart;i < iCardCount;i++)
	{
		if (iCardList[i] == iCard)
			return i;
	}
	return -1;
}

//����˳��������
BYTE CUpGradeGameLogic::GetBulkBySpecifyVariationSequence(BYTE iCardList[], int iCardCount, int iSequence)
{
	int iFirstMax = 0, iSecondMax = 0, iThirdMax = 0;//�ҳ���һ��,�ڶ���,���������,����С��
	BYTE temp[18] = { 0 };
	for (int i = 0;i < iCardCount;i++)						//�ƶ���
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for (int i = 17;i > 0;i++)
	{
		if (temp[i] == iSequence)
		{
			iFirstMax = i;						//������2Ҳ������A
			for (int j = i - 1;j > 0;j--)
			{
				if (temp[j] == iSequence)//�ҵ��ڶ�����˳�ѭ��(�޵������)//������AҲ���Է�A
				{
					iSecondMax = j;
					for (int k = j - 1;j > 0;j--)
					{
						if (temp[k] == iSequence)//��ڵ�������˳�ѭ��	//���Ǵ���Ҳ���Բ�����
						{
							iThirdMax = k;
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	if (iFirstMax == 15)	//����2,�ٿ��Ƿ����A
	{
		if (iSecondMax == 14)		//����A
		{
			if (iThirdMax == 0)		//�����ڵ�����,Ҳֻ��A2������
				return 2;

			return iThirdMax;		//���� A2�����������AA2233
		}
		return iSecondMax;
	}
	return 0;
}

//��ȡָ��˳�����Ƶ���Сֵ(iSequence ����˳�ӵ��������Ϊ
BYTE  CUpGradeGameLogic::GetBulkBySpecifySequence(BYTE iCardList[], int iCardCount, int iSequence)
{
	int temp[18] = { 0 };
	for (int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for (int k = 0; k < 15; k++)
	{
		if (temp[k] == iSequence)
		{
			return k;
		}
	}
	return 0;
}

//�ҳ�һ����С��������
int  CUpGradeGameLogic::GetBulkBySepcifyMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax/*1 or 255*/, bool bExtVal)
{
	int CardNum = GetCardBulk(iCardList[0], false);

	if (MinOrMax == 1) //����С��
	{
		for (int i = 1; i < iCardCount; i++)
		{
			if (GetCardBulk(iCardList[i], false) < CardNum)
				CardNum = GetCardBulk(iCardList[i], false);
		}
	}
	else if (MinOrMax == 255)
	{
		for (int i = 1; i < iCardCount; i++)
		{
			if (GetCardBulk(iCardList[i], false) > CardNum)
				CardNum = GetCardBulk(iCardList[i], false);
		}
	}

	//���ص��� GetCardBulk() �õ���ֵ
	return CardNum;
}

/////////////////////////////////////////////////////////////////////////
/**
* @info ��ȡ����
* @param iCardList[] ��
* @param iCardCount �Ƶ�����
* @param bExlVol ?
*/
BYTE CUpGradeGameLogic::GetCardShape(BYTE iCardList[], int iCardCount, bool bExlVol)
{
	if (IsOnlyOne(iCardList, iCardCount) && (m_iCardShape&(0x01))) return UG_ONLY_ONE; //����
	if (IsDouble(iCardList, iCardCount) && (m_iCardShape&(0x01 << 1))) return UG_DOUBLE;	 //����
	if (IsThreeX(iCardList, iCardCount, 0) && (m_iCardShape&(0x01 << 2))) return UG_THREE;	 //����

	if (IsThreeX(iCardList, iCardCount, 1) && (m_iCardShape&(0x01 << 3))) return UG_THREE_ONE; //����һ
	//if (IsThreeX(iCardList, iCardCount, 2)&&(m_iCardShape&(0x01<<4))) return UG_THREE_TWO; //������
	if (IsThreeX(iCardList, iCardCount, 3) && (m_iCardShape&(0x01 << 5)))	return UG_THREE_DOUBLE;	//������

	/* ˳���а��� ͬ��˳,�������ж��Ƿ�ͬ��˳,������ǣ����ж��Ƿ���˳�ӣ������˳�ӣ�����һ���˳����*/
	//if (IsStraightFlush(iCardList, iCardCount)&&(m_iCardShape&(0x01<<7))) return UG_STRAIGHT_FLUSH; //ͬ��˳
	if (IsStraight(iCardList, iCardCount) && (m_iCardShape&(0x01 << 6))) return UG_STRAIGHT;            //˳��	
	if (IsDoubleSequence(iCardList, iCardCount) && (m_iCardShape&(0x01 << 8))) return UG_DOUBLE_SEQUENCE;  //����

	if (IsThreeXSequence(iCardList, iCardCount, 3) && (m_iCardShape&(0x01 << 12))) return UG_THREE_DOUBLE_SEQUENCE; //����������
	//if (IsThreeXSequence(iCardList, iCardCount, 2)&&(m_iCardShape&(0x01<<11))) return UG_THREE_TWO_SEQUENCE; //����������
	if (IsThreeXSequence(iCardList, iCardCount, 1) && (m_iCardShape&(0x01 << 10))) return UG_THREE_ONE_SEQUENCE; //��������һ
	if (IsThreeXSequence(iCardList, iCardCount, 0) && (m_iCardShape&(0x01 << 9))) return UG_THREE_SEQUENCE; //����
	if (IsLianSanDaiYi(iCardList, iCardCount)) return UG_THREE_ONE_SEQUENCE; //�����������һ

	if (IsFourX(iCardList, iCardCount, 4) && (m_iCardShape&(0x01 << 16))) return UG_FOUR_TWO_DOUBLE;		//�Ĵ�����(Ҫ���Ƕ���)
	if (IsFourX(iCardList, iCardCount, 3) && (m_iCardShape&(0x01 << 15))) return UG_FOUR_TWO;//UG_FOUR_ONE_DOUBLE;		//�Ĵ�һ��(Ҫ��ɶ�)
	if (IsFourX(iCardList, iCardCount, 2) && (m_iCardShape&(0x01 << 14))) return UG_FOUR_TWO;			//�Ĵ���(��Ҫ��ɶ�)
	//if	(IsFourX(iCardList,iCardCount,1)&&(m_iCardShape&(0x01<<13))) return UG_FOUR_ONE;			//�Ĵ�һ

	//if (IsFourXSequence(iCardList,iCardCount,4)) return UG_FOUR_TWO_DOUBLE_SEQUENCE;	//��˳������
	//if (IsFourXSequence(iCardList,iCardCount,2)) return UG_FOUR_TWO_SEQUENCE;	//��˳��������
	//if (IsFourXSequence(iCardList,iCardCount,0)) return UG_FOUR_SEQUENCE;	//��˳

	if (IsKingBomb(iCardList, iCardCount) && (m_iCardShape&(0x01 << 20))) return UG_KING_BOMB;//��ը
	if (IsBomb(iCardList, iCardCount) && (m_iCardShape&(0x01 << 19))) return UG_BOMB; //4������ͬ���ƣ�ը��

////	if (IsVariationFourXSequence(iCardList,iCardCount,0)) return UG_VARIATION_FOUR_SEQUENCE;	//������˳
////	if (IsVariationFourXSequence(iCardList,iCardCount,1)) return UG_VARIATION_FOUR_ONE_SEQUENCE;	//������˳������
////	if (IsVariationFourXSequence(iCardList,iCardCount,2)) return UG_VARIATION_FOUR_TWO_SEQUENCE;	//������˳��������
////	if (IsVariationFourXSequence(iCardList,iCardCount,3)) return UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE;	//������˳��һ��
////	if (IsVariationFourXSequence(iCardList,iCardCount,4)) return UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE;	//������˳������
//
//	/* ˳���а��� ͬ��˳,�������ж��Ƿ�ͬ��˳,������ǣ����ж��Ƿ���˳�ӣ������˳�ӣ�����һ���˳����*/
////	if (IsStraightFlush(iCardList, iCardCount)) return UG_STRAIGHT_FLUSH; //ͬ��˳
//	//if	(IsFlush(iCardList,iCardCount)) return UG_FLUSH;					//ͬ��(��˳��)
//	if  (IsStraight(iCardList, iCardCount)) return UG_STRAIGHT;            //˳��	
////	if	(IsVariationStraight(iCardList,iCardCount,1)) return UG_VARIATION_STRAIGHT;				//���ֵ�˳
//
////	if	(IsVariationDoubleSequence(iCardList,iCardCount,2)) return UG_VARIATION_STRAIGHT;		//����˫˳
//	if (IsDoubleSequence(iCardList, iCardCount)) return UG_DOUBLE_SEQUENCE;  //����
//
////	if (IsThreeSequenceDoubleSequence(iCardList,iCardCount)) return UG_THREE_SEQUENCE_DOUBLE_SEQUENCE;//����
////	if (IsVariationThreeSequenceDoubleSequence(iCardList,iCardCount)) return UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE;//���ֺ���
//
//	if (IsThreeXSequence(iCardList, iCardCount, 3)) return UG_THREE_DOUBLE_SEQUENCE; //����������
////	if (IsThreeXSequence(iCardList, iCardCount, 2)) return UG_THREE_TWO_SEQUENCE; //����������
//	if (IsThreeXSequence(iCardList, iCardCount, 1)) return UG_THREE_ONE_SEQUENCE; //��������һ
//	if (IsThreeXSequence(iCardList, iCardCount, 0)) return UG_THREE_SEQUENCE; //����
//
////	if (IsVariationThreeXSequence(iCardList, iCardCount, 3)) return UG_THREE_DOUBLE_SEQUENCE; //��������������
////	if (IsVariationThreeXSequence(iCardList, iCardCount, 2)) return UG_THREE_TWO_SEQUENCE; //��������������
////	if (IsVariationThreeXSequence(iCardList, iCardCount, 1)) return UG_THREE_ONE_SEQUENCE; //������������һ
////	if (IsVariationThreeXSequence(iCardList, iCardCount, 0)) return UG_THREE_SEQUENCE; //��������
//
////	if (IsMaster510K(iCardList, iCardCount)) return UG_MASTER_510K; //510Kͬ��ը��
////	if (IsSlave510K(iCardList, iCardCount)) return UG_SLAVE_510K;            //510Kը��

	return UG_ERROR_KIND;
}

//����
BOOL CUpGradeGameLogic::IsDouble(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount != 2)
		return FALSE;
	return IsSameNumCard(iCardList, iCardCount, bExtVal);
}

//3 �� 0,1or2,or3
BOOL CUpGradeGameLogic::IsThreeX(BYTE iCardList[], int iCardCount, int iX, bool bExtVal)
{
	if (iCardCount > 5 || iCardCount < 3)
	{
		return FALSE;
	}
	if (GetCountBySpecifyNumCount(iCardList, iCardCount, 3) != 1)//�Ƿ��������
	{
		return FALSE;
	}
	switch (iX)
	{
	case 0:
		return iCardCount == 3;//IsSameNumCard(iCardList, iCardCount, bExtVal);//����
		break;
	case 1:
		return iCardCount == 4;//������
		break;
	case 2:
		return iCardCount == 5;//�����ţ����ԷǶ��ӣ�
		break;
	case 3:					//��һ��
		return GetCountBySpecifyNumCount(iCardList, iCardCount, 2) == 1;//�Ƿ���ڶ���
		break;
	default:
		break;
	}
	return false;
}

//�Ĵ�1or2
BOOL CUpGradeGameLogic::IsFourX(BYTE iCardList[], int iCardCount, int iX)
{
	if (iCardCount > 8 || iCardCount < 4)
		return false;

	if (GetCountBySpecifyNumCount(iCardList, iCardCount, 4) != 1)//�Ƿ����ĸ�����
		return false;

	switch (iX)
	{
	case 0:
		return iCardCount == 4;//����
	case 1:
		return iCardCount == 5;//�Ĵ�1��
	case 2:
		return iCardCount == 6;//�Ĵ�2(��Ҫ��ɶ�)
	case 3:
		return (iCardCount == 6 && 1 == GetCountBySpecifyNumCount(iCardList, iCardCount, 2));//Ҫ��ɶ�
	case 4:
		return (iCardCount == 8 && 2 == GetCountBySpecifyNumCount(iCardList, iCardCount, 2));	//�Ĵ�2��
	}

	return FALSE;
}

//��ը
BOOL CUpGradeGameLogic::IsKingBomb(BYTE iCardList[], int iCardCount)			//�Ƿ�Ϊ��ը(ץ��������)
{
	if (iCardCount != KING_COUNT)
		return false;

	for (int i = 0;i < iCardCount;i++)
		if (iCardList[i] != 0x4e && iCardList[i] != 0x4f)
			return false;
	return true;
}

//4+���� ը��
BOOL CUpGradeGameLogic::IsBomb(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount < 4)
		return FALSE;

	return IsSameNumCard(iCardList, iCardCount, bExtVal); //�Ƿ�����ͬ����
}

//ͬ��ը��
BOOL CUpGradeGameLogic::IsBombSameHua(BYTE iCardList[], int iCardCount)
{
	if (!IsBomb(iCardList, iCardCount)) return false;
	if (!IsSameHuaKind(iCardList, iCardCount)) return false;
	return TRUE;
}

//ͬ��(��ͬ��)
BOOL CUpGradeGameLogic::IsFlush(BYTE iCardList[], int iCardCount)
{
	return IsSameHuaKind(iCardList, iCardCount);
}

//ͬ��˳ 5��ͬ��������
BOOL CUpGradeGameLogic::IsStraightFlush(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (!IsSameHuaKind(iCardList, iCardCount, bExtVal)) return FALSE; //ͬ����

	if (!IsStraight(iCardList, iCardCount, bExtVal)) return FALSE; //˳�ӣ�
	return TRUE;
}

//�Ƿ��Ǳ���˳��(A2345)��23456
BOOL CUpGradeGameLogic::IsVariationStraight(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount < 5)
		return false;
	return IsVariationSequence(iCardList, iCardCount, 1);
}

//�Ƿ���˳��ָ������
BOOL CUpGradeGameLogic::IsStraight(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount < 5)
		return false;
	return IsSequence(iCardList, iCardCount, 1);
}

//�Ƿ��Ǳ�������AA22��2233��
BOOL CUpGradeGameLogic::IsVariationDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount % 2 != 0 || iCardCount < 4)
		return false;

	return IsVariationSequence(iCardList, iCardCount, 2);
}

//�Ƿ�������
BOOL CUpGradeGameLogic::IsDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount % 2 != 0 || iCardCount < 6)
		return false;

	return IsSequence(iCardList, iCardCount, 2);
}

//������˳����˳
BOOL CUpGradeGameLogic::IsVariationThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount < 10)		//��˳����2��˳Ҳ���ٶ�
		return false;

	BYTE iBuffer3[45], iBuffer2[45];
	BOOL bValue3 = false, bValue2 = false;	//��˳,��˳�Ƿ�Ϊ˳,
	int TackOutCount3 = 0, TackOutCount2 = 0;

	TackOutCount3 = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer3, 3);//����
	TackOutCount2 = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer2, 2);//����

	if (TackOutCount3 <= 0 || TackOutCount2 <= 0 || TackOutCount3 + TackOutCount2 != iCardCount || TackOutCount3 / 3 != TackOutCount2 / 2)
		return FALSE;

	bValue3 = IsVariationSequence(iBuffer3, TackOutCount3, 3);
	bValue2 = (IsVariationSequence(iBuffer2, TackOutCount2, 2) || IsSequence(iBuffer2, TackOutCount2, 2));
	return bValue3&&bValue2;
}

//��˳����˳
BOOL CUpGradeGameLogic::IsThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount < 10)		//��˳����2��˳Ҳ���ٶ�
		return false;

	BYTE iBuffer3[45], iBuffer2[45];
	BOOL bValue3 = false, bValue2 = false;	//��˳,��˳�Ƿ�Ϊ˳,
	int TackOutCount3 = 0, TackOutCount2 = 0;

	TackOutCount3 = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer3, 3);//����
	TackOutCount2 = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer2, 2);//����
	if (TackOutCount3 <= 0 || TackOutCount2 <= 0 || TackOutCount3 + TackOutCount2 != iCardCount || TackOutCount3 / 3 != TackOutCount2 / 2)
		return FALSE;
	bValue3 = IsSequence(iBuffer3, TackOutCount3, 3);
	//	TCHAR sz[200];
	//wsprintf(sz,"%d",bValue3);
	//	WriteStr(sz);
	bValue2 = (IsVariationSequence(iBuffer2, TackOutCount2, 2) || IsSequence(iBuffer2, TackOutCount2, 2));
	//	TCHAR sz[200];
	//	wsprintf(sz,"bValue3=%d,bValue2=%d==����2˳%d,��׼��˳%d",bValue3,bValue2,IsVariationSequence(iBuffer2,TackOutCount2,2),IsSequence(iBuffer2,TackOutCount2,2));
	//	WriteStr(sz);
	return bValue3&&bValue2;
}


//����������x
BOOL CUpGradeGameLogic::IsVariationThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX, bool bExtVal)
{
	if (iCardCount < 6)		//��˳����2
		return false;

	BYTE iBuffer[45];
	int TackOutCount = 0;
	switch (iSeqX)
	{
	case 0:
		if (iCardCount % 3 != 0)
			return false;
		return IsVariationSequence(iCardList, iCardCount, 3);
		break;
	case 1://����
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 3);
		if (TackOutCount > 0 && TackOutCount / 3 * 4 == iCardCount)
			return IsVariationSequence(iBuffer, TackOutCount, 3);
		break;
	case 2://������
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 3);
		if (TackOutCount > 0 && TackOutCount / 3 * 5 == iCardCount)
			return IsVariationSequence(iBuffer, TackOutCount, 3);
	case 3://��һ��
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 3);
		if (TackOutCount > 0 && TackOutCount / 3 * 5 == iCardCount
			&&GetCountBySpecifyNumCount(iCardList, iCardCount, 2))
			return IsVariationSequence(iBuffer, TackOutCount, 3);

		break;
	}
	return false;
}

//�������� 0,1 or 2
BOOL CUpGradeGameLogic::IsThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX, bool bExtVal)
{
	if (iCardCount < 6)		//��˳����2
		return false;

	BYTE iBuffer[45];
	int TackOutCount = 0;
	switch (iSeqX)
	{
	case 0:
		if (iCardCount % 3 != 0)
			return false;
		return IsSequence(iCardList, iCardCount, 3);
		break;
	case 1://����
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 3);
		if (TackOutCount > 0 && TackOutCount / 3 * 4 == iCardCount)
			return IsSequence(iBuffer, TackOutCount, 3); //2011-6-28 �޸�333444�������55
			//&& (TackOutCount/3==GetCountBySpecifyNumCount(iCardList,iCardCount,1));//����Ҫ��333444���ܴ�55;
		break;
	case 2://������
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 3);
		if (TackOutCount > 0 && TackOutCount / 3 * 5 == iCardCount)
			return IsSequence(iBuffer, TackOutCount, 3);
	case 3://����
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 3);
		if (TackOutCount > 0 && TackOutCount / 3 * 5 == iCardCount
			&&GetCountBySpecifyNumCount(iCardList, iCardCount, 2) == TackOutCount / 3)
			return IsSequence(iBuffer, TackOutCount, 3);

		break;
	}
	return false;
}

//������˳����
BOOL CUpGradeGameLogic::IsVariationFourXSequence(BYTE iCardList[], int iCardCount, int iSeqX)
{
	if (iCardCount < 8)		//��˳����2
		return false;

	BYTE iBuffer[45];
	int TackOutCount = 0;
	switch (iSeqX)
	{
	case 0:
		if (iCardCount % 4 != 0)
			return false;
		return IsVariationSequence(iCardList, iCardCount, 4);
		break;

	case 1://������
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 4);
		if (TackOutCount > 0 && TackOutCount / 4 * 5 == iCardCount)
			return IsVariationSequence(iBuffer, TackOutCount, 4);
		break;

	case 2://������(���ԷǶ��ӣ�
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 4);
		if (TackOutCount > 0 && TackOutCount / 4 * 6 == iCardCount)
			return IsVariationSequence(iBuffer, TackOutCount, 4);

	case 3://��һ��
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 4);
		if (TackOutCount > 0 && TackOutCount / 4 * 6 == iCardCount
			&&TackOutCount / 4 == GetBulkBySpecifyCardCount(iCardList, iCardCount, 2))
			return IsVariationSequence(iBuffer, TackOutCount, 4);

	case 4://(�����ԣ�
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 4);
		if (TackOutCount > 0 && TackOutCount / 4 * 6 == iCardCount
			&&TackOutCount / 2 == GetBulkBySpecifyCardCount(iCardList, iCardCount, 2))
			return IsVariationSequence(iBuffer, TackOutCount, 4);
		break;
	}
	return FALSE;
}

//��˳����
BOOL CUpGradeGameLogic::IsFourXSequence(BYTE iCardList[], int iCardCount, int iSeqX)
{
	if (iCardCount < 8)		//��˳����2
		return false;

	BYTE iBuffer[45];
	int TackOutCount = 0;
	switch (iSeqX)
	{
	case 0:
		if (iCardCount % 4 != 0)
			return false;
		return IsSequence(iCardList, iCardCount, 4);
		break;

	case 1://������
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 4);
		if (TackOutCount > 0 && TackOutCount / 4 * 5 == iCardCount)
			return IsSequence(iBuffer, TackOutCount, 4);
		break;

	case 2://������(���ԷǶ��ӣ�
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 4);
		if (TackOutCount > 0 && TackOutCount / 4 * 6 == iCardCount)
			return IsSequence(iBuffer, TackOutCount, 4);

	case 3://��һ��
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 4);
		if (TackOutCount > 0 && TackOutCount / 4 * 6 == iCardCount
			&&TackOutCount / 4 == GetBulkBySpecifyCardCount(iCardList, iCardCount, 2))
			return IsSequence(iBuffer, TackOutCount, 4);

	case 4://(�����ԣ�
		TackOutCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iBuffer, 4);
		if (TackOutCount > 0 && TackOutCount / 4 * 6 == iCardCount
			&&TackOutCount / 2 == GetBulkBySpecifyCardCount(iCardList, iCardCount, 2))
			return IsSequence(iBuffer, TackOutCount, 4);
		break;
	}
	return FALSE;
}

//�������3��1
bool CUpGradeGameLogic::IsLianSanDaiYi(BYTE iCardList[], int iCardCount)
{
	if (iCardList == NULL || iCardCount % 4 != 0 || iCardCount < 8)
	{
		return false;
	}
	BYTE temp[18] = { 0 };
	int iTemp[18] = { 0 };
	for (int i = 0;i < iCardCount;i++)
		temp[GetCardBulk(iCardList[i])]++;

	int iNum1 = 0, iNum3 = 0, iNum4 = 0;
	int iIndex = 0;
	for (int i = 0; i < 15; i++)
	{
		if (temp[i] == 1)
		{
			iNum1++;
		}
		else if (temp[i] == 3)
		{
			iNum3++;
			iTemp[iIndex++] = i;
		}
		else if (temp[i] == 4)
		{
			iNum4++;
			iTemp[iIndex++] = i;
		}
	}
	//�����������жϣ�����ը�����������0
	if (iNum4 > 0 && iNum1 > 0 && (iNum3 + iNum4) == (iCardCount / 4))
	{
		bool bSign = true;
		for (int i = 0; i < iIndex - 1; i++)
		{
			if (abs(iTemp[i] - iTemp[i + 1]) != 1)
			{
				bSign = false;
			}
		}
		return bSign;
	}
	return false;
}

//�ж��Ƿ���510K ը��
BOOL CUpGradeGameLogic::IsSlave510K(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount != 3) return false;
	BYTE Test[18] = { 0 };
	for (int i = 0; i < iCardCount; i++)
	{
		Test[GetCardNum(i)] ++;
	}

	return (Test[5] == Test[10] == Test[13] == 1);
}

//�ж��Ƿ���510K ͬ��
BOOL CUpGradeGameLogic::IsMaster510K(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount != 3) return false; //��������

	if (!IsSameHuaKind(iCardList, iCardCount, bExtVal)) return false; //ͬ�� ��

	if (!IsSlave510K(iCardList, iCardCount, bExtVal)) return false; //510K ��	

	return true;
}
////////////////////////////////////////////////////////////////////////////////////


//�Զ��ҳ����Գ�����
BOOL CUpGradeGameLogic::AutoOutCard(BYTE iHandCard[], int iHandCardCount, //��ǰ����������е�������
	BYTE iBaseCard[], int iBaseCardCount, //ǰһ�����Ƶ��˳���������
	BYTE iResultCard[], int & iResultCardCount, //�ҵ��Ľ��
	BOOL bFirstOut) //��ǰ����Ƿ�����
{
	iResultCardCount = 0;
	if (bFirstOut) //���ֳ����ұ�һ����
	{
		TackOutCardBySpecifyCardNum(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iHandCard[iHandCardCount - 1]);
	}
	else //����
	{
		//�����е������ҳ��������ϴ����
		TackOutCardMoreThanLast(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, false);

		if (!CanOutCard(iResultCard, iResultCardCount, iBaseCard, iBaseCardCount, iHandCard, iHandCardCount))
		{
			iResultCardCount = 0;
		}
	}

	return TRUE;
}


//����һ���ȵ�ǰ���
BOOL CUpGradeGameLogic::TackOutCardMoreThanLast(BYTE iHandCard[], int iHandCardCount,
	BYTE iBaseCard[], int iBaseCardCount,
	BYTE iResultCard[], int &iResultCardCount,
	bool bExtVal)
{
	BYTE iTempCard[45];
	iResultCardCount = 0;
	int iBaseShape = GetCardShape(iBaseCard, iBaseCardCount); //�������Ƶ�����
	//	TCHAR sz[200];
	//	wsprintf(sz,"��������iBaseShape=%d",iBaseShape);
	//	WriteStr(sz);

	switch (iBaseShape)
	{
	case UG_ONLY_ONE: //����
	case UG_DOUBLE:   //����
	case UG_THREE:    //����
	case UG_BOMB:	//���� ը��
	{
		//����1,2,3,or4����
		BYTE iCount = TackOutBySepcifyCardNumCount(iHandCard, iHandCardCount, iTempCard, iBaseCardCount);

		if (iCount > 0)
		{
			BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, iBaseCard[0], iBaseCardCount, false);
			CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*iBaseCardCount);

			if (CompareOnlyOne(iBaseCard[0], iResultCard[0]))
			{
				iResultCardCount = iBaseCardCount;
			}
		}
		break;
	}
	//case UG_THREE:    //����Ҳ���������������ȡ
	case UG_THREE_ONE: //����һ
	{
		if (TackOutThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 1))
			return TRUE;
		break;
	}
	case UG_THREE_TWO: //��������
	{
		if (TackOutThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 2))
			return TRUE;
		break;
	}
	case UG_THREE_DOUBLE:	//����һ��
	{
		if (TackOutThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 3))
			return TRUE;
		break;
	}
	case UG_FLUSH:		//ͬ��
	{
		//if(TackOutStraightFlush(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
		//	return true;
		break;
	}

	case UG_STRAIGHT: //˳��
		/*if(TackOutStraightFlush(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
		{
		return TRUE; //������ͬ�Ƶ��ͬ��˳
		}*/
		if (TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 1))
		{
			return TRUE; //�����Ƶ���˳��
		}
		break;
	case UG_STRAIGHT_FLUSH: //ͬ��˳
	{
		if (TackOutStraightFlush(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
			return true;
	}
	break;
	case UG_DOUBLE_SEQUENCE: //����
	{
		if (TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 2))
		{
			return TRUE; //�����Ƶ���˳��
		}
		break;
	}
	case UG_THREE_SEQUENCE: //����
	{
		if (TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 3))
		{
			return TRUE;
		}
		break;
	}

	case UG_THREE_ONE_SEQUENCE: //����һ������
	{
		if (TrackOut3XSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 1))
			break;
	}
	case UG_THREE_TWO_SEQUENCE: //������������
		if (TrackOut3XSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 2))
			//if(TrackOut3XSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
		{
			return TRUE;
		}
	case UG_THREE_DOUBLE_SEQUENCE://����������
	{
		if (TrackOut3XSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, 3))
			return TRUE;
		break;
	}
	case UG_THREE_SEQUENCE_DOUBLE_SEQUENCE:
	{
		if (TrackOut3Sequence2Sequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
			return true;
		break;
	}
	//case UG_FOUR_ONE_SEQUENCE:

	case UG_SLAVE_510K: //ֻ����ͬ����ѹ,�����Ҵ������,�������������
		//	break;
	case UG_MASTER_510K:
		TrackOut510K(iHandCard, iHandCardCount, iResultCard, iResultCardCount, true); //�ҳ�ͬ�� 510K
		if (GetCardHuaKind(iBaseCard[0], TRUE) >= GetCardHuaKind(iResultCard[0], TRUE)) //�Ƚϻ�ɫ
		{
			iResultCardCount = 0;
		}
		break;
	default:
		iResultCardCount = 0;
	}

	if (iResultCardCount == 0) //û�ҵ�ͬ���͵Ĵ���,���Ҵ�һ�������
	{
		memset(iResultCard, 0, sizeof(BYTE) * 45);
		switch (iBaseShape)
		{
		case UG_ONLY_ONE: //���Բ����,��������ѹ���ƻ��߶���
		{
			if (TackOutCardByNoSameShape(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount))
				return TRUE;
			break;
		}
		case UG_DOUBLE:
		{
			if (TackOutCardByNoSameShape(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount))
				return TRUE;
			break;
		}
		case UG_BOMB:
		{
			//����û�ҵ���ͬ�����Ĵ�ը��,���������������
			if (TackOutBomb(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCardCount + 1))
				return TRUE;
		}
		break;
		default: //����Ҳ�����Ķ��ӵ��ƾ��Ҵ������,warning�˴�����break;
		{
			return TRUE;
		}
		}
	}

	if (iResultCardCount == 0)
	{
		memset(iResultCard, 0, sizeof(BYTE) * 45);
		TackOutMoreThanLastShape(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount);
	}
	//				if(TackOutMoreThanLastShape(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount))
	//				{
	//					return TRUE;
	//				}
	return TRUE;
}

//ֱ����ȡ�������ϴ������
BOOL  CUpGradeGameLogic::TackOutMoreThanLastShape(BYTE iCardList[], int iCardCount,
	BYTE iResultCard[], int &iResultCardCount,
	BYTE iBaseCard[], int iBaseCardCount)
{
	iResultCardCount = 0;
	int ishape = GetCardShape(iBaseCard, iBaseCardCount);
	switch (ishape)
	{
		/*	case UG_ONLY_ONE: //����
		case UG_DOUBLE:   //����
		case UG_THREE:    //����

		case UG_DOUBLE_SEQUENCE: //����
		case UG_THREE_SEQUENCE:  //������
		case UG_THREE_ONE:				//����1
		case UG_THREE_ONE_SEQUENCE:		//����˳
		case UG_THREE_TWO:				//����2
		case UG_THREE_TWO_SEQUENCE:		//����2˳
		case UG_THREE_DOUBLE:			//������
		case UG_THREE_DOUBLE_SEQUENCE:	//������˳

		case UG_FOUR_ONE:				//�Ĵ�һ
		case UG_FOUR_TWO:				//�Ĵ���
		case UG_FOUR_TWO_DOUBLE:		//�Ĵ�����

		case UG_STRAIGHT:				 //˳��
		case UG_STRAIGHT_FLUSH:			//ͬ��˳
		if(TrackOut510K(iCardList, iCardCount, iResultCard, iResultCardCount)) //510K ը��
		{
		break;
		}

		if(TrackOut510K(iCardList,iCardCount,iResultCard,iResultCardCount,true)) //ͬ��510K
		{
		break;
		}

		TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount); //��ը��

		break;*/
	case UG_SLAVE_510K: //һ��510K
	{
		if (TrackOut510K(iCardList, iCardCount, iResultCard, iResultCardCount, true))
			break;
		else
			TackOutBomb(iCardList, iCardCount, iResultCard, iResultCardCount);
		break;
	}
	case UG_MASTER_510K: //510K ͬ��
	{
		TackOutBomb(iCardList, iCardCount, iResultCard, iResultCardCount);
		break;
	}
	default:
		TackOutBomb(iCardList, iCardCount, iResultCard, iResultCardCount); //��ը��
		break;
	}
	return true;
}

//��ȡ����������0, 1 or 2 ���״����Ǽ�,�� iBaseCount-3 ������
BYTE CUpGradeGameLogic::TackOutThreeX(BYTE iCardList[], int iCardCount,
	BYTE iBaseCard[], int iBaseCount,
	BYTE iResultCard[], int &iResultCount, int iValue)
{
	iResultCount = 0;
	if (iCardCount < iBaseCount)
		return FALSE;
	BYTE iTempCard[45];
	int threecard = GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 3);//���������ŵĵ���
	//3�����ܸ���
	BYTE iCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iTempCard, 3);

	if (iCount > 0)//��ȡ�������������
	{
		BYTE byCardTemp = 0x00;
		for (int i = 0; i < iBaseCount; ++i)
		{
			if (threecard == GetCardBulk(iBaseCard[i]))
			{
				byCardTemp = iBaseCard[i];
				break;
			}
		}
		if (0x00 == byCardTemp)
		{
			return FALSE;
		}

		BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, byCardTemp, 3, true);//����ֵ��ȥ
		//if(Step == 0)
		//	return FALSE;
		CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE) * 3);
		//TCHAR sz[200];
		//wsprintf(sz,"Step=%d,iBaseCount=%d",Step,iBaseCount);
		//WriteStr(sz,7,7);

		//if(CompareOnlyOne(iBaseCard[0], iResultCard[0]))			//���ڴ�������step����Ϊ0�ý���һ�αȽϴ���
		if (threecard >= GetBulkBySpecifyCardCount(iResultCard, 3, 3))
			return FALSE;
		//iResultCount = 3;
		//else
		//	return FALSE;
	}
	else
		return FALSE;
	//��ԭֵ����
	BYTE Tmp[45];
	int iTempCount = iCardCount;
	::CopyMemory(Tmp, iCardList, sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard, 3, Tmp, iTempCount);
	iTempCount -= 3;
	int destCount = iBaseCount - 3;
	//	TCHAR sz[200];
	//	wsprintf(sz,"iValue=%d,destCount=%d",iValue,destCount);
	//	WriteStr(sz,8,8);
	switch (iValue)
	{
	case 1:
	case 2:
	{
		iCount = TackOutBySepcifyCardNumCount(Tmp, iTempCount, iTempCard, 1);
		if (iCount >= destCount)//���ҵ�����
		{
			CopyMemory(&iResultCard[3], iTempCard, sizeof(BYTE)*destCount);
			iResultCount = iBaseCount;
			break;
		}
		//�����������
		iCount = TackOutBySepcifyCardNumCount(Tmp, iTempCount, iTempCard, 2);
		if (iCount >= destCount)
		{
			CopyMemory(&iResultCard[3], iTempCard, sizeof(BYTE)*destCount);
			iResultCount = iBaseCount;
			break;
		}

		//��������������
		iCount = TackOutBySepcifyCardNumCount(Tmp, iTempCount, iTempCard, 3);
		if (iCount < 3)//��һ�����޷���
			break;
		CopyMemory(&iResultCard[3], iTempCard, sizeof(BYTE)*destCount);
		iResultCount = iBaseCount;
		break;
	}
	case 3:
	{
		iCount = TackOutBySepcifyCardNumCount(Tmp, iTempCount, iTempCard, 2);
		if (iCount > 0)
		{
			CopyMemory(&iResultCard[3], iTempCard, sizeof(BYTE)*destCount);
			iResultCount = iBaseCount;
			break;
		}
		//��������������
		iCount = TackOutBySepcifyCardNumCount(Tmp, iTempCount, iTempCard, 3);
		if (iCount < 3)//��һ�����޷���
			break;
		CopyMemory(&iResultCard[3], iTempCard, sizeof(BYTE)*destCount);
		iResultCount = iBaseCount;
		break;

	}
	default:
		iResultCount = 0;
		break;
	}
	//		wsprintf(sz,"iResultCount=%d,iBaseCount=%d",iResultCount,iBaseCount);
	//	WriteStr(sz,8,8);
	if (iResultCount == iBaseCount)
		return  TRUE;
	iResultCount = 0;
	return FALSE;
	//return (iResultCount == iBaseCount);
}
//��ȡ����
BOOL  CUpGradeGameLogic::TrackOut3Sequence2Sequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount)
{
	iResultCardCount = 0;
	if (iCardCount < iBaseCount)	//��������
		return false;
	BYTE tmpBaseCard[45];
	int tmpbaseCardCount = 0, destCardCount = 0;
	//�������Ƶ������������
	tmpbaseCardCount = TackOutBySepcifyCardNumCount(iBaseCard, iBaseCount, tmpBaseCard, 3);
	if (tmpbaseCardCount < 6)	//������������
		return FALSE;
	//����ȡ����������˳
	if (!TackOutSequence(iCardList, iCardCount, tmpBaseCard, tmpbaseCardCount, iResultCard, iResultCardCount, 3))
		return FALSE;
	//�����Ƹ���һ��(�Ƴ���˳��)
	BYTE TMP[45];
	int TmpCount = iCardCount;
	::CopyMemory(TMP, iCardList, sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard, iResultCardCount, TMP, TmpCount);
	TmpCount -= iResultCardCount;
	destCardCount = iBaseCount - iResultCardCount;	//��������

	BYTE twoList[45];
	int twoCount;
	//�������ƵĶ�˳�������
	tmpbaseCardCount = TackOutBySepcifyCardNumCount(iBaseCard, iBaseCount, tmpBaseCard, 2);
	if (!TackOutSequence(TMP, TmpCount, tmpBaseCard, tmpbaseCardCount, twoList, twoCount, 2, true))
		return false;
	//	int TwoSequenceLen = (iBaseCount- tmpbaseCardCount)/2;
	//	tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,3);
	::CopyMemory(&iResultCard[iResultCardCount], twoList, sizeof(BYTE)*twoCount);
	iResultCardCount += twoCount;
	return true;
}
//��ȡָ��������˳
BOOL  CUpGradeGameLogic::TrackOut3XSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int xValue)
{
	iResultCardCount = 0;
	if (iCardCount < iBaseCount)	//��������
		return false;
	BYTE tmpBaseCard[45];
	int tmpbaseCardCount = 0, destCardCount = 0;
	//�������Ƶ������������
	tmpbaseCardCount = TackOutBySepcifyCardNumCount(iBaseCard, iBaseCount, tmpBaseCard, 3);
	if (tmpbaseCardCount < 6)	//������������
		return FALSE;
	//TCHAR sz[200];
	//wsprintf(sz,"��˳����ȡ֮ǰ%d",iResultCardCount);
	//WriteStr(sz);	
	//����ȡ����������˳
	if (!TackOutSequence(iCardList, iCardCount, tmpBaseCard, tmpbaseCardCount, iResultCard, iResultCardCount, 3))
		return FALSE;
	//TCHAR sz[200];
	//wsprintf(sz,"��˳����ȡ�ɹ�%d",iResultCardCount);
	//WriteStr(sz);
	//�����Ƹ���һ��
	BYTE TMP[45];
	int TmpCount = iCardCount;
	::CopyMemory(TMP, iCardList, sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard, iResultCardCount, TMP, TmpCount);
	TmpCount -= iResultCardCount;
	destCardCount = iBaseCount - iResultCardCount;	//��������

	switch (xValue)
	{
	case 1:
	case 2:
	{
		tmpbaseCardCount = TackOutBySepcifyCardNumCount(TMP, TmpCount, tmpBaseCard, 1);//�յ���
		if (tmpbaseCardCount >= destCardCount)
		{
			::CopyMemory(&iResultCard[iResultCardCount], tmpBaseCard, sizeof(BYTE)*destCardCount);//����
			iResultCardCount += destCardCount;
		}
		else
		{
			::CopyMemory(&iResultCard[iResultCardCount], tmpBaseCard, sizeof(BYTE)*tmpbaseCardCount);
			iResultCardCount += tmpbaseCardCount;
			destCardCount -= tmpbaseCardCount;
			tmpbaseCardCount = TackOutBySepcifyCardNumCount(TMP, TmpCount, tmpBaseCard, 2);//�ö��Ʋ�
			if (tmpbaseCardCount >= destCardCount)
			{
				::CopyMemory(&iResultCard[iResultCardCount], tmpBaseCard, sizeof(BYTE)*destCardCount);
				iResultCardCount += destCardCount;
			}
			else
			{
				::CopyMemory(&iResultCard[iResultCardCount], tmpBaseCard, sizeof(BYTE)*tmpbaseCardCount);
				iResultCardCount += tmpbaseCardCount;
				destCardCount -= tmpbaseCardCount;
				tmpbaseCardCount = TackOutBySepcifyCardNumCount(TMP, TmpCount, tmpBaseCard, 3);//��������
				//
				if (tmpbaseCardCount >= destCardCount)
				{
					::CopyMemory(&iResultCard[iResultCardCount], tmpBaseCard, sizeof(BYTE)*destCardCount);
					iResultCardCount += destCardCount;
				}
			}
		}
		break;
	}
	case 3:
	{
		tmpbaseCardCount = TackOutBySepcifyCardNumCount(TMP, TmpCount, tmpBaseCard, 2);//�ն���
		if (tmpbaseCardCount >= destCardCount)
		{
			::CopyMemory(&iResultCard[iResultCardCount], tmpBaseCard, sizeof(BYTE)*destCardCount);
			iResultCardCount += destCardCount;
		}
		else
		{
			::CopyMemory(&iResultCard[iResultCardCount], tmpBaseCard, sizeof(BYTE)*tmpbaseCardCount);
			iResultCardCount += tmpbaseCardCount;
			destCardCount -= tmpbaseCardCount;
			//tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,3);//����������
			TackOutMuchToFew(TMP, TmpCount, tmpBaseCard, tmpbaseCardCount, 3, 2);	//������������ɶ�����
			if (tmpbaseCardCount >= destCardCount)//������Թ���
			{
				::CopyMemory(&iResultCard[iResultCardCount], tmpBaseCard, sizeof(BYTE)*destCardCount);
				iResultCardCount += destCardCount;
			}
		}
		break;
	}
	default:
		break;
	}
	//wsprintf(sz,"iResultCardCount=%d,iBaseCount=%d",iResultCardCount,iBaseCount);
	//WriteStr(sz);
	if (iResultCardCount == iBaseCount)
		return true;
	iResultCardCount = 0;
	return FALSE;
	return (iResultCardCount == iBaseCount);
	//	return FALSE;
}


//��д��ȡ���ŵ�˳��,���� or ����
BOOL CUpGradeGameLogic::TackOutSequence(BYTE iCardList[], int iCardCount, //���е���
	BYTE iBaseCard[], int iBaseCount,   //������������, �Ƶĸ���
	BYTE iResultCard[], int &iResultCount, //�ҵ�����
	int xSequence, BOOL bNoComp)							//˳�ӵĸ���
{
	iResultCount = 0;
	BYTE iTack[45];
	int iTackCount = iCardCount;
	//����һ��
	::CopyMemory(iTack, iCardList, sizeof(BYTE)*iCardCount);
	BYTE iBuffer[45];
	int iBufferCount = 0;
	int iBaseStart, iDestStart = 0, iDestEnd = 0;
	int iSequenceLen = iBaseCount;
	int temp[18] = { 0 };
	int num = 0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack, iTackCount, iBuffer, iBufferCount);
	RemoveCard(iBuffer, iBufferCount, iTack, iTackCount);
	iTackCount -= iBufferCount;
	//����һ��ϵͳ����������(����������С->�����
	SortCard(iTack, NULL, iTackCount, TRUE);
	//�û����������
	for (int i = 0;i < iTackCount;i++)
	{
		temp[GetCardBulk(iTack[i])]++;
	}

	switch (xSequence)
	{
		//��˳
	case 1:
		iSequenceLen = iBaseCount;
		if (!bNoComp)
			iBaseStart = GetSequenceStartPostion(iBaseCard, iBaseCount, 1);
		else
			iBaseStart = 2;
		for (int i = iBaseStart + 1;i < 15;i++)
		{
			if (temp[i] >= 1)
			{
				if (iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if (iDestEnd == iSequenceLen)
					break;
			}
			else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if (iDestEnd != iSequenceLen)
			return false;
		//��ȡ����
		for (int j = 0;j < iTackCount;j++)
		{
			if (GetCardBulk(iTack[j]) == iDestStart)//�ҵ�һ����
			{
				iResultCard[iResultCount++] = iTack[j];
				iDestStart++;
				iDestEnd--;
				//break;
			}
			//�Ѿ���ȫ
			if (iDestEnd == 0)
			{
				return true;
			}
		}
		break;
	case 2:
		iSequenceLen = iBaseCount / 2;
		if (!bNoComp)
			iBaseStart = GetSequenceStartPostion(iBaseCard, iBaseCount, 2);
		else
			iBaseStart = 2;
		for (int i = iBaseStart + 1;i < 15;i++)
		{
			if (temp[i] >= 2)
			{
				if (iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if (iDestEnd == iSequenceLen)
					break;
			}
			else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if (iDestEnd != iSequenceLen)
			return false;
		num = 0;
		//��ȡ����
		for (int j = 0;j < iTackCount;j++)
		{
			if (GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;
			}

			if (num == 2)//һ���Ѿ��ҵ�
			{
				num = 0;
				iDestStart++;
				iDestEnd--;
				//�Ѿ���ȫ
				if (iDestEnd == 0)
					return true;
				//break;
				//i = 0;
				//continue;
			}
		}
		break;
	case 3:
		iSequenceLen = iBaseCount / 3;
		if (!bNoComp)
			iBaseStart = GetSequenceStartPostion(iBaseCard, iBaseCount, 3);
		else
			iBaseStart = 2;
		for (int i = iBaseStart + 1;i < 15;i++)
		{
			if (temp[i] >= 3)
			{
				if (iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if (iDestEnd == iSequenceLen)
					break;
			}
			else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if (iDestEnd != iSequenceLen)
			return false;
		num = 0;
		//��ȡ����
		for (int j = 0;j < iTackCount;j++)
		{
			if (GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;

				if (num == 3)//�ҵ�����
				{
					num = 0;
					iDestStart++;
					iDestEnd--;
					//�Ѿ���ȫ
					if (iDestEnd == 0)
						return true;
				}
			}
		}

		break;
	}
	return FALSE;
}

//��ȡͬ���
BOOL CUpGradeGameLogic::TackOutStraightFlush(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount)
{
	iResultCardCount = 0;
	if (iCardCount < iBaseCount)
		return false;
	BYTE iBaseMinCard = GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1);//�����˳������С����
	BYTE iTack[45];
	int iTackCount = iCardCount;
	//����һ��
	::CopyMemory(iTack, iCardList, sizeof(BYTE)*iCardCount);
	BYTE iBuffer[45];
	int iBufferCount = 0;
	int iDestStart = 0, iDestEnd = 0;
	int iSequenceLen = iBaseCount;
	int temp[18] = { 0 };
	int num = 0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack, iTackCount, iBuffer, iBufferCount);
	RemoveCard(iBuffer, iBufferCount, iTack, iTackCount);
	iTackCount -= iBufferCount;

	SortCard(iTack, NULL, iTackCount, TRUE);

	BYTE iTempKind[45];
	int iTempKindCount = 0;
	//TCHAR sz[200];
	//wsprintf(sz,"iTackCount=%d,iBaseCount=%d",iTackCount,iBaseCount);
	//WriteStr(sz);
	//�û����������
	for (int kind = 0;kind <= 48;kind += 16)
	{	//��ȡ����
		iResultCardCount = 0;
		iTempKindCount = TackOutByCardKind(iTack, iTackCount, iTempKind, kind);
		//wsprintf(sz,"kind=%d,iTempKindCount=%d",kind,iTempKindCount);
		//WriteStr(sz);
		if (iTempKindCount >= iBaseCount)					//��������
		{
			for (int i = 0;i < iTempKindCount;i++)
			{
				temp[GetCardBulk(iTempKind[i])]++;
			}

			for (int i = iBaseMinCard + 1;i < 15;i++)//�Զ������б���
			{
				if (temp[i] >= 1)		//ĳ��ɫ����
				{
					if (iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if (iDestEnd == iSequenceLen)
						break;
				}
				else
				{
					iDestStart = 0;
					iDestEnd = 0;
				}
			}
			//wsprintf(sz,"iDestEnd=%d,iCardCount=%d",iDestEnd,iCardCount);
			//WriteStr(sz);

			if (iDestEnd != iBaseCount)	//ĳ�ֻ�ɫ������,������һ�ֻ�ɫ
				continue;
			//��ȡ����
			for (int j = 0;j < iTempKindCount;j++)
			{
				if (GetCardBulk(iTempKind[j]) == iDestStart)
				{
					iResultCard[iResultCardCount++] = iTempKind[j];
					iDestStart++;
					iDestEnd--;
				}
				//�Ѿ���ȫ
				if (iDestEnd == 0)
					return true;
			}
		}

	}

	return FALSE;
}

//�õ�˳�ӵ���ʼλ��
int CUpGradeGameLogic::GetSequenceStartPostion(BYTE iCardList[], int iCardCount, int xSequence)
{
	BYTE temp[18] = { 0 };
	int Postion = 0;
	for (int i = 0;i < iCardCount;i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for (int i = 0;i < 18;i++)
	{
		if (temp[i] == xSequence)
			return i;
	}
	return Postion;
}

//��ȡ510K
BOOL CUpGradeGameLogic::TrackOut510K(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount, bool bExtVal)
{
	iResultCardCount = 0;
	BYTE temp[48] = { 0 };
	BYTE huasei[4][16] = { 0 };
	int k = 0, num[4] = { 0 };
	//�õ�510K����
	for (int i = 0; i < iCardCount; i++)
	{
		int n = GetCardNum(iCardList[i]);
		if (n == 5 || n == 10 || n == 13)
		{
			temp[k++] = iCardList[i];
			int kind = GetCardHuaKind(iCardList[i], true) >> 4;

			huasei[kind][num[kind]++] = iCardList[i];
		}
	}
	//5,10,k��Ŀ����3��
	if (num[0] + num[1] + num[2] + num[3] < 3)
		return false;
	//Ҫ����510K��������3��
	if (bExtVal && num[0] < 3 && num[1] < 3 && num[2] < 3 && num[3] < 3)
		return false;
	for (int i = 0;i < 4;i++)
	{
		if (Test510K(huasei[i], num[i]))//ĳһ��ɫ�Ƿ�Ϊ��510K
		{
			Copy510K(huasei[i], num[i], iResultCard, iResultCardCount);

			if (bExtVal) //�Ƿ���Ҫ��ȡ��510K
				return true;
			else
			{
				RemoveCard(iResultCard, iResultCardCount, huasei[i], num[i]);//����510K�Ƴ�ѡ����ɫ����
				RemoveCard(iResultCard, iResultCardCount, temp, k);	//����510K�Ƴ�510K����
				num[i] -= iResultCardCount;
				k -= iResultCardCount;
			}
			//			return true;
		}
	}

	if (bExtVal) return false; //��Ҫͬ��510K

	if (Test510K(temp, k))
	{
		Copy510K(temp, k, iResultCard, iResultCardCount);
		return true;
	}
	return FALSE;
}

//����510K
BOOL CUpGradeGameLogic::Copy510K(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount)
{
	iResultCardCount = 0;
	BYTE five, ten, k;
	for (int i = 0;i < iCardCount;i++)
	{
		if (GetCardNum(iCardList[i]) == 5)
			five = iCardList[i];
		else if (GetCardNum(iCardList[i]) == 10)
			ten = iCardList[i];
		else k = iCardList[i];
	}
	iResultCard[0] = five;
	iResultCard[1] = ten;
	iResultCard[2] = k;
	iResultCardCount = 3;
	return  TRUE;
}

//�����Ƿ�Ϊ5,10k
BOOL CUpGradeGameLogic::Test510K(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	BOOL five = false, ten = false, k = false;
	for (int i = 0;i < iCardCount;i++)
	{
		if (GetCardNum(iCardList[i]) == 5)
			five = true;
		else if (GetCardNum(iCardList[i]) == 10)
			ten = true;
		else
			k = true;
	}
	//��5,10,k
	if (five&&ten&&k)
		return true;
	return false;
}


//���
BOOL CUpGradeGameLogic::TackOutCardByNoSameShape(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount, BYTE iBaseCard[], int iBaseCardCount)
{
	if (iCardCount < 1)
	{
		return FALSE;
	}
	iResultCardCount = 0;
	BYTE temp[18] = { 0 };
	int t = GetCardBulk(iBaseCard[0], false); //�õ��������Ǹ��Ƶ�ֵ
	for (int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i], false)]++;
	}

	//��(ը�Ʋ���)
	for (int i = 0; i < 18; i++)
	{
		if (temp[i] < 4 &&               //��ը����
			temp[i] > iBaseCardCount &&  //�����������ƶ�
			i > t)                      //�����ִ�
		{
			for (int j = 0; j < iCardCount; j++)  ///��С������(���ǵ��Ƶ���ȡ)
			{
				if (GetCardBulk(iCardList[j], false) == i)
				{
					iResultCard[iResultCardCount++] = iCardList[j];

					if (iResultCardCount == iBaseCardCount)
					{
						return TRUE;
					}
				}
			}
		}
	}
	return false;
}

///����ָ��������ȡ
BOOL CUpGradeGameLogic::TackOutCardBySpecifyCard(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iSepcifyCard)
{
	if (iCardCount < 1)
	{
		return FALSE;
	}

	if (iBaseCardCount > 0 && (GetCardBulk(iBaseCard[0], false) >= GetCardBulk(iSepcifyCard, false)))
	{
		return FALSE;
	}

	iResultCardCount = 0;

	BYTE temp[18] = { 0 };

	int t = GetCardBulk(iBaseCard[0], false); //�õ��������Ǹ��Ƶ�ֵ

	for (int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i], false)]++;
	}

	iResultCard[iResultCardCount++] = iSepcifyCard;

	for (int j = iCardCount - 1; j >= 0; j--)  ///��С������(���ǵ��Ƶ���ȡ)
	{
		if (GetCardBulk(iCardList[j], false) == GetCardBulk(iSepcifyCard, false)
			&& iCardList[j] != iSepcifyCard)
		{
			if (temp[GetCardBulk(iSepcifyCard, false)] >= 4 && iBaseCardCount != 0)
			{
				return FALSE;
			}

			iResultCard[iResultCardCount++] = iCardList[j];

			if (iResultCardCount == iBaseCardCount)
			{
				return TRUE;
			}
		}
	}

	if (iBaseCardCount == 0 && iResultCardCount > 0)
	{
		return TRUE;
	}

	return FALSE;
}


//�Ƿ���Գ���
BOOL CUpGradeGameLogic::CanOutCard(BYTE iOutCard[], int iOutCount,  //Ҫ������
	BYTE iBaseCard[], int iBaseCount,//Ҫѹ����
	BYTE iHandCard[], int iHandCount,//���е���
	bool bFirstOut)
{
	BYTE iOutCardShape = GetCardShape(iOutCard, iOutCount);
	if (iOutCardShape == UG_ERROR_KIND) //���Ͳ���
	{
		return FALSE;
	}
	if (bFirstOut)
	{
		return TRUE;
	}

	BYTE iBaseCardShape = GetCardShape(iBaseCard, iBaseCount); //�����ϵ�����
	if (iBaseCardShape > iOutCardShape)						//����<
	{
		return FALSE;
	}
	if (iBaseCardShape < iOutCardShape)						//����>
	{
		if (UG_SLAVE_510K <= iOutCardShape)					//ը��
		{
			return TRUE;
		}
		//����һ��������Ҳ���Դ����˳�Ӻ�˳�Ӵ�С�Ƚ�
		if (iBaseCount != iOutCount) //��������
			return FALSE;

		switch (iBaseCardShape)
		{
		case UG_STRAIGHT:									//ͬ��혴������
		{
			if (iOutCardShape == UG_STRAIGHT_FLUSH)
				return true;
		}
		case UG_VARIATION_STRAIGHT:							//��С����˳��
		{
			if (iOutCardShape == UG_STRAIGHT)			//����˳������Ч���ֵС������˳���������
				return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount, 1) < GetBulkBySpecifySequence(iOutCard, iOutCount, 1);
			return false;
		}

		case UG_VARIATION_DOUBLE_SEQUENCE://��С����˳��
		{
			if (iOutCardShape == UG_DOUBLE_SEQUENCE)	//����˳������Ч���ֵС������˳���������
				return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount, 2) < GetBulkBySpecifySequence(iOutCard, iOutCount, 2);
			return false;
		}

		case UG_VARIATION_THREE_SEQUENCE:		//������˳
		case UG_VARIATION_THREE_ONE_SEQUENCE://������˳
		case UG_VARIATION_THREE_TWO_SEQUENCE://����������˳
		case UG_VARIATION_THREE_DOUBLE_SEQUENCE://����������˳
		case UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE://������˳����˳
		{
			if (iOutCardShape == iBaseCardShape + 1)
				return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount, 3) < GetBulkBySpecifySequence(iOutCard, iOutCount, 3);
			return false;
		}
		case UG_VARIATION_FOUR_SEQUENCE:		//������˳
		case UG_VARIATION_FOUR_ONE_SEQUENCE:	//�����Ĵ�һ˳
		case UG_VARIATION_FOUR_TWO_SEQUENCE:	//�����Ĵ���˳
		case UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE://�����Ĵ�һ��˳
		case UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE://�����Ĵ�����˳
		{
			if (iOutCardShape == iBaseCardShape + 1)
				return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount, 4) < GetBulkBySpecifySequence(iOutCard, iOutCount, 4);
			return false;
		}
		case UG_THREE_TWO://����һ������������
		{
			if (iOutCardShape == UG_THREE_DOUBLE)
				return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 3) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 3);
			return false;
		}
		case UG_THREE_TWO_SEQUENCE://����һ���(�����)>���������
		{
			if (iOutCardShape == UG_THREE_DOUBLE_SEQUENCE || iOutCardShape == UG_THREE_SEQUENCE_DOUBLE_SEQUENCE)
				return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 3) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 3);
			return false;
		}
		case UG_FOUR_TWO://�Ď�һ�����Ď�����
		{
			if (iOutCardShape == UG_FOUR_ONE_DOUBLE)
				return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 4);
			return false;
		}
		case UG_FOUR_TWO_SEQUENCE://�Ď�һ��혣��Ď������
		{
			if (iOutCardShape == UG_FOUR_ONE_DOUBLE_SEQUENCE)
				return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 4);
			return false;
		}
		case UG_THREE_DOUBLE_SEQUENCE:	//����������혎���
		{
			if (iOutCardShape == UG_THREE_SEQUENCE_DOUBLE_SEQUENCE)
				return GetBulkBySpecifySequence(iBaseCard, iBaseCount, 3) < GetBulkBySpecifySequence(iOutCard, iOutCount, 3);
			return false;
		}
		}
		return false;
	}

	switch (iBaseCardShape)			//��������һ��
	{
	case UG_ONLY_ONE:  //����
	case UG_DOUBLE:    //����
	case UG_THREE:     //����
	{
		return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1);
	}
	case UG_BOMB: //4+�� ը��
	{
		if (iBaseCount > iOutCount) //�������ը����
			return FALSE;
		if (iBaseCount == iOutCount) //������ͬ,�ȵ���
			return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1);
		return TRUE;
	}

	case UG_FLUSH:			//ͬ��(��˳�ӣ��Ƚ�ͬ����������
	{
		return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 255) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 255);
	}
	case UG_STRAIGHT_FLUSH: //ͬ��˳
	case UG_STRAIGHT:		//˳��
	case UG_DOUBLE_SEQUENCE: //����
	case UG_THREE_SEQUENCE:  //����  
	case UG_FOUR_SEQUENCE:	//��˳
	{
		if (iOutCount != iBaseCount)
			return FALSE;
		return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1);
	}
	case UG_THREE_ONE:		//����һ
	case UG_THREE_TWO:		//������
	case UG_THREE_DOUBLE:	//������
		//��һ�������Ƶ��Ƶ��С������
		//return (SearchThreeCard(iBaseCard, iBaseCount) < SearchThreeCard(iOutCard, iOutCount));
	{
		return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 3) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 3);
	}
	case UG_FOUR_ONE:						//�Ĵ�һ
	case UG_FOUR_TWO:						//�Ĵ���
	case UG_FOUR_ONE_DOUBLE:				//�Ĵ�һ��
	case UG_FOUR_TWO_DOUBLE:				//�Ĵ�����
	{
		return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 4);
	}

	case UG_THREE_ONE_SEQUENCE: //2+������һ
	case UG_THREE_TWO_SEQUENCE: //2+��������
	case UG_THREE_DOUBLE_SEQUENCE://������˳
	case UG_THREE_SEQUENCE_DOUBLE_SEQUENCE:		//��˳����˳(����)
	{
		if (iOutCount != iBaseCount)
			return FALSE;
		return(GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 3) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 3));
	}
	case UG_FOUR_ONE_SEQUENCE:					//��˳
	case UG_FOUR_TWO_SEQUENCE:
	case UG_FOUR_ONE_DOUBLE_SEQUENCE:
	case UG_FOUR_TWO_DOUBLE_SEQUENCE:
	{
		return(GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 4));
	}
	case UG_MASTER_510K: //ͬ��510K����ɫ:���� > ���� > ÷�� > ��Ƭ
	{
		return (GetCardHuaKind(iBaseCard[0], true) < GetCardHuaKind(iOutCard[0], true)); //�Ȼ�ɫ
	}
	case UG_SLAVE_510K: //��510K��һ����
	{
		return FALSE;
	}
	//�������δ���
	case UG_VARIATION_STRAIGHT://��˳
	{
		if (iOutCount != iBaseCount)
			return FALSE;
		return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount, 1) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount, 1));

		break;
	}
	case UG_VARIATION_DOUBLE_SEQUENCE://��˳
	{
		if (iOutCount != iBaseCount)
			return FALSE;
		return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount, 2) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount, 2));
		break;
	}
	case UG_VARIATION_THREE_SEQUENCE://��˳
	case UG_VARIATION_THREE_ONE_SEQUENCE://����һ˳
	case UG_VARIATION_THREE_TWO_SEQUENCE://������˳
	case UG_VARIATION_THREE_DOUBLE_SEQUENCE://������˳
	case UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE://��˳����˳
	{
		if (iOutCount != iBaseCount)
			return FALSE;
		return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount, 3) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount, 3));
		break;
	}
	case UG_VARIATION_FOUR_SEQUENCE:		//������˳
	case UG_VARIATION_FOUR_ONE_SEQUENCE:	//�����Ĵ�һ˳
	case UG_VARIATION_FOUR_TWO_SEQUENCE:	//�����Ĵ���˳
	case UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE://�����Ĵ�һ��˳
	case UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE://�����Ĵ�����˳
	{
		if (iOutCount != iBaseCount)
			return FALSE;
		return(GetBulkBySpecifyCardCount(iBaseCard, iBaseCount, 4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount, 4));

		return false;
	}
	}
	return FALSE;
}

//��ȡ����ը��Ϊ�ᷴ��˳,˫˳,��˳��׼��
BOOL CUpGradeGameLogic::TackOutAllBomb(BYTE iCardList[], int iCardCount,
	BYTE iResultCard[], int &iResultCardCount, int iNumCount)
{
	iResultCardCount = 0;
	BYTE bCardBuffer[45];
	BYTE bombcount = GetBombCount(iCardList, iCardCount, iNumCount);
	if (bombcount < 0)
		return false;
	for (int i = iNumCount;i < 9;i++)
	{
		int count = TackOutBySepcifyCardNumCount(iCardList, iCardCount, bCardBuffer, i);
		if (count > 0)
		{
			::CopyMemory(&iResultCard[iResultCardCount], bCardBuffer, sizeof(BYTE)*count);
			iResultCardCount += count;
			break;
		}
	}
	return true;
}

//��ȡը��
BOOL CUpGradeGameLogic::TackOutBomb(BYTE iCardList[], int iCardCount,
	BYTE iResultCard[], int &iResultCardCount, int iNumCount)
{
	iResultCardCount = 0;
	BYTE bCardBuffer[45];
	BYTE bombcount = GetBombCount(iCardList, iCardCount, iNumCount);
	if (bombcount < 0)
		return false;
	for (int i = iNumCount;i < 9;i++)
	{
		int count = TackOutBySepcifyCardNumCount(iCardList, iCardCount, bCardBuffer, i);
		if (count > 0)
		{
			::CopyMemory(iResultCard, bCardBuffer, sizeof(BYTE)*i);
			iResultCardCount = i;
			break;
		}
	}
	if (iResultCardCount == 0)
		TackOutKingBomb(iCardList, iCardCount, iResultCard, iResultCardCount);
	return true;
}

//��ȡ��ը
BOOL CUpGradeGameLogic::TackOutKingBomb(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount)
{
	iResultCardCount = 0;

	BYTE bCardBuf[8];
	int kingcount = 0;
	int SingKing = KING_COUNT / 2;
	int count = TackOutBySpecifyCard(iCardList, iCardCount, bCardBuf, kingcount, 0x4e);
	if (count != SingKing)
		return false;

	::CopyMemory(iResultCard, bCardBuf, sizeof(BYTE)*count);

	count = TackOutBySpecifyCard(iCardList, iCardCount, bCardBuf, kingcount, 0x4f);
	if (count != SingKing)
	{
		return false;
	}
	::CopyMemory(&(iResultCard[SingKing]), bCardBuf, sizeof(BYTE)*count);
	return iResultCardCount = KING_COUNT;
}
///�϶�ѡ����
void CUpGradeGameLogic::DragCardAutoSetValidCard(BYTE iUpCardList[], int iUpCardCount, BYTE bResult[], int & bResultCount)
{
	int iSingleResultCount = 0, iDoubleResultCount = 0;
	BYTE bSingleResult[45], bDoubleResult[45];

	bResultCount = 0;
	memset(bResult, 0, sizeof(bResult));
	memset(bSingleResult, 0, sizeof(bSingleResult));
	memset(bDoubleResult, 0, sizeof(bDoubleResult));

	BYTE bMinCard = GetCardMinOrMax(iUpCardList, iUpCardCount, 1, true); // ��ȡ��С��
	BYTE bMaxCard = GetCardMinOrMax(iUpCardList, iUpCardCount, 255, true); // ��ȡ�����
	BYTE bLength = abs(GetCardNum(bMinCard) - GetCardNum(bMaxCard)) + 1; // ����

	// ��������˳�ӡ�����Ҫ��
	if (255 == bMinCard || 255 == bMaxCard || 3 > bLength)
		return;

	for (char i = bLength; i >= 5; i--) //˳����������
		if (TackOutSequence(iUpCardList, iUpCardCount, NULL, i, bSingleResult, iSingleResultCount, 1, TRUE))
			break;

	for (char i = bLength * 2; i >= 6; i--) //������������
		if (TackOutSequence(iUpCardList, iUpCardCount, NULL, i, bDoubleResult, iDoubleResultCount, 2, TRUE))
			break;

	if (0 < iSingleResultCount && iSingleResultCount > iDoubleResultCount / 2)
	{
		memcpy(bResult, bSingleResult, sizeof(bSingleResult));
		bResultCount = iSingleResultCount;
	}
	else if (0 < iDoubleResultCount)
	{
		memcpy(bResult, bDoubleResult, sizeof(bDoubleResult));
		bResultCount = iDoubleResultCount;
	}
}
///��ҵ����֮��������ȡ�ƿؼ�
void  CUpGradeGameLogic::AITrackOutCard(BYTE  iCardList[], int iCardCount,     //��ҵ���������
	BYTE  iUpCardList[], int iUpCardCount,  //��ҵ����������
	BYTE  iBaseCardList[], int iBaseCardCount, //��ҵ������ϵ�������
	BYTE  bResult[], int & bResultCount)      ///��ȡ���Ľ��
{

	if (iUpCardCount <= 0 || 0 == iBaseCardCount)  //������ƺ������ϵ������Ϊ�յľͲ�����ȡ
	{
		return;
	}

	BYTE iCardValue = iUpCardList[0];
	BYTE iTempBaseCardList[45];
	memset(iTempBaseCardList, 0, sizeof(iTempBaseCardList));
	int  iTempBaseCardCount = iBaseCardCount;
	memcpy(iTempBaseCardList, iBaseCardList, sizeof(BYTE)* iBaseCardCount);
	bResultCount = 0;

	BYTE  iCardShape = GetCardShape(iBaseCardList, iBaseCardCount);

	if (0 == iBaseCardCount)
	{
		if (TRUE == TackOutCardBySpecifyCard(iCardList, iCardCount, bResult, bResultCount, iTempBaseCardList, iTempBaseCardCount, iCardValue))
		{
			return;
		}
	}

	if (iCardShape == UG_ERROR_KIND)
	{
		return;
	}

	if (iCardShape == UG_DOUBLE)
	{
		if (TRUE == TackOutCardBySpecifyCard(iCardList, iCardCount, bResult, bResultCount, iTempBaseCardList, iTempBaseCardCount, iCardValue))
		{
			return;
		}
	}

	while (0 == GetCountBySpecifyCard(iTempBaseCardList, iTempBaseCardCount, iCardValue))
	{
		AutoOutCard(iCardList, iCardCount, iTempBaseCardList, iTempBaseCardCount, bResult, bResultCount, FALSE);

		if (bResultCount == 0)
		{
			break;
		}

		iTempBaseCardCount = bResultCount;
		memcpy(iTempBaseCardList, bResult, sizeof(BYTE)* bResultCount);

		memset(bResult, 0, sizeof(bResult));
		bResultCount = 0;
	}

	if (GetCountBySpecifyCard(iTempBaseCardList, iTempBaseCardCount, iCardValue) == 0)
	{
		memset(iTempBaseCardList, 0, sizeof(iTempBaseCardList));
		iTempBaseCardCount = 0;
	}

	bResultCount = iTempBaseCardCount;
	memcpy(bResult, iTempBaseCardList, sizeof(BYTE)* bResultCount);

	return;
}
//�ҳ�һ����С��������
BYTE CUpGradeGameLogic::GetCardMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax/*1 or 255*/, bool bExtVal)
{
	int nIndex = 0;
	int CardNum;

	if (MinOrMax == 1) //����С��
	{
		CardNum = 65536;
		for (int i = 0; i < iCardCount; i++)
		{
			// ������ 2 ����
			if (bExtVal && (2 == GetCardNum(iCardList[i]) || 0x4E == iCardList[i] || 0x4F == iCardList[i]))
				continue;

			if (GetCardBulk(iCardList[i], false) < CardNum)
			{
				CardNum = GetCardBulk(iCardList[i], false);
				nIndex = i;
			}
		}
	}
	else if (MinOrMax == 255)
	{
		CardNum = -1;
		for (int i = 0; i < iCardCount; i++)
		{
			if (bExtVal && (2 == GetCardNum(iCardList[i]) || 0x4E == iCardList[i] || 0x4F == iCardList[i]))
				continue;

			if (GetCardBulk(iCardList[i], false) > CardNum)
			{
				CardNum = GetCardBulk(iCardList[i], false);
				nIndex = i;
			}
		}
	}

	if (bExtVal && (65536 == CardNum || -1 == CardNum))
		return 255;

	return iCardList[nIndex];
}

//����������
void CUpGradeGameLogic::MatchDDZ(BYTE iCardArray[], int iCardCount)
{
	BYTE _byCardArray[54] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
		0x4E, 0x4F //С�����
	};
	int i = 0;
	BYTE byCardList[216];
	for (i = 0; i < iCardCount - 2; i++)
	{
		int j = (i % 4) * 13 + i / 4;
		byCardList[i] = _byCardArray[j];
	}
	byCardList[i] = 0x4E;
	byCardList[i + 1] = 0x4F;
	int iCount_ = 90;
	BYTE byTemp4[4];
	while (--iCount_ >= 0)
	{
		int iRandIndex1 = (CUtil::GetRandNum() % 13) * 4;
		int iRandIndex2 = (CUtil::GetRandNum() % 13) * 4;
		if (iRandIndex1 == iRandIndex2)
		{
			iRandIndex1 = 0;
			iRandIndex2 = 4;
		}

		memcpy(byTemp4, &byCardList[iRandIndex1], sizeof(BYTE) * 4);
		memcpy(&byCardList[iRandIndex1], &byCardList[iRandIndex2], sizeof(BYTE) * 4);
		memcpy(&byCardList[iRandIndex2], byTemp4, sizeof(BYTE) * 4);
	}
	BYTE byTemp = 255;
	int iIndex1 = 0, iIndex2 = 0;
	int iRandCount_ = 35;
	for (int i = 0; i < iRandCount_; i++)
	{
		iIndex1 = CUtil::GetRandNum() % iCardCount;
		iIndex2 = CUtil::GetRandNum() % iCardCount;
		if (iIndex1 == iIndex2)
		{
			iIndex2 = (iIndex2 + 6) % iCardCount;
		}
		byTemp = byCardList[iIndex1];
		byCardList[iIndex1] = byCardList[iIndex2];
		byCardList[iIndex2] = byTemp;
		if (iIndex1 > 1 && iIndex2 > 1 && CUtil::GetRandNum() % 2 == 1)
		{
			byTemp = byCardList[iIndex1 - 1];
			byCardList[iIndex1 - 1] = byCardList[iIndex2 - 1];
			byCardList[iIndex2 - 1] = byTemp;
		}
	}
	//����һ������
	int iQiePaiIndex = CUtil::GetRandNum() % iCardCount;
	int iCardIndex = 0;
	for (int i = iQiePaiIndex; i < iCardCount; i++)
	{
		iCardArray[iCardIndex++] = byCardList[i];
	}
	for (int i = 0; i < iQiePaiIndex; i++)
	{
		iCardArray[iCardIndex++] = byCardList[i];
	}

	/*
static const BYTE m_CardArray[54] =
{
	0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
	0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
	0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
	0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
	0x4E, 0x4F //С�����
};*/
////����
//iCardArray[0]=0x01;
//iCardArray[1]=0x11;
//iCardArray[2]=0x21;
//iCardArray[3]=0x31;
//iCardArray[4]=0x02;
//iCardArray[5]=0x12;
//iCardArray[6]=0x22;
//iCardArray[7]=0x32;
//iCardArray[8]=0x03;
//iCardArray[9]=0x13;
//iCardArray[10]=0x23;
//iCardArray[11]=0x33;
//iCardArray[12]=0x04;
//iCardArray[13]=0x14;
//iCardArray[14]=0x24;
//iCardArray[15]=0x34;
//iCardArray[16]=0x05;

//iCardArray[17]=0x06;
//iCardArray[18]=0x16;
//iCardArray[19]=0x26;
//iCardArray[20]=0x36;
//iCardArray[21]=0x07;
//iCardArray[22]=0x17;
//iCardArray[23]=0x27;
//iCardArray[24]=0x37;
//iCardArray[25]=0x08;
//iCardArray[26]=0x18;
//iCardArray[27]=0x28;
//iCardArray[28]=0x38;
//iCardArray[29]=0x09;
//iCardArray[30]=0x19;
//iCardArray[31]=0x29;
//iCardArray[32]=0x39;
//iCardArray[33]=0x0A;

//iCardArray[34]=0x0B;
//iCardArray[35]=0x1B;
//iCardArray[36]=0x2B;
//iCardArray[37]=0x3B;
//iCardArray[38]=0x0C;
//iCardArray[39]=0x1C;
//iCardArray[40]=0x2C;
//iCardArray[41]=0x3C;
//iCardArray[42]=0x0D;
//iCardArray[43]=0x1D;
//iCardArray[44]=0x2D;
//iCardArray[45]=0x3D;
//iCardArray[46]=0x15;
//iCardArray[47]=0x25;
//iCardArray[48]=0x35;
//iCardArray[49]=0x1A;
//iCardArray[50]=0x2A;

//iCardArray[51]=0x3A;
//iCardArray[52]=0x4E;
//iCardArray[53]=0x4F;
}

//�ϲ�������������㷨
void CUpGradeGameLogic::MatchMergeDDZ(BYTE byCardArray1[], int iCount1, BYTE byCardArray2[], int iCount2, bool bNoXiPai)
{
	static unsigned long lAddTime = 0;
	if (lAddTime >= 65535)
		lAddTime = 0;
	memset(m_cdType, 0, sizeof(m_cdType));
	for (int i = 0;i < iCount1;i++)
	{
		int j = GetCardBulk(byCardArray1[i]);
		m_cdType[j].byCard[m_cdType[j].byNum++] = byCardArray1[i];
	}
	for (int i = 0;i < iCount2;i++)
	{
		int j = GetCardBulk(byCardArray2[i]);
		m_cdType[j].byCard[m_cdType[j].byNum++] = byCardArray2[i];
	}
	BYTE byList[90];
	int iCount = 0;
	memset(byList, 0, sizeof(byList));
	vector<int> vecIndex;
	for (int i = 3; i < 18; i++)
	{
		if (m_cdType[i].byNum == 0)
		{
			continue;
		}
		vecIndex.push_back(i);
	}
	while (true)
	{
		int iVecSize = vecIndex.size();
		if (iVecSize <= 0)
		{
			break;
		}
		srand((unsigned)GetCurrentTime() + (lAddTime++));
		int iRandIndex = CUtil::GetRandNum() % iVecSize;
		int iIndex = vecIndex[iRandIndex];
		memcpy(&byList[iCount], m_cdType[iIndex].byCard, sizeof(BYTE)*m_cdType[iIndex].byNum);
		iCount += m_cdType[iIndex].byNum;
		if (iRandIndex != iVecSize - 1)
		{
			vecIndex[iRandIndex] = vecIndex[iVecSize - 1];
		}
		vecIndex.pop_back();
	}
	int iRandCount_ = 0;
	if (bNoXiPai)
	{
		iRandCount_ = CUtil::GetRandNum() % 4 + 2;
	}
	else
	{
		iRandCount_ = CUtil::GetRandNum() % 6 + 3;
	}
	//���������
	for (int i = 0; i < iRandCount_; i++)
	{
		srand((unsigned)GetCurrentTime() + (lAddTime++));
		int iTempIndex1 = CUtil::GetRandNum() % (iCount1 + iCount2);
		int iTempIndex2 = CUtil::GetRandNum() % (iCount1 + iCount2);
		if (iTempIndex1 == iTempIndex2)
		{
			continue;
		}
		BYTE byTemp = byList[iTempIndex1];
		byList[iTempIndex1] = byList[iTempIndex2];
		byList[iTempIndex2] = byTemp;
		if (iTempIndex1 > 1 && iTempIndex2 > 1 && CUtil::GetRandNum() % 3 != 1)
		{
			byTemp = byList[iTempIndex1 - 1];
			byList[iTempIndex1 - 1] = byList[iTempIndex2 - 1];
			byList[iTempIndex2 - 1] = byTemp;
		}
	}
	if (iCount != iCount1 + iCount2)
	{
		return;
	}
	memcpy(byCardArray1, &byList[0], sizeof(BYTE)*iCount1);
	memcpy(byCardArray2, &byList[iCount1], sizeof(BYTE)*iCount2);
	return;
}