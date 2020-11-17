#pragma once
#include "CMyListCtrl.h"

// CGroupChat 对话框

class CGroupChat : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupChat)

public:
	CGroupChat(CString GroupName,CString m_mname,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGroupChat();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GroupChat };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//群聊名称
	CString m_name;
	//我的用户名
	CString m_mname;
	
public:
	virtual BOOL OnInitDialog();
	CMyListCtrl m_listCtrl_groupMember;
	afx_msg void OnBnClickedGroupchatsend();
	CEdit m_txtCtrl_edit;
	CEdit m_txtCtrl_msg;
	afx_msg void OnBnClickedGroupchatclose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
protected:
	afx_msg LRESULT OnCuGrecivemsg(WPARAM wParam, LPARAM lParam);
};
