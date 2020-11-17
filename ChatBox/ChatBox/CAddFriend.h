#pragma once


// CAddFriend 对话框

class CAddFriend : public CDialogEx
{
	DECLARE_DYNAMIC(CAddFriend)

public:
	CAddFriend(int mID,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddFriend();
	int m_mID;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_addFriend };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_txtCtrl_userName;
	afx_msg void OnBnClickedadd();
};
