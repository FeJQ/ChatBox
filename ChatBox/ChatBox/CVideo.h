#pragma once


// CVideo 对话框

class CVideo : public CDialogEx
{
	DECLARE_DYNAMIC(CVideo)

public:
	CVideo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CVideo();

	HWND m_myCapWnd;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_video };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
