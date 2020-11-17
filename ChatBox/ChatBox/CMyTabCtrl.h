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

	//Summary:��ʼ��TabCtrl
	//Parameter count:�ɱ��������
	//Parameter ...�ɱ����((TCHAR*)tab����,(CDialogEx*)tab���ڶ���,(DWORD)����ID)
	void initTabCtrl(int count, ...);
private:
	vector<CDialogEx*> CDlgs;
protected:
	DECLARE_MESSAGE_MAP()
public:
	//�л�ѡ�
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


