// CAddFriend.cpp: 实现文件
//

#include "stdafx.h"
#include "CAddFriend.h"
#include "afxdialogex.h"
#include "resource.h"
#include "DBManager.h"
#include "Communication.h"
#include "Packs.h"

// CAddFriend 对话框

IMPLEMENT_DYNAMIC(CAddFriend, CDialogEx)

CAddFriend::CAddFriend(int mID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_addFriend, pParent)
{
	this->m_mID = mID;
}

CAddFriend::~CAddFriend()
{
}

void CAddFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtCtrl_userName);
}


BEGIN_MESSAGE_MAP(CAddFriend, CDialogEx)
	ON_BN_CLICKED(ID_add, &CAddFriend::OnBnClickedadd)
END_MESSAGE_MAP()


// CAddFriend 消息处理程序


//添加
void CAddFriend::OnBnClickedadd()
{
	//获取输入的用户名
	CString friendName;
	m_txtCtrl_userName.GetWindowTextW(friendName);
	int fID = DBManager::GetUserIDByName(CW2A(friendName));

	int result = DBManager::AddFriend(m_mID, fID);
	if (result == 1)
	{
		MessagePack pack(m_mID, fID,0);
		int sendSize=Communication::MySendMessage(&pack);
		if(sendSize>0)
		{
			MessageBox(_T("发送成功!"));
		}
		else
		{
			MessageBox(_T("与服务器断开连接!"),_T("错误"),MB_ICONERROR);
		}		
	}
	else if (result == 0)
	{
		MessageBox(_T("发送失败!"));

	}
	else if (result == 2)
	{
		MessageBox(_T("你们已经是好友啦!"));
	}
}
