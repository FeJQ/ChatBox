
// ChatBoxDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatBox.h"
#include "ChatBoxDlg.h"
#include "afxdialogex.h"
#include "Communication.h"
#include "DBManager.h"
#include "Tool.h"
#include "CFriendChat.h"
#include "CGroupChat.h"
#include "CUnreadBox.h"
#include "CAddFriend.h"
#include <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatBoxDlg 对话框



CChatBoxDlg::CChatBoxDlg(char* userName, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Main, pParent)
{

	m_unreadCount = 0;
	strcpy(m_userName, userName);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Main_tree, m_treeCtrl_mainlist);
	DDX_Control(pDX, IDC_unreadMsg, m_labCtrl_unread);
	DDX_Control(pDX, IDC_Main_Username, m_labCtrl_Name);
}

BEGIN_MESSAGE_MAP(CChatBoxDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_Main_tree, &CChatBoxDlg::OnDblclkMainTree)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(CU_MESSAGE, &CChatBoxDlg::OnCuMessage)
	ON_MESSAGE(CU_CLOSEWIN, &CChatBoxDlg::OnCuClosewin)
	ON_MESSAGE(CU_OPENMESSAGE, &CChatBoxDlg::OnCuOpenmessage)
	ON_BN_CLICKED(IDC_Main_addFriend, &CChatBoxDlg::OnBnClickedMainaddfriend)
END_MESSAGE_MAP()


// CChatBoxDlg 消息处理程序

BOOL CChatBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	MainForm = this->GetSafeHwnd();
	MyName.Format(_T("%s"), CStringW(m_userName));	

	HTREEITEM hFriend;
	map<int, vector<string>>  fList;
	int fCount = DBManager::GetFriendList(m_userName, fList);
	string fStr;
	fStr += "我的好友(";
	fStr += to_string(fCount);
	fStr += ")";
	hFriend = m_treeCtrl_mainlist.InsertItem(Tool::toCString(fStr), 0, 0);
	for (int i = 0; i < fCount; i++)
	{
		m_treeCtrl_mainlist.InsertItem(Tool::toCString(fList[i][0]), 1, 1, hFriend, TVI_LAST);
	}


	HTREEITEM hGroup;
	map<int, vector<string>>  gList;
	int gCount = DBManager::GetGroupList(m_userName, gList);
	string gStr;
	gStr += "我的群组(";
	gStr += to_string(gCount);
	gStr += ")";
	hGroup = m_treeCtrl_mainlist.InsertItem(Tool::toCString(gStr), 0, 0);
	for (int i = 0; i < gCount; i++)
	{
		m_treeCtrl_mainlist.InsertItem(Tool::toCString(gList[i][0]), 1, 1, hGroup, TVI_LAST);
	}

	ReloadUnreadCount();

	CString uname;
	uname.Format(_T("%s"), CStringW(m_userName));
	m_labCtrl_Name.SetWindowTextW(uname);
	CreateThread(NULL, NULL, Communication::ReciveMessage, this->GetSafeHwnd(), NULL, NULL);

	//上线及时检测是否有未处理的验证消息
	map<int, vector<string>> unreadValidation;
	int MyID = DBManager::GetUserIDByName(m_userName);
	int count=DBManager::GetUnreadValidation(MyID,unreadValidation);
	for (int i = 0; i < count; i++)
	{
		CString msg;
		msg.Format(_T("用户%s请求添加您为好友,是否同意?"), Tool::toCString(DBManager::GetUserNameByID(atoi(unreadValidation[i][1].c_str()))));
		int result = MessageBox(msg, _T("提示"), MB_YESNO);
		if (result == IDYES)
		{
			DBManager::AgreeAdd(MyID, atoi(unreadValidation[i][1].c_str()));
		}
		else if (result == IDNO)
		{
			DBManager::RefuseAdd(MyID, atoi(unreadValidation[i][1].c_str()));
		}
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//刷新未读消息
void CChatBoxDlg::ReloadUnreadCount()
{
	m_unreadCount = DBManager::GetUnreadCount(DBManager::GetUserIDByName(m_userName));
	CString unread;
	unread.Format(_T("未读 %d 条"), m_unreadCount);
	m_labCtrl_unread.SetWindowTextW(unread);
}

//双击好友或群聊列表
void CChatBoxDlg::OnDblclkMainTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//打开聊天窗口
	*pResult = 0;
	HTREEITEM hItem = m_treeCtrl_mainlist.GetSelectedItem();
	HTREEITEM hParent = m_treeCtrl_mainlist.GetParentItem(hItem);
	if (hParent != NULL)
	{
		string parentNodeName = Tool::toString(m_treeCtrl_mainlist.GetItemText(hParent));
		//好友名和群聊名
		string name = Tool::toString(m_treeCtrl_mainlist.GetItemText(hItem));
		//父节点名字
		string parentName = parentNodeName.substr(0, 8);
		if (parentName == "我的好友")
		{
			CString mname;
			mname.Format(_T("%s"), CStringW(m_userName));
			//开启窗口
			CFriendChat *fc = new CFriendChat(Tool::toCString(name), mname);
			fc->Create(IDD_FriendChat, this);
			fc->ShowWindow(SW_SHOW);
			m_fWins[name] = fc->GetSafeHwnd();
			//刷新未读消息数
			ReloadUnreadCount();
		}
		else if (parentName == "我的群组")
		{
			CString mname;
			mname.Format(_T("%s"), CStringW(m_userName));
			CGroupChat *gc = new CGroupChat(Tool::toCString(name), mname);
			gc->Create(IDD_GroupChat, this);
			gc->ShowWindow(SW_SHOW);
			m_gWins[name] = gc->GetSafeHwnd();
			//刷新未读消息数
			ReloadUnreadCount();
		}
	}

}


