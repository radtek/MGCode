// MyMFCPropertyGridCtrl.cpp : ʵ���ļ�
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


// CMyMFCPropertyGridCtrl ��Ϣ�������

void CMyMFCPropertyGridCtrl::AdjustLayout()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_nMyLeftColumnWidth)
		m_nLeftColumnWidth = m_nMyLeftColumnWidth;

	return CMFCPropertyGridCtrl::AdjustLayout();
}
