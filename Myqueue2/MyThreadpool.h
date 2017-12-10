#pragma once
#include <queue>
#include <Windows.h>
#include <list>
#include <process.h>

using namespace std;

#define _DEF_ITASK_LEN   2000
#define arr_len		1


class CMyLock
{
public:
	CMyLock()
	{
		InitializeCriticalSection(&m_cs);
	}
	~CMyLock()
	{
		DeleteCriticalSection(&m_cs);
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}
	void UnLock()
	{
		LeaveCriticalSection(&m_cs);
	}
private:
	CRITICAL_SECTION m_cs;
};
class Itask
{
public:
	Itask(){}
	virtual ~Itask(){}
public:
	virtual void RunItask() = 0;
};
template <typename T>
class Myqueue
{
public:
	Myqueue()
	{
		ZeroMemory(arr,sizeof(arr));
		arr_front = 0;
		arr_rear = 0;
		arr_size = 0;
		InitializeCriticalSection(&m_pop);
		InitializeCriticalSection(&m_push);
		m_hSem = CreateSemaphore(0,0,1,NULL);
	}
	~Myqueue()
	{
		CloseHandle(m_hSem);
		m_hSem = NULL;
		DeleteCriticalSection(&m_pop);
		DeleteCriticalSection(&m_push);

	}
	bool Push(T Itask )
	{

		if(isFull())
			WaitForSingleObject(m_hSem,INFINITE);
		EnterCriticalSection(&m_push);
		if(isFull()) 
			WaitForSingleObject(m_hSem,INFINITE);
		arr[arr_rear] = Itask;
		arr_rear = (arr_rear + 1)%arr_len;
		arr_size += 1;
		LeaveCriticalSection(&m_push);
		return true;
	}
	T Pop()
	{ 
		if(isEmpty())  return NULL;
		EnterCriticalSection(&m_pop);
		if(isEmpty()) return NULL;
		T temp;
		temp = arr[arr_front];
		arr[arr_front] = NULL;
		arr_front = (arr_front +1)%arr_len;
		arr_size -=1;

		if(arr_size + 1 == arr_len)
			ReleaseSemaphore(m_hSem,1,0);
		LeaveCriticalSection(&m_pop);
		return temp;
	}
	bool isEmpty()
	{
		if(arr_size == 0) return true;
		return false;
	}
	bool isFull()
	{
		if(arr_size == arr_len)  return true;
		return false;
	}
private:
	CRITICAL_SECTION m_push;
	CRITICAL_SECTION m_pop;
	HANDLE m_hSem;
	int arr_front;
	int arr_rear;
	int arr_size;

	T arr[arr_len];
};
class CMyThreadpool
{
public:
	CMyThreadpool(void);
	~CMyThreadpool(void);
public:
	//1.创建一些线程
	bool CreateThreadpool(long lMinThreadNum,long lMaxThreadNum,long lItaskLen = _DEF_ITASK_LEN);
	//2.销毁线程
	void DestroyThreadpool();
	//3.线程函数
	static unsigned __stdcall ThreadProc( void * );
	//4.投递任务
	bool Push(Itask *pItask);
private:
	Myqueue<Itask*> m_qItask;
	HANDLE m_hSemphore;
	list<HANDLE>  m_lstThread;
	bool  m_bflagQuit;
	long   m_lCreateThreadNum;
	volatile	long   m_lRunThreadNum;
	long   m_lMaxThreadNum;
	CMyLock m_lock;
};

