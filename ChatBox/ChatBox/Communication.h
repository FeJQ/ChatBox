#pragma once
#include "Packs.h"
#define CU_MESSAGE WM_USER+10

class Communication
{
public:
	Communication();
	~Communication();

	//Summary:连接服务器
	//Return:连接是否成功
	static bool ConnectToServer();

	//Summary:断开连接
	static void Disconnect();

	//Summary:接收服务器消息
	//Parameter pf:回调函数指针
	static DWORD CALLBACK ReciveMessage(LPVOID lparam);

	//Summary:发送消息
	//Parameter msg:消息包结构体
	//Return:返回成功发送的数据大小(字节)
	static int MySendMessage(PMSGPACK  pack);

	
};

