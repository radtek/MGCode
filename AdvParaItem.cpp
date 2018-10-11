#include "stdafx.h"
#include "AdvParaItem.h"
#include "resource.h"

#define AFX_PROP_HAS_BUTTON 0x0002

IMPLEMENT_DYNAMIC(CAdvParaItem, CMFCPropertyGridProperty)

CAdvParaItem::CAdvParaItem(const CString& strName, LPCTSTR lpszDescr)
: CMFCPropertyGridProperty(strName, COleVariant((LPCTSTR)"=>"), lpszDescr)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
	m_bAllowEdit = FALSE;
	m_pShowAdvParaDlg = NULL;
}

CAdvParaItem::~CAdvParaItem(void)
{
}

void CAdvParaItem::OnClickButton(CPoint point)
{
	if (!(g_CurUserInfo.dwRight1 & 1024))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	m_bButtonIsDown = TRUE;
	typedef UINT (WINAPI *HS_SHOWDLG)(LPVOID pInfo, int nSize);
	if (m_pShowAdvParaDlg)
	{
		if (((HS_SHOWDLG)m_pShowAdvParaDlg)(&m_para, sizeof(PARA)) == IDOK)
		{
			m_pWndList->OnPropertyChanged(this);
			m_bButtonIsDown = FALSE;
			return;
		}
		else
		{
			m_bButtonIsDown = FALSE;
			return;
		}
	}
	else
	{
		AfxMessageBox("Load Adv Para Dialog Failed");
		m_bButtonIsDown = FALSE;
		return;
	}
}

void CAdvParaItem::SetFunction(LPVOID pFunc)
{
	m_pShowAdvParaDlg = pFunc;
}

void CAdvParaItem::SetAdvPara(PARA *para)
{
	m_para = *para;
}

void CAdvParaItem::GetAdvPara(PARA *para)
{
	*para = m_para;
}