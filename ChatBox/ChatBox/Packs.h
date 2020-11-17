#pragma once
#include <time.h>
#include <tchar.h>
#include <stdlib.h>
#include "Tool.h"
enum PACK
{
	//ע��
	PACK_Register=0,
	//��½
	PACK_Login,
	//��Ϣ
	PACK_Message,	
	//��֤
	PACK_Validation,
};


//Summary:��Ϣ��
typedef struct MessagePack
{
	MessagePack(){}
	//��Ϣ��ʼ��
	MessagePack(wchar_t*_msg, long _senderID, long _reciverID,int _isGroup)
	{
		
		this->pack = PACK::PACK_Message;
		this->t = time(NULL);		
		wcscpy_s(this->msg, _countof(msg), _msg);
		//����
		Tool:: xor ((char*)msg);
		this->lenMsg = wcslen(this->msg);
		this->senderID = _senderID;		
		this->isGroup = _isGroup;
		if (_isGroup == 1)
		{
			this->GroupID = _reciverID;
		}
		else
		{
			this->reciverID = _reciverID;
		}
	}	
	//�˺���Ϣ��ʼ��
	MessagePack(PACK pack, char*_userName, char*_password)
	{
		this->pack = pack;
		this->t = time(NULL);
		if (_userName != NULL)
		{
			strcpy(this->userName, _userName);
			this->lenUserName = strlen(this->userName);
		}
		else
		{
			this->lenUserName = 0;
		}
		if (_password != NULL)
		{
			strcpy(this->password, _password);
			this->lenPassword = strlen(this->password);
		}
		else
		{
			this->lenPassword = 0;
		}
	}
	//��֤��Ϣ��ʼ��
	MessagePack( long _senderID, long _reciverID, int _isGroup)
	{
		this->pack = PACK::PACK_Validation;
		this->t = time(NULL);
		this->senderID = _senderID;
		this->reciverID = _reciverID;
		this->isGroup = _isGroup;
	}

	//��Ϣ����
	PACK pack;
	//����ʱ��
	long t;
	//�û���
	char userName[16];
	//�û�������
	int lenUserName;
	//����
	char password[16];
	//���볤��
	int lenPassword;

	//��Ϣ����
	wchar_t msg[1024];
	//��Ϣ����
	int lenMsg;
	//������ID
	long senderID;
	//������ID
	long reciverID;
	//�Ƿ�ΪȺ��
	int isGroup;
	//Ⱥ��ID
	long GroupID;


}*PMSGPACK;


