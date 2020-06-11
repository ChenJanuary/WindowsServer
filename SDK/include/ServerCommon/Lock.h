#pragma once

class CRWLock;
class CSignedLock;

class KERNEL_CLASS CSignedLockObject
{
private:
	int				m_iLockCount;		//��������
	CSignedLock* m_pLockObject;		//��������

public:
	CSignedLockObject(CSignedLock * pLockObject, bool bAutoLock);
	~CSignedLockObject();

public:
	inline void Lock();
	inline void UnLock();
};

class KERNEL_CLASS CSignedLock
{
	friend class CSignedLockObject;

private:
	CRITICAL_SECTION	m_csLock;

public:
	inline CSignedLock() { InitializeCriticalSection(&m_csLock); }
	inline ~CSignedLock() { DeleteCriticalSection(&m_csLock); }

private:
	inline void Lock() { EnterCriticalSection(&m_csLock); }
	inline void UnLock() { LeaveCriticalSection(&m_csLock); }
};