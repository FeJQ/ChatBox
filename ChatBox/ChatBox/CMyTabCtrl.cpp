// CMyTabCtrl.cpp: 实现文件
//

#include "stdafx.h"
#include "CMyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}
//Summary:初始化TabCtrl
//Parameter count:可变参数数量
//Parameter ...可变参数((TCHAR*)tab标题,(CDialogEx*)tab窗口对象,(DWORD)窗口ID)
void CMyTabCtrl::initTabCtrl(int count, ...)
{
	this->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	va_list ap;
	va_start(ap, count);
	for (int i = 0; i < count; i++)
	{
		//接收参数
		TCHAR *pTitle = va_arg(ap, TCHAR*);
		CDialogEx *pCDlg = va_arg(ap, CDialogEx *);
		DWORD ID = va_arg(ap, DWORD);

		//添加选项卡
		this->InsertItem(i, pTitle);

		//创建选项卡窗体
		pCDlg->Create(ID, this);

		//移动选项卡至合适位置
		CRect rect;
		GetClientRect(rect);
		rect.DeflateRect(1, 23, 1, 1);
		pCDlg->MoveWindow(rect, 1);

		//添加对话框到管理池
		CDlgs.push_back(pCDlg);
	}
	CDlgs[0]->ShowWindow(SW_SHOW);
	va_end(ap);
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序



//Summary:切换选项卡
void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//获取当前选中选项卡的索引
	int index = GetCurSel();
	for (int i = 0; i < CDlgs.size(); i++)
	{
		if (i == index)
		{
			CDlgs[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			CDlgs[i]->ShowWindow(SW_HIDE);
		}
	}
}
