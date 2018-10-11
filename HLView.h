// HLView.h : interface of the CHLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLVIEW_H__B8BC1851_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_HLVIEW_H__B8BC1851_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Zoomview.h"
#include "HLDoc.h"
#include "XDC.h"
#include "XTracker.h"
#include "Quad.h"	// Added by ClassView
#include "CMarkTicps.h"


//鼠标拖动旋转光标类型  //zg add
#define MOUSE_ROTATE  6
#define MOUSE_LR_DRAG 7
#define MOUSE_TB_DRAG 8

#define ID_MOUSE_ROTATE 6500
#define ID_SORT_OBJECT_LIST (WM_USER + 20)
#define ID_ASORT_OBJECT_LIST (WM_USER + 21)
#define ID_G_OBJECT_LIST (WM_USER + 22)
#define ID_SEL_OBJECT_LIST (WM_USER + 23)

class CHLView : public CZoomView
{
protected: // create from serialization only
	CHLView();
	DECLARE_DYNCREATE(CHLView)

// Attributes
public:
	CHLDoc* GetDocument();
// Operations
public:
	virtual void NotifyZoom(void);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHLView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
//	BOOL OnEraseBkgnd(CDC* pDC);
	void JoinPoint();
	void SplitPoint(double dbPoint);
	void AddPoint(CPoint &point);
	void OnChangeconvert();
	void DrawRect();
	BOOL HitOtherObject(CPoint &point);
	void DelDot();
	void ReplaceDot();
	void DrawRotateTrack(XDC *pDC);
	BOOL IsBitSigned(unsigned short n, unsigned int nBit);
	void DrawIcon(CDC *pDC, int x, int y, unsigned short *pIconMap);
	void RotatePreviewStrokeList(STROKELIST *pStrokeList,CDot CenterPoint, double dbAngle);
	void EmptyStroke(STROKELIST *pStrokeList);
	void PreviewStroke(STROKELIST *pStrokeList);
	void InitIcon();
	void PreviewRotateTracker(CQuad rect);
	double CalAngle(double cx, double cy);
	void TrackRotate(CPoint OrgPoint,BOOL bShift,BOOL bControl);
	void DrawBox(XDC *pDC);

	void DrawMark(CDC* pDC);
	void SortPro();
	void AutoSortPro();

	void AutoSortDraw(CDC *pDc);

	void SetGroupNum(int nNum);
	void ClearMark();

	void AlignBottomCmd();
	void AlignTopCmd();
	void AlignRightCmd();
	void AlignLeftCmd();
	void DistanceYCmd();
	void DistanceXCmd();
	void EqualWhCmd();
	void EqualHeightCmd();
	void EqualWidthCmd();

	void ShowSpecSet(BOOL bShow);
	void ShowTextInfo();
	void ShowBarInfo();

	virtual ~CHLView();
	int m_nResult;
	XTracker m_track;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:

