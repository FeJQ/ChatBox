// CAccountDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CAccountDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CLoginDlg.h"
#include "CRegisterDlg.h"
#include "Communication.h"
#include <tchar.h>
// CAccountDlg 对话框

IMPLEMENT_DYNAMIC(CAccountDlg, CDialogEx)

CAccountDlg::CAccountDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Account, pParent)
{

}

CAccountDlg::~CAccountDlg()
{
}

void CAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Account_tab, m_objAccountTabCtrl);
}


BEGIN_MESSAGE_MAP(CAccountDlg, CDialogEx)
END_MESSAGE_MAP()


// CAccountDlg 消息处理程序



BOOL CAccountDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_objAccountTabCtrl.initTabCtrl(2,
		_T("登录"), new CLoginDlg(), IDD_Login,
		_T("注册"), new CRegisterDlg(), IDD_Register
	);
	
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
