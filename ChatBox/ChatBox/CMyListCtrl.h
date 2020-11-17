#pragma once


// CMyListCtrl
extern HWND  MainForm;
extern CString MyName;
extern int fID;
extern int mID;
class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
	//初始化
	void init();	
	//添加行
	void insert(CString str);

	//加载群成员列表
	void loadGroupMember();
	//添加成员
	void addMember(CString str1, CString str2);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


