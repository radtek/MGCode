// ZoomView.cpp : implementation file
//

#include "stdafx.h"
#include "ZoomView.h"
#include "Resource.h"
#include "DlgSetLinePos.h"

#include "ArrayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_MEASURE_LINE_SIZE 256
#define LONGSIZE	8
#define SHORTSIZE	4
#define MIDSIZE	 6
#define VIEW_GRADE	21
#define MIN_SIZE  4
#define MAXLEN 10

double g_dbGrade[VIEW_GRADE] = {0.001,0.002,0.005,0.01,0.02,0.05,
0.1,0.2,0.5,1,2,5,10,20,50,
								100,200,500,1000,2000,5000};

/////////////////////////////////////////////////////////////////////////////
// CZoomView

IMPLEMENT_DYNCREATE(CZoomView, CView)

extern BOOL g_bSure;
CZoomView::CZoomView()
{ 
	m_pArrayWnd = NULL;

	m_bLBDowm=m_bDrag=FALSE;
	m_nRulerWidth=20;
	m_ptCurPos.x=m_ptCurPos.y=0;
	m_ModeSelect=MODE_ZOOMOFF;
	m_bRButDown = FALSE;
	m_dbViewScale = 1.1;

	m_arrayMeasureLine.SetSize(MAX_MEASURE_LINE_SIZE);
	m_nMeasureLineCount = 0;
	m_bCanMoveMeasureLine = FALSE;
	m_nMeasureLineIndex = 0;
	m_bCanSetLinePos = FALSE;
	m_bMeasureLineX = FALSE;
	m_bMeasureLineY = FALSE;
	m_dbMeasureLineX = 0.0;
	m_dbMeasureLineY = 0.0;
}

CZoomView::~CZoomView()
{
}

BEGIN_MESSAGE_MAP(CZoomView, CView)
	//{{AFX_MSG_MAP(CZoomView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomView drawing

void CZoomView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CZoomView diagnostics

#ifdef _DEBUG
void CZoomView::AssertValid() const
{
	CView::AssertValid();
}

void CZoomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZoomView message handlers

void CZoomView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bRButDown = FALSE;
	SendMessage(WM_NCPAINT);
	ReleaseCapture();
	
	if (m_bCanMoveMeasureLine)
	{
		//移动测量线
		MoveMeasureLine(point);
		return;
	}
	if (point.x==0 && point.y==0)
	{
		//添加测量线条
		DoMeasureLine();
		return;
	}

	if((m_ModeSelect==MODE_ZOOMOPER)||(m_ModeSelect==MODE_ZOOMSEL))//drag mode
	{
		m_bLBDowm=TRUE;
		m_ptPrev=m_ptNow=point;	
		m_ptResev=point;
	}
	
	OnArrayPro(point);
	
	CView::OnLButtonDown(nFlags, point);
}


void CZoomView::OnLButtonUp(UINT nFlags, CPoint point) 
{   
	//drag mode
	if((m_ModeSelect==MODE_ZOOMOPER)||(m_ModeSelect==MODE_ZOOMSEL))
	{
		if(m_bLBDowm && !m_bDrag)
		{
			CDC *pDC=GetDC();
			CPen *OldPen;
			//clear the last rect
			int mod=pDC->SetROP2(R2_NOTXORPEN);
			CPen WhitePen(PS_DOT,1,RGB(0,0,255));
			OldPen=pDC->SelectObject(&WhitePen);
			pDC->MoveTo(m_ptPrev);
			pDC->LineTo(m_ptNow.x,m_ptPrev.y);
			pDC->LineTo(m_ptNow);
			pDC->LineTo(m_ptPrev.x,m_ptNow.y);
			pDC->LineTo(m_ptPrev);
			pDC->SelectObject(OldPen);
			WhitePen.DeleteObject();
			
			double tx,ty,dbx,dby;
			tx=m_ptPrev.x;
			ty=m_ptPrev.y;
			m_DC.LogicToPhysic(tx,ty);
			dbx=point.x;
			dby=point.y;
			m_DC.LogicToPhysic(dbx,dby);
			m_DC.m_dbPhysicX=(tx+dbx)/2;
			m_DC.m_dbPhysicY=(ty+dby)/2;
			double dbWid=abs(m_ptPrev.x-point.x);
			double dbHei=abs(m_ptPrev.y-point.y);
			if(dbWid*dbHei>0)
			{
				if(m_rect.Width()/dbWid>m_rect.Height()/dbHei)
				{
					double dbVal = m_DC.GetScale();
					dbVal*=m_rect.Height()/dbHei;
					m_DC.SetScale(dbVal);
				}
				else
				{
					double dbVal = m_DC.GetScale();
					dbVal*=m_rect.Width()/dbWid;
					m_DC.SetScale(dbVal);
				}
				Invalidate();
				ReleaseDC(pDC);
			}
			pDC->SetROP2(mod);
		}
		m_bLBDowm=FALSE;
		SendMessage(WM_NCPAINT);
	}
	CView::OnLButtonUp(nFlags, point);
}

