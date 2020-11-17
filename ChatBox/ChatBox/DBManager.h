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

	//�����û�����ȡ�û�ID
	static long GetUserIDByName(char*userName);
	//�����û�ID��ȡ�û���
	static string GetUserNameByID(long UserID);
	//����Ⱥ������ȡȺ��ID
	static long GetGroupIDByName(char*groupName);
	//����ȺID��ȡȺ����
	static string GetGroupNameByID(long GroupID);
	//��½
	static int Login(char*userName, char*password);
	//ע��
	static int Register(char*userName, char*password);
	//��ȡ�����б�
	static int GetFriendList(char*userName, map<int, vector<string>>  &result);
	//��ȡȺ���б�
	static int GetGroupList(char*userName, map<int, vector<string>>  &result);
	//������Ϣ
	static int SendMsg(char*senderNamme, char*reciverName, char*msg, int isGroup);
	//��ѯ�Ƿ�ΪȺ��
	static int IsGroup(char*name);
	//��ȡ���ļ�¼
	static int GetFriendChatRecords(long MyID, long FriendID, map<int, vector<string>>  &result);
	//��ȡȺ�ļ�¼
	static int GetGroupChatRecords(long GroupID, map<int, vector<string>>  &result);
	//��ȡδ����Ϣ��
	static int GetUnreadCount(int UserID);
	//��ȡ���Ժ��ѵ�δ����Ϣ��
	static int GetUnreadCountFromFriend(int MyID, int FriendID);
	//��ȡ����Ⱥ���δ����Ϣ��
	static int GetUnreadCountFromGroup(int MyID,int GroupID);
	//��ȡδ����Ϣ
	static int GetUnreadMsg(int UserID, map<int, vector<string>> &result);
	//����Ѷ�
	static int SetHaveRead(long SenderID, long ReciverID,int isGroup=0);
	//��ѯ�����ҵĳ�Ա�б�
	static int GetGroupMember(char*GroupName, map<int, vector<string>> &result);
	//��Ӻ���
	static int AddFriend(long MyID, long FriendID);
	//ͬ�����
	static int AgreeAdd(long MyID,long FriendID);
	//�ܾ����
	static int RefuseAdd(long MyID, long FriendID);
	//��ȡδ�������֤��Ϣ
	static int GetUnreadValidation(long MyID, map<int, vector<string>>& result);
};

