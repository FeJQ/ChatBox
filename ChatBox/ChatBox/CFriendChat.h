#pragma once
// CFriendChat 对话框

class CFriendChat : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendChat)

public:
	CFriendChat(CString _fname, CString _mname, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFriendChat();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FriendChat };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	//好友用户名
	CString m_fname;
	//我的用户名
	CString m_mname;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFriendchatsend();
	virtual void OnCancel();
	afx_msg void OnBnClickedFriendchatclose();
	CEdit m_txtCtrl_edit;
	CEdit m_txtCtrl_msg;
protected:
	afx_msg LRESULT OnCuRecivemsg(WPARAM wParam, LPARAM lParam);
public:
	CString m_txtVal_msg;
	CString m_txtVal_edit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedshowvideo();
};
