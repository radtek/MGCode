#if !defined(AFX_COLORBUTTON_H__816EC421_3672_11D6_81B4_00E04C538386__INCLUDED_)
#define AFX_COLORBUTTON_H__816EC421_3672_11D6_81B4_00E04C538386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorButton window
#include "ColorBox.h"

class CColorButton : public CButton
{
// Construction
public:
	CColorButton();

// Attributes
public:
	CColorBox m_CBox;
	BOOL Create(DWORD dwStyle, const RECT& rect,CWnd* pParentWnd, UINT nID);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void PostMes();
	COLORREF GetColor();
	void ShowWin();
	int GetColorBox();
	virtual ~CColorButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__816EC421_3672_11D6_81B4_00E04C538386__INCLUDED_)
