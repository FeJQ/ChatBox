#pragma once

// CMyTreeCtrl

class CMyTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMyTreeCtrl)

public:
	CMyTreeCtrl();
	virtual ~CMyTreeCtrl();

	//��ʼ��treeCtrl
	void initTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


