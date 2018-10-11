#if !defined(AFX_COLORSTATIC_H__F3A55F0A_3BB3_4841_AF6D_9FF37C52754C__INCLUDED_)
#define AFX_COLORSTATIC_H__F3A55F0A_3BB3_4841_AF6D_9FF37C52754C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : header file
//
#include "ColorBox.h"

#define NUM_COR 24


/////////////////////////////////////////////////////////////////////////////
// CColorStatic window

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();

// Attributes
public:

// Operations
public:
	BOOL Create(int x,int y,int cx,int cy,CWnd* pParentWnd);
	void SelectColorOk();
	void OnLButtonUp(UINT nFlags, CPoint point);
	void AddColor(COLORREF col);
	void ClearColor();
	int GetCurSel();
	void SetCurSel(int index);
	COLORREF GetSelColor();
	int OnLButtonDown(UINT nFlags, CPoint point);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorStatic();

	// Generated message map functions
protected:
	HICON m_hUpCon;
	HICON m_hDownCon;
	CButton m_btnUp;
	CButton m_btnChg;
	CButton m_btnAdd;
	CButton m_btnSub;
	CButton m_btnDown;

	CRect m_rtCol;
	CRect m_rtCur;

	int m_nHeight;
	int m_nWidth;
	int m_iCurCol;
	int m_iStartcol;
	COLORREF GetColor(int iIndex);
	CList<COLORREF, COLORREF> m_lstColor;

	CColorBox m_CBox;
	int ItemFromPoint (CPoint pt);
	
	//{{AFX_MSG(CColorStatic)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnChgLayer ();
	afx_msg void OnAdd ();
	afx_msg void OnSub ();
	afx_msg void OnButtonCtrl(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATIC_H__F3A55F0A_3BB3_4841_AF6D_9FF37C52754C__INCLUDED_)
