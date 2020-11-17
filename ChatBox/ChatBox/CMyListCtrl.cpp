// CMyListCtrl.cpp: 实现文件
//

#include "stdafx.h"
#include "CMyListCtrl.h"
#include "DBManager.h"

// CMyListCtrl
HWND  MainForm = { 0 };
CString MyName;
int fID = 0;
int mID = 0;
IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}

//初始化控件
void CMyListCtrl::init()
{
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // 整行选择、网格线
	InsertColumn(0, _T("消息"), LVCFMT_LEFT, 220);
	InsertColumn(1, _T("数量"), LVCFMT_LEFT, 113);
}

//添加行
void CMyListCtrl::insert(CString str)
{
	str.Replace(_T("来自"), _T(""));
	str.Replace(_T("的消息"), _T(""));
	CString fromName = _T("来自") + str + _T("的消息");
	for (int i = 0; i < GetItemCount(); i++)
	{
		if (GetItemText(i, 0) == fromName)
		{
			CString countStr;
			int isGroup = DBManager::IsGroup(CW2A(str));
			//未读消息数
			int count = 0;
			if (isGroup)
			{
				count = DBManager::GetUnreadCountFromGroup(DBManager::GetUserIDByName(CW2A(MyName)), DBManager::GetGroupIDByName(CW2A(str)));
			}
			else
			{
				count = DBManager::GetUnreadCountFromFriend(DBManager::GetUserIDByName(CW2A(MyName)), DBManager::GetUserIDByName(CW2A(str)));
			}
			countStr.Format(_T("%d条"), count);

			SetItemText(i, 0, fromName);
			SetItemText(i, 1, countStr);
			return;
		}
	}
	int nRow = InsertItem(GetItemCount(), fromName);
	CString countStr;
	countStr.Format(_T("%d条"), 1);
	SetItemText(nRow, 1, countStr);
}
//加载群成员列
void CMyListCtrl::loadGroupMember()
{
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // 整行选择、网格线
	InsertColumn(0, _T("身份"), LVCFMT_LEFT, 60);
	InsertColumn(1, _T("用户名"), LVCFMT_LEFT, 130);
}
//添加群成员
void CMyListCtrl::addMember(CString str1, CString str2)
{
	int nRow = InsertItem(GetItemCount(), str1);
	SetItemText(nRow, 1, str2);
}



BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CMyListCtrl 消息处理程序



//双击读取消息
void CMyListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDblClk(nFlags, point);
	int selRow = GetSelectionMark();
	CString fname = GetItemText(selRow, 0);
	if (fname.GetLength() < 1)
	{
		return;
	}
	int r1 = fname.Replace(_T("来自"), _T(""));
	int r2 = fname.Replace(_T("的消息"), _T(""));
	if (r1 != 0 && r2 != 0)
	{
		DeleteItem(selRow);
	}
	else
	{
		fname = GetItemText(selRow, 1);
	}
	::SendMessage(MainForm, CU_OPENMESSAGE, (WPARAM)&fname, NULL);
}
