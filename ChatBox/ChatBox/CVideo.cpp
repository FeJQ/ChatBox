// CVideo.cpp: 实现文件
//

#include "stdafx.h"
#include "CVideo.h"
#include "afxdialogex.h"
#include "resource.h"
// CVideo 对话框

IMPLEMENT_DYNAMIC(CVideo, CDialogEx)

CVideo::CVideo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_video, pParent)
{

}

CVideo::~CVideo()
{
}

void CVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideo, CDialogEx)
END_MESSAGE_MAP()


// CVideo 消息处理程序


BOOL CVideo::OnInitDialog()
{


	return TRUE;  
				  
}
