#if !defined(AFX_XZOOMWND_H__670360F7_6EBF_4BA0_BFD3_9A952F33CBB8__INCLUDED_)
#define AFX_XZOOMWND_H__670360F7_6EBF_4BA0_BFD3_9A952F33CBB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XZoomWnd.h : header file
//
#include "XZoomView.h"

/////////////////////////////////////////////////////////////////////////////
// XZoomWnd window

class XZoomWnd : public CWnd
{
// Construction
public:
	XZoomWnd();

// Attributes
public:
	XZoomView* m_wndView;
	CFrameWnd* m_pFrame;

// Operations
public:
	void XZoomWnd::RecalcLayout();
	XZoomView* GetActiveView(void);
	virtual BOOL DestroyWindow();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XZoomWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPreviewSpeed(double dbSpeed);
	void PreviewStop();
	void PreviewStart();
	void SetStrokeList(StrokeList* plist);
	void SetZoomScale(double dbScale);
	void CreateZoomEx(CRect& rect,CWnd* pParent,UINT nID,double cx,double cy);
	void SetZoomPage(double x,double y);
	virtual ~XZoomWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(XZoomWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XZOOMWND_H__670360F7_6EBF_4BA0_BFD3_9A952F33CBB8__INCLUDED_)