void CZoomView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	FlashCurPos(m_ptCurPos);
	m_ptCurPos = point;
	FlashCurPos(m_ptCurPos);
	
	CView::OnMouseMove(nFlags,point);
	if(MODE_ZOOMSEL==m_ModeSelect)
	{
		if(m_bLBDowm && !m_bDrag)
		{
			CDC *pDC=GetDC();
			CPen *OldPen;
			//clear the last rect
			int mod=pDC->SetROP2(R2_NOTXORPEN);//反色掉
			CPen BlackPen(PS_DOT,1,RGB(0,0,255));
			OldPen=pDC->SelectObject(&BlackPen);
			pDC->MoveTo(m_ptPrev);
			pDC->LineTo(m_ptNow.x,m_ptPrev.y);
			pDC->LineTo(m_ptNow);
			pDC->LineTo(m_ptPrev.x,m_ptNow.y);
			pDC->LineTo(m_ptPrev);
			pDC->SelectObject(OldPen);
			
			//draw new rect
			OldPen=pDC->SelectObject(&BlackPen);
			pDC->MoveTo(m_ptPrev);
			pDC->LineTo(point.x,m_ptPrev.y);
			pDC->LineTo(point);
			pDC->LineTo(m_ptPrev.x,point.y);
			pDC->LineTo(m_ptPrev);
			pDC->SelectObject(OldPen);
			BlackPen.DeleteObject();
			m_ptNow=point;
			pDC->SetROP2(mod);
			ReleaseDC(pDC);
			SendMessage(WM_NCPAINT);
		}
	}
	if((MODE_ZOOMOPER==m_ModeSelect)|m_bRButDown)
	{	
		if((m_bLBDowm && m_bDrag)|m_bRButDown)
		{
			double x,dx,y,dy;
			x=m_ptResev.x;
			y=m_ptResev.y;
			dx=point.x;
			dy=point.y;
			m_DC.LogicToPhysic(x,y);
			m_DC.LogicToPhysic(dx,dy);
			m_DC.m_dbPhysicX-=(dx-x);
			m_DC.m_dbPhysicY-=(dy-y);
			Invalidate();
			SendMessage(WM_NCPAINT);
			m_ptResev=point;
		}
	}
}

void CZoomView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&m_rect);
	double dbWidth,dbHeight;
	dbWidth=m_rect.Width();
	dbHeight=m_rect.Height();
	m_DC.m_ptCenter.x=(int)dbWidth/2;
	m_DC.m_ptCenter.y=(int)dbHeight/2;
	m_DC.SetViewRect(dbWidth,dbHeight);
	SendMessage(WM_NCPAINT);
}

XDC *CZoomView:: PrepareDC(CDC *pDC)
{
	m_DC.m_pDC=pDC;
	return &m_DC;
}

BOOL CZoomView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta>0)
	{
		ZoomIn();
	}
	else
	{
		ZoomOut();
	}

	return TRUE;

}

void CZoomView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	lpncsp->rgrc[0].left+=m_nRulerWidth;
	lpncsp->rgrc[0].top +=m_nRulerWidth;
	CView::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CZoomView::OnNcPaint() 
{
	// TODO: Add your message handler code here
	CDC *pDC=GetWindowDC();
	DrawRuler(pDC);
	ReleaseDC(pDC);
	// Do not call CView::OnNcPaint() for painting messages
//	CZoomViewB::OnNcPaint();
}


void CZoomView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bRButDown = TRUE;
	m_ptPrev=m_ptNow=point;	
	m_ptResev=point;
	SetCapture();
	CView::OnMButtonDown(nFlags, point);
}

void CZoomView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bRButDown = FALSE;
	SendMessage(WM_NCPAINT);
	ReleaseCapture();
	CView::OnMButtonUp(nFlags, point);
}


void CZoomView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnRButtonDown(nFlags, point);
}

void CZoomView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CView::OnRButtonUp(nFlags, point);
}


BOOL CZoomView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (message == WM_LBUTTONDOWN && nHitTest == HTNOWHERE)
	{
		SendMessage(WM_LBUTTONDOWN, 0, 0);
	}
	
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CZoomView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bCanSetLinePos)
	{
		CMeasureLine &line = m_arrayMeasureLine[m_nMeasureLineIndex];
		CDlgSetLinePos dlg;
		CString strTemp;
		strTemp.Format("%.3f", line.m_dbPos);
		dlg.m_dbLinePos = atof(strTemp);
		if (dlg.DoModal() == IDOK)
		{
			line.m_dbPos = dlg.m_dbLinePos;
			this->Invalidate();
		}
	}
	
	CView::OnLButtonDblClk(nFlags, point);
}

////////////////////////
void CZoomView::SetArrayDlg(CWnd *pWnd)
{
	m_pArrayWnd = pWnd;

	return;
}


BOOL CZoomView::Track(CPoint &point, CRect &rect, CWnd *pWnd)//是否选中对象
{
	BOOL bFlag=FALSE;

	AfxLockTempMaps();
	pWnd->SetCapture();

	if (CWnd::GetCapture() != pWnd)
	{
		return FALSE;
	}
	
	CPoint ptthis    = point;
	       m_ptSPrev  = point;
	       m_ptSNow   = m_ptSPrev;

	MSG msg;
	for(;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				if(bFlag)
				{
					LastBrush(pWnd, m_ptSPrev, ptthis);
				}

				rect.left   = point.x;
				rect.right  = ptthis.x;
				rect.top    = point.y;
				rect.bottom = ptthis.y;

				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{
				bFlag    = TRUE;
				ptthis.x = (int)(short)LOWORD(msg.lParam);
				ptthis.y = (int)(short)HIWORD(msg.lParam);
				DrawRect(pWnd,m_ptSPrev,m_ptSNow,ptthis);
				m_ptSNow = ptthis;
				
				break;
			}
		default:
			{
				break;
			}
		} // end switch
	} // end for

