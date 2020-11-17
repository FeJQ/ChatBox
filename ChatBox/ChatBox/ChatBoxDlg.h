
// ChatBoxDlg.h: 头文件
//

#pragma once
#include "CMyTreeCtrl.h"
#include "CMyStatic.h"
#include <map>
using namespace std;

// CChatBoxDlg 对话框
class CChatBoxDlg : public CDialogEx
{
	// 构造
public:
	CChatBoxDlg(char* userName, CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Main };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	//用户名
	char m_userName[16];
	//未读消息数
	int m_unreadCount;
	//已打开的单聊窗口
	map<string,HWND> m_fWins;
	//已打开的群聊窗口
	map<string, HWND> m_gWins;

public:
	CMyTreeCtrl m_treeCtrl_mainlist;
	afx_msg void OnDblclkMainTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CMyStatic m_labCtrl_unread;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_labCtrl_Name;

	//刷新未读消息数量
	void ReloadUnreadCount();
protected:
	afx_msg LRESULT OnCuMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCuClosewin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCuOpenmessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedMainaddfriend();
};
