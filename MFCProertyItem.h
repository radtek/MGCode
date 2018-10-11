#pragma once

#include "afxpropertygridctrl.h"

#define WM_PG_ITEMCHANGED WM_USER+486

class CMFCProertyItem :
	public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CMFCProertyItem)
public:
	CMFCProertyItem(const CString& strName, 
		const COleVariant& varValue, 
		LPCTSTR lpszDescr = NULL, 
		DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, 
		LPCTSTR lpszEditTemplate = NULL, 
		LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCProertyItem(void);
	virtual BOOL OnEndEdit();
	void SetOwnerWnd(CWnd *pWnd);
protected:
	CWnd *m_pWnd;
};

