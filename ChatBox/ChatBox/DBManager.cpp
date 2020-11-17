#include "stdafx.h"
#include "DBManager.h"
#include "Tool.h"

DBManager::DBManager()
{
}


DBManager::~DBManager()
{
}


//Summary:根据用户名获取ID
//Parameter:用户名
//Return:用户ID
long DBManager::GetUserIDByName(char * userName)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select User_ID from cb_user where User_Name='%s'"), CStringW(userName));
	map<int, vector<string>>  result;
	if (db.Query(CW2A(sql), result) > 0)
	{
		return atoi(result[0][0].c_str());
	}
	return 0;
}

//Summary:根据用户ID获取用户名
//Parameter:用户ID
//Return:用户名
string DBManager::GetUserNameByID(long UserID)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select User_Name from cb_user where User_ID=%ld"), UserID);
	map<int, vector<string>>  result;
	if (db.Query(CW2A(sql), result) > 0)
	{
		return result[0][0];
	}
	return string();
}

//Summary:根据群组名获取群ID
//Parameter:群组名
//Return:群组ID
long DBManager::GetGroupIDByName(char * groupName)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select Group_ID from cb_group where Group_Name='%s'"), CStringW(groupName));
	map<int, vector<string>>  result;
	if (db.Query(CW2A(sql), result) > 0)
	{
		return atoi(result[0][0].c_str());
	}
	return 0;
}

//Summary:根据群ID获取群组名
//Parameter:群组ID
//Return:群组名
string DBManager::GetGroupNameByID(long GroupID)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select Group_Name from cb_group where Group_ID=%ld"), GroupID);
	map<int, vector<string>>  result;
	if (db.Query(CW2A(sql), result) > 0)
	{
		return result[0][0];
	}
	return string();
}


//Summary:登陆
//Patameter:账号.密码
//Return:是否成功
int DBManager::Login(char * userName, char * password)
{
	password = Tool::Encrypt(password);
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_user where User_Name='%s' and User_Password='%s'"), CStringW(userName), CStringW(password));
	map<int, vector<string>>  result;
	if (db.Query(CW2A(sql), result) > 0)
	{
		return 1;
	}
	return 0;
}

//Summary:注册
//Patameter:账号.密码
//Return:是否成功(0为失败,1为成功,2为用户名重复)
int DBManager::Register(char * userName, char * password)
{
	password = Tool::Encrypt(password);
	DBHelper db;
	CString sql1;
	sql1.Format(_T("select * from cb_user where User_Name = '%s'"), CStringW(userName));
	map<int, vector<string>>  result;

	if (db.Query(CW2A(sql1), result) > 0)
	{
		return 2;
	}

	CString sql2;
	sql2.Format(_T("insert into cb_user(User_Name,User_Password) values('%s','%s')"), CStringW(userName), CStringW(password));
	if (db.Exec(CW2A(sql2)) > 0)
	{
		return 1;
	}
	return 0;
}

//Summary:获取好友列表
//Parameter:用户名,好友表
//Return:好友数
int DBManager::GetFriendList(char * userName, map<int, vector<string>>  &result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select User_Name ,User_PicPath,User_RegisterTime,User_isOnline from  cb_user where User_ID in( select Friendship_OtherSideID from cb_friendship where Friendship_OurSideID=(select User_ID from cb_user where User_Name='%s'))"), CStringW(userName));
	return db.Query(CW2A(sql), result);
}

//获取群聊列表
//Parameter:用户名,群聊表
//Return:群聊数
int DBManager::GetGroupList(char * userName, map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select Group_Name,Group_PicPath,Group_CreateTime from cb_group where Group_ID in( select GroupMember_GroupID from cb_groupmember where GroupMember_UserID=(select User_ID from cb_user where User_Name='%s'))"), CStringW(userName));
	return db.Query(CW2A(sql), result);
}


