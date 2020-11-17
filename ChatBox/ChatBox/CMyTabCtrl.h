#pragma once
#include <vector>
using namespace std;

// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

	//Summary:初始化TabCtrl
	//Parameter count:可变参数数量
	//Parameter ...可变参数((TCHAR*)tab标题,(CDialogEx*)tab窗口对象,(DWORD)窗口ID)
	void initTabCtrl(int count, ...);
private:
	vector<CDialogEx*> CDlgs;
protected:
	DECLARE_MESSAGE_MAP()
public:
	//切换选项卡
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