ExitLoop:
	{
		ReleaseCapture();
		AfxUnlockTempMaps(FALSE);	
		return TRUE;
	}
	
}


void CZoomView::DrawSelQuadList()
{
	if (m_pArrayWnd)
	{
		CArrayDlg *pArrayDlg = (CArrayDlg*)m_pArrayWnd;

		CDC *pDC=this->GetDC();
		XDC* pXDc = PrepareDC(pDC);

		if(m_SelQuad.IsEmpty())
		{
			int nCnt = pArrayDlg->m_QuadList.GetSize();
			for (int i = 0; i < nCnt; i++)
			{
				pArrayDlg->m_QuadList[i].m_bSel = FALSE;
			}
			ReleaseDC(pDC);
			return;
		}

		pArrayDlg->DrawSelTraker(pXDc, m_SelQuad, this, pArrayDlg->m_QuadList);

		ReleaseDC(pDC);
	}

	return;
}

void CZoomView::UpdateSelQuad()
{
	m_SelQuad.SetEmpty();

	if (m_pArrayWnd)
	{
		CArrayDlg *pArrayDlg = (CArrayDlg*)m_pArrayWnd;
		int nCnt = pArrayDlg->m_QuadList.GetSize();
		for (int j = 0; j < nCnt; j++)
		{
			if (TRUE == pArrayDlg->m_QuadList[j].m_bSel)
			{
				m_SelQuad.Compose(pArrayDlg->m_QuadList[j].m_QD);
			}
		}
	}

	return;
}

void CZoomView::OnArrayPro(CPoint &point) 
{
	if (m_pArrayWnd)
	{
		CArrayDlg *pArrayDlg = (CArrayDlg*)m_pArrayWnd;
		CRect rect;
		pArrayDlg->MulObjSel(point, rect, this);
		CQuad quad=(CQuad)rect;
		LogicToPhysic(quad);
		quad.Normal();

		int nCnt = pArrayDlg->m_QuadList.GetSize();
		for (int i = 0; i < nCnt; i++)
		{
			pArrayDlg->m_QuadList[i].m_bSel = FALSE;
		}
		m_SelQuad.SetEmpty();

		int nSel = -1;
		for (int j = 0; j < nCnt; j++)
		{
			if (quad.Include(pArrayDlg->m_QuadList[j].m_QD))
			{
				pArrayDlg->m_QuadList[j].m_bSel = TRUE;
				SELQUAD selQuad;
				selQuad = pArrayDlg->m_QuadList[j];
				m_SelQuad.Compose(selQuad.m_QD);
				nSel = j;
			}
		}

		pArrayDlg->DispSelObjProp(nSel);
		pArrayDlg->SetRedMarkQuadlist(pArrayDlg->m_QuadList);

		this->Invalidate();
	}

	return;
}

void CZoomView::DrawRuler(CDC *pDC)
{
	int cx,cy;
	
	//画右部和下部的非客户区域
	cx = 10;
	cy = m_rect.Height()+m_nRulerWidth+5;
	pDC->FillSolidRect(m_rect.Width()+m_nRulerWidth,1,cx,cy,RGB(255,255,255));
	
	cx = m_rect.Width()+m_nRulerWidth;
	cy = 10;
	pDC->FillSolidRect(2,m_rect.Height()+m_nRulerWidth,cx,cy,RGB(255,255,255));
	//画线及背景
	cy = m_nRulerWidth+1;
	cx = m_rect.Width()+m_nRulerWidth+1;
	pDC->FillSolidRect(0,0,cx,cy,RGB(255,255,255));
	pDC->SetBkMode(TRANSPARENT);
	
    CPen newPen(PS_SOLID, 1, (COLORREF)0);  // black pen,
	//  2 pixels wide
    CPen* pOldPen = pDC->SelectObject(&newPen);
    pDC->MoveTo(0, cy);
    pDC->LineTo(cx, cy);
	pDC->MoveTo(0,0);
	pDC->LineTo(cx,0);
    pDC->SelectObject(pOldPen); 
	newPen.DeleteObject();
	
	cx = m_nRulerWidth+1;
	cy = m_rect.Height()+m_nRulerWidth;
	pDC->FillSolidRect(0,cx+2,cx,cy,RGB(255,255,255));
	
    CPen newPen1(PS_SOLID, 1, (COLORREF)0); 
    pOldPen = pDC->SelectObject(&newPen1);
    pDC->MoveTo(cx,0);
    pDC->LineTo(cx,cy);
	pDC->MoveTo(0,0);
	pDC->LineTo(0,cy);
    pDC->SelectObject(pOldPen); 
	newPen1.DeleteObject();
	
	DrawMark(pDC);
	DrawCurPos(pDC,m_ptCurPos);
	pDC->FillSolidRect(1,1,m_nRulerWidth-1,m_nRulerWidth-1,RGB(255,255,255));
	//写字
    CFont fontTest1;
    fontTest1.CreateFont(13, 0,0,0, 50, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "Arial");
    CFont* pOldFont = pDC->SelectObject(&fontTest1);
    pDC->TextOut(2, 2, "mm");
    pDC->SelectObject(pOldFont);
	fontTest1.DeleteObject();
}

