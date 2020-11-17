#include "stdafx.h"
#include "Communication.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Packs.h"
#pragma comment(lib,"ws2_32.lib") 



//�ͻ���socket
SOCKET sclient;

Communication::Communication()
{
}


Communication::~Communication()
{

}

//Summary:���ӷ�����
//Return:�����Ƿ�ɹ�
bool Communication::ConnectToServer()
{
	const char ip[16] = "127.0.0.1";
	//��ʼ��winsock   
	WSADATA wsaD;
	WSAStartup(MAKEWORD(1, 1), &wsaD);

	//�����ͻ���socket   
	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���ӷ�����   
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;

	struct in_addr s;
	inet_pton(AF_INET, ip, (void *)&s);
	servaddr.sin_addr.s_addr = s.s_addr;    //ָ��������

	servaddr.sin_port = htons(60556);       //ָ���˿�            
	char flag[4];
	gets_s(flag);

	int result = connect(sclient, (struct sockaddr*)&servaddr, sizeof(servaddr)); //ͨ���׽�����������

	return !result;
}

//Summary:�Ͽ�����
void Communication::Disconnect()
{
	closesocket(sclient);
}


//Summary:���շ�������Ϣ
//Parameter pf:�ص�����ָ��
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

//Summary:������Ϣ
//Parameter msg:��Ϣ���ṹ��
int Communication::MySendMessage(PMSGPACK pack)
{
	return send(sclient, (char*)pack, sizeof(MessagePack), 0);
}


