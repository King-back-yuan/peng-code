//#include <iostream>
//#include <windows.h>
//#include "mythreadpool.h"
//
//using namespace std;
////
////
////
////
////#define arr_len 10
////class Myqueue
////{
////public:
////	Myqueue()
////	{
////		ZeroMemory(arr,sizeof(arr));
////		arr_front = 0;
////		arr_rear = 0;
////		arr_size = 0;
////	}
////	~Myqueue()
////	{
////		
////	}
////	bool Push(Itask* Itask )
////	{
////		if(isFull()) return false;
////		arr[arr_rear] = Itask;
////		arr_rear = (arr_rear + 1)%arr_len;
////		arr_size += 1;
////		return true;
////	}
////	Itask* Pop(int* val)
////	{ 
////		if(isEmpty())  return NULL;
////		Itask* temp;
////		temp = arr[arr_front];
////		arr_front = (arr_front +1)%arr_len;
////		return temp;
//// 	}
////	bool isEmpty()
////	{
////		if(arr_size == 0) return true;
////		return false;
////	}
////	bool isFull()
////	{
////		if(arr_size == arr_len)  return true;
////		return false;
////	}
////private:
////	
////	int arr_front;
////	int arr_rear;
////	int arr_size;
////public:
////	Itask* arr[arr_len];
////};
//int main()
//{
//
//	int a;
//	system("pause");
//	return 0;
//	
//}