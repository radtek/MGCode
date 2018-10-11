// ColorBox.cpp : implementation file
//

#include "stdafx.h"
#include "ColorBox.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBox

CColorBox::CColorBox()
{
	m_bShow=FALSE;
	CRect r;

	for(int i=0;i<15;i++)
		for(int j=0;j<7;j++)
		{
			r.left = j*15+5;
			r.right = j*15+15;
			r.top = i*15+5;
			r.bottom = i*15+15;
			m_RectList[i*7+j]=r;
		}

	r = m_RectList[0];
	r.left-=2;
	r.top-=2;
	r.right+=2;
	r.bottom+=2;
	m_CurRect=r;

	m_nCurBox=0;
}

CColorBox::~CColorBox()
{
}


BEGIN_MESSAGE_MAP(CColorBox, CWnd)
	//{{AFX_MSG_MAP(CColorBox)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColorBox message handlers

BOOL CColorBox::CreateColorBox(int x, int y, int cx, int cy, CWnd *pParentWnd)
{
	if(pParentWnd == NULL)
		return FALSE;

	CRect rect(x,y,x+cx,y+cy);
	CWnd::CreateEx(0,NULL,NULL,WS_CHILD|WS_POPUP|WS_BORDER, rect.left, rect.top, 
					rect.Width(), rect.Height(), pParentWnd->GetSafeHwnd(),0);

	m_pParentWnd = pParentWnd;
	return TRUE;
}

BOOL CColorBox::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect,r;
	GetClientRect(&rect);

	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->SelectStockObject(WHITE_PEN);
	pDC->Rectangle(&rect);

	for(int i=0;i<105;i++)
	{
		CBrush brush(GetColor(i));
		pDC->FillRect(&m_RectList[i],&brush);
		brush.DeleteObject();
	}

	pDC->SelectStockObject(BLACK_PEN);
	pDC->SelectStockObject(NULL_BRUSH);

	pDC->Rectangle(&m_CurRect);

	return TRUE;
}

void CColorBox::ShowWin()
{
	m_bShow=TRUE;

	if(m_bShow){
		::ShowWindow(this->GetSafeHwnd(),SW_SHOW);
	}
	else{
		::ShowWindow(this->GetSafeHwnd(),SW_HIDE);
	}

	Invalidate(FALSE);
	UpdateWindow();
}

void CColorBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bShow=FALSE;
	ShowWindow(SW_HIDE);

	CColorStatic* pParent = (CColorStatic*)m_pParentWnd;
	pParent->SelectColorOk();
}

void CColorBox::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

	if (m_bShow)
	{
		ShowWindow(SW_HIDE);
		CColorStatic* pParent = (CColorStatic*)m_pParentWnd;
		pParent->SelectColorOk();
	}
	m_bShow=FALSE;
}

COLORREF CColorBox::GetColor(int iIndex)
{
	
	if(iIndex>=0&&iIndex<=16)
	{
		switch(iIndex)
		{
		case 0:
			return RGB(0,0,0);
		case 1:
			return RGB(255,0,0);
		case 2:
			return RGB(0,255,0);
		case 3:
			return RGB(0,0,255);
		case 4:
			return RGB(255,255,0);
		case 5:
			return RGB(255,0,255);
		case 6:
			return RGB(0,255,255);
		case 7:
			return RGB(198,0,198);
		case 8:
			return RGB(192,0,0);
		case 9:
			return RGB(0,192,0);
		case 10:
			return RGB(0,0,192);
		case 11:
			return RGB(192,192,0);
		case 12:
			return RGB(192,0,192);
		case 13:
			return RGB(0,192,192);
		case 14:
			return RGB(192,192,192);
		case 15:
			return RGB(198,198,0);
		default:
			return RGB(0,0,0);
		}
	}

	
	 
	BYTE red,green,blue;
	red=2*iIndex%255;
	green=(red*red*red+iIndex*iIndex+10)%255;
	blue=(green*green*green+red*red+5)%255;

	COLORREF color(RGB(red,green,blue));
	
	return color;
	
}

void CColorBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_CurRect.PtInRect(point))
	{
		CRect r;
		InvalidateRect(&m_CurRect);

		int i;
		i=(int)((point.y-5)/15)*7+(int)((point.x-5)/15);

		r = m_RectList[i];
		r.left-=2;
		r.top-=2;
		r.right+=2;
		r.bottom+=2;
		m_CurRect = r;

		InvalidateRect(&m_CurRect);
		m_nCurBox=i;
	}
}
