// ExternView.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ExternView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExternView

IMPLEMENT_DYNCREATE(CExternView, CZoomView)

CExternView::CExternView()
{
}

CExternView::~CExternView()
{
}


BEGIN_MESSAGE_MAP(CExternView, CZoomView)
	//{{AFX_MSG_MAP(CExternView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExternView drawing

void CExternView::OnDraw(CDC* pDC)
{
	CExternDoc* pDoc = GetDocument();
	//DrawMeasureLine(pDC);
	XDC *pXDC=PrepareDC(pDC);
	pDoc->DrawList(pXDC);
	pDoc->DrawAlterList(pXDC);
}

/////////////////////////////////////////////////////////////////////////////
// CExternView diagnostics

#ifdef _DEBUG
void CExternView::AssertValid() const
{
	CZoomView::AssertValid();
}

void CExternView::Dump(CDumpContext& dc) const
{
	CZoomView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExternView message handlers

CExternDoc* CExternView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExternDoc)));
	return (CExternDoc*)m_pDocument;
}
