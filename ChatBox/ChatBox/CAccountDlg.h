#pragma once
#include "CMyTabCtrl.h"

// CAccountDlg 对话框

class CAccountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAccountDlg)

public:
	CAccountDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAccountDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Account };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyTabCtrl m_objAccountTabCtrl;
	virtual BOOL OnInitDialog();
};
