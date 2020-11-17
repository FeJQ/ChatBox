// CFriendChat.cpp: 实现文件
//

#include "stdafx.h"
#include "CFriendChat.h"
#include "afxdialogex.h"
#include <Windows.h>
#include "resource.h"
#include "Packs.h"
#include "DBManager.h"
#include "Communication.h"
#include "Tool.h"
#include <time.h>
#include "CMyListCtrl.h"
#include "CVideo.h"

// CFriendChat 对话框

IMPLEMENT_DYNAMIC(CFriendChat, CDialogEx)

CFriendChat::CFriendChat(CString _fname, CString _mname, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FriendChat, pParent)
	, m_txtVal_msg(_T(""))
	, m_txtVal_edit(_T(""))
{
	this->m_fname = _fname;
	this->m_mname = _mname;
}

CFriendChat::~CFriendChat()
{
}

void CFriendChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FriendChat_edit, m_txtCtrl_edit);
	DDX_Control(pDX, IDC_FriendChat_msg, m_txtCtrl_msg);
	DDX_Text(pDX, IDC_FriendChat_msg, m_txtVal_msg);
	DDX_Text(pDX, IDC_FriendChat_edit, m_txtVal_edit);
}


BEGIN_MESSAGE_MAP(CFriendChat, CDialogEx)
	ON_BN_CLICKED(IDC_FriendChat_send, &CFriendChat::OnBnClickedFriendchatsend)
	ON_BN_CLICKED(IDC_FriendChat_close, &CFriendChat::OnBnClickedFriendchatclose)
	ON_MESSAGE(CU_RECIVEMSG, &CFriendChat::OnCuRecivemsg)
	ON_BN_CLICKED(IDC_showVideo, &CFriendChat::OnBnClickedshowvideo)
END_MESSAGE_MAP()


// CFriendChat 消息处理程序


BOOL CFriendChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowTextW(m_fname);
	//获取ID
	int fID=DBManager::GetUserIDByName(CW2A(m_fname));
	int mID= DBManager::GetUserIDByName(CW2A(m_mname));
	string fName = DBManager::GetUserNameByID(fID);
	string mName = DBManager::GetUserNameByID(mID);
	//标记已读
	DBManager::SetHaveRead(fID, mID);	
	//获取聊天记录
	map<int, vector<string>> result;
	DBManager::GetFriendChatRecords(mID, fID, result);
	string str = string();
	string name = string();
	for (int i = result.size()-1; i >=0 ; i--)
	{
		if (atoi(result[i][1].c_str()) == fID)
		{
			name = fName;
		}
		else if (atoi(result[i][1].c_str()) == mID)
		{
			name = mName;
		}

		//Tool:: xor (result[i][5]);
		str = str + "\r\n\r\n" +  result[i][4] + "    " + name + "\r\n" + result[i][5];
	}	
	m_txtCtrl_msg.SetWindowTextW(Tool::toCString(str));
	m_txtCtrl_msg.LineScroll(m_txtCtrl_msg.GetLineCount());
	return TRUE;

}


//发送
void CFriendChat::OnBnClickedFriendchatsend()
{
	//获取消息内容
	CString msg;
	m_txtCtrl_edit.GetWindowTextW(msg);
	if (msg.GetLength() > 0)
	{
		//组装消息包
		MessagePack pack(
			msg.AllocSysString(),
			DBManager::GetUserIDByName(CW2A(m_mname)),
			DBManager::GetUserIDByName(CW2A(m_fname)),
			0
		);
		int sendSize = Communication::MySendMessage(&pack);
		if (sendSize > 0)
		{

			SYSTEMTIME st;
			CString strTime;
			GetLocalTime(&st);
			strTime.Format(_T("%d/%d/%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			//发送成功
			m_txtCtrl_edit.SetWindowTextW(_T(""));
			CString originalMsg;
			m_txtCtrl_msg.GetWindowTextW(originalMsg);
			m_txtCtrl_msg.SetWindowTextW(originalMsg + "\r\n\r\n" + strTime + "    " + m_mname + "\r\n" + msg);
			UpdateData(TRUE);
			m_txtCtrl_msg.LineScroll(m_txtCtrl_msg.GetLineCount());
		}
	}
}

//加载接收到的消息
afx_msg LRESULT CFriendChat::OnCuRecivemsg(WPARAM wParam, LPARAM lParam)
{
	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format(_T("%d/%d/%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	MessagePack *pMsg = (MessagePack*)wParam;
	CString originalMsg;
	m_txtCtrl_msg.GetWindowTextW(originalMsg);
	m_txtCtrl_msg.SetWindowTextW(originalMsg + "\r\n\r\n"+strTime + "    " + m_fname + "\r\n" + pMsg->msg);
	UpdateData(TRUE);
	m_txtCtrl_msg.LineScroll(m_txtCtrl_msg.GetLineCount());

	//标记已读
	//获取ID
	fID = DBManager::GetUserIDByName(CW2A(m_fname));
    mID = DBManager::GetUserIDByName(CW2A(m_mname));
	DBManager::SetHaveRead(fID, mID);
	return 0;
}


//退出
void CFriendChat::OnCancel()
{
	::SendMessage(this->GetParent()->GetSafeHwnd(), CU_CLOSEWIN, (WPARAM)&m_fname, 0);
	CDialogEx::OnCancel();
	//delete this;
}

//关闭
void CFriendChat::OnBnClickedFriendchatclose()
{
	::SendMessage(this->GetParent()->GetSafeHwnd(), CU_CLOSEWIN, (WPARAM)&m_fname, 0);
	CDialogEx::OnCancel();
	//delete this;
}


//回车发送消息
BOOL CFriendChat::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetDlgItem(IDC_FriendChat_edit)->GetSafeHwnd() == ::GetFocus())
		{
			OnBnClickedFriendchatsend();
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//视频聊天
void CFriendChat::OnBnClickedshowvideo()
{
	CVideo *video = new CVideo();
	video->Create(IDD_video, this);
	video->ShowWindow(SW_SHOW);
}
