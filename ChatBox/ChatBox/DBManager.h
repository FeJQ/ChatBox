#pragma once
#include "DBHelper.h"
#include <vector>
#include <string>
using namespace std;
class DBManager
{
public:
	DBManager();
	~DBManager();

	//根据用户名获取用户ID
	static long GetUserIDByName(char*userName);
	//根据用户ID获取用户名
	static string GetUserNameByID(long UserID);
	//根据群组名获取群组ID
	static long GetGroupIDByName(char*groupName);
	//根据群ID获取群组名
	static string GetGroupNameByID(long GroupID);
	//登陆
	static int Login(char*userName, char*password);
	//注册
	static int Register(char*userName, char*password);
	//获取好友列表
	static int GetFriendList(char*userName, map<int, vector<string>>  &result);
	//获取群聊列表
	static int GetGroupList(char*userName, map<int, vector<string>>  &result);
	//发送消息
	static int SendMsg(char*senderNamme, char*reciverName, char*msg, int isGroup);
	//查询是否为群组
	static int IsGroup(char*name);
	//获取单聊记录
	static int GetFriendChatRecords(long MyID, long FriendID, map<int, vector<string>>  &result);
	//获取群聊记录
	static int GetGroupChatRecords(long GroupID, map<int, vector<string>>  &result);
	//获取未读消息数
	static int GetUnreadCount(int UserID);
	//获取来自好友的未读消息数
	static int GetUnreadCountFromFriend(int MyID, int FriendID);
	//获取来自群组的未读消息数
	static int GetUnreadCountFromGroup(int MyID,int GroupID);
	//获取未读消息
	static int GetUnreadMsg(int UserID, map<int, vector<string>> &result);
	//标记已读
	static int SetHaveRead(long SenderID, long ReciverID,int isGroup=0);
	//查询聊天室的成员列表
	static int GetGroupMember(char*GroupName, map<int, vector<string>> &result);
	//添加好友
	static int AddFriend(long MyID, long FriendID);
	//同意添加
	static int AgreeAdd(long MyID,long FriendID);
	//拒绝添加
	static int RefuseAdd(long MyID, long FriendID);
	//获取未处理的验证消息
	static int GetUnreadValidation(long MyID, map<int, vector<string>>& result);
};

