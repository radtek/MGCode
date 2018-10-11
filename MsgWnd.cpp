// MsgWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MsgWnd.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------
#define WIN_WIDTH	181
#define WIN_HEIGHT	116
/////////////////////////////////////////////////////////////////////////////
// CMsgWnd

CMsgWnd::CMsgWnd()
{
	m_bShow=false;
	m_strMessage="Message To...";
	m_strCaption="HAN'S Messager";
	InitializeCriticalSection(&m_cri);
	m_nOpenTime = 0;
	m_nFlashTime = 0;
}

CMsgWnd::~CMsgWnd()
{
	DeleteCriticalSection(&m_cri);
}

BEGIN_MESSAGE_MAP(CMsgWnd, CWnd)
	//{{AFX_MSG_MAP(CMsgWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgWnd message handlers
void CMsgWnd::CreateMsgWindow()
{
	if(m_bShow){
		return;
	}
	int cy=WIN_HEIGHT;
	int cx=WIN_WIDTH;
	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	int y=rect.bottom-rect.top;
	int x=rect.right-rect.left;
	y=y-WIN_HEIGHT-1;
	x=x-WIN_WIDTH-1;

	m_bShow = true;
	CreateEx(0,
		     AfxRegisterWndClass(
			 0,
			 ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_ARROW)),(HBRUSH)(COLOR_DESKTOP+1),NULL),
			 "",
			 WS_POPUP|WS_EX_TOPMOST,
			 x,y,cx,cy,
			 AfxGetMainWnd()->GetSafeHwnd(),//确定主窗口的设置//NULL,//
			 NULL,
			 NULL);
}

void CMsgWnd::SetPromptMessage(CString strMsg)
{
 	Lock();
	m_strMessage = strMsg;
 	UnLock();
}

void CMsgWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	dc.FillSolidRect(&rect,RGB(255,255,255));
	dc.Rectangle(&rect);
	CRect rt = rect;
	rt.top += 20;
	rt.bottom -= 5;
	rt.left += 5;
	rt.right -= 5;
	dc.Rectangle(&rt);

	CFont font;
	font.CreatePointFont(90,"Impact");
	CFont* pOldFont = dc.SelectObject(&font);
	dc.SetTextColor(RGB(0,64,128));
    dc.SetBkMode(TRANSPARENT);
	dc.TextOut(10,5,m_strCaption);

	CFont fontTest1;
	fontTest1.CreateFont(13,0,0,0, 50, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "Arial");
	dc.SelectObject(&fontTest1);

	//信息显示
	rt.left += 2;
	rt.top += 2;
	rt.bottom -= 15;
 	Lock();
	dc.DrawText(m_strMessage,-1,&rt,DT_LEFT|DT_WORDBREAK|DT_VCENTER);
 	UnLock();
	//时间显示
	rt.top = rt.bottom;
	rt.bottom += 15;
	DrawTime(&dc,rt);

	dc.SelectObject(pOldFont);
}

void CMsgWnd::DrawTime(CDC *pDC, CRect &rt)
{
	CString strTime;
	int day,hour,min,ms;
	double sec;
	DWORD dwTime = m_nOpenTime*m_nFlashTime;

	ms=dwTime%1000;
	dwTime/=1000;

	sec=dwTime%60;
	dwTime/=60;
	sec+=(ms/1000.0);

	min=dwTime%60;
	dwTime/=60;

	hour=dwTime%24;
	day=dwTime/24;

	strTime.Format("Time:%d:%2d:%2d:%6.3f",day,hour,min,sec);

	pDC->DrawText(strTime,&rt,DT_LEFT);
}

void CMsgWnd::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case 0:
		{
			m_nOpenTime++;
			CRect rect;
			GetClientRect(&rect);
			CRect rt = rect;
			rt.top += 20;
			rt.bottom -= 5;
			rt.left += 5;
			rt.right -= 5;
			InvalidateRect(&rt);
			break;
		}
	}

	CWnd::OnTimer(nIDEvent);
}

int CMsgWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	
	return 0;
}

void CMsgWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
}

void CMsgWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
}

void CMsgWnd::SetPromptCaption(CString strCaption)
{
	m_strCaption = strCaption;
}

void CMsgWnd::Close()
{
	if(m_bShow){
		SendMessage(WM_CLOSE);
		m_bShow = false;
	}
}

void CMsgWnd::Lock()
{
	EnterCriticalSection(&m_cri);
}

void CMsgWnd::UnLock()
{
	LeaveCriticalSection(&m_cri);
}

void CMsgWnd::SetViewTimer(int nms)
{
	if(nms==0){
		KillTimer(0);
		m_nFlashTime = 0;
	}
	else{
		m_nOpenTime = 0;
		m_nFlashTime = nms;
		SetTimer(0,nms,NULL);
	}
}
