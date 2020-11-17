// CLoginDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "DBManager.h"
#include "ChatBoxDlg.h"
#include "Communication.h"

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Login, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Login_EditID, m_txtCtrl_userName);
	DDX_Control(pDX, IDC_Login_EditPwd, m_txtCtrl_password);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Login_btnLogin, &CLoginDlg::OnBnClickedLoginbtnlogin)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->m_txtCtrl_userName.SetWindowTextW(_T("admin"));
	this->m_txtCtrl_password.SetWindowTextW(_T("123456"));

	return TRUE;

}


//登陆
void CLoginDlg::OnBnClickedLoginbtnlogin()
{
	CString CSname, CSpwd;
	m_txtCtrl_userName.GetWindowTextW(CSname);
	m_txtCtrl_password.GetWindowTextW(CSpwd);

	char name[17], pwd[17];

	strcpy(name, CW2A(CSname));
	strcpy(pwd, CW2A(CSpwd));

	//验证账号密码
	int result = DBManager::Login(name, pwd);
	switch (result)
	{
	case 0:MessageBox(_T("用户名或密码错误!"), _T("提示"), MB_ICONERROR); break;
	case 1:
	{
		//连接服务器
		Communication::ConnectToServer();
		result = Communication::MySendMessage(&MessagePack(PACK::PACK_Login, name, NULL));
		if (result <= 0)
		{
			MessageBox(_T("服务器未响应,请稍候重试!"), _T("提示"), MB_ICONERROR);
			return;
		}
		//登陆成功
		GetParent()->GetParent()->SendMessage(WM_CLOSE);
		CChatBoxDlg cbdlg(name);
		cbdlg.DoModal();

	}
	default:
		break;
	}
}



