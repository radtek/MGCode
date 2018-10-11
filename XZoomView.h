#if !defined(AFX_XZOOMVIEW_H__58E18853_BEBD_453F_A413_CD581C4C0AAE__INCLUDED_)
#define AFX_XZOOMVIEW_H__58E18853_BEBD_453F_A413_CD581C4C0AAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XZoomView.h : header file
//
#include "zoomview.h"

/////////////////////////////////////////////////////////////////////////////
// XZoomView view

class XZoomView : public CZoomView
{
protected:
	XZoomView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(XZoomView)

// Attributes
public:
	CRITICAL_SECTION m_cri;
	double m_dbPreviewSpeed;
	BOOL m_bRun;
	double m_dbViewX;
	double m_dbViewY;
	StrokeList m_strokelist;
	CWinThread* m_pThread;
// Operations
public:
	void CalCurPos(CStroke* pStroke);
	void DrawBall();
	void SetPreviewSpeed(double dbSpeed);
	void PreviewStop();
	void PreViewStart();
	void SetStrokeList(StrokeList* plist);
	void SetViewPage(double x,double y);
	void ZoomPage();
	BOOL MouseWheel(UINT nFlags, short zDelta, CPoint pt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XZoomView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~XZoomView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(XZoomView)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawCurBall(double x,double y,COLORREF col,int  iCount);
	void DrawBox(XDC* pDC);
	void Lock();
	void UnLock();

public:
	void DrawArray(CQuad &quad, CArray<OBJPOS, OBJPOS> &dtArray, CArray<SELQUAD, SELQUAD> &QuadList);
	void UpdateDrawArray(CQuad &quad, CArray<OBJPOS, OBJPOS> &dtArray, CArray<SELQUAD, SELQUAD> &QuadList);
	void DelAllStrk();

	//视图位置
	COLORREF m_viewcolor;
	double m_viewx;
	double m_viewy;
	//实际
	COLORREF m_color;	//中间球的色彩
	double m_curxpos;	//球的位置
	double m_curypos;
	int m_niIndex;		//定时器次数
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XZOOMVIEW_H__58E18853_BEBD_453F_A413_CD581C4C0AAE__INCLUDED_)
