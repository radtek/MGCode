// BitmapClient.cpp : implementation file
//

#include "stdafx.h"
#include "HL.h"
#include "BitmapClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapClient

CBitmapClient::CBitmapClient()
{
	VERIFY(m_bmp.LoadBitmap(IDB_HANS_LASER));
}

CBitmapClient::~CBitmapClient()
{
}


BEGIN_MESSAGE_MAP(CBitmapClient, CWnd)
	//{{AFX_MSG_MAP(CBitmapClient)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBitmapClient message handlers

void CBitmapClient::OnSize(UINT nType, int cx, int cy) 
{
	// TODO: Add your message handler code here
	CWnd::OnSize(nType, cx, cy);
	RedrawWindow(NULL,NULL,
	RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	return;
}

BOOL CBitmapClient::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnEraseBkgnd(pDC);

	BITMAP bm;
	CDC dcMem;

	VERIFY(m_bmp.GetObject(sizeof(bm),(LPVOID)&bm));
	
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBMP=(CBitmap *)dcMem.SelectObject(&m_bmp);

	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt((rect.right-bm.bmWidth)/2,(rect.bottom-bm.bmHeight)/2,
		bm.bmWidth,bm.bmHeight,&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBMP);
	return TRUE;
}
