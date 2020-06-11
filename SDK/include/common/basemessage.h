#pragma once

///������汾
#define DEV_LIB_VER				5									///������汾

///��������
#define MAX_SEND_SIZE			2044								//�����Ϣ��
#define NET_HEAD_SIZE			sizeof(NetMessageHead)				//���ݰ�ͷ

///֧�����Ͷ���
#define SUP_NORMAL_GAME			0x01								///��ͨ��Ϸ
#define SUP_MATCH_GAME			0x02								///������Ϸ
#define SUP_MONEY_GAME			0x04								///�����Ϸ


/********************************************************************************************/
//	ϵͳ�ں�ʹ�� ��MAINID ʹ�� 0 - 49 ��
/********************************************************************************************/

///������Ϣ
#define MDM_NO_FAILE					0			///��ʧ����Ϣ

///������Ϣ
#define MDM_CONNECT						1			///������Ϣ����
///����������Ϣ��־
#define ASS_NET_TEST					1			///�������
#define ASS_CONNECT_SUCCESS 			3			///���ӳɹ�

/********************************************************************************************/

// ��Կ
#define SECRET_KEY  192068

#pragma pack(1)
//�������ݰ��ṹͷ
/*
����uIdentification��˵����
1��ǰ�� -------> logonserver �� logonserver -------> ǰ�ˣ� uIdentification = roomID
2��logonserver -----> gameserver �� gameserver -----> logonserver��uIdentification = userID
*/
struct NetMessageHead
{
	UINT						uMessageSize;						///���ݰ���С
	UINT						uMainID;							///����������
	UINT						uAssistantID;						///������������ ID
	UINT						uHandleCode;						///���ݰ��������
	UINT						uIdentification;					///��ݱ�ʶ����ͬ��Э�������в�ͬ�ĺ��壩

	NetMessageHead()
	{
		memset(this, 0, sizeof(NetMessageHead));
	}
};

//���ķ���ͷ
struct CenterServerMessageHead
{
	UINT msgID;				// ID
	int userID;				// �û�ID���߾��ֲ�id
	CenterServerMessageHead()
	{
		memset(this, 0, sizeof(CenterServerMessageHead));
	}
};

// ���ķ�������Ϣͷ
struct PlatformMessageHead
{
	NetMessageHead				MainHead;	//����ͷ
	CenterServerMessageHead		AssHead;	//����ͷ

	PlatformMessageHead()
	{
		memset(this, 0, sizeof(PlatformMessageHead));
	}
};

#pragma pack()