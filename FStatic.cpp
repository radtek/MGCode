// FStatic.cpp : implementation file
//

#include "stdafx.h"
#include "HL.h"
#include "FStatic.h"
#include "HLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CFStatic::m_nMoveModel=0;
double CFStatic::m_dbMoveDis=1.0;

/////////////////////////////////////////////////////////////////////////////
// CFStatic

CFStatic::CFStatic()
{
	m_bActive=FALSE;
	m_nMoveModel=0;
	m_bRun=FALSE;
	m_bEnable=TRUE;
	m_bRegisterMouseLeave = FALSE;
}

CFStatic::~CFStatic()
{
}


BEGIN_MESSAGE_MAP(CFStatic, CStatic)
	//{{AFX_MSG_MAP(CFStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFStatic message handlers

void CFStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_bEnable)
		return;
	CRect rect;
	GetClientRect(&rect);
	m_bActive=rect.PtInRect(point);
	Invalidate();

	if (!m_bRegisterMouseLeave)
	{
		
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&tme);
		
		m_bRegisterMouseLeave = TRUE;
	}
}

void CFStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	CBrush ActBrush,NoBrush,BlackBrush,PosBrush,GrayBrush;
	CString str;
	ActBrush.CreateSolidBrush(RGB(0,255,0));
	NoBrush.CreateSolidBrush(RGB(0,200,0));
	GrayBrush.CreateSolidBrush(RGB(244,244,244));

	if (m_bEnable)
	{
		if(m_bActive)
			dc.FillRect(&rect,&ActBrush);
		else
			dc.FillRect(&rect,&NoBrush);
	}
	else
	{
		dc.FillRect(&rect, &GrayBrush);
	}
	
	dc.DrawEdge(&rect,BDR_SUNKENINNER|EDGE_SUNKEN,BF_RECT);

	ActBrush.DeleteObject();
	NoBrush.DeleteObject();
	BlackBrush.DeleteObject();
	PosBrush.DeleteObject();

	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -12;
	strcpy(lf.lfFaceName, "MS Shell Dlg");
	VERIFY(font.CreateFontIndirect(&lf));
	CFont* def_font = dc.SelectObject(&font);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,255));
	GetButString(str);
	dc.TextOut(8,8,str);

	dc.SelectObject(def_font);
	font.DeleteObject();
}

void CFStatic::InActive()
{
	if(m_bActive){
		m_bActive=FALSE;
		Invalidate();

		if(m_bRun)
			ProcDirStop();
	}
}

void CFStatic::SetOper(int nOper)
{
	m_nOper=nOper;
}

void CFStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{ 
	if(!m_bEnable)
		return; 

	if(m_nOper==MOVE_MODEL)
	{
		m_nMoveModel++;
		if (m_nMoveModel>3){ 
			m_nMoveModel=0;
		}	 

		Invalidate();
	}
	else
		ProcDirRun();
}

void CFStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(!m_bEnable)
		return;
	if(m_nOper!=MOVE_MODEL && (m_nMoveModel==0 || m_nMoveModel==1 || m_nMoveModel == 4)){
		if(m_bRun)
			ProcDirStop();
	}
}

void CFStatic::GetButString(CString &str)
{
	switch(m_nOper)
	{
	case MOVE_LEFT:
		str.LoadString(IDS_LEFT);
		break;
	case MOVE_RIGHT:
		str.LoadString(IDS_RIGHT);
		break;
	case MOVE_TOP:
		str.LoadString(IDS_TOP);
		break;
	case MOVE_BOTTOM:
		str.LoadString(IDS_BOTTOM);
		break;
	case MOVE_MODEL:
		if(m_nMoveModel==0)
			str.LoadString(IDS_SLOW);
		else if (m_nMoveModel==1)
			str.LoadString(IDS_FAST);
		else if (m_nMoveModel==2)
			str.LoadString(IDS_DISTANCE);
		else if (m_nMoveModel==3)
			str.LoadString(IDS_DISTANCE10);
		else
			str.LoadString(IDS_DIE_SPACE);		
		break;
	case MOVE_UP:
		str.LoadString(IDS_UP);
		break;
	case MOVE_DOWN:
		str.LoadString(IDS_DOWN);
		break;
	}
}

void CFStatic::ProcDirRun()
{
	double dbCurX = g_pMotion->GetPos(eMaskX);
	double dbCurY = g_pMotion->GetPos(eMaskY);
	double dbCurZ = g_pMotion->GetPos(eMaskZ);

	CString str;
	str.Format("model=%d,currentpos(%.3f,%.3f,%.3f),moveR=%.3f\n",m_nMoveModel,dbCurX, dbCurY, dbCurZ, m_dbMoveDis);
	OutputDebugString(str);

	CMDIFrameWnd *pMainWnd=(CMDIFrameWnd *)AfxGetMainWnd();
	CMDIChildWnd *pChildWnd=pMainWnd->MDIGetActive();
	CHLDoc *pDoc= NULL;
	if(pChildWnd)
		pDoc=(CHLDoc *)pChildWnd->GetActiveDocument();
	
	double dbMoveDis=0;;

	//移动模式：0代表慢移, 1代表快移, 2代表1X定移, 3代表10X定移
	int nSpeedType=eSpeedSlow;
	switch (m_nMoveModel)
	{
	case 0:
		dbMoveDis = 5000;
		break;
	case 1:
		nSpeedType = eSpeedFast;
		dbMoveDis = 5000;
		break;
	case 2:
		dbMoveDis = m_dbMoveDis;
		break;
	case 3:
		dbMoveDis = m_dbMoveDis*10;
		break;
	case 4:
		nSpeedType = eSpeedHome;
		dbMoveDis = 5000;
		break;
	}

	switch(m_nOper)
	{
	case MOVE_LEFT:
		g_pMotion->StartAMove(eMaskX, dbCurX-dbMoveDis, nSpeedType);
		break;
	case MOVE_RIGHT:
		g_pMotion->StartAMove(eMaskX, dbCurX+dbMoveDis, nSpeedType);
		break;
	case MOVE_TOP:
		g_pMotion->StartAMove(eMaskY, dbCurY+dbMoveDis, nSpeedType);
		break;
	case MOVE_BOTTOM:
		g_pMotion->StartAMove(eMaskY, dbCurY-dbMoveDis, nSpeedType);
		break;
	case MOVE_UP:
		g_pMotion->StartAMove(eMaskZ, dbCurZ-dbMoveDis, nSpeedType);
		break;
	case MOVE_DOWN:
		g_pMotion->StartAMove(eMaskZ, dbCurZ+dbMoveDis, nSpeedType);
		break;
	}
	m_bRun=TRUE;
}

void CFStatic::ProcDirStop()
{
	g_pMotion->Stop();
}

void CFStatic::EnableWin(BOOL bFlag)
{
	m_bEnable=bFlag;
	EnableWindow(m_bEnable);
	Invalidate();
}

void CFStatic::SetFast(BOOL bFast)
{
	m_nMoveModel=bFast;
	Invalidate();
}

LRESULT CFStatic::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bRegisterMouseLeave = FALSE;
	m_bActive = FALSE;
	Invalidate();

	if (m_bRun)
		OnLButtonUp(0, CPoint(0));
	return 0;
}