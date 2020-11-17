#include "stdafx.h"
#include "Communication.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Packs.h"
#pragma comment(lib,"ws2_32.lib") 



//客户端socket
SOCKET sclient;

Communication::Communication()
{
}


Communication::~Communication()
{

}

//Summary:连接服务器
//Return:连接是否成功
bool Communication::ConnectToServer()
{
	const char ip[16] = "127.0.0.1";
	//初始化winsock   
	WSADATA wsaD;
	WSAStartup(MAKEWORD(1, 1), &wsaD);

	//创建客户端socket   
	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//连接服务器   
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;

	struct in_addr s;
	inet_pton(AF_INET, ip, (void *)&s);
	servaddr.sin_addr.s_addr = s.s_addr;    //指定服务器

	servaddr.sin_port = htons(60556);       //指定端口            
	char flag[4];
	gets_s(flag);

	int result = connect(sclient, (struct sockaddr*)&servaddr, sizeof(servaddr)); //通过套接字连接主机

	return !result;
}

//Summary:断开连接
void Communication::Disconnect()
{
	closesocket(sclient);
}


//Summary:接收服务器消息
//Parameter pf:回调函数指针
DWORD Communication::ReciveMessage(LPVOID lparam)
{
	HWND hwnd = (HWND)lparam;
	while (true)
	{
		Sleep(200);
		MessagePack *pack = new MessagePack;
		int len = recv(sclient, (char*)pack, sizeof(MessagePack), 0);
		if (len > 0)
		{
			SendMessage(hwnd, CU_MESSAGE, (WPARAM)pack, NULL);
		}
		delete pack;
	}
	return 0;
}

//Summary:发送消息
//Parameter msg:消息包结构体
int Communication::MySendMessage(PMSGPACK pack)
{
	return send(sclient, (char*)pack, sizeof(MessagePack), 0);
}


