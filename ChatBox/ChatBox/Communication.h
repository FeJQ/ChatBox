#pragma once
#include "Packs.h"
#define CU_MESSAGE WM_USER+10

class Communication
{
public:
	Communication();
	~Communication();

	//Summary:���ӷ�����
	//Return:�����Ƿ�ɹ�
	static bool ConnectToServer();

	//Summary:�Ͽ�����
	static void Disconnect();

	//Summary:���շ�������Ϣ
	//Parameter pf:�ص�����ָ��
	static DWORD CALLBACK ReciveMessage(LPVOID lparam);

	//Summary:������Ϣ
	//Parameter msg:��Ϣ���ṹ��
	//Return:���سɹ����͵����ݴ�С(�ֽ�)
	static int MySendMessage(PMSGPACK  pack);

	
};

