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
	//��ʼ��
	void init();	
	//�����
	void insert(CString str);

	//����Ⱥ��Ա�б�
	void loadGroupMember();
	//��ӳ�Ա
	void addMember(CString str1, CString str2);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


