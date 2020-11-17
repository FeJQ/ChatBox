// CRegisterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CRegisterDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "Communication.h"
#include "DBManager.h"

// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Register, pParent)
	, m_txtVal_UserName(_T(""))
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Account_Register_EditNick, m_txtCtrl_userName);
	DDX_Control(pDX, IDC_IDC_Account_Register_EditPwd1, m_txtCtrl_password1);
	DDX_Control(pDX, IDC_IDC_Account_Register_EditPwd2, m_txtCtrl_password2);
	DDX_Text(pDX, IDC_Account_Register_EditNick, m_txtVal_UserName);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Register_btnReg, &CRegisterDlg::OnBnClickedRegisterbtnreg)
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序


//注册
void CRegisterDlg::OnBnClickedRegisterbtnreg()
{
	CString CSname, CSpwd1, CSpwd2;
	m_txtCtrl_userName.GetWindowTextW(CSname);
	m_txtCtrl_password1.GetWindowTextW(CSpwd1);
	m_txtCtrl_password2.GetWindowTextW(CSpwd2);

	char name[32], pwd1[32], pwd2[32];

	strcpy_s(name, _countof(name), CW2A(CSname));
	strcpy_s(pwd1, _countof(name), CW2A(CSpwd1));
	strcpy_s(pwd2, _countof(name), CW2A(CSpwd2));


	int lenName, lenPwd1;
	lenName = strlen(name);
	lenPwd1 = strlen(pwd1);

	if (lenName < 4 || lenName > 16)
	{
		MessageBox(_T("用户名大小应在4-16位之间"), _T("错误"), MB_ICONERROR);
		return;
	}
	if (lenPwd1 < 6 || lenPwd1 > 16)
	{
		MessageBox(_T("密码长度应在8-16位之间!"), _T("错误"), MB_ICONERROR);
		return;
	}
	if (strcmp(pwd1, pwd2))
	{
		MessageBox(_T("两次输入密码不匹配!"), _T("错误"), MB_ICONERROR);
		return;
	}	
	 int result = DBManager::Register(name, pwd1);

	switch (result)
	{
	case 0:MessageBox(_T("注册失败!"), _T("提示"), MB_ICONERROR); break;
	case 1:MessageBox(_T("注册成功!"), _T("提示")); break;
	case 2:MessageBox(_T("用户名已存在!"), _T("提示"), MB_ICONWARNING); break;
	default:break;
	}
}

