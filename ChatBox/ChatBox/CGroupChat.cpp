// CGroupChat.cpp: 实现文件
//

#include "stdafx.h"
#include "CGroupChat.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CMyListCtrl.h"
#include "DBManager.h"
#include "Tool.h"
#include "Packs.h"
#include "Communication.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

// CGroupChat 对话框

IMPLEMENT_DYNAMIC(CGroupChat, CDialogEx)

CGroupChat::CGroupChat(CString _name, CString m_mname, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GroupChat, pParent)
{
	this->m_name = _name;
	this->m_mname = m_mname;
}

CGroupChat::~CGroupChat()
{
}

void CGroupChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GroupChat_list, m_listCtrl_groupMember);
	DDX_Control(pDX, IDC_GroupChat_edit, m_txtCtrl_edit);
	DDX_Control(pDX, IDC_GroupChat_msg, m_txtCtrl_msg);
}


BEGIN_MESSAGE_MAP(CGroupChat, CDialogEx)
	ON_BN_CLICKED(IDC_GroupChat_send, &CGroupChat::OnBnClickedGroupchatsend)
	ON_BN_CLICKED(IDC_GroupChat_close, &CGroupChat::OnBnClickedGroupchatclose)
	ON_MESSAGE(CU_GRECIVEMSG, &CGroupChat::OnCuGrecivemsg)
END_MESSAGE_MAP()


// CGroupChat 消息处理程序


BOOL CGroupChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowTextW(m_name);
	//获取群成员
	map<int, vector<string>> result;
	int memberCount = DBManager::GetGroupMember(CW2A(m_name), result);
	m_listCtrl_groupMember.loadGroupMember();
	for (int i = 0; i < result.size(); i++)
	{
		m_listCtrl_groupMember.addMember(Tool::toCString(result[i][1]), Tool::toCString(result[i][2]));
	}
	//获取群消息记录
	map<int, vector<string>> groupMsg;
	int msgCount = DBManager::GetGroupChatRecords(DBManager::GetGroupIDByName(CW2A(m_name)), groupMsg);
	string str = string();
	for (int i = 0; i < msgCount; i++)
	{
		str = str + +"\r\n\r\n" + groupMsg[i][0] + "    " + groupMsg[i][1] + "\r\n" + groupMsg[i][2];
	}
	m_txtCtrl_msg.SetWindowTextW(Tool::toCString(str));
	int gID = DBManager::GetGroupIDByName(CW2A(m_name));
	int mID = DBManager::GetUserIDByName(CW2A(m_mname));
	DBManager::SetHaveRead(gID, mID,1);
	m_txtCtrl_msg.LineScroll(m_txtCtrl_msg.GetLineCount());
	return TRUE;
}

//发送消息
void CGroupChat::OnBnClickedGroupchatsend()
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
			DBManager::GetGroupIDByName(CW2A(m_name)),
			1
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
			m_txtCtrl_msg.LineScroll(m_txtCtrl_msg.GetLineCount());
		}
	}
	m_txtCtrl_msg.LineScroll(m_txtCtrl_msg.GetLineCount());
}

//关闭窗口
void CGroupChat::OnCancel()
{
	::SendMessage(this->GetParent()->GetSafeHwnd(), CU_CLOSEWIN, (WPARAM)&m_name, 1);
	CDialogEx::OnCancel();
}

//关闭窗口
void CGroupChat::OnBnClickedGroupchatclose()
{
	::SendMessage(this->GetParent()->GetSafeHwnd(), CU_CLOSEWIN, (WPARAM)&m_name, 1);
	CDialogEx::OnCancel();
}

//快捷发送
BOOL CGroupChat::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetDlgItem(IDC_GroupChat_edit)->GetSafeHwnd() == ::GetFocus())
		{
			OnBnClickedGroupchatsend();
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


//加载接收到消息
afx_msg LRESULT CGroupChat::OnCuGrecivemsg(WPARAM wParam, LPARAM lParam)
{
	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format(_T("%d/%d/%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	MessagePack *pMsg = (MessagePack*)wParam;
	CString originalMsg;
	m_txtCtrl_msg.GetWindowTextW(originalMsg);
	m_txtCtrl_msg.SetWindowTextW(originalMsg + "\r\n\r\n" + strTime + "    " + m_name + "\r\n" + pMsg->msg);
	UpdateData(TRUE);
	m_txtCtrl_msg.LineScroll(m_txtCtrl_msg.GetLineCount());

	int gID = DBManager::GetGroupIDByName(CW2A(m_name));
	int mID = DBManager::GetUserIDByName(CW2A(m_mname));
	DBManager::SetHaveRead(gID, mID,1);
	return 0;
}
