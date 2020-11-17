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

	COLORREF m_crText;			// 字体颜色
	COLORREF m_crBackColor;		// 背景颜色
	HBRUSH	 m_hBrush;			// 画刷
	LOGFONT	 m_lf;				// 字体大小
	CFont	 m_font;			// 字体
	bool	m_bTran;			// 是否透明
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStnClicked();
	afx_msg void OnMouseLeave();
};


