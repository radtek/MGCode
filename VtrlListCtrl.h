#if !defined(AFX_VTRLLISTCTRL_H__9F8BE06E_8C6B_44FF_B13A_8D6C21FB082A__INCLUDED_)
#define AFX_VTRLLISTCTRL_H__9F8BE06E_8C6B_44FF_B13A_8D6C21FB082A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VtrlListCtrl.h : header file
//

#include <afxtempl.h>
#include "DsfListCtrl.h"

typedef struct DsfItemData 
{
	UINT m_nType;
	LPVOID m_pData;
}DsfItemData;

enum DsfItemDataType
{
	DIDT_TREE,
	DIDT_DSFOBJ
};

/////////////////////////////////////////////////////////////////////////////
// CVtrlListCtrl window
typedef struct VtrlItem
{
	int iImage;
	int iKey;
	DsfItemData itemData;
}VtrlItem;

class CVtrlListCtrl : public CDsfListCtrl
{
// Construction
public:
	CVtrlListCtrl();

private:
	CArray<VtrlItem, VtrlItem&> m_Items;
	CMap<int, int, CString, const CString&> m_keyStrings;
	DWORD m_dwItemCount;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVtrlListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBackColor(COLORREF crColor);
	void UpdateListCtrl();
	BOOL DeleteAllItems();
	BOOL DeleteItem(int nItem);
	int InsertItem(int nItem, int iKey, int iImage);
	void ScrollToItem(int index);
	BOOL GetStringAt(int iKey, CString &str);
	void SetStringAt(int iKey, const CString &str);
	int GetItemCount() const;
	const DsfItemData &GetItemData(int nItem) const;
	void SetItemData(int nItem, const DsfItemData &itemData);
	virtual ~CVtrlListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVtrlListCtrl)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);  
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VTRLLISTCTRL_H__9F8BE06E_8C6B_44FF_B13A_8D6C21FB082A__INCLUDED_)
