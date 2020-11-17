#pragma once
#include <time.h>
#include <tchar.h>
#include <stdlib.h>
#include "Tool.h"
enum PACK
{
	//注册
	PACK_Register=0,
	//登陆
	PACK_Login,
	//消息
	PACK_Message,	
	//验证
	PACK_Validation,
};


//Summary:消息包
typedef struct MessagePack
{
	MessagePack(){}
	//消息初始化
	MessagePack(wchar_t*_msg, long _senderID, long _reciverID,int _isGroup)
	{
		
		this->pack = PACK::PACK_Message;
		this->t = time(NULL);		
		wcscpy_s(this->msg, _countof(msg), _msg);
		//加密
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
	//账号信息初始化
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
	//验证信息初始化
	MessagePack( long _senderID, long _reciverID, int _isGroup)
	{
		this->pack = PACK::PACK_Validation;
		this->t = time(NULL);
		this->senderID = _senderID;
		this->reciverID = _reciverID;
		this->isGroup = _isGroup;
	}

	//消息类型
	PACK pack;
	//发送时间
	long t;
	//用户名
	char userName[16];
	//用户名长度
	int lenUserName;
	//密码
	char password[16];
	//密码长度
	int lenPassword;

	//消息内容
	wchar_t msg[1024];
	//消息长度
	int lenMsg;
	//发送者ID
	long senderID;
	//接收者ID
	long reciverID;
	//是否为群聊
	int isGroup;
	//群聊ID
	long GroupID;


}*PMSGPACK;


