//#pragma once 
//#include <Windows.h>
//#include "MyThreadpool.h"
//using namespace std;
//
//#define arr_len 20
//////环形缓冲区
////template <class T>
////class MyQueue
////{
////public:
////	//1.初始化队列--设置队列的长度
////	//bool  InitQueue(int nlen)
////	//{
////	//	return true;
////	//}
////	//2.push
////	
////	//3.pop
////
////	//4.销毁队列
////
////
////	//5.判断队列是否为空
////
////	//6.获得队列大小
////
////
////private:
////
////	T arr[];
////};
//
//环形缓冲区
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
		CloseHandle(&m_hSem);
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