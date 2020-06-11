#pragma once

#include "DataBase.h"

// ��Ϸ���첽�¼�  2001-3000

//////////////////////////////////////////////////////////////////////
#define	LOADER_ASYNC_EVENT_UPLOAD_VIDEO		2001		// �ϴ�¼��
#define	LOADER_ASYNC_EVENT_HTTP				2002		// http����

//////////////////////////////////////////////////////////////////////
#pragma pack(1)
// �ϴ�¼��
struct LoaderAsyncUploadVideo
{
	DataBaseLineHead dataBaseHead;	//���ݰ�ͷ
	char videoCode[20];

	LoaderAsyncUploadVideo()
	{
		memset(this, 0, sizeof(LoaderAsyncUploadVideo));
	}
};

// HTTP ����
struct LoaderAsyncHTTP
{
	DataBaseLineHead dataBaseHead;	//���ݰ�ͷ
	char url[1024];
	int userID;
	BYTE postType;

	LoaderAsyncHTTP()
	{
		memset(this, 0, sizeof(LoaderAsyncHTTP));
	}
};

#pragma pack()
//////////////////////////////////////////////////////////////////////