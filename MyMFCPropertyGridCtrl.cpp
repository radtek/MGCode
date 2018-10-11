// MyMFCPropertyGridCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "MyMFCPropertyGridCtrl.h"


// CMyMFCPropertyGridCtrl

IMPLEMENT_DYNAMIC(CMyMFCPropertyGridCtrl, CMFCPropertyGridCtrl)

CMyMFCPropertyGridCtrl::CMyMFCPropertyGridCtrl()
{
	m_nMyLeftColumnWidth = 0;
}

CMyMFCPropertyGridCtrl::~CMyMFCPropertyGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyMFCPropertyGridCtrl, CMFCPropertyGridCtrl)
//	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMyMFCPropertyGridCtrl 消息处理程序

void CMyMFCPropertyGridCtrl::AdjustLayout()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_nMyLeftColumnWidth)
		m_nLeftColumnWidth = m_nMyLeftColumnWidth;

	return CMFCPropertyGridCtrl::AdjustLayout();
}
