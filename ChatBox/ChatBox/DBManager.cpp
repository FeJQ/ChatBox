#include "stdafx.h"
#include "DBManager.h"
#include "Tool.h"

DBManager::DBManager()
{
}


DBManager::~DBManager()
{
}


//Summary:�����û�����ȡID
//Parameter:�û���
//Return:�û�ID
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

//Summary:�����û�ID��ȡ�û���
//Parameter:�û�ID
//Return:�û���
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

//Summary:����Ⱥ������ȡȺID
//Parameter:Ⱥ����
//Return:Ⱥ��ID
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

//Summary:����ȺID��ȡȺ����
//Parameter:Ⱥ��ID
//Return:Ⱥ����
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


//Summary:��½
//Patameter:�˺�.����
//Return:�Ƿ�ɹ�
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

//Summary:ע��
//Patameter:�˺�.����
//Return:�Ƿ�ɹ�(0Ϊʧ��,1Ϊ�ɹ�,2Ϊ�û����ظ�)
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

//Summary:��ȡ�����б�
//Parameter:�û���,���ѱ�
//Return:������
int DBManager::GetFriendList(char * userName, map<int, vector<string>>  &result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select User_Name ,User_PicPath,User_RegisterTime,User_isOnline from  cb_user where User_ID in( select Friendship_OtherSideID from cb_friendship where Friendship_OurSideID=(select User_ID from cb_user where User_Name='%s'))"), CStringW(userName));
	return db.Query(CW2A(sql), result);
}

//��ȡȺ���б�
//Parameter:�û���,Ⱥ�ı�
//Return:Ⱥ����
int DBManager::GetGroupList(char * userName, map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select Group_Name,Group_PicPath,Group_CreateTime from cb_group where Group_ID in( select GroupMember_GroupID from cb_groupmember where GroupMember_UserID=(select User_ID from cb_user where User_Name='%s'))"), CStringW(userName));
	return db.Query(CW2A(sql), result);
}


//Summary:������Ϣ
//Parameter:
//		senderNamme:�������û���
//		reciverName:�������û���
//		msg:��Ϣ����
//		isGroup:�Ƿ�ΪȺ��
//Return:��Ӱ������
int DBManager::SendMsg(char * senderNamme, char * reciverName, char * msg, int isGroup)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("insert into cb_message(Message_SenderID,Message_ReciverID,Message_Content,Message_isForGroup) values((select User_ID from cb_user where User_Name='%s'),(select User_ID from cb_user where User_Name='%s'),'%s',%d)", CStringW(senderNamme), CStringW(reciverName), CStringW(msg), isGroup));
	return db.Exec(CW2A(sql));
}

//Summary:��ѯ�Ƿ�ΪȺ��
//Parameter:
//		name:����
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

//Summary:��ȡ���ļ�¼
//Parameter:
//		senderID:������ID
//		reciverID:������ID
//		result:��¼���
//Return:��Ϣ����
int DBManager::GetFriendChatRecords(long MyID, long FriendID, map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T(
		"select * from cb_message  where (Message_SenderID = %ld and Message_ReciverID = %ld) or (Message_SenderID = %ld and Message_ReciverID = %ld) and Message_isForGroup = 0  ORDER BY Message_SendTime desc limit 300"), MyID, FriendID, FriendID, MyID
	);
	return db.Query(CW2A(sql), result);
}


//Summary:��ȡȺ�ļ�¼
//Parameter:
//		GroupID:Ⱥ��ID
//		result:��¼���
//Return:��Ϣ����
int DBManager::GetGroupChatRecords(long GroupID, map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select Message_SendTime,User_Name,Message_Content from cb_message left join cb_user on Message_SenderID=cb_user.User_ID where  Message_isForGroup = 1 and Message_GroupID=%ld  GROUP BY Message_Timestamp limit 300"), GroupID);
	return db.Query(CW2A(sql), result);
}


//Summary:��ȡδ����Ϣ��
//Parameter:
//		UserID:�û�ID
//Return:����
int DBManager::GetUnreadCount(int UserID)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_message where Message_ReciverID = %d and Message_IsHaveRead=0"), UserID);
	map<int, vector<string>> result;
	return db.Query(CW2A(sql), result);
}

//Summary:��ȡ���Ժ��ѵ�δ����Ϣ��
//Parameter:
//		MyID:�ҵ�ID
//		FriendID:����ID
//Return:����
int DBManager::GetUnreadCountFromFriend(int MyID, int FriendID)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_message where Message_ReciverID = %d and Message_SenderID=%d and Message_IsHaveRead=0 and Message_isForGroup = 0"), MyID, FriendID);
	map<int, vector<string>> result;
	return db.Query(CW2A(sql), result);
}


//Summary:��ȡ����Ⱥ���δ����Ϣ��
//Parameter:
//		MyID:�ҵ�ID
//		GroupID:Ⱥ��ID
//Return:����
int DBManager::GetUnreadCountFromGroup(int MyID, int GroupID)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_message where Message_ReciverID = %d and Message_GroupID=%d and Message_IsHaveRead=0 and Message_isForGroup = 1"), MyID, GroupID);
	map<int, vector<string>> result;
	return db.Query(CW2A(sql), result);
}