//窗口背景重绘
BOOL CChatBoxDlg::OnEraseBkgnd(CDC* pDC)
{
	//设置brush为希望的背景颜色
	CBrush backBrush(RGB(255, 255, 255));

	//保存旧的brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	return TRUE;
}

int aMsg = 0;
//设置文本颜色
HBRUSH CChatBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
		return b;//这句好有用
	}
	if (pWnd->GetDlgCtrlID() == IDC_Main_Username)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
		return b;//这句好有用
	}
	if (pWnd->GetDlgCtrlID() == IDC_unreadMsg)
	{
		MessageBox(_T("fss"));
	}
	return hbr;
}

//接收到服务器发来的消息
afx_msg LRESULT CChatBoxDlg::OnCuMessage(WPARAM wParam, LPARAM lParam)
{
	MessagePack *pMsg = (MessagePack*)wParam;
	if (pMsg->pack == PACK::PACK_Message)
	{
		//解密
		Tool:: xor ((char*)aMsg);
		//如果已打开与该消息发送者的聊天框,则不进入未读消息盒
		if (!pMsg->isGroup)
		{
			//单聊
			for (map<string, HWND>::iterator it = m_fWins.begin(); it != m_fWins.end(); it++)
			{
				if (DBManager::GetUserNameByID(pMsg->senderID) == it->first)
				{
					//已打开聊天窗口
					HWND fHwnd = it->second;
					::SendMessage(fHwnd, CU_RECIVEMSG, (WPARAM)pMsg, NULL);
				}
				ReloadUnreadCount();
				return 0;
			}
			//未打开聊天窗口
			//进入消息盒		
			HWND fHwnd = unreadHWND;
			string * p = new string;
			*p = "来自" + DBManager::GetUserNameByID(pMsg->senderID) + "的消息";
			::SendMessage(fHwnd, CU_MESSAGEQUEUE1, (WPARAM)(int*)p, NULL);
			ReloadUnreadCount();
		}
		else
		{
			//群聊
			for (map<string, HWND>::iterator it = m_gWins.begin(); it != m_gWins.end(); it++)
			{
				if (DBManager::GetGroupNameByID(pMsg->GroupID) == it->first)
				{
					//已打开聊天窗口
					HWND fHwnd = it->second;
					::SendMessage(fHwnd, CU_GRECIVEMSG, (WPARAM)pMsg, NULL);
					return 0;
				}
				ReloadUnreadCount();
			}

			//未打开聊天窗口
			//进入消息盒		
			HWND fHwnd = unreadHWND;
			string * p = new string;
			*p = "来自" + DBManager::GetGroupNameByID(pMsg->GroupID) + "的消息";
			::SendMessage(fHwnd, CU_MESSAGEQUEUE1, (WPARAM)(int*)p, NULL);
			ReloadUnreadCount();
		}
	}
	else if (pMsg->pack == PACK::PACK_Validation)
	{
		CString msg;
		msg.Format(_T("用户%s请求添加您为好友,是否同意?"), Tool::toCString(DBManager::GetUserNameByID(pMsg->senderID)));
		int result = MessageBox(msg, _T("提示"), MB_YESNO);
		if (result== IDYES)
		{
			DBManager::AgreeAdd(pMsg->reciverID,pMsg->senderID);
		}
		else if(result==IDNO)
		{
			DBManager::RefuseAdd(pMsg->reciverID, pMsg->senderID);
		}
	}

	return 0;
}