//Summary:发送消息
//Parameter:
//		senderNamme:发送者用户名
//		reciverName:接收者用户名
//		msg:消息内容
//		isGroup:是否为群聊
//Return:受影响行数
int DBManager::SendMsg(char * senderNamme, char * reciverName, char * msg, int isGroup)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("insert into cb_message(Message_SenderID,Message_ReciverID,Message_Content,Message_isForGroup) values((select User_ID from cb_user where User_Name='%s'),(select User_ID from cb_user where User_Name='%s'),'%s',%d)", CStringW(senderNamme), CStringW(reciverName), CStringW(msg), isGroup));
	return db.Exec(CW2A(sql));
}

//Summary:查询是否为群组
//Parameter:
//		name:名字
//Return:1|0
int DBManager::IsGroup(char * name)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_group where Group_Name='%s'"), CString(name));
	map<int, vector<string>> result;
	if (db.Query(CW2A(sql), result) > 0)
	{
		return 1;
	}
	return 0;
}

//Summary:获取单聊记录
//Parameter:
//		senderID:接收者ID
//		reciverID:发送者ID
//		result:记录结果
//Return:消息数量
int DBManager::GetFriendChatRecords(long MyID, long FriendID, map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T(
		"select * from cb_message  where (Message_SenderID = %ld and Message_ReciverID = %ld) or (Message_SenderID = %ld and Message_ReciverID = %ld) and Message_isForGroup = 0  ORDER BY Message_SendTime desc limit 300"), MyID, FriendID, FriendID, MyID
	);
	return db.Query(CW2A(sql), result);
}


//Summary:获取群聊记录
//Parameter:
//		GroupID:群组ID
//		result:记录结果
//Return:消息数量
int DBManager::GetGroupChatRecords(long GroupID, map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select Message_SendTime,User_Name,Message_Content from cb_message left join cb_user on Message_SenderID=cb_user.User_ID where  Message_isForGroup = 1 and Message_GroupID=%ld  GROUP BY Message_Timestamp limit 300"), GroupID);
	return db.Query(CW2A(sql), result);
}


//Summary:获取未读消息数
//Parameter:
//		UserID:用户ID
//Return:数量
int DBManager::GetUnreadCount(int UserID)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_message where Message_ReciverID = %d and Message_IsHaveRead=0"), UserID);
	map<int, vector<string>> result;
	return db.Query(CW2A(sql), result);
}

//Summary:获取来自好友的未读消息数
//Parameter:
//		MyID:我的ID
//		FriendID:好友ID
//Return:数量
int DBManager::GetUnreadCountFromFriend(int MyID, int FriendID)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_message where Message_ReciverID = %d and Message_SenderID=%d and Message_IsHaveRead=0 and Message_isForGroup = 0"), MyID, FriendID);
	map<int, vector<string>> result;
	return db.Query(CW2A(sql), result);
}


//Summary:获取来自群组的未读消息数
//Parameter:
//		MyID:我的ID
//		GroupID:群组ID
//Return:数量
int DBManager::GetUnreadCountFromGroup(int MyID, int GroupID)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_message where Message_ReciverID = %d and Message_GroupID=%d and Message_IsHaveRead=0 and Message_isForGroup = 1"), MyID, GroupID);
	map<int, vector<string>> result;
	return db.Query(CW2A(sql), result);
}

//Summary:获取未读消息
//Parameter:
//		UserID:用户ID
//Return:数量
int DBManager::GetUnreadMsg(int UserID, map<int, vector<string>> & result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_message where Message_ReciverID = %d and Message_IsHaveRead=0 "), UserID);
	return db.Query(CW2A(sql), result);
}

//Summary:标记已读
//Parameter:
//		SenderID:发送者ID,群聊ID
//		ReciverID:接收者ID,我的ID
//Return:受影响行数
int DBManager::SetHaveRead(long SenderID, long ReciverID, int isGroup)
{
	DBHelper db;
	CString sql;
	if (isGroup == 0)
	{
		sql.Format(_T("update cb_message set Message_IsHaveRead=1 where Message_SenderID=%d and Message_ReciverID=%d"), SenderID, ReciverID);
	}
	else
	{
		sql.Format(_T("update cb_message set Message_IsHaveRead=1 where Message_ReciverID=%d and Message_GroupID=%d"), ReciverID, SenderID);
	}
	return db.Exec(CW2A(sql));
}

