#include "MyThreadpool.h"


CMyThreadpool::CMyThreadpool(void):m_lCreateThreadNum(0),
	m_lRunThreadNum(0),m_lMaxThreadNum(0)
{
	m_hSemphore = NULL;
	m_bflagQuit = true;
}


CMyThreadpool::~CMyThreadpool(void)
{

}


bool CMyThreadpool::CreateThreadpool(long lMinThreadNum,long lMaxThreadNum,long lItaskLen)
{
	if(lMinThreadNum <=0 || lMaxThreadNum < lMinThreadNum)
	{
		return false;
	}
	//创建线程所需的资源
	m_hSemphore = CreateSemaphore(NULL,0,lMaxThreadNum,NULL);

	//创建一些线程
	for(long i = 0;i <lMinThreadNum;i++)
	{
		HANDLE hthread = (HANDLE)_beginthreadex(NULL,0,&ThreadProc,this,0,0);
		if(hthread)
		{
			m_lstThread.push_back(hthread);
		}
	}
	m_lCreateThreadNum = lMinThreadNum;
	m_lMaxThreadNum = lMaxThreadNum;
	return true;
}
void CMyThreadpool::DestroyThreadpool()
{
	m_bflagQuit = false;
	auto ite = m_lstThread.begin();
	while(ite != m_lstThread.end())
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100))
			TerminateThread(*ite,-1);

		CloseHandle(*ite);
		*ite = NULL;

		ite++;
	}
	if(m_hSemphore)
	{
		CloseHandle(m_hSemphore);
		m_hSemphore = NULL;
	}
}
unsigned __stdcall CMyThreadpool::ThreadProc( void * lpvoid)
{
	CMyThreadpool *pthis = (CMyThreadpool*)lpvoid;
	Itask *pItask = NULL;
	while(pthis->m_bflagQuit)
	{
		//等信号量
		if(WAIT_TIMEOUT == WaitForSingleObject(pthis->m_hSemphore,100))
			continue;
		//从睡眠态----工作态
		pthis->m_lRunThreadNum++;
		//从队列中取任务
		while(/*!pthis->m_qItask.isempty()*/!pthis->m_qItask.isEmpty())
		{
			
			//pItask = pthis->m_qItask.front();
			//pthis->m_qItask.pop();

			pItask = pthis->m_qItask.Pop();
	
			//执行任务
 			pItask->RunItask();

			delete pItask;
			pItask = NULL;
		}

		//从工作态----睡眠态
		pthis->m_lRunThreadNum--;

	}
	return 0;
}
bool CMyThreadpool::Push(Itask *pItask)
{
	//投递任务
	if(pItask == NULL)return false;


	m_qItask.Push(pItask);


	//释放信号量

	//1有空闲线程
	if(m_lRunThreadNum < m_lCreateThreadNum)
	{
		ReleaseSemaphore(m_hSemphore,1,NULL);
	}
	else if(m_lCreateThreadNum < m_lMaxThreadNum) //2.没有空闲线程，但没达到上限
	{
		//创建线程
		HANDLE hthread = (HANDLE)_beginthreadex(NULL,0,&ThreadProc,this,0,0);
		if(hthread)
		{
			m_lstThread.push_back(hthread);
		}
		m_lCreateThreadNum++;
		//释放信号量
		ReleaseSemaphore(m_hSemphore,1,NULL);

	}
	else//3.达到上限
	{
		//====
	}


	
	



	return true;
}