void CZoomView::DrawMark(CDC *pDC)
{
	//画标尺及刻度标识
	//横向
	int nViewSizeWidth = m_rect.Width();
	int n = nViewSizeWidth/MIN_SIZE;
	n--;
	double dbPhysicWidth = nViewSizeWidth/m_DC.m_dbScale;
	double dbn = (dbPhysicWidth/n)*10;

	int i=0;
	for(i=0;i<VIEW_GRADE;i++)
	{
		if(dbn<g_dbGrade[i]){
			break;
		}
	}
	i = (i>=VIEW_GRADE?VIEW_GRADE-1:i);
	double dbCenter = (int)(m_DC.m_dbPhysicX/g_dbGrade[i])*g_dbGrade[i];

	int m = 0;		//中间点计数
	int j=0;
	for(j=0;j<n/2+1;j++)
	{
		double x1,y1,x2,y2,y;
		x1=dbCenter-j*g_dbGrade[i]/10;
		y=0;
		m_DC.PhysicToLogic(x1,y);
		x1+=m_nRulerWidth+1;
		x2=x1;
		y2=m_nRulerWidth;
		if((m%10)==0){
			y1 = m_nRulerWidth-LONGSIZE;
		}
		else if((m%5)==0){
			y1 = m_nRulerWidth-MIDSIZE;
		}
		else{
			y1 = m_nRulerWidth-SHORTSIZE;
		}
		if(x1>m_nRulerWidth){
			DrawLine(pDC,(int)x1,(int)y1,(int)x2,(int)y2);
		}
		if((m%10)==0){
			//写字
			char szInfo[32];
			double dbPos=dbCenter-j*g_dbGrade[i]/10;
			if(i<3){
				sprintf(szInfo,"%.3f",dbPos);
			}
			else if(i<6){
				sprintf(szInfo,"%.2f",dbPos);
			}
			else if(i<9){
				sprintf(szInfo,"%.1f",dbPos);
			}
			else{
				sprintf(szInfo,"%.0f",dbPos);
			} 
			DrawText(pDC,(int)x1,0,0,szInfo);
		}

		x1=dbCenter+j*g_dbGrade[i]/10;
		y=0;
		m_DC.PhysicToLogic(x1,y);
		x1+=m_nRulerWidth+1;
		x2=x1;
		y2=m_nRulerWidth;
		if((m%10)==0){
			y1 = m_nRulerWidth-LONGSIZE;
		}
		else if((m%5)==0){
			y1 = m_nRulerWidth-MIDSIZE;
		}
		else{
			y1 = m_nRulerWidth-SHORTSIZE;
		}
		if(x1<m_rect.Width()+m_nRulerWidth){
			DrawLine(pDC,(int)x1,(int)y1,(int)x2,(int)y2);
		}

		if((m%10)==0){
			//写字
			char szInfo[32];
			double dbPos=dbCenter+j*g_dbGrade[i]/10;
			if(i<3){
				sprintf(szInfo,"%.3f",dbPos);
			}
			else if(i<6){
				sprintf(szInfo,"%.2f",dbPos);
			}
			else if(i<9){
				sprintf(szInfo,"%.1f",dbPos);
			}
			else{
				sprintf(szInfo,"%.0f",dbPos);
			}
			DrawText(pDC,(int)x1,0,0,szInfo);
		}
		m++;
	}
	//纵向
	int nViewSizeHeight = m_rect.Height();
	n = nViewSizeHeight/MIN_SIZE;
	n--;
	double dbPhysicHeight = nViewSizeHeight/m_DC.m_dbScale;
	dbn = (dbPhysicHeight/n)*10;

	for(i=0;i<VIEW_GRADE;i++)
	{
		if(dbn<g_dbGrade[i]){
			break;
		}
	}
	i = (i>=VIEW_GRADE?VIEW_GRADE-1:i);
	dbCenter = (int)(m_DC.m_dbPhysicY/g_dbGrade[i])*g_dbGrade[i];

	m = 0;		//中间点计数
	for(j=0;j<n/2+1;j++)
	{
		double x1,y1,x2,y2,x;
		y1=dbCenter-j*g_dbGrade[i]/10;
		x=0;
		m_DC.PhysicToLogic(x,y1);
		y1+=m_nRulerWidth+1;
		y2=y1;
		x2=m_nRulerWidth;
		if((m%10)==0){
			x1 = m_nRulerWidth-LONGSIZE;
		}
		else if((m%5)==0){
			x1 = m_nRulerWidth-MIDSIZE;
		}
		else{
			x1 = m_nRulerWidth-SHORTSIZE;
		}

		if(y1>m_nRulerWidth){
			DrawLine(pDC,(int)x1,(int)y1,(int)x2,(int)y2);
		}
		if((m%10)==0){
			//写字
			char szInfo[32];
			double dbPos=dbCenter-j*g_dbGrade[i]/10;
			if(i<3){
				sprintf(szInfo,"%.3f",dbPos);
			}
			else if(i<6){
				sprintf(szInfo,"%.2f",dbPos);
			}
			else if(i<9){
				sprintf(szInfo,"%.1f",dbPos);
			}
			else{
				sprintf(szInfo,"%.0f",dbPos);
			}
			DrawText(pDC,0,(int)y1,90,szInfo);
		}

		y1=dbCenter+j*g_dbGrade[i]/10;
		x=0;
		m_DC.PhysicToLogic(x,y1);
		y1+=m_nRulerWidth+1;
		y2=y1;
		x2=m_nRulerWidth;
		if((m%10)==0){
			x1 = m_nRulerWidth-LONGSIZE;
		}
		else if((m%5)==0){
			x1 = m_nRulerWidth-MIDSIZE;
		}
		else{
			x1 = m_nRulerWidth-SHORTSIZE;
		}
		if(y1<m_rect.Height()){
			DrawLine(pDC,(int)x1,(int)y1,(int)x2,(int)y2);
		}

		if((m%10)==0){
			//写字
			char szInfo[32];
			double dbPos=dbCenter+j*g_dbGrade[i]/10;
			if(i<3){
				sprintf(szInfo,"%.3f",dbPos);
			}
			else if(i<6){
				sprintf(szInfo,"%.2f",dbPos);
			}
			else if(i<9){
				sprintf(szInfo,"%.1f",dbPos);
			}
			else{
				sprintf(szInfo,"%.0f",dbPos);
			}
			DrawText(pDC,0,(int)y1,90,szInfo);
		}
		m++;
	}
}