//聊天窗口被关闭
afx_msg LRESULT CChatBoxDlg::OnCuClosewin(WPARAM wParam, LPARAM lParam)
{
	CString *fname = (CString*)wParam;
	int isGroup = (int)lParam;
	if (isGroup)
	{
		for (map<string, HWND>::iterator it = m_gWins.begin(); it != m_gWins.end(); it++)
		{
			if (*fname == Tool::toCString(it->first))
			{
				m_gWins.erase(it);
				return 0;
			}
		}
	}
	else
	{
		for (map<string, HWND>::iterator it = m_fWins.begin(); it != m_fWins.end(); it++)
		{
			if (*fname == Tool::toCString(it->first))
			{
				m_fWins.erase(it);
				return 0;
			}
		}
	}
	return 0;
}

//打开聊天窗口
afx_msg LRESULT CChatBoxDlg::OnCuOpenmessage(WPARAM wParam, LPARAM lParam)
{
	CString * name = (CString*)wParam;
	int isGroup = DBManager::IsGroup(CW2A(*name));
	if (!isGroup)
	{
		//单聊
		for (map<string, HWND>::iterator it = m_fWins.begin(); it != m_fWins.end(); it++)
		{
			if (it->first == Tool::toString(*name))
			{
				//如果窗体已打开
				::SetFocus(it->second);
				ReloadUnreadCount();
				return 0;
			}
		}
		CString mname;
		mname.Format(_T("%s"), CString(m_userName));

		CFriendChat *fc = new CFriendChat(*name, mname);
		fc->Create(IDD_FriendChat, this);
		fc->ShowWindow(SW_SHOW);
		string sn = Tool::toString(*name);
		m_fWins[sn] = fc->GetSafeHwnd();
		ReloadUnreadCount();
	}
	else
	{
		//群聊
		for (map<string, HWND>::iterator it = m_gWins.begin(); it != m_gWins.end(); it++)
		{
			if (it->first == Tool::toString(*name))
			{
				//如果窗体已打开
				::SetFocus(it->second);
				ReloadUnreadCount();
				return 0;
			}
		}
		CString mname;
		mname.Format(_T("%s"), CStringW(m_userName));

		CGroupChat *gc = new CGroupChat(*name, mname);
		gc->Create(IDD_GroupChat, this);
		gc->ShowWindow(SW_SHOW);
		string sn = Tool::toString(*name);
		m_gWins[sn] = gc->GetSafeHwnd();
		ReloadUnreadCount();
	}
	return 0;
}

//查找好友
void CChatBoxDlg::OnBnClickedMainaddfriend()
{
	int mID = DBManager::GetUserIDByName(m_userName);
	CAddFriend add(mID);
	add.DoModal();
}
