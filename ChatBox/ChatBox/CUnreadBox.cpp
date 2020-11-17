// CUnreadBox.cpp: 实现文件
//

#include "stdafx.h"
#include "CUnreadBox.h"
#include "afxdialogex.h"
#include "resource.h"
#include <string>
#include "Tool.h"
#include "DBManager.h"
using namespace std;

HWND unreadHWND;
// CUnreadBox 对话框

IMPLEMENT_DYNAMIC(CUnreadBox, CDialogEx)

CUnreadBox::CUnreadBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UnreadBox, pParent)
{
	
	
}

CUnreadBox::~CUnreadBox()
{
}
//加载未读消息
void CUnreadBox::loadUnreadMsg()
{
	map<int, vector<string>> result;
	DBManager::GetUnreadMsg(DBManager::GetUserIDByName(CW2A(MyName)), result);
	for (int i = 0; i < result.size(); i++)
	{
		string fname = string();
		if (result[i][6] == string("0")|| result[i][6]==string())
		{
			fname=DBManager::GetUserNameByID(atoi(result[i][1].c_str()));
		}
		else
		{
			fname=DBManager::GetGroupNameByID(atoi(result[i][7].c_str()));
		}
		m_listCtrl_unread.insert(Tool::toCString(fname));
	}
}


void CUnreadBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl_unread);
}


BEGIN_MESSAGE_MAP(CUnreadBox, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(CU_ONLOADUNREAD, &CUnreadBox::OnCuOnloadunread)
	ON_MESSAGE(CU_MESSAGEQUEUE1, &CUnreadBox::OnCuMessagequeue1)
END_MESSAGE_MAP()


//未读消息盒子
BOOL CUnreadBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->SetWindowTextW(_T("未读消息"));
	m_listCtrl_unread.init();

	unreadHWND = this->GetSafeHwnd();
	return TRUE;  
}

//重新绘制
BOOL CUnreadBox::OnEraseBkgnd(CDC* pDC)
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

//添加消息到消息列表
afx_msg LRESULT CUnreadBox::OnCuMessagequeue1(WPARAM wParam, LPARAM lParam)
{
	string *fname = (string*)wParam;
	m_listCtrl_unread.insert(Tool::toCString(*fname));
	delete fname;
	return 0;
}


//加载未读列表
afx_msg LRESULT CUnreadBox::OnCuOnloadunread(WPARAM wParam, LPARAM lParam)
{
	loadUnreadMsg();
	return 0;
}


