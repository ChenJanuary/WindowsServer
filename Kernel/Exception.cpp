#include "pch.h"
#include "Exception.h"
#include <exception>

//��̬��������
bool CException::m_bShowError = false;					//�Ƿ���ʾ����


CException::CException(const char * szErrorMessage, unsigned int uErrorCode, bool bAutoDelete)
{
	m_uErrorCode = uErrorCode;
	m_bAutoDelete = bAutoDelete;
	if ((szErrorMessage != NULL) && (szErrorMessage[0] != 0))
	{
		if (strlen(szErrorMessage) < (sizeof(m_szMessage) / sizeof(m_szMessage[0])))
		{
			strcpy(m_szMessage, szErrorMessage);
		}
		else
		{
			strcpy(m_szMessage, TEXT("�쳣������Ϣ̫��"));
		}
	}
	else
	{
		strcpy(m_szMessage, TEXT("û���쳣��Ϣ"));
	}

	if (m_bShowError == true)
	{
		printf("%s\n", m_szMessage);
	}
}


CException::~CException()
{
}

//��ȡ�������
UINT CException::GetErrorCode() const
{
	if (this != NULL) return m_uErrorCode;
	return 0;
}

//ɾ������
bool CException::Delete()
{
	if ((this != NULL) && (m_bAutoDelete == true))
	{
		delete this;
		return true;
	}
	return false;
}

//��ȡ������Ϣ
TCHAR const * CException::GetErrorMessage(TCHAR * szBuffer, int iBufLength) const
{
	if (this != NULL)
	{
		//����������Ϣ
		if ((szBuffer != NULL) && (iBufLength > 0))
		{
			int iCopyLength = __min(iBufLength - 1, ::lstrlen(m_szMessage));
			szBuffer[iCopyLength] = 0;
			::CopyMemory(szBuffer, m_szMessage, iCopyLength * sizeof(TCHAR));
		}
		return m_szMessage;
	}
	return NULL;
}

//�����Ƿ���ʾ����
bool CException::ShowErrorMessage(bool bShowError)
{
	m_bShowError = bShowError;
	return m_bShowError;
}

//////////////////////////////////////////////////////////////////////////

// ��ýṹ���쳣��Ϣ��
PEXCEPTION_POINTERS CWin32Exception::ExceptionInformation()
{
	return m_pException;
}

// ��ýṹ���쳣���롣
DWORD CWin32Exception::ExceptionCode()
{
	if (NULL != m_pException)
		return m_nSE;
	else
		return 0;
}

// ת���쳣����
void CWin32Exception::TransWin32Exception(unsigned int u, EXCEPTION_POINTERS* pExp)
{
	throw CWin32Exception(u, pExp);
}

// CRT�쳣
void CWin32Exception::TerminateHandler()
{
	throw EX_CRT_TERMINATE;
}

// �����쳣����
void CWin32Exception::SetWin32ExceptionFunc()
{
	// �����쳣����ת������
	_set_se_translator(CWin32Exception::TransWin32Exception);

	// CRT�쳣
	set_terminate(CWin32Exception::TerminateHandler);
}

// �����쳣�����ȡ����
const char * CWin32Exception::GetDescByCode(unsigned int uCode)
{
	switch (uCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		return "�߳�Υ���дû���ʵ�Ȩ�޵������ַ��";

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return "�߳��ڵײ�Ӳ��֧�ֵı߽����·�������Խ�硣";

	case EXCEPTION_BREAKPOINT:
		return "����һ���ϵ㡣";

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return "�߳���ͼ�ڲ�֧�ֶ����Ӳ���϶�дδ��������ݡ�";

	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return "�������ֵ��ʾ̫С�����ܱ�ʾһ����׼�ĸ���ֵ��";

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return "�̳߳��������";

	case EXCEPTION_FLT_INEXACT_RESULT:
		return "�������Ĳ�������׼ȷ�Ĵ���С����";

	case EXCEPTION_FLT_INVALID_OPERATION:
		return "����쳣����������б���û���г������������쳣��";

	case EXCEPTION_FLT_OVERFLOW:
		return "���������ָ����������Ӧ���͵����ֵ��";

	case EXCEPTION_FLT_STACK_CHECK:
		return "���������ջԽ����������";

	case EXCEPTION_FLT_UNDERFLOW:
		return "���������ָ��û�г�����Ӧ���͵����ֵ��";

	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return "�߳���ͼִ����Чָ�";

	case EXCEPTION_INT_OVERFLOW:
		return "���������Ľ��ռ���˽����������λ��";

	case EXCEPTION_STACK_OVERFLOW:
		return "ջ�����";

	default:
		return "������";
	}
	return "";
}

//���������־
void CWin32Exception::OutputWin32Exception(const char * str, ...)
{
	try
	{
		FILE *fp = NULL;
		if (fopen_s(&fp, "C:\\HM-Dump-v1.0\\������¼.log", "a") != 0)
		{
			return;
		}

		if (NULL == fp)
		{
			return;
		}

		char szPath[MAX_PATH] = "";
		GetCurrentDirectory(MAX_PATH, szPath);

		SYSTEMTIME time;
		GetLocalTime(&time);

		char cTime[64] = "";
		sprintf_s(cTime, sizeof(cTime), "[%04d/%02d/%02d-%02d:%02d:%02d] ", time.wYear, time.wMinute, time.wDay, time.wHour, time.wMinute, time.wSecond);
		fprintf_s(fp, cTime);

		va_list arg;

		va_start(arg, str);
		vfprintf_s(fp, str, arg);
		fprintf_s(fp, "��path=%s��\n", szPath);

		fclose(fp);
	}
	catch (...)
	{
		exit(0);
	}
}