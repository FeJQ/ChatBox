// CMyStatic.cpp: 实现文件
//

#include "stdafx.h"
#include "CMyStatic.h"
#include "resource.h"
#include "CUnreadBox.h"

// CMyStatic

IMPLEMENT_DYNAMIC(CMyStatic, CStatic)


CUnreadBox unrDlg;

CMyStatic::CMyStatic()
{
	m_crBackColor = RGB(255, 255, 255);	
	unrDlg.Create(IDD_UnreadBox, this);
}

CMyStatic::~CMyStatic()
{
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(STN_CLICKED, &CMyStatic::OnStnClicked)
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CMyStatic 消息处理程序



bool isCreated = FALSE;

void CMyStatic::OnMouseHover(UINT nFlags, CPoint point)
{
	
}





//鼠标移入
void CMyStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT   tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = HOVER_DEFAULT;
	tme.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tme);

	__super::OnMouseMove(nFlags, point);

	m_crBackColor = RGB(230, 230, 230);
	RedrawWindow();
	CStatic::OnMouseMove(nFlags, point);
}
//鼠标移出
void CMyStatic::OnMouseLeave()
{
	m_crBackColor = RGB(255, 255, 255);
	RedrawWindow();	
	CStatic::OnMouseLeave();
}


//点击
void CMyStatic::OnStnClicked()
{	
	if (isCreated == TRUE)
	{
		isCreated = FALSE;
		//unrDlg.SendMessage(WM_CLOSE);
		unrDlg.ShowWindow(SW_HIDE);
	}
	else
	{
		CRect rectDlg;
		GetWindowRect(rectDlg);
		unrDlg.MoveWindow(rectDlg.left - 380, rectDlg.top - 235, 357, 235);
		isCreated = TRUE;
		unrDlg.ShowWindow(SW_SHOW);
		unrDlg.loadUnreadMsg();
	}
}




