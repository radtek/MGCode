#if !defined(AFX_COLORBOX_H__AF922EAF_3668_11D6_81B4_00E04C538386__INCLUDED_)
#define AFX_COLORBOX_H__AF922EAF_3668_11D6_81B4_00E04C538386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBox.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CColorBox window

class CColorBox : public CWnd
{
// Construction
public:
	CColorBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nCurBox;
	void ShowWin();
	BOOL m_bShow;
	BOOL CreateColorBox(int x,int y,int cx,int cy,CWnd* pParentWnd);
	virtual ~CColorBox();
	COLORREF GetColor(int iIndex);

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRect m_CurRect;
	CRect m_RectList[105];
	CWnd* m_pParentWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBOX_H__AF922EAF_3668_11D6_81B4_00E04C538386__INCLUDED_)
