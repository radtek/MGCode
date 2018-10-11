// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PTY_OUTLINE	3	//移动边框
#define PTY_FILL	5	//移动填充体

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
	m_hUpCon = AfxGetApp()->LoadIcon(IDI_UP);
	m_hDownCon = AfxGetApp()->LoadIcon(IDI_DOWN);

	m_iStartcol = 0;
	m_iCurCol = 0;
}

CColorStatic::~CColorStatic()
{
	if (m_btnUp) {
		m_btnUp.DestroyWindow();
	}

	if (m_btnDown) {
		m_btnDown.DestroyWindow();
	}

	if (m_btnSub) {
		m_btnSub.DestroyWindow();
	}

	if (m_btnAdd) {
		m_btnAdd.DestroyWindow();
	}

	m_lstColor.RemoveAll();
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHG, OnChgLayer)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_SUB, OnSub)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_UP, IDC_DOWN, OnButtonCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers


BOOL CColorStatic::Create(int x, int y, int cx, int cy, CWnd *pParentWnd)
{
	if(pParentWnd == NULL)
		return FALSE;

	CRect rt(x,y,x+cx,y+cy);
	if (CStatic::Create("", WS_VISIBLE|WS_BORDER|WS_GROUP|SS_BLACKFRAME, rt, (CWnd*)pParentWnd) == FALSE) {
		return FALSE;
	}

	if (!m_CBox.CreateColorBox(0,0,110,230,this)) {
		return FALSE;
	}

	GetClientRect(&rt);
	
	CRect rtUp = rt;
	rtUp.bottom = rtUp.top + rtUp.Width()/2;
	m_btnUp.Create("", WS_VISIBLE|BS_ICON, rtUp, this, IDC_UP);
	m_btnUp.SetIcon(m_hUpCon);
	
	CRect rtDown = rt;
	rtDown.top = rtDown.bottom - rtDown.Width()/2;
	m_btnDown.Create("", WS_VISIBLE|BS_ICON, rtDown, this, IDC_DOWN);
	m_btnDown.SetIcon(m_hDownCon);

	CRect rtSub = rt;
	rtSub.bottom = rtDown.top;
	rtSub.top = rtSub.bottom-rtSub.Width()/2;
	m_btnSub.Create("-", WS_VISIBLE, rtSub, this, IDC_SUB);

	CRect rtAdd = rt;
	rtAdd.bottom = rtSub.top;
	rtAdd.top = rtAdd.bottom-rtAdd.Width()/2;
	m_btnAdd.Create("+", WS_VISIBLE, rtAdd, this, IDC_ADD);

	CRect rtChg = rt;
	rtChg.bottom = rtAdd.top;
	rtChg.top = rtChg.bottom-rtChg.Width()/2;
	m_btnChg.Create("换层", WS_VISIBLE, rtChg, this, IDC_CHG);

	m_rtCol = rt;
	//方框的起始
	m_rtCol.top = rtUp.bottom+1;
	m_rtCol.bottom = rtAdd.top-1;

	//小方框大小
	m_nHeight = m_rtCol.Height() / NUM_COR;
	m_nWidth = m_rtCol.Width();
	m_rtCur = m_rtCol;
	m_rtCur.bottom = m_rtCur.top + m_nHeight;
	int r = m_nHeight < m_nWidth ?  m_nHeight : m_nWidth;
	m_rtCur.DeflateRect((m_nWidth-r)/2, (m_nHeight-r)/2);
		
	for (int i = 0; i < 16; ++i) {
		m_lstColor.AddTail(GetColor(i));
	}
	return TRUE;
}

BOOL CColorStatic::OnEraseBkgnd(CDC* pDC) 
{
	CBrush bk;
	bk.CreateSysColorBrush(COLOR_BTNFACE);

	CRect rt = m_rtCol;
	CBrush* pOldBrush = pDC->SelectObject(&bk);
	pDC->Rectangle(rt);
	pDC->SelectObject(pOldBrush);
	bk.DeleteObject();
	for (int i = m_iStartcol; i < m_lstColor.GetCount(); ++i) {
		rt.top = m_rtCol.top + (i-m_iStartcol)*m_nHeight;
		rt.bottom = rt.top + m_nHeight;

		CBrush brush(m_lstColor.GetAt(m_lstColor.FindIndex(i)));
		pOldBrush = pDC->SelectObject(&brush);
		pDC->Rectangle(&rt);
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
	}
	
	CBrush brush(RGB(255,255,255));
	pOldBrush = pDC->SelectObject(&brush);
	pDC->Ellipse(m_rtCur);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	
	return CStatic::OnEraseBkgnd(pDC);
}


void CColorStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//在颜色区域
	if (m_rtCol.PtInRect(point)) {
		int iSelCol = (point.y-m_rtCol.top)/m_nHeight+m_iStartcol;
		if (iSelCol >= m_lstColor.GetCount()) {
			return;
		}

		if (iSelCol == m_iCurCol) {
			return;
		}

		InvalidateRect(m_rtCur);
		m_iCurCol = iSelCol;
		m_rtCur = m_rtCol;
		m_rtCur.top = m_rtCol.top + m_nHeight*(m_iCurCol-m_iStartcol);
		m_rtCur.bottom = m_rtCur.top + m_nHeight;
		int r = m_nHeight < m_nWidth ?  m_nHeight : m_nWidth;
		m_rtCur.DeflateRect((m_nWidth-r)/2, (m_nHeight-r)/2);
		InvalidateRect(m_rtCur);
	}
}

int CColorStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_rtCol.PtInRect(point))
	{
		int iSelCol = (point.y-m_rtCol.top)/m_nHeight+m_iStartcol;
		if (iSelCol < m_lstColor.GetCount())
		{
			return iSelCol;
		}
	}
	return -1;
}

void CColorStatic::OnButtonCtrl(UINT nID)
{
	switch(nID) {
		case IDC_UP:
			m_iStartcol++;
			if (m_iStartcol>=m_lstColor.GetCount()) {
				m_iStartcol = m_lstColor.GetCount()-1;
				return;
			}
			break;
		case IDC_DOWN:
			m_iStartcol--;
			if (m_iStartcol<0) {
				m_iStartcol = 0;
				return;
			}
			break;
	}

	m_rtCur = m_rtCol;
	m_rtCur.top = m_rtCol.top + m_nHeight*(m_iCurCol-m_iStartcol);
	m_rtCur.bottom = m_rtCur.top + m_nHeight;
	int r = m_nHeight < m_nWidth ?  m_nHeight : m_nWidth;
	m_rtCur.DeflateRect((m_nWidth-r)/2, (m_nHeight-r)/2);
		
	Invalidate();
}

void CColorStatic::OnChgLayer()
{
	//将当前图形移至其它层
	HCURSOR hcur=::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MOVE));
	::SetCursor(hcur);
	int index = -1;
	int mode;
	SetCapture();
	for (;;)
	{	
		mode=PTY_OUTLINE;
		//消息处理
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		switch (msg.message)
		{
		case WM_RBUTTONDOWN:
			mode=PTY_FILL;
		case WM_LBUTTONDOWN:
			{
				CPoint pt;
				pt.x = (int)(short)LOWORD(msg.lParam);
				pt.y = (int)(short)HIWORD(msg.lParam);
				
				index=ItemFromPoint(pt);
				if(index != -1)
				{
					//有效
					goto ExitLoop;
				}
				else
				{
					//无效
					CString str;
					str.LoadString(IDS_LAYER_MOVE);

					if(AfxMessageBox(str,MB_YESNO|MB_ICONQUESTION)==IDNO)
						goto ExitLoop;
					else
					{
						ReleaseCapture();
						::SetCursor(hcur);
						SetCapture();
					}
				}
			}
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}
ExitLoop:
	ReleaseCapture();
	if(index != -1)
	{
		m_iCurCol = index;
	}
}

void CColorStatic::OnAdd() 
{
	CRect rect;
	GetWindowRect(&rect);

	int left,bottom;
	left=rect.left;
	bottom=rect.bottom;

	int MaxR,MaxB;
	MaxR = GetSystemMetrics(SM_CXSCREEN);
	MaxB = GetSystemMetrics(SM_CYSCREEN);

	//左右边界处理；
	if(left<5)
		left=5;
	if(left>MaxR-110)
		left=MaxR-110;

	//上下边界处理；
	if(bottom<5)
		bottom=5;
	if(bottom>MaxB-260)
		bottom=MaxB-260;

	m_CBox.MoveWindow(left,bottom,110,230);
	m_CBox.ShowWin();
}

void CColorStatic::SelectColorOk()
{
	CRect rt = m_rtCol;
	rt.top += (m_lstColor.GetCount()-m_iStartcol)*m_nHeight;
	rt.bottom = rt.top + m_nHeight;

	COLORREF col = m_CBox.GetColor(m_CBox.m_nCurBox);	
	AddColor(col);
	Invalidate();
}

void CColorStatic::OnSub() 
{
	CString str;
	if(m_iCurCol==0)
	{
		str.LoadString(IDS_LAYER_OTHER);
		AfxMessageBox(str,MB_ICONWARNING);
		return;
	}

	CString frt;
	frt.LoadString(IDS_LAYER_DEL);
	str.Format(frt,m_iCurCol);
	if(AfxMessageBox(str,MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		return;
	}	

	m_lstColor.RemoveAt(m_lstColor.FindIndex(m_iCurCol));
	if (m_iCurCol >= m_lstColor.GetCount()) {
		m_iCurCol = m_lstColor.GetCount()-1;
	}

	Invalidate();
}

COLORREF CColorStatic::GetColor(int iIndex)
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

void CColorStatic::AddColor(COLORREF col)
{
	m_lstColor.AddTail(col);
}

void CColorStatic::ClearColor()
{
	m_lstColor.RemoveAll();
}

int CColorStatic::GetCurSel()
{
	return m_iCurCol;
}

void CColorStatic::SetCurSel(int index)
{
	m_iCurCol = index;
	m_rtCur = m_rtCol;
	m_rtCur.top += m_iCurCol * m_nHeight;
	m_rtCur.bottom = m_rtCur.top + m_nHeight;
	int r = m_nHeight < m_nWidth ?  m_nHeight : m_nWidth;
	m_rtCur.DeflateRect((m_nWidth-r)/2, (m_nHeight-r)/2);
	Invalidate();
}

COLORREF CColorStatic::GetSelColor()
{
	COLORREF col = m_lstColor.GetAt(m_lstColor.FindIndex(m_iCurCol));
	return col;
}


int CColorStatic::ItemFromPoint (CPoint pt)
{
	int iSel = -1;

	pt.y += m_rtCol.top;
	if (!m_rtCol.PtInRect(pt)) {
		return iSel;
	}

	iSel = (pt.y - m_rtCol.top)/m_nHeight + m_iStartcol;
	if ( iSel >= m_lstColor.GetCount()) {
		iSel = -1;
	}

	return iSel;
}