void CZoomView::DrawCurPos(CDC *pDC,POINT pt)
{
	CPen* pOldPen;
    CPen newPen2(PS_SOLID, 1,RGB(0,0,255));
	int mod;
	mod=pDC->SetROP2(R2_NOTXORPEN);//反色掉
    pOldPen = pDC->SelectObject(&newPen2);
	int bx,by,ex,ey;
	ex = bx = pt.x+m_nRulerWidth;
	by = 2;
	ey = m_nRulerWidth-2;
    pDC->MoveTo(bx,by);
    pDC->LineTo(ex,ey);
	
	by = ey = pt.y+m_nRulerWidth;
	bx = 2;
	ex = m_nRulerWidth-2;
    pDC->MoveTo(bx,by);
    pDC->LineTo(ex,ey);
    pDC->SelectObject(pOldPen); 
	newPen2.DeleteObject();
	pDC->SetROP2(mod);
}

void CZoomView::DrawLine(CDC *pDC, int x, int y, int x2, int y2)
{
	pDC->MoveTo(x,y);
	pDC->LineTo(x2,y2);
}

void CZoomView::DrawText(CDC *pDC, int x, int y, double angle, char *str)
{
	CFont fontTest1;
	fontTest1.CreateFont(13,0,int(angle)*10,0, 50, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "Arial");
	CFont* pOldFont = pDC->SelectObject(&fontTest1);
	UINT nPos = pDC->SetTextAlign(TA_CENTER);
	pDC->TextOut(x,y,str);
	pDC->SelectObject(pOldFont);
	fontTest1.DeleteObject();
	pDC->SetTextAlign(nPos);
}

UINT CZoomView::GetZoomMode()
{
	return (UINT)m_ModeSelect;
}

void CZoomView::SetZoomMode(UINT mode)
{
	m_ModeSelect=(MODESELECT)mode;
	m_bDrag = (m_ModeSelect!=MODE_ZOOMSEL);
}

void CZoomView::ZoomIn()
{
	double dbVal = m_DC.GetScale()*m_dbViewScale;
	m_DC.SetScale(dbVal);
	Invalidate();
	SendMessage(WM_NCPAINT);
}

void CZoomView::ZoomOut()
{
	double dbVal = m_DC.GetScale()/m_dbViewScale;
	m_DC.SetScale(dbVal);
	Invalidate();
	SendMessage(WM_NCPAINT);
}

void CZoomView::Drag()
{
	m_bDrag=TRUE;
}

void CZoomView::NoDrag()
{
	m_bDrag=FALSE;
}

void CZoomView::ViewDPtoLP(LPPOINT lpPoints,int nCount)
{
	double x,y;
	
	for(int i=0;i<nCount;i++)
	{
		x = (*(lpPoints+i)).x;
		y = (*(lpPoints+i)).y;
		LogicToPhysic(x,y);
		(*(lpPoints+i)).x = (int)x;
		(*(lpPoints+i)).y = (int)y;
	}
}

void CZoomView::ViewLPtoDP(LPPOINT lpPoints,int nCount)
{
	//	double x,y;
	//
	//	for(int i=0;i<nCount;i++)
	//	{
	//		x = (*(lpPoints+i)).x;
	//		y = (*(lpPoints+i)).y;
	//		PhysicToLogic(x,y);
	//		(*(lpPoints+i)).x = (int)x;
	//		(*(lpPoints+i)).y = (int)y;
	//	}
}

void CZoomView::DoZoomIn(double x, double y,double CentX,double CentY)
{
	if(x*y==0)
		return;
	double dbVal;
	if(m_rect.Width()/x<m_rect.Height()/y)
		dbVal=m_rect.Width()/x;
	else 
		dbVal=m_rect.Height()/y;
	m_DC.SetScale(dbVal);
	
	m_DC.m_dbPhysicY= CentY;
	m_DC.m_dbPhysicX= CentX;
	Invalidate();
	SendMessage(WM_NCPAINT);
}

void CZoomView::PTLogicToPhysic(POINT* pt)
{
	double x,y;
	x = pt->x;
	y = pt->y;
	LogicToPhysic(x,y);
	
	pt->x = (int)x;
	pt->y = (int)y;
}

void CZoomView::PTPhysicToLogic(POINT* pt)
{
	double x,y;
	x = pt->x;
	y = pt->y;
	PhysicToLogic(x,y);
	
	pt->x = (int)x;
	pt->y = (int)y;
}

void CZoomView::LogicToPhysic(QUAD &quad)
{
	m_DC.LogicToPhysic(quad.left,quad.top);
	m_DC.LogicToPhysic(quad.right,quad.bottom);
}

