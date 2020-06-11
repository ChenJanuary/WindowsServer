#pragma once

#pragma pack(1)

///��Ϸ�����Ϣ�ṹ �������� DLL �����ȡ��
struct ServerDllInfoStruct
{
	///��Ϸ�����Ϣ
	UINT						uServiceVer;						///����汾
	UINT						uNameID;							///��Ϸ���� ID
	UINT						uSupportType;						///֧������
	UINT						uDeskPeople;						///��Ϸ����
	char						szGameName[61];						///��Ϸ����
	char						szGameTable[61];					///���ݿ������
	char						szDLLFileName[61];					///�ļ�����

	///������Ϣ
	char						szWriter[61];						///���������
	char						szDllNote[51];						///��ע��Ϣ
};

//�������������Ϣ�ṹ	���������������ݣ�
struct ManageInfoStruct
{
	bool						bPrivate;							//�Ƿ�˽�з�
	char						szGameTable[31];					///��Ϣ������
	char						szGameRoomName[61];					///����������
	///�������ݿ���Ϣ
	char						szSQLIP[16];						///�������ݿ��ַ
	char						szSQLName[61];						///�������ݿ��û�����
	char						szSQLPass[61];						///�������ݿ�����
	///��½���ݿ���Ϣ
	char						szLogonSQLIP[16];					///��½���ݿ��ַ
	///�������ݿ���Ϣ
	char						szNativeSQLIP[61];					///�������ݿ��ַ
	///��ս��������Ϣ��
	char						szBattleGameTable[31];
	char						szLockTable[31];					///����������
	char						szIPRuleTable[31];					///IP ���Ʊ�����
	char						szNameRuleTable[31];				///UserName���Ʊ�����
	///��Ϸ��Ϣ
	UINT						uNameID;							///��Ϸ���� ID
	UINT						dwRoomRule;							///��Ϸ���ù���
	UINT						dwUserPower;						///��Ϸ����Ȩ��
	///������Ϣ
	UINT						uComType;							///��Ϸ����
	UINT						uDeskType;							///��Ϸ�����ͣ���ҪΪ��Ӱټ�����
	UINT						uBasePoint;							///��Ϸ������
	UINT						uLessPoint;							///��Ϸ�����ٷ���
	UINT						uMaxPoint;							///��Ϸ��������
	UINT						uMaxLimite;							///���ⶥֵ
	UINT						uRunPublish;						///���ܿ۷�
	UINT						uTax;								///˰�ձ���
	UINT						uListenPort;						///�����˿�
	UINT						uMaxPeople;							///���������Ŀ
	int							iSocketSecretKey;					///socket���ܵ���Կ
	char						szCenterIP[24];						///���ķ�ip
	///��������Ϣ
	UINT						uRoomID;							///��Ϸ������ ID ����
	int							iRoomType;							///��������
	int							iRoomSort;							///�������
	int							iRoomLevel;							///����ȼ�
	UINT						uDeskCount;							///��Ϸ����Ŀ
	UINT						uStopLogon;							///ֹͣ��½����
	char						bCanCombineDesk;					///�Ƿ��������

	ManageInfoStruct()
	{
		memset(this, 0, sizeof(ManageInfoStruct));
	}
};

//�ں����� �������� DLL ����������ã�
struct KernelInfoStruct
{
	UINT						uNameID;							//��Ϸ���� ID ����
	UINT						uDeskPeople;						//��Ϸ����

	UINT						uMinDeskPeople;						//������Ϸ֧��M-N������Ϸ

	BYTE						bMaxVer;							//�����߰汾
	BYTE						bLessVer;							//�����Ͱ汾
	BYTE						bStartSQLDataBase;					//ʹ�����ݿ�����
	BYTE						bNativeDataBase;					//ʹ�ñ������ݿ�
	BYTE						bLogonDataBase;						//ʹ�õ�½���ݿ�
	BYTE						bStartTCPSocket;					//�Ƿ���������

	KernelInfoStruct()
	{
		memset(this, 0, sizeof(KernelInfoStruct));
	}
};

#pragma pack()

