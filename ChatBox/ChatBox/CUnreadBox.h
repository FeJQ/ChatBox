#pragma once
#define CU_MESSAGEQUEUE WM_USER+30
#include "CMyListCtrl.h"
// CUnreadBox 对话框

extern HWND unreadHWND;
class CUnreadBox : public CDialogEx
{
	DECLARE_DYNAMIC(CUnreadBox)

public:
	CUnreadBox(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CUnreadBox();
	//加载未读消息
	void loadUnreadMsg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UnreadBox };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
public:
	CMyListCtrl m_listCtrl_unread;
protected:
	afx_msg LRESULT OnCuOnloadunread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCuMessagequeue1(WPARAM wParam, LPARAM lParam);
};
