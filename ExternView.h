#if !defined(AFX_EXTERNVIEW_H__A185C80D_043D_4CD5_8579_E667822818E3__INCLUDED_)
#define AFX_EXTERNVIEW_H__A185C80D_043D_4CD5_8579_E667822818E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExternView.h : header file
//

#include "zoomview.h"
#include "ExternDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CExternView view

class CExternView : public CZoomView
{
protected:
	CExternView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CExternView)

// Attributes
public:

// Operations
public:
	CExternDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExternView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CExternView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CExternView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTERNVIEW_H__A185C80D_043D_4CD5_8579_E667822818E3__INCLUDED_)
