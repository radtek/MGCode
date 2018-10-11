#include "stdafx.h"
#include "MFCProertyItem.h"

IMPLEMENT_DYNAMIC(CMFCProertyItem, CMFCPropertyGridProperty)

	CMFCProertyItem::CMFCProertyItem(const CString& strName, 
									const COleVariant& varValue, 
									LPCTSTR lpszDescr, 
									DWORD_PTR dwData,
									LPCTSTR lpszEditMask, 
									LPCTSTR lpszEditTemplate, 
									LPCTSTR lpszValidChars) 
									: CMFCPropertyGridProperty(strName, 
															varValue, 
															lpszDescr, 
															dwData, 
															lpszEditMask, 
															lpszEditTemplate, 
															lpszValidChars), m_pWnd(NULL)
{
}


CMFCProertyItem::~CMFCProertyItem(void)
{
}

BOOL CMFCProertyItem::OnEndEdit()
{
	if (m_pWnd != NULL)
	{
		m_pWnd->SendMessage(WM_PG_ITEMCHANGED, 0, 0);
	}
	return CMFCPropertyGridProperty::OnEndEdit();
}

void CMFCProertyItem::SetOwnerWnd(CWnd *pWnd)
{
	m_pWnd = pWnd;
}