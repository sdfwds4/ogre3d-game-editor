



/*
系统临界区对象
2008 12 28
*/

#pragma once


class k_CriticalSection
{
public:
	k_CriticalSection()
	{
		ZeroMemory( &mLock, sizeof(mLock) );
		Create();
	}

	~k_CriticalSection()
	{
		Destroy();
	}

	void Create()
	{
		::InitializeCriticalSection( &mLock );
	}

	void Destroy()
	{
		::DeleteCriticalSection( &mLock );
	}

	void EnterLock()
	{
		::EnterCriticalSection( &mLock );
	}

	void LeaveLock()
	{
		::LeaveCriticalSection( &mLock );
	}

protected:

	CRITICAL_SECTION mLock;
};

class k_FastLock
{
public:
	k_FastLock()
	{

	}
	~k_FastLock()
	{
		mpCS->LeaveLock();
		mpCS = NULL;
	}

	void Lock( k_CriticalSection* pCS )
	{
		mpCS = pCS;
		mpCS->EnterLock();
	}

protected:
	k_CriticalSection* mpCS;

};

#define MAKE_CriticalSection k_FastLock lock;\
	lock.Lock(&mCS_Lock);

#define CriticalSection_Object k_CriticalSection mCS_Lock