//Summary:��ȡδ����Ϣ
//Parameter:
//		UserID:�û�ID
//Return:����
int DBManager::GetUnreadMsg(int UserID, map<int, vector<string>> & result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_message where Message_ReciverID = %d and Message_IsHaveRead=0 "), UserID);
	return db.Query(CW2A(sql), result);
}

//Summary:����Ѷ�
//Parameter:
//		SenderID:������ID,Ⱥ��ID
//		ReciverID:������ID,�ҵ�ID
//Return:��Ӱ������
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

//Summary:��ѯȺ���Ա�б�
//Parameter:
//		GroupName:Ⱥ������
//		result:���
//Return:����
int DBManager::GetGroupMember(char * GroupName, map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select User_ID,GroupIdentity_Name,User_Name from  cb_groupmember   left join cb_user   on cb_groupmember.GroupMember_UserID=cb_user.User_ID LEFT JOIN cb_groupidentity on cb_groupmember.GroupMember_Identity=cb_groupidentity.GroupIdentity_ID where  GroupMember_GroupID = (select Group_ID from cb_group where Group_Name='%s') order by GroupIdentity_ID"), CStringW(GroupName));
	return db.Query(CW2A(sql), result);
}

//Summary:��Ӻ���
//Parameter:
//		MyID:�ҵ�ID
//		FriendID:����ID
//Return:��� 1Ϊ�ɹ�,0Ϊʧ��,2Ϊ���Ǻ���
int DBManager::AddFriend(long MyID, long  FriendID)
{
	DBHelper db;
	//����Ƿ��ѷ�������δ����
	CString sql1;
	sql1.Format(_T("select * from cb_validation where Validation_SenderID=%d and Validation_ReciverID=%d and Validation_IsHaveRead=0"), MyID, FriendID);
	map<int, vector<string>> result;
	if (db.Query(CW2A(sql1), result) > 0)
	{
		return 1;
	}
	//����Ƿ����Ǻ���
	CString sql2;
	sql2.Format(_T("select * from cb_friendship where Friendship_OurSideID=%d and Friendship_OtherSideID=%d"), MyID, FriendID);
	if (db.Query(CW2A(sql2), result) > 0)
	{
		return 2;
	}
	//�����������
	CString sql3;
	sql3.Format(_T("insert into cb_validation(Validation_SenderID,Validation_ReciverID) values(%d,%d)"), MyID, FriendID);
	if (db.Exec(CW2A(sql3)))
	{
		return 1;
	}
	return 0;
}

//Summary:ͬ�����
//Parameter:
//		MyID:�ҵ�ID
//		FriendID:����ID
//Return:��� 1Ϊ�ɹ�,0Ϊʧ��
int DBManager::AgreeAdd(long MyID, long FriendID)
{
	DBHelper db;
	//������֤��
	CString sql1;
	sql1.Format(_T("update cb_validation set Validation_IsHaveRead=1 where Validation_SenderID=%d and Validation_ReciverID=%d "), FriendID, MyID);
	if (db.Exec(CW2A(sql1)) == 0)
	{
		return 0;
	}
	//��ӶԷ�Ϊ�ҵĺ���
	CString sql2;
	sql2.Format(_T("insert into cb_friendship(Friendship_OurSideID,Friendship_OtherSideID) values(%d,%d)"), MyID, FriendID);
	int result1 = db.Exec(CW2A(sql2));

	//�����Ϊ�Է��ĺ���
	CString sql3;
	sql3.Format(_T("insert into cb_friendship(Friendship_OurSideID,Friendship_OtherSideID) values(%d,%d)"), FriendID, MyID);
	int result2 = db.Exec(CW2A(sql3));

	if (result1 > 0 && result2 > 0)
	{
		return 1;
	}
	return 0;
}

//Summary:�ܾ����
//Parameter:
//		MyID:�ҵ�ID
//		FriendID:����ID
//Return:��� 1Ϊ�ɹ�,0Ϊʧ��
int DBManager::RefuseAdd(long MyID, long FriendID)
{
	DBHelper db;
	//������֤��
	CString sql1;
	sql1.Format(_T("update cb_validation set Validation_IsHaveRead=1 where Validation_SenderID=%d and Validation_ReciverID=%d "), FriendID, MyID);
	if (db.Exec(CW2A(sql1)) == 0)
	{
		return 0;
	}
	return 1;
}

//Summary:��ȡδ�������֤��Ϣ
//Parameter:MyID�ҵ�ID
//Return:����
int DBManager::GetUnreadValidation(long MyID,map<int, vector<string>>& result)
{
	DBHelper db;
	CString sql;
	sql.Format(_T("select * from cb_validation where Validation_ReciverID=%d and Validation_IsHaveRead=0"),MyID);
	return db.Query(CW2A(sql),result);
}



