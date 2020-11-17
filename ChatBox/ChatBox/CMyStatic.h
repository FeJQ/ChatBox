#pragma once


// CMyStatic

class CMyStatic : public CStatic
{
	DECLARE_DYNAMIC(CMyStatic)

public:
	CMyStatic();
	virtual ~CMyStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);

	COLORREF m_crText;			// ������ɫ
	COLORREF m_crBackColor;		// ������ɫ
	HBRUSH	 m_hBrush;			// ��ˢ
	LOGFONT	 m_lf;				// �����С
	CFont	 m_font;			// ����
	bool	m_bTran;			// �Ƿ�͸��
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStnClicked();
	afx_msg void OnMouseLeave();
};