void CZoomView::PhysicToLogic(QUAD &quad)
{
	m_DC.PhysicToLogic(quad.left,quad.top);
	m_DC.PhysicToLogic(quad.right,quad.bottom);
}

void CZoomView::LogicToPhysic(double& x,double& y)
{
	m_DC.LogicToPhysic(x,y);
}

void CZoomView::PhysicToLogic(double& x,double& y)
{
	m_DC.PhysicToLogic(x,y);
}

void CZoomView::LenLogicToPhysic(double* pdbVal,int nCount)
{
	double t;
	
	for(int i=0;i<nCount;i++)
	{
		t = *(pdbVal+i);
		m_DC.LenLogicToPhysic(t);
		*(pdbVal+i) = t;
	}
}

void CZoomView::LenPhysicToLogic(double* pdbVal,int nCount)
{
	double t;
	
	for(int i=0;i<nCount;i++)
	{
		t = *(pdbVal+i);
		m_DC.LenPhysicToLogic(t);
		*(pdbVal+i) = t;
	}
}

void CZoomView::ActualLen(double &dbx)
{
	dbx/=m_DC.GetScale();
}

void CZoomView::SetViewScale(double dbScale)
{
	m_dbViewScale = dbScale;
}

void CZoomView::DoMeasureLine()
{
	AfxLockTempMaps();
	this->SetCapture();
	
	if (CWnd::GetCapture() != this)
	{
		return;
	}
	
	CPoint firstPt(0,0);
	BOOL bGetFirstPt = TRUE;
	GetCursorPos(&firstPt);
	ScreenToClient(&firstPt);
	CMeasureLine line;
	LONG lOldPos = 0;
	LONG lNewPos = 0;
	UINT nLineType = CMeasureLine::LEFT_TO_RIGHT;
	BOOL bHasOldPos=FALSE;
	
	MSG msg;
	for(;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				CPoint point;
				point.x = LOWORD(msg.lParam); 
				point.y = HIWORD(msg.lParam); 
				if (m_rect.PtInRect(point))
				{
					double dbx = point.x;
					double dby = point.y;
					LogicToPhysic(dbx, dby);
					if (m_nMeasureLineCount == MAX_MEASURE_LINE_SIZE)
					{
//						AfxMessageBox(IDS_MAX_MEASURE_LINE, MB_OK|MB_ICONWARNING);
						goto ExitLoop;
					}
					if (firstPt.x > 0)
					{
						line.m_dbPos = dby;
						line.m_nType = CMeasureLine::LEFT_TO_RIGHT;
						m_arrayMeasureLine[m_nMeasureLineCount] = line;
						m_nMeasureLineCount++;
					}
					else
					{
						line.m_dbPos = dbx;
						line.m_nType = CMeasureLine::TOP_TO_BOTTOM;
						m_arrayMeasureLine[m_nMeasureLineCount] = line;
						m_nMeasureLineCount++;
					}
				}
				
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{
				CPoint point(0,0);
				point.x = (short)(LOWORD(msg.lParam)); 
				point.y = (short)(HIWORD(msg.lParam)); 
				if (!bGetFirstPt)
				{
					firstPt = point;
					bGetFirstPt = TRUE;
					if (firstPt.x<=0 && firstPt.y<=0)
					{
						goto ExitLoop;
					}
				}
				if (firstPt.x > 0)
				{
					lNewPos = point.y;
					nLineType = CMeasureLine::LEFT_TO_RIGHT;
				}
				else
				{
					lNewPos = point.x;
					nLineType = CMeasureLine::TOP_TO_BOTTOM;
				}
				if(DrawMeasureLineTemp(lOldPos, bHasOldPos, lNewPos, nLineType, point))
				{
					lOldPos = lNewPos;
					bHasOldPos = TRUE;
				}
				break;
			}
		default:
			{
				break;
			}
		} // end switch
	} // end for
	
ExitLoop:
	{
		ReleaseCapture();
		AfxUnlockTempMaps(FALSE);	
		this->Invalidate();
		return;
	}
}

void CZoomView::DrawMeasureLine(CDC *pDc)
{
	for(int i=0; i< m_nMeasureLineCount; i++)
	{
		POINT point1, point2;
		CMeasureLine line = m_arrayMeasureLine[i];
		if (line.m_nType == CMeasureLine::LEFT_TO_RIGHT)
		{
			double x = 0;
			double y = line.m_dbPos;
			PhysicToLogic(x, y);
			point1.x = m_rect.left;
			point1.y = (LONG)y;
			point2.x = m_rect.right;
			point2.y = (LONG)y;
		}
		else
		{
			double x = line.m_dbPos;
			double y = 0;
			PhysicToLogic(x, y);
			point1.x = (LONG)x;
			point1.y = m_rect.top;
			point2.x = (LONG)x;
			point2.y = m_rect.bottom;
		}
		CPen newPen(PS_DASHDOTDOT, 1, (COLORREF)RGB(117,117,202));  // black pen,
		//  2 pixels wide
		CPen* pOldPen = pDc->SelectObject(&newPen);
		pDc->MoveTo(point1);
		pDc->LineTo(point2);
		pDc->SelectObject(pOldPen); 
		newPen.DeleteObject();
	}
}