//Summary:查询群组成员列表
//Parameter:
//		GroupName:群组名称
//		result:结果
//Return:行数
int DBManager::GetGroupMember(char * GroupName, map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select User_ID,GroupIdentity_Name,User_Name from  cb_groupmember   left join cb_user   on cb_groupmember.GroupMember_UserID=cb_user.User_ID LEFT JOIN cb_groupidentity on cb_groupmember.GroupMember_Identity=cb_groupidentity.GroupIdentity_ID where  GroupMember_GroupID = (select Group_ID from cb_group where Group_Name='%s') order by GroupIdentity_ID"), CStringW(GroupName));
	return db.Query(CW2A(sql), result);
}

//Summary:添加好友
//Parameter:
//		MyID:我的ID
//		FriendID:好友ID
//Return:结果 1为成功,0为失败,2为已是好友
int DBManager::AddFriend(long MyID, long  FriendID)
{
	DBHelper db;
	//检查是否已发送请求但未处理
	CString sql1;
	sql1.Format(_T("select * from cb_validation where Validation_SenderID=%d and Validation_ReciverID=%d and Validation_IsHaveRead=0"), MyID, FriendID);
	map<int, vector<string>> result;
	if (db.Query(CW2A(sql1), result) > 0)
	{
		return 1;
	}
	//检查是否已是好友
	CString sql2;
	sql2.Format(_T("select * from cb_friendship where Friendship_OurSideID=%d and Friendship_OtherSideID=%d"), MyID, FriendID);
	if (db.Query(CW2A(sql2), result) > 0)
	{
		return 2;
	}
	//发送添加请求
	CString sql3;
	sql3.Format(_T("insert into cb_validation(Validation_SenderID,Validation_ReciverID) values(%d,%d)"), MyID, FriendID);
	if (db.Exec(CW2A(sql3)))
	{
		return 1;
	}
	return 0;
}

//Summary:同意添加
//Parameter:
//		MyID:我的ID
//		FriendID:好友ID
//Return:结果 1为成功,0为失败
int DBManager::AgreeAdd(long MyID, long FriendID)
{
	DBHelper db;
	//更改验证表
	CString sql1;
	sql1.Format(_T("update cb_validation set Validation_IsHaveRead=1 where Validation_SenderID=%d and Validation_ReciverID=%d "), FriendID, MyID);
	if (db.Exec(CW2A(sql1)) == 0)
	{
		return 0;
	}
	//添加对方为我的好友
	CString sql2;
	sql2.Format(_T("insert into cb_friendship(Friendship_OurSideID,Friendship_OtherSideID) values(%d,%d)"), MyID, FriendID);
	int result1 = db.Exec(CW2A(sql2));

	//添加我为对方的好友
	CString sql3;
	sql3.Format(_T("insert into cb_friendship(Friendship_OurSideID,Friendship_OtherSideID) values(%d,%d)"), FriendID, MyID);
	int result2 = db.Exec(CW2A(sql3));

	if (result1 > 0 && result2 > 0)
	{
		return 1;
	}
	return 0;
}

//Summary:拒绝添加
//Parameter:
//		MyID:我的ID
//		FriendID:好友ID
//Return:结果 1为成功,0为失败
int DBManager::RefuseAdd(long MyID, long FriendID)
{
	DBHelper db;
	//更改验证表
	CString sql1;
	sql1.Format(_T("update cb_validation set Validation_IsHaveRead=1 where Validation_SenderID=%d and Validation_ReciverID=%d "), FriendID, MyID);
	if (db.Exec(CW2A(sql1)) == 0)
	{
		return 0;
	}
	return 1;
}

//Summary:获取未处理的验证消息
//Parameter:MyID我的ID
//Return:数量
int DBManager::GetUnreadValidation(long MyID,map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_validation where Validation_ReciverID=%d and Validation_IsHaveRead=0"),MyID);
	return db.Query(CW2A(sql),result);
}



