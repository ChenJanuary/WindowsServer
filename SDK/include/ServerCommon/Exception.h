#pragma once

///***********************************************************************************************///

///ϵͳ����
#define EX_UNKNOWN_ERROR			0						///δ֪����
#define EX_VIRTUAL_MEM_LOW			1						///�����ڴ治��
#define EX_HEAP_MEM_LOW				2						///���ڴ治��

///���ݿ����
#define EX_DATA_CONNECT				100						///���ݿ����Ӵ���
#define EX_DATA_USER				101						///���ݿ����ִ���
#define EX_DATA_PASS				102						///���ݿ��������
#define EX_DATA_EXEC				103						///���ݿ�ִ�д���

///�������
#define EX_SOCKET_CREATE			200						///���罨������
#define EX_SOCKET_SEND_ERROR		201						///���緢�ʹ���
#define EX_SOCKET_RECV_ERROR		202						///������ܴ���

///�������
#define EX_SERVICE_START			300						///��������
#define EX_SERVICE_PAUSE			301						///������ͣ
#define EX_SERVICE_STOP				302						///����ֹͣ
#define EX_SERVICE_BUSY				303						///����æ
#define EX_SERVICE_UNKNOW			304						///δ֪������Ϣ

///CRT �쳣
#define EX_CRT_INVALID_PARAM		401						///ϵͳ��������,��⵽�Ƿ��Ĳ���
#define EX_CRT_PURECALL				402						///���麯�����ô���
#define EX_CRT_NEW					403						///�����ڴ����
#define EX_CRT_TERMINATE			404						///CRT����һ��δ�������C++���ͻ��쳣ʱ���������terminate()����

///�Զ������
///***********************************************************************************************///

//�쳣��
class KERNEL_CLASS CException
{
protected:
	UINT					m_uErrorCode;					///������
	bool					m_bAutoDelete;					///�Ƿ��Զ�ɾ��
	TCHAR					m_szMessage[255];				///������Ϣ
	static bool				m_bShowError;					///�Ƿ���ʾ����

public:
	inline CException(const char * szErrorMessage, unsigned int uErrorCode = EX_UNKNOWN_ERROR, bool bAutoDelete = true);
	virtual ~CException();

public:
	//��ȡ�������
	virtual UINT GetErrorCode() const;
	//��ȡ������Ϣ
	virtual TCHAR const * GetErrorMessage(TCHAR * szBuffer, int iBufLength) const;
	//ɾ������
	virtual bool Delete();
	//�����Ƿ���ʾ����
	static bool ShowErrorMessage(bool bShowError);
};

class KERNEL_CLASS CWin32Exception
{
public:
	CWin32Exception(unsigned int n, PEXCEPTION_POINTERS pException)
		: m_nSE(n), m_pException(pException) {}

	// ��ýṹ���쳣��Ϣ��
	PEXCEPTION_POINTERS ExceptionInformation();

	// ��ýṹ���쳣���롣
	DWORD ExceptionCode();

	// ת���쳣����
	static void TransWin32Exception(unsigned int u, EXCEPTION_POINTERS* pExp);

	// CRT�쳣
	static void TerminateHandler();

	// �����쳣����
	static void SetWin32ExceptionFunc();

	// �����쳣�����ȡ����
	static const char * GetDescByCode(unsigned int uCode);

	//���������־
	static void OutputWin32Exception(const char * str, ...);
private:
	// �ṹ���쳣����
	unsigned int m_nSE;
	// �ṹ���쳣��Ϣ
	PEXCEPTION_POINTERS m_pException;
};