	//{{AFX_MSG(CHLView)
	afx_msg void OnUpdatePos(CCmdUI *pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomPage();
	afx_msg void OnDrawClick();
	afx_msg void OnUpdateDrawClick(CCmdUI* pCmdUI);
	afx_msg void OnDrawHand();
	afx_msg void OnUpdateDrawHand(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawRectangle();
	afx_msg void OnUpdateDrawRectangle(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnZoomSelect();
	afx_msg void OnZoomAll();
	afx_msg void OnDrawText();
	afx_msg void OnUpdateDrawText(CCmdUI* pCmdUI);
	afx_msg void OnDrawEllipse();
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFileImport();
    afx_msg void OnLoadObjList();
	afx_msg void OnDrawGrid();
	afx_msg void OnUpdateDrawGrid(CCmdUI* pCmdUI);
	afx_msg void OnDrawPolygon();
	afx_msg void OnUpdateDrawPolygon(CCmdUI* pCmdUI);
	afx_msg void OnDrawCodebar();
	afx_msg void OnUpdateDrawCodebar(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHelppath();
	afx_msg void OnZoomoff();
	afx_msg void OnZoomoper();
	afx_msg void OnUpdateZoomoff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomoper(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDrawDrag();
	afx_msg void OnUpdateDrawDrag(CCmdUI* pCmdUI);
	afx_msg void OnMoveaccu();
	afx_msg void OnConvert();
	afx_msg void OnDeldot();
	afx_msg void OnJoindot();
	afx_msg void OnSplitdot();
    afx_msg void OnSelAllStroke();
    afx_msg void OnSetCurDotToStart();
    afx_msg void OnFilterPoint();
    afx_msg void OnReverseStroke();
    afx_msg void OnFilterCurDocPoint();
    afx_msg void OnKillAngle();
    afx_msg void OnSortObj();
    afx_msg void OnUpdateSortObj(CCmdUI* pCmdUI);
	afx_msg void OnAutoSortObj();
    afx_msg void OnUpdateAutoSortObj(CCmdUI* pCmdUI);
    afx_msg void OnGSortObj();
    afx_msg void OnUpdateGSortObj(CCmdUI* pCmdUI);
	afx_msg void OnSelSortObj();
    afx_msg void OnUpdateSelSortObj(CCmdUI* pCmdUI);
	afx_msg void OnAlignBottom();
	afx_msg void OnAlignLeft();
	afx_msg void OnAlignRight();
	afx_msg void OnAlignTop();
	afx_msg void OnDistanceX();
	afx_msg void OnDistanceY();
	afx_msg void OnEqualHeight();
	afx_msg void OnEqualWidth();
	afx_msg void OnEqualWh();
    afx_msg void OnRightMng();
    afx_msg void OnChangeUser();
	afx_msg void OnDrawDot();
	afx_msg void OnUpdateDrawDot(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void InvalidateSort();
	void DispSelObjProp();
	CString m_strAutoSortInfo;
	UINT m_nDrawTool;			//当前的绘图工具

	static CView *m_pActiveView;

private:
	void DrawTracker(XDC *pDC);
//	CTracker m_tracker;           // 跟踪器
	HCURSOR m_hCursorCur;         // 当前光标
	HCURSOR m_hCursorClick;	      // 点取光标
	HCURSOR m_hCursorShape;		  // 结点光标
	HCURSOR m_hCursorCross;		  // 十字线光标
	HCURSOR m_hCursorCrossRot;	  //
	HCURSOR m_hCursorDoxMove;     // 画布移动
	HCURSOR m_hCursorZoomIn;      // 局部放大
    HCURSOR m_hCursorMLineH;      // 局部放大
    HCURSOR m_hCursorMLineV;      // 局部放大
	HCURSOR m_hCursorRotateDrag;
	HCURSOR m_hCursorTBDrag;
	HCURSOR m_hCursorLRDrag;

	HCURSOR m_hCursorDot;  

	int     m_nRotateDragType;
	BOOL    m_bRotate;
	CPoint  m_ptCur;			  // 当前位置
	HCURSOR m_hDrag;

    CStroke m_RotateRectStroke; 
	unsigned short m_DragRotateIcon[8][14]; 
	CBitmap m_LTBitmap;  
	CBitmap m_RTBitmap;
	CBitmap m_RBBitmap;
	CBitmap m_LBBitmap;
	CBitmap m_TBBitmap;
	CBitmap m_LRBitmap;

	CPoint m_ptSave;

private:
	CQuad m_quadDown;
	BOOL  m_bLBDown;
	BOOL m_bActive;
	

// 	CDC* m_pDC;
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnClose();
//	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void MarkTicps(BOOL bShow,BOOL bMarkTicps = TRUE);
	afx_msg void OnJointstrokeCurdoc();
};

#ifndef _DEBUG  // debug version in HLView.cpp
inline CHLDoc* CHLView::GetDocument()
   { return (CHLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HLVIEW_H__B8BC1851_DE24_11D3_845D_444553540001__INCLUDED_)