void CZoomView::MoveMeasureLine(CPoint ptDown)
{
	AfxLockTempMaps();
	this->SetCapture();
	
	if (CWnd::GetCapture() != this)
	{
		return;
	}
	
	LONG lOldPos = 0;
	LONG lNewPos = 0;
	UINT nLineType = CMeasureLine::LEFT_TO_RIGHT;
	BOOL bHasOldPos=FALSE;
	
	MSG msg;
	for(;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				CPoint point;
				point.x = LOWORD(msg.lParam); 
				point.y = HIWORD(msg.lParam); 
				if (m_rect.PtInRect(point))
				{
					if(ptDown != point)
					{
						double dbx = point.x;
						double dby = point.y;
						LogicToPhysic(dbx, dby);
						CMeasureLine &line = m_arrayMeasureLine[m_nMeasureLineIndex];
						if (line.m_nType == CMeasureLine::LEFT_TO_RIGHT)
						{
							line.m_dbPos = dby;
						}	
						else
						{
							line.m_dbPos = dbx;
						}
					}
				}
				else
				{
					//m_arrayMeasureLine.RemoveAt(m_posMeasureLine);
					m_nMeasureLineCount--;
					for (BYTE i=m_nMeasureLineIndex; i<m_nMeasureLineCount; i++)
					{
						m_arrayMeasureLine[i] = m_arrayMeasureLine[i+1];
					}
				}
				
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{
				CPoint point(0,0);
				point.x = (short)(LOWORD(msg.lParam)); 
				point.y = (short)(HIWORD(msg.lParam)); 
				CMeasureLine line = m_arrayMeasureLine[m_nMeasureLineIndex];
				nLineType = line.m_nType;
				if (line.m_nType == CMeasureLine::LEFT_TO_RIGHT)
				{
					lNewPos = point.y;
				}
				else
				{
					lNewPos = point.x;
				}
				if(DrawMeasureLineTemp(lOldPos, bHasOldPos, lNewPos, nLineType, point))
				{
					lOldPos = lNewPos;
					bHasOldPos = TRUE;
				}
				break;
			}
		default:
			{
				break;
			}
		} // end switch
	} // end for
	
ExitLoop:
	{
		ReleaseCapture();
		AfxUnlockTempMaps(FALSE);	
		this->Invalidate();
		return;
	}
}

BOOL CZoomView::DrawMeasureLineTemp(LONG lOldPos, BOOL &bHasOldPos, LONG lNewPos, UINT nLineType, CPoint point)
{
	CDC  *pDC = this->GetDC();
	
	int mod=pDC->SetROP2(R2_NOTXORPEN);//反色掉
	CPen newPen(PS_DOT, 1, (COLORREF)0);
	CPen *OldPen=pDC->SelectObject(&newPen);
	
	POINT point1, point2, point3, point4;
	
	if (bHasOldPos)
	{
		if (nLineType == CMeasureLine::LEFT_TO_RIGHT)
		{
			point1.x = m_rect.left;
			point1.y = lOldPos;
			point2.x = m_rect.right;
			point2.y = lOldPos;
		}
		else
		{
			point1.x = lOldPos;
			point1.y = m_rect.top;
			point2.x = lOldPos;
			point2.y = m_rect.bottom;
		}
		pDC->MoveTo(point1);
		pDC->LineTo(point2);
	}
	
	if (!m_rect.PtInRect(point))
	{
		bHasOldPos = FALSE;
		pDC->SelectObject(OldPen);
		pDC->SetROP2(mod);
		return FALSE;
	}
	
	if (nLineType == CMeasureLine::LEFT_TO_RIGHT)
	{
		point3.x = m_rect.left;
		point3.y = lNewPos;
		point4.x = m_rect.right;
		point4.y = lNewPos;
	}
	else
	{
		point3.x = lNewPos;
		point3.y = m_rect.top;
		point4.x = lNewPos;
		point4.y = m_rect.bottom;
	}
	pDC->MoveTo(point3);
	pDC->LineTo(point4);
	
	pDC->SelectObject(OldPen);
	newPen.DeleteObject();
	pDC->SetROP2(mod);
	ReleaseDC(pDC);
	return TRUE;
}

void CZoomView::AlignMeasureLineCenter(CPoint &point, CPoint ptDown, CQuad quad)
{
	m_bMeasureLineX = FALSE;
	m_bMeasureLineY = FALSE;
	CDot dot = quad.Center();
	double dbHeight = quad.Height()/2;
	double dbWidth = quad.Width()/2;
	PhysicToLogic(dot.x, dot.y);
	PhysicToLogic(quad);
	for(int i=0; i<m_nMeasureLineCount; i++)
	{
		CMeasureLine line = m_arrayMeasureLine[i];
		if (line.m_nType == CMeasureLine::LEFT_TO_RIGHT)
		{
			double dbx = 0;
			double dby = line.m_dbPos;
			PhysicToLogic(dbx, dby);
			long dist = (long)dby - (long)point.y;
			if (labs(dist) < 15)
			{
				point.y = ptDown.y + (long)dby - (long)dot.y;
				m_bMeasureLineY = TRUE;
				m_dbMeasureLineY = line.m_dbPos;
			}
			else
			{
				long lHeight = (long)(quad.Height()/2);
				dist = (long)dby - point.y;
				long tempDis = labs(dist)-labs(lHeight);
				if (labs(tempDis) < 15)
				{
					if (point.y > dby)
					{
						point.y = ptDown.y + (long)dby - (long)quad.top;
						m_bMeasureLineY = TRUE;
						m_dbMeasureLineY = line.m_dbPos - dbHeight;
					}
					else
					{
						point.y = ptDown.y + (long)dby - (long)quad.bottom;
						m_bMeasureLineY = TRUE;
						m_dbMeasureLineY = line.m_dbPos + dbHeight;
					}
				}
			}
		}
		if (line.m_nType == CMeasureLine::TOP_TO_BOTTOM)
		{
			double dbx = line.m_dbPos;
			double dby = 0;
			PhysicToLogic(dbx, dby);
			long dist = (long)dbx - (long)point.x;
			if (labs(dist) < 15)
			{
				point.x = ptDown.x + (long)dbx - (long)dot.x;
				m_bMeasureLineX = TRUE;
				m_dbMeasureLineX = line.m_dbPos;
			}
			else
			{
				long lWidth = (long)(quad.Width()/2);
				dist = (long)dbx - point.x;
				long tempDis = labs(dist)-labs(lWidth);
				if (labs(tempDis) < 15)
				{
					if (point.x > dbx)
					{
						point.x = ptDown.x + (long)dbx - (long)quad.left;
						m_bMeasureLineX = TRUE;
						m_dbMeasureLineX = line.m_dbPos + dbWidth;
					}
					else
					{
						point.x = ptDown.x + (long)dbx - (long)quad.right;
						m_bMeasureLineX = TRUE;
						m_dbMeasureLineX = line.m_dbPos - dbWidth;
					}
				}
			}
		}
	}
}

