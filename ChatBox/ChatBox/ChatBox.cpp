
// ChatBox.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ChatBox.h"
#include "ChatBoxDlg.h"
#include "CAccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatBoxApp

BEGIN_MESSAGE_MAP(CChatBoxApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChatBoxApp 构造

CChatBoxApp::CChatBoxApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CChatBoxApp 对象

CChatBoxApp theApp;


// CChatBoxApp 初始化

BOOL CChatBoxApp::InitInstance()
{
	CWinApp::InitInstance();


	CAccountDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();


#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

