// XZoomWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "XZoomWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// XZoomWnd

XZoomWnd::XZoomWnd()
{
	m_pFrame = NULL;
	m_wndView = NULL;
}

XZoomWnd::~XZoomWnd()
{
}


BEGIN_MESSAGE_MAP(XZoomWnd, CWnd)
	//{{AFX_MSG_MAP(XZoomWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// XZoomWnd message handlers
int XZoomWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;
	GetClientRect(rect);

	m_pFrame= new CFrameWnd();
	m_pFrame->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rect,this);
	m_pFrame->SetParent(this);

	CRuntimeClass* pViewClass;
	pViewClass =	RUNTIME_CLASS(XZoomView);
	XZoomView* pNewView=(XZoomView*)pViewClass->CreateObject();
	
	pNewView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rect,m_pFrame,ID_ZOOM_VIEW);
	
	if (pNewView != NULL)
	{
		// the new view is there, but invisible and not active...
		pNewView->ShowWindow(SW_SHOW);
		pNewView->OnInitialUpdate();
		pNewView->SetActiveWindow();

 		RecalcLayout();
	}
	m_wndView = pNewView;
	return 0;
}

BOOL XZoomWnd::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void XZoomWnd::RecalcLayout()
{
	CWnd* pWnd = (CWnd*) GetActiveView();
	CRect rect;
	GetClientRect(&rect);
}

XZoomView* XZoomWnd::GetActiveView(void)
{
	return m_wndView;
}

void XZoomWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_pFrame && m_wndView)
	{
		m_pFrame->SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);
		m_wndView->SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);
	}
}

BOOL XZoomWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	return m_wndView->MouseWheel(nFlags, zDelta, pt);
}

void XZoomWnd::SetZoomPage(double x, double y)
{
	if(m_wndView)
		m_wndView->SetViewPage(x,y);
}

void XZoomWnd::CreateZoomEx(CRect &rect, CWnd *pParent, UINT nID, double cx, double cy)
{
	Create(NULL,_T("MINI"),WS_VISIBLE|WS_CHILD,rect,pParent,nID);
	ShowWindow(SW_SHOW);
 	SetZoomPage(cx,cy);
}

BOOL XZoomWnd::DestroyWindow()
{
	m_wndView->DestroyWindow();
	m_pFrame->DestroyWindow();
	return CWnd::DestroyWindow();
}

void XZoomWnd::SetZoomScale(double dbScale)
{
	m_wndView->SetViewScale(dbScale);
}

void XZoomWnd::SetStrokeList(StrokeList* plist)
{
	m_wndView->SetStrokeList(plist);
}

void XZoomWnd::PreviewStart()
{
	m_wndView->PreViewStart();
}

void XZoomWnd::PreviewStop()
{
	m_wndView->PreviewStop();
}

void XZoomWnd::SetPreviewSpeed(double dbSpeed)
{
	m_wndView->SetPreviewSpeed(dbSpeed);
}
