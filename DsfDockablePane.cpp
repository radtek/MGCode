// DsfDockablePane.cpp : implementation file
//

#include "stdafx.h"
#include "HL.h"
#include "DsfDockablePane.h"


// CDsfDockablePane

IMPLEMENT_DYNAMIC(CDsfDockablePane, CDockablePane)

CDsfDockablePane::CDsfDockablePane()
{

}

CDsfDockablePane::~CDsfDockablePane()
{
}


BEGIN_MESSAGE_MAP(CDsfDockablePane, CDockablePane)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CDsfDockablePane message handlers


void CDsfDockablePane::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	GetClientRect(&rectTree);

//	dc.FillSolidRect(rectTree, ::GetSysColor(COLOR_3DFACE));
	dc.FillSolidRect(rectTree, RGB(213,229,255));
	//dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

BOOL CDsfDockablePane::OnShowControlBarMenu(CPoint point)
{
	return FALSE;
}

void CDsfDockablePane::OnPaneContextMenu(CWnd* pParentFrame,CPoint point)
{
	return;
}