void CZoomView::AlignMeasureLineTopBottom(CPoint &point)
{
	m_bMeasureLineX = FALSE;
	m_bMeasureLineY = FALSE;
	for(int i=0; i<m_nMeasureLineCount; i++)
	{
		CMeasureLine line = m_arrayMeasureLine[i];
		if (line.m_nType == CMeasureLine::LEFT_TO_RIGHT)
		{
			double dbx = 0;
			double dby = line.m_dbPos;
			PhysicToLogic(dbx, dby);
			int dist = (int)dby - (int)point.y;
			if (abs(dist) < 10)
			{
				point.y = (long)dby;
				m_bMeasureLineY = TRUE;
				m_dbMeasureLineY = line.m_dbPos;
				break;
			}
		}
	}
}

void CZoomView::AlignMeasureLineLeftRight(CPoint &point)
{
	m_bMeasureLineX = FALSE;
	m_bMeasureLineY = FALSE;
	for (int i=0; i<m_nMeasureLineCount; i++)
	{
		CMeasureLine line = m_arrayMeasureLine[i];
		if (line.m_nType == CMeasureLine::TOP_TO_BOTTOM)
		{
			double dbx = line.m_dbPos;
			double dby = 0;
			PhysicToLogic(dbx, dby);
			int dist = (int)dbx - (int)point.x;
			if (abs(dist) < 10)
			{
				point.x = (long)dbx;
				m_bMeasureLineX = TRUE;
				m_dbMeasureLineX = line.m_dbPos;
				break;
			}
		}
	}
}

void CZoomView::AlignMeasureLine(int nResult, CPoint &point, CPoint ptDown, CQuad quadTemp)
{
	switch (nResult)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		AlignMeasureLineLeftRight(point);
		AlignMeasureLineTopBottom(point);
		break;
	case 4:
	case 6:
		AlignMeasureLineTopBottom(point);
		break;
	case 5:
	case 7:
		AlignMeasureLineLeftRight(point);
		break;
	case 8:
		AlignMeasureLineCenter(point, ptDown, quadTemp);
		break;
	default:
		break;
	}
}

void CZoomView::LastBrush(CWnd *pWnd, CPoint &ptf, CPoint &ptl)
{
	CDC *pDC=pWnd->GetDC();
	CPen *OldPen = NULL;
	
	int mod=pDC->SetROP2(R2_NOTXORPEN);//反色掉
	
	CPen BluePen(PS_DOT,1,RGB(0,0,255));
	OldPen=pDC->SelectObject(&BluePen);
	
	pDC->MoveTo(ptf);
	pDC->LineTo(ptl.x,ptf.y);
	pDC->LineTo(ptl);
	pDC->LineTo(ptf.x,ptl.y);
	pDC->LineTo(ptf);
	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();
  
	pWnd->ReleaseDC(pDC);
	
	return;
}

void CZoomView::DrawRect(CWnd *pWnd,CPoint &ptf,CPoint &ptl,CPoint &point)
{
   	CDC  *pDC = pWnd->GetDC();
	CPen *OldPen = NULL;
	
	int mod=pDC->SetROP2(R2_NOTXORPEN);//反色掉
	CPen BluePen(PS_DOT,1,RGB(0,0,255));
	OldPen=pDC->SelectObject(&BluePen);
	
	pDC->MoveTo(ptf);
	pDC->LineTo(ptl.x,ptf.y);
	pDC->LineTo(ptl);
	pDC->LineTo(ptf.x,ptl.y);
	pDC->LineTo(ptf);
	
	pDC->SelectObject(OldPen);
	OldPen=pDC->SelectObject(&BluePen);
	
	pDC->MoveTo(ptf);
	pDC->LineTo(point.x,ptf.y);
	pDC->LineTo(point);
	pDC->LineTo(ptf.x,point.y);
	pDC->LineTo(ptf);
	
	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();
	pDC->SetROP2(mod);
	pWnd->ReleaseDC(pDC);
	
	return;
}

void CZoomView::FlashCurPos(POINT pt)
{
	CDC* pDC=GetWindowDC();
	DrawCurPos(pDC,pt);
	ReleaseDC(pDC);
}