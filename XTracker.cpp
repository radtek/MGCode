// XTracker.cpp: implementation of the XTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "XTracker.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define HITBORDER 3
#define MIN 1e-15

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern HICON g_hIcoList[20];


void XTracker::SelectStrokeBySelPt(CWnd *pWnd)
{
	CDot selDot;
	if (!FindSelListFirstPt(selDot))
	{
		return;
	}

	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	BOOL     bFind   = FALSE;
	while(pos)
	{
		pStroke = m_StrokeList.GetNext(pos);
		if (pStroke)
		{
			if ((pStroke->DotIsInStroke(selDot)) && (pStroke->m_bTag))
			{
				bFind = TRUE;
				break;
			}
		}
	}// end while

	if (!bFind) 
	{
		return;
	}

	InverDrawDot(pWnd);
	ClearDot();

	AddStrokeToEditList(pStroke);
	FillRect(pWnd);
	DataEXChange();

	return;
}

void XTracker::SetDotToStart(CWnd *pWnd)
{
	CDot SelDt;
	int nCount = FindSelListFirstPt(SelDt);
	if (nCount > 1)
	{
		CString str("");
		str.LoadString(IDS_SELONEDOT_ONLY);
		AfxMessageBox(str);

		return;
	}

	InverDrawDot(pWnd);
	ClearDot();
	
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos)
	{
		pStroke = m_StrokeList.GetNext(pos);
		if (pStroke)
		{
			if (pStroke->m_bTag)
			{
				pStroke->SetDotToStart(SelDt);
				break;
			}
		}
	}// end while

	AddStrokeToEditList(pStroke);
	FillRect(pWnd);
	DataEXChange();

	return;
}

void XTracker::FilterPoint(CWnd *pWnd, double dbPresic, double dbAngle)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos)
	{
		pStroke = m_StrokeList.GetNext(pos);
		if (pStroke)
		{
			if (pStroke->m_bTag)
			{
				pStroke->FilterPoint(dbPresic, dbAngle);
			}
		}

	}// end while
	
	return;
}

void XTracker::ReverseStroke(CWnd *pWnd)
{
	CDot SelDt;
	if (!FindSelListFirstPt(SelDt))
	{
		return;
	}

	InverDrawDot(pWnd);
	ClearDot();
	
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos)
	{
		pStroke = m_StrokeList.GetNext(pos);
		if (pStroke)
		{
			if (pStroke->m_bTag)
			{
				pStroke->SetStrokeReverse(SelDt);
				break;
			}
		}
	}// end while

	AddStrokeToEditList(pStroke);
	FillRect(pWnd);
	DataEXChange();

	return;
}

void XTracker::DotHead::InverDrawDot(CWnd *pWnd)//反画
{
	if(NULL == m_pHead)
	{
		return;
	}

	CHLView *pView=(CHLView*)pWnd;
	CDC *pDC      =pView->GetDC();
	CRect rect;
	CQuad quad;
	CDot  dot;
	DotSelectList *plist=m_pHead;
	double dbdata=2;
	pView->ActualLen(dbdata);
	while(plist)
	{
		dot=plist->m_Node;
		quad.left=dot.x-dbdata;
		quad.right=dot.x+1.75*dbdata;
		quad.top=dot.y+dbdata;
		quad.bottom=dot.y-1.75*dbdata;
		pView->PhysicToLogic(quad);
		rect=(CRect)quad;
		pDC->FillSolidRect(&rect,RGB(255,255,255));
		plist=plist->m_pNext;
	}
	pView->ReleaseDC(pDC);
}

XTracker::XTracker()
{
	m_quadSave.left=m_quadSave.right=m_quadSave.top=m_quadSave.bottom=0;
	m_ptNow=m_ptPrev=CPoint(0,0);
	m_HITBORDER=0;
	m_pDotHead=m_pDotTail=NULL;
}

XTracker::~XTracker()
{
}

int XTracker:: HitPoint(CTree *pTree,CPoint &point,CWnd *pWnd)
{
	for(int i=0;i<8;i++)
	{
		if(CtrlPoint(i,point,pTree,pWnd))
		{
			return i;
		}
	}
	
	if(InRect(point,pTree,pWnd))
	{
		return 8;
	}

	return 9;
}

BOOL XTracker::CtrlPoint(int i, CPoint &point,CTree *pTree,CWnd *pWnd)
{
	if(IsPtInRect(i,point,pTree,pWnd))
	{
		return TRUE;
	}
	
	return FALSE;
}


BOOL XTracker::InRect(CPoint &point,CTree *pTree,CWnd *pWnd)
{
	double dx=point.x;
	double dy=point.y;
	m_quadSave     = pTree->GetLimit();
	CHLView *pView = (CHLView*)pWnd;
	pView->LogicToPhysic(dx,dy);

	if(dx>=m_quadSave.left && dx<=m_quadSave.right && dy<=m_quadSave.top && dy>=m_quadSave.bottom)
	{
		return TRUE;
	}

	return FALSE;
}

void XTracker:: UpdateRectCtrl()
{
	(m_quad[0]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[0]).right = m_quadSave.left;
	(m_quad[0]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[0]).bottom= m_quadSave.top;

	(m_quad[1]).left  = m_quadSave.right;
	(m_quad[1]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[1]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[1]).bottom= m_quadSave.top;

	(m_quad[2]).left  = m_quadSave.right;
	(m_quad[2]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[2]).top   = m_quadSave.bottom;
	(m_quad[2]).bottom= m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[3]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[3]).right = m_quadSave.left;
	(m_quad[3]).top   = m_quadSave.bottom;
	(m_quad[3]).bottom= m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[4]).left  = (m_quadSave.left+m_quadSave.right)/2-m_HITBORDER;
	(m_quad[4]).right = (m_quadSave.left+m_quadSave.right)/2+m_HITBORDER;
	(m_quad[4]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[4]).bottom= m_quadSave.top;

	(m_quad[5]).left  = m_quadSave.right;
	(m_quad[5]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[5]).top   = (m_quadSave.top+m_quadSave.bottom)/2+m_HITBORDER;
	(m_quad[5]).bottom= (m_quadSave.top+m_quadSave.bottom)/2-m_HITBORDER;

	(m_quad[6]).left  = (m_quadSave.left+m_quadSave.right)/2-m_HITBORDER;
	(m_quad[6]).right = (m_quadSave.left+m_quadSave.right)/2+m_HITBORDER;
	(m_quad[6]).top   = m_quadSave.bottom;
	(m_quad[6]).bottom= m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[7]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[7]).right = m_quadSave.left;
	(m_quad[7]).top   = (m_quadSave.top+m_quadSave.bottom)/2+m_HITBORDER;
	(m_quad[7]).bottom= (m_quadSave.top+m_quadSave.bottom)/2-m_HITBORDER;

	return;
}


BOOL XTracker::IsPtInRect(int i, CPoint &point,CTree *pTree,CWnd *pWnd)
{
	double dx = point.x;
	double dy = point.y;

	m_quadSave    = pTree->GetLimit();
	CHLView *pView= (CHLView*)pWnd;
	pView->LogicToPhysic(dx,dy);
	m_HITBORDER=HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();

	CQuad quad=m_quad[i];
	if(dx>=(m_quad[i]).left && dx<=(m_quad[i]).right && dy>=(m_quad[i]).bottom && dy<=(m_quad[i]).top)
	{
		return TRUE;
	}
	
	return FALSE;
}



//////////////////////////////////////////////////////////////////////////
//                0 _____4_____  1				
//				   |           |
//				  7|     8     | 5
//				   |           |
//                 |___________| 
//                3      6       2
//////////////////////////////////////////////////////////////////////////
void XTracker::FromHandle(int nResult, double &ratioX, double &ratioY, double &MoveX, double &MoveY, CPoint ptOrg, CPoint &point,CQuad &quad,CDot &dot,CWnd *pWnd)
{
	CHLView *pView=(CHLView*)pWnd;
	CDot dot2;
	dot2.x=point.x;
	dot2.y=point.y;
	double dx,dy;
	pView->LogicToPhysic(dot2.x,dot2.y);
	if (pView->m_bMeasureLineX)
	{
		dot2.x = pView->m_dbMeasureLineX;
	}
	if (pView->m_bMeasureLineY)
	{
		dot2.y = pView->m_dbMeasureLineY;
	}
	switch(nResult)
	{
	case 0:
		{
			dx=dot2.x-dot.x;
			dy=dot2.y-dot.y;
	
			ratioX=dx/(quad.left-dot.x);
			ratioY=dy/(quad.top-dot.y);
			MoveX=MoveY=0;
			ratioX<ratioY ? ratioY=ratioX :ratioX=ratioY;
			break;
		}
	case 1:
		{
			dx=dot2.x-dot.x;
			dy=dot2.y-dot.y;
			
			ratioX=dx/(quad.right-dot.x);
			ratioY=dy/(quad.top-dot.y);
			MoveX=MoveY=0;
			ratioX<ratioY ? ratioY=ratioX :ratioX=ratioY;
			break;
		}
	case 2:
		{
			dx=dot2.x-dot.x;
			dy=dot2.y-dot.y;
			
			ratioX=dx/(quad.right-dot.x);
			ratioY=dy/(quad.bottom-dot.y);
			MoveX=MoveY=0;
			ratioX<ratioY ? ratioY=ratioX :ratioX=ratioY;
			break;
		}
	case 3:
		{
			dx=dot2.x-dot.x;
			dy=dot2.y-dot.y;
			
			ratioX=dx/(quad.left-dot.x);
			ratioY=dy/(quad.bottom-dot.y);
			ratioX<ratioY ? ratioY=ratioX :ratioX=ratioY;
			MoveX=MoveY=0;
			break;
		}
	case 4:
		{
			
			ratioY=(dot2.y-dot.y)/(quad.top-dot.y);
			ratioX=1;
			MoveX=MoveY=0;
			break;
		}
	case 5:
		{
		
			ratioX=(dot2.x-dot.x)/(quad.right-dot.x);
			ratioY=1;
			MoveX=MoveY=0;
			break;
		}
	case 6:
		{	
			
			ratioY=(dot2.y-dot.y)/(quad.bottom-dot.y);
			ratioX=1;
			
			MoveX=MoveY=0;
			break;
		}
	case 7:
		{
			ratioX=(dot2.x-dot.x)/(quad.left-dot.x);
			ratioY=1;
			MoveX=MoveY=0;
			break;
		}
	case 8:
		{	
			MoveX=point.x-ptOrg.x;
			MoveY=point.y-ptOrg.y;
			pView->ActualLen(MoveX);
			pView->ActualLen(MoveY);
			MoveY*=-1;
			ratioX=ratioY=1;
			break;
		}
	case 9:
		{
			break;
		}
	}
}

int XTracker::PtHitQuad(CPoint &pt, CQuad &quad,CWnd *pWnd)
{
	CHLView *pView=(CHLView *)pWnd;
	m_quadSave=quad;
	m_HITBORDER=HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();
	CDot dot, dt;
	dot.x=pt.x;
	dot.y=pt.y;
	dt = dot;
	pView->LogicToPhysic(dot.x,dot.y);
	for(int i=0; i < 8; i++)
	{
		CDot mDot = dt;
		CQuad  mQuad = m_quad[i];
		pView->PhysicToLogic(mQuad.left, mQuad.top);
		pView->PhysicToLogic(mQuad.right, mQuad.bottom);
		switch(i)
		{
		case 0:
			{
				mDot.SetOffset(10, 10);
				break;
			}
		case 1:
			{
				mDot.SetOffset(-10, 10);
				break;
			}
		case 2:
			{
				mDot.SetOffset(-10, -10);
				break;
			}
		case 3:
			{
				mDot.SetOffset(10, -10);
				break;
			}
		case 4:
			{
				mDot.SetOffset(0, 10);
				break;
			}
		case 5:
			{
				mDot.SetOffset(-10, 0);
				break;
			}
		case 6:
			{
				mDot.SetOffset(0, -10);
				break;
			}
		case 7:
			{
				mDot.SetOffset(10, 0);
				break;
			}
		default:
			{
				break;
			}
		}

		if(mQuad.DotIn(mDot))
		{
			switch(i)
			{
			case 0:
				{
					mDot.SetOffset(-10, -10);
					break;
				}
			case 1:
				{
					mDot.SetOffset(10, -10);
					break;
				}
			case 2:
				{
					mDot.SetOffset(10, 10);
					break;
				}
			case 3:
				{
					mDot.SetOffset(-10, 10);
					break;
				}
			case 4:
				{
					mDot.SetOffset(0, -10);
					break;
				}
			case 5:
				{
					mDot.SetOffset(10, 0);
					break;
				}
			case 6:
				{
					mDot.SetOffset(0, 10);
					break;
				}
			case 7:
				{
					mDot.SetOffset(-10, 0);
					break;
				}
			default:
				{
					break;
				}
			}

			pt.x = (long)(mDot.x);
			pt.y = (long)(mDot.y);

			return i;
		}
	}

	double dbScale = HITBORDER;
	pView->ActualLen(dbScale);
	CDot dtC = quad.Center();
	CQuad mQD;
	double dblen = 1.5 * dbScale;
	mQD.left   = dtC.x - dblen;
	mQD.top    = dtC.y + dblen;
	mQD.right  = dtC.x + dblen;
	mQD.bottom = dtC.y - dblen;
	if(mQD.DotIn(dot))
	{
		return 8;
	}

	return 9;
}

BOOL XTracker::GetDragPara(int &nResult,CQuad &quad,CDot &dot, double &ratioX, double &ratioY, double &MoveX, double &MoveY,CWnd *pWnd)
{
	if (9 == nResult) 
	{
		return FALSE;
	}

	AfxLockTempMaps();
	pWnd->SetCapture();
	if (CWnd::GetCapture() != pWnd)
	{
		return FALSE;
	}
	
	CPoint point=m_ptDown;
	m_OldQuad.Initial();
	m_NewQuad     = quad;
	CQuad quadTemp= quad;
	ModifyDot(nResult,dot,quad);
	CDot dotTemp=dot;
	MSG msg;
	for(;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				//最后一次修改得到最终结果
				if (8 == nResult) 
				{
					Handle(nResult,ratioX,ratioY,MoveX,MoveY,dot,point,quad,pWnd);
				}
				m_NewQuad.Initial();
				if (m_OldQuad.IsEmpty()) 
				{
					ratioX = 1.0;
					ratioY = 1.0;
				}
				else
				{
					DrawQuad(nResult,dotTemp,ratioX,ratioY,MoveX,MoveY,m_OldQuad,m_NewQuad,pWnd);
				}
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{		
				point.x=(int)(short)LOWORD(msg.lParam);
				point.y=(int)(short)HIWORD(msg.lParam);
				CHLView *pView = (CHLView*)pWnd;
				pView->AlignMeasureLine(nResult, point, m_ptMeasureLine, quad);
				FromHandle(nResult,ratioX,ratioY,MoveX,MoveY,m_ptDown,point,quadTemp,dotTemp,pWnd);
				DrawQuad(nResult,dotTemp,ratioX,ratioY,MoveX,MoveY,m_OldQuad,m_NewQuad,pWnd);
				if(nResult==8)
				m_OldQuad=m_NewQuad;
				m_ptDown =point;
				break;
			}
		}
	}
ExitLoop:
	{ 
		ReleaseCapture();
		AfxUnlockTempMaps();
		return TRUE;
	}
}


void XTracker::DrawQuad(int nResult,CDot &dot, double &ratioX, double &ratioY, double &MoveX, double &MoveY,CQuad &quad1,CQuad &quad2,CWnd *pWnd)
{
	//dot 为不动点,画虚线外包框
	CHLView *pView = (CHLView*)pWnd;
	CDC     *pDC   = pView->GetDC();
	XDC     *pXDC  = pView->PrepareDC(pDC);
	CPen  BlackPen(PS_DOT,1,RGB(0,0,0));
	CPen *OldPen   = pDC->SelectObject(&BlackPen);
	CQuad oldQuad  = quad1;
	CQuad Quad     = quad2;

	CDot Dot;
	double tx(0),ty(0);
	double dx(0),dy(0);
	dx = dot.x;
	dy = dot.y;

	// 反色掉
	int mod=pDC->SetROP2(R2_NOTXORPEN);
	//draw Quad
	{
		pXDC->MoveTo(oldQuad.left,oldQuad.top);
		pXDC->LineTo(oldQuad.right,oldQuad.top);
		pXDC->LineTo(oldQuad.right,oldQuad.bottom);
		pXDC->LineTo(oldQuad.left,oldQuad.bottom);
		pXDC->LineTo(oldQuad.left,oldQuad.top);
		
        //比例缩放加平移
		Dot.x = quad2.left;
		Dot.y = quad2.top;
		tx    = Dot.x-dx;
		Dot.x = ratioX*tx+dx;
		ty    = Dot.y-dy;
		Dot.y = ratioY*ty+dy;

		Quad.left = Dot.x;
		Quad.top  = Dot.y;
		Quad.left+= MoveX;
		Quad.top += MoveY;

		Dot.x = quad2.right;
		Dot.y = quad2.top;
		tx    = Dot.x-dx;
		Dot.x = ratioX*tx+dx;
		ty    = Dot.y-dy;
		Dot.y = ratioY*ty+dy;

		Quad.right = Dot.x;
		Quad.top   = Dot.y;
		Quad.right+= MoveX;
		Quad.top  += MoveY;

		Dot.x = quad2.right;
		Dot.y = quad2.bottom;
		tx    = Dot.x - dx;
		Dot.x = ratioX * tx + dx;
		ty    = Dot.y - dy;
		Dot.y = ratioY * ty + dy;

		Quad.right   = Dot.x;
		Quad.bottom  = Dot.y;
		Quad.right  += MoveX;
		Quad.bottom += MoveY;

		Dot.x = quad2.left;
		Dot.y = quad2.bottom;
		tx    = Dot.x - dx;
		Dot.x = ratioX * tx + dx;
		ty    = Dot.y - dy;
		Dot.y = ratioY * ty+dy;

		Quad.left   = Dot.x;
		Quad.bottom = Dot.y;
		Quad.left   += MoveX;
		Quad.bottom += MoveY;

        //重绘		
		pXDC->MoveTo(Quad.left,Quad.top);
		pXDC->LineTo(Quad.right,Quad.top);
		pXDC->LineTo(Quad.right,Quad.bottom);
		pXDC->LineTo(Quad.left,Quad.bottom);
		pXDC->LineTo(Quad.left,Quad.top);

		if(nResult==8)
		{
			quad2=Quad;
		}
		else
		{
			quad1=Quad;
		}
	}

	pDC->SetROP2(mod);
	pDC->SelectObject(OldPen);
	BlackPen.DeleteObject();
	pView->ReleaseDC(pDC);

	return;
}



void XTracker::ModifyDot(int nResult,CDot &dot,CQuad &quad)
{
	switch(nResult)
	{
	case 0:
		{
			dot.x=quad.right;
			dot.y=quad.bottom;
			break;
		}
	case 1:
		{
			dot.x=quad.left;
			dot.y=quad.bottom;
			break;
		}
	case 2:
		{
			dot.x=quad.left;
			dot.y=quad.top;
			break;
		}
	case 3:
		{
			dot.x=quad.right;
			dot.y=quad.top;
			break;
		}
	case 4:
		{
			dot.x=(quad.right+quad.left)/2;
			dot.y=quad.bottom;
			break;
		}
	case 5:
		{
			dot.x=quad.left;
			dot.y=(quad.top+quad.bottom)/2;
			break;
		}
	case 6:
		{	
			dot.x=(quad.left+quad.right)/2;
			dot.y=quad.top;
			break;
		}
	case 7:
		{
			dot.x=quad.right;
			dot.y=(quad.top+quad.bottom)/2;
			break;
		}
	case 8:
		{	
			dot.x=m_ptDown.x;
			dot.y=m_ptDown.y;
			m_dtQuadCenter.x = (quad.left+quad.right)/2;
			m_dtQuadCenter.y = (quad.top+quad.bottom)/2;
			break;
		}
	case 9:
		{
			break;
		}
	}
}

void XTracker::Handle(int nResult, double &ratioX, double &ratioY, double &MoveX, double &MoveY, CPoint dot, CPoint &point,CQuad &quad, CWnd *pWnd)
{
	CHLView *pView = (CHLView*)pWnd;
	CDot dot2;
	dot2.x=point.x;
	dot2.y=point.y;
	double dx,dy;
	pView->LogicToPhysic(dot2.x,dot2.y);			
	if (pView->m_bMeasureLineX)
	{
		dot2.x = pView->m_dbMeasureLineX;
	}
	if (pView->m_bMeasureLineY)
	{
		dot2.y = pView->m_dbMeasureLineY;
	}
	switch(nResult)
	{
	case 0:
		{
			dx=dot2.x-dot.x;
			dy=dot2.y-dot.y;
			
			ratioX=dx/(quad.left-dot.x);
			ratioY=dy/(quad.top-dot.y);
			ratioX<ratioY ? ratioY=ratioX :ratioX=ratioY;
			MoveX=MoveY=0;
			break;
		}
	case 1:
		{
			dx=dot2.x-dot.x;
			dy=dot2.y-dot.y;
			
			ratioX=dx/(quad.right-dot.x);
			ratioY=dy/(quad.top-dot.y);
			ratioX<ratioY ? ratioY=ratioX :ratioX=ratioY;
			MoveX=MoveY=0;
			break;
		}
	case 2:
		{
			dx=dot2.x-dot.x;
			dy=dot2.y-dot.y;
			
			ratioX=dx/(quad.right-dot.x);
			ratioY=dy/(quad.bottom-dot.y);
			ratioX<ratioY ? ratioY=ratioX :ratioX=ratioY;
			MoveX=MoveY=0;
			break;
		}
	case 3:
		{
			dx=dot2.x-dot.x;
			dy=dot2.y-dot.y;
			
			ratioX=dx/(quad.left-dot.x);
			ratioY=dy/(quad.bottom-dot.y);
			ratioX<ratioY ? ratioY=ratioX :ratioX=ratioY;
			MoveX=MoveY=0;
			break;
		}
	case 4:
		{
			
			ratioY=(dot2.y-dot.y)/(quad.top-dot.y);
			ratioX=1;
			MoveX=MoveY=0;
			break;
		}
	case 5:
		{
		
			ratioX=(dot2.x-dot.x)/(quad.right-dot.x);
			ratioY=1;
			MoveX=MoveY=0;
			break;
		}
	case 6:
		{	
			
			ratioY=(dot2.y-dot.y)/(quad.bottom-dot.y);
			ratioX=1;
			
			MoveX=MoveY=0;
			break;
		}
	case 7:
		{
			ratioX=(dot2.x-dot.x)/(quad.left-dot.x);
			ratioY=1;
			MoveX=MoveY=0;
			break;
		}
	case 8:
		{	
			MoveX=point.x-dot.x;
			MoveY=point.y-dot.y;
			pView->ActualLen(MoveX);
			pView->ActualLen(MoveY);
			MoveY*=-1;
			if (pView->m_bMeasureLineX)
			{
				MoveX = dot2.x - m_dtQuadCenter.x;
			}
			if (pView->m_bMeasureLineY)
			{
				MoveY = dot2.y - m_dtQuadCenter.y;
			}
			ratioX=ratioY=1;
			break;
		}
	case 9:
		{
			break;
		}
	}
}

int XTracker::GetResult(CPoint &point, CQuad &quad,CWnd *pWnd)
{
	int nResult=-1;
	nResult=PtHitQuad(point,quad,pWnd);
	m_ptDown.x=point.x;
	m_ptDown.y=point.y;
	return nResult;
}

void XTracker::DrawSelTraker(XDC *pDC, CQuad &Quad,CWnd *pWnd, CArray<SELQUAD, SELQUAD> &QuadList)
{
	DrawTracker(pDC,Quad,pWnd);

	int nCount = QuadList.GetSize();
	if (nCount <= 1)
	{
		return;
	}

	BOOL bFill  = FALSE;
	int  nIndex = -1;
	
	for (int i =0; i < nCount; i++) 
	{
		if (!QuadList[i].m_bSel)
		{
			continue;
		}

		if (bFill)
		{
			QuadList[i].m_bFill = FALSE;
			CTree *pTree = ((CTree *)QuadList[i].m_pGroupObj);
			pTree->SetMainSelect(FALSE);
		}

		if (QuadList[i].m_bFill) 
		{
			DrawSelObjTracker(pDC, QuadList[i].m_QD, TRUE, pWnd);
			bFill = TRUE;
		}
		else
		{
			DrawSelObjTracker(pDC, QuadList[i].m_QD, FALSE, pWnd);
		}

		nIndex = i;
	}

	if ((!bFill) && (nIndex >= 0))
	{
		QuadList[nIndex].m_bFill = TRUE;
		CTree *pTree = ((CTree *)QuadList[nIndex].m_pGroupObj);
		pTree->SetMainSelect(TRUE);
		DrawSelObjTracker(pDC, QuadList[nIndex].m_QD, TRUE, pWnd);
	}

	return;
}

void XTracker::DrawSelObjTracker(XDC *pDC, CQuad &Quad, BOOL bFillQuad, CWnd *pWnd)
{
	CPen BluePen(PS_SOLID, 0, BLUE);
	CPen *OldPen=pDC->SelectObject(&BluePen);
	double x,y;
	x=y=1.5;
	CHLView *pView=(CHLView*)pWnd;
	CQuad quad=Quad;
	pView->ActualLen(x);
	pView->ActualLen(y);
	quad.Inflate(x,y);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
	m_quadSave=quad;
	m_HITBORDER=HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();
	for(int i=0;i<8;i++)
	{
		if ((7 == i) || (6 == i) || (5 == i) || (8 == i) || (4 == i))
		{
			continue;
		}
		pDC->MoveTo(m_quad[i].left,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].top);
		if (bFillQuad)
		{
			pDC->FillSolidQuad(m_quad[i],BLUE);
		}
	}
	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();

	return;
}


void XTracker::DrawTracker(XDC *pDC, CQuad &Quad,CWnd *pWnd)
{
	//draw outline
	CPen BluePen(PS_DOT,0,BLUE);
	CPen *OldPen=pDC->SelectObject(&BluePen);
	double x,y;
	x=y=1.5;
	CHLView *pView=(CHLView*)pWnd;
	CQuad quad=Quad;
	pView->ActualLen(x);
	pView->ActualLen(y);
	quad.Inflate(x,y);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
	m_quadSave=quad;
	m_HITBORDER=HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();

	CDC *pMDC = pView->GetDC();
	for(int i=0; i<8; i++)
	{
		CQuad  mQuad = m_quad[i];
		switch(i)
		{
		case 0:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[3]);
				break;
			}
		case 1:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[2]);
				break;
			}
		case 2:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[3]);
				break;
			}
		case 3:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[2]);
				break;
			}
		case 4:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(0, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[1]);
				break;
			}
		case 5:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, 0);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[4]);
				break;
			}
		case 6:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(0, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[1]);
				break;
			}
		case 7:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, 0);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[4]);
				break;
			}
		default:
			{
				break;
			}
		}
/*		pDC->MoveTo(m_quad[i].left,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].top);
		pDC->FillSolidQuad(m_quad[i],BLUE);
*/
	}

	CDot dot  = Quad.Center();
	pView->PhysicToLogic(dot.x, dot.y);
	pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[0]);

	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();
 
	pView->ReleaseDC(pMDC);
}

void XTracker::Draw(CQuad &Quad, CWnd *pWnd)
{
	CPen BluePen(PS_DOT,0,BLUE);
	double x,y;
	x=y=1;
	CHLView *pView=(CHLView*)pWnd;
	pView->ActualLen(x);
	pView->ActualLen(y);
	CQuad quad=Quad;
	quad.Inflate(x,y);
	CDC *pdc=pView->GetDC();
	XDC *pDC=pView->PrepareDC(pdc);
	CPen *OldPen=pDC->SelectObject(&BluePen);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
	m_quadSave=quad;
	m_HITBORDER=HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();
	for(int i=0;i<8;i++)
	{
		pDC->MoveTo(m_quad[i].left,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].top);
		pDC->FillSolidQuad(m_quad[i],BLUE);
	}
	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();
	pView->ReleaseDC(pdc);
}

void XTracker::InverDraw(CQuad &Quad, CWnd *pWnd)
{
	CPen BluePen(PS_DOT,0,BLUE);
	double x,y;
	x=y=1;
	CHLView *pView=(CHLView*)pWnd;
	pView->ActualLen(x);
	pView->ActualLen(y);
	CQuad quad=Quad;
	quad.Inflate(x,y);
	CDC *pdc=pView->GetDC();
	XDC *pDC=pView->PrepareDC(pdc);
	CPen *OldPen=pDC->SelectObject(&BluePen);
	int mod=pDC->SetROP2(R2_NOTXORPEN);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
	m_quadSave=quad;
	m_HITBORDER=HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();
	for(int i=0;i<8;i++)
	{
		pDC->MoveTo(m_quad[i].left,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].top);
		pDC->FillSolidQuad(m_quad[i],BLUE);
	}
	pDC->SetROP2(mod);
	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();
	pView->ReleaseDC(pdc);
}

void XTracker::GetTrueRect(CRect &rect)
{
	rect=m_rect;
	rect.InflateRect(9,9);
}

void XTracker::SetPhysicQuad(CWnd *pWnd, CQuad &quad)
{
	CHLView *pView = (CHLView*)pWnd;
	CQuad quadTemp = quad;
	pView->PhysicToLogic(quadTemp);
	m_rect = (CRect)quadTemp;
}

void XTracker::SingleModify(CPoint &point,CWnd *pWnd)
{
	AfxLockTempMaps();
	pWnd->SetCapture();
	if (CWnd::GetCapture() != pWnd)
	{
		return;
	}

	MSG msg;
	CQuad quad;
	CPoint point1,point2;
	point1=point;
	for(;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{
				point2.x=(int)(short)LOWORD(msg.lParam);
				point2.y=(int)(short)HIWORD(msg.lParam);
				DrawNode(point1,point2,pWnd);
				point1=point2;
			}
		default:
			{
				break;
			}
		}
	}
ExitLoop:
	{ 
		ReleaseCapture();
		AfxUnlockTempMaps(FALSE);
	}
}

void XTracker::DrawNode(CPoint &pt1,CPoint &pt2, CWnd *pWnd)//pt1为旧坐标，pt2为新坐标
{
	if(m_pDotHead==NULL)
	{
		return;
	}
	
	DotHead *phd=m_pDotHead;
	CHLView *pView=(CHLView*)pWnd;
	CDot dot1,dot2;
	CDC *pDC=pView->GetDC();
	CPen GreenPen(PS_DOT,1,GREEN);
	CPen *OldPen=pDC->SelectObject(&GreenPen);
	int mode=pDC->SetROP2(R2_NOTXORPEN);
	XDC *pXDC=pView->PrepareDC(pDC);
	DotSelectList *pList;
	CDot dt;		
	while(phd)
	{
		dot1.x=pt1.x;
		dot1.y=pt1.y;
		dot2.x=pt2.x;
		dot2.y=pt2.y;
		pView->LogicToPhysic(dot1.x,dot1.y);
		pView->LogicToPhysic(dot2.x,dot2.y);
		pList=phd->m_pHead;
		while(pList)
		{
			//先画右边
			dt=pList->m_Node;
			pXDC->MoveTo(dt.x,dt.y);
			if(pList->bRight && !pList->bRightDraw)
				pXDC->LineTo(pList->m_pRight->x,pList->m_pRight->y);
			pList=pList->m_pNext;
		}
		pList=phd->m_pTail;
		while(pList)
		{
			//再画左边
			dt=pList->m_Node;
			pXDC->MoveTo(dt.x,dt.y);
			if(!pList->bLeftDraw)
				pXDC->LineTo(pList->m_pLeft->x,pList->m_pLeft->y);
			pList=pList->m_pFront;
		}
		//数据转换
		pList=phd->m_pHead;
		while(pList)
		{
			dt=pList->m_Node;
			dt.x+=dot2.x-dot1.x;
			dt.y+=dot2.y-dot1.y;
			pList->m_Node=dt;
			pList=pList->m_pNext;
		}
		//重绘
		pList=phd->m_pHead;
		while(pList)
		{
			//先画右边
			dt=pList->m_Node;
			pXDC->MoveTo(dt.x,dt.y);
			if(pList->bRight && !pList->bRightDraw)
				pXDC->LineTo(pList->m_pRight->x,pList->m_pRight->y);
			pList=pList->m_pNext;
		}
		pList=phd->m_pTail;
		while(pList)
		{
			//再画左边
			dt=pList->m_Node;
			pXDC->MoveTo(dt.x,dt.y);
			if(!pList->bLeftDraw)
				pXDC->LineTo(pList->m_pLeft->x,pList->m_pLeft->y);
			pList=pList->m_pFront;
		}
			phd=phd->m_pNext;
	}
	pDC->SetROP2(mode);
	pDC->SelectObject(OldPen);
	GreenPen.DeleteObject();
	pView->ReleaseDC(pDC);
}


void XTracker::ClearStrokeList()
{
	POSITION pre,pos;
	CStroke *pStroke;
	pos=m_StrokeList.GetHeadPosition();
	while(pos)
	{
		pre=pos;
		pStroke=m_StrokeList.GetNext(pos);
		m_StrokeList.RemoveAt(pre);
	}
}

BOOL XTracker::HaveObject()
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	if(pos!=NULL)
		return TRUE;
	return FALSE;
}

BOOL XTracker::HitSelectDot(CPoint &point, CWnd *pWnd)
{
	CDot dotTest;
	CHLView *pView=(CHLView*)pWnd;
	dotTest.x=point.x;
	dotTest.y=point.y;
	pView->LogicToPhysic(dotTest.x,dotTest.y);
	double dbdata=3;
	pView->ActualLen(dbdata);
	BOOL bRet =  HitSelectDotB(dotTest, dbdata);
	return bRet;
}


BOOL XTracker::HitOtherDot(CPoint &point, CWnd *pWnd)
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if(pStroke->IsOnDot(point,pWnd) && !HitSelectDot(point,pWnd))
			return TRUE;
	}
	return FALSE;
}

void XTracker::GetDot(CPoint &point,CWnd *pWnd)
{
	CDot dot;
	dot.x=point.x;
	dot.y=point.y;
	CHLView *pView=(CHLView*)pWnd;
	pView->LogicToPhysic(dot.x,dot.y);
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if(pStroke->m_bTag)
		{
			AddDotHead(pStroke,dot,pWnd);
		}
	}

	return;
}

void XTracker::GetDot(CQuad &quad)
{
	CQuad quadb;
	quadb = quad;
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if(pStroke->m_bTag)
		{
			AddDotHeadB(pStroke,quadb);
		}
	}

	return;
}

void XTracker::SearchDot(CPoint &point,CWnd *pWnd)
{
	CStroke *pStroke = NULL;
	POSITION pos=m_StrokeList.GetHeadPosition();
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		pStroke->SetFlag(FALSE);
	}

	CHLView *pView=(CHLView*)pWnd;
	CDot dot;
	dot.x=point.x;
	dot.y=point.y;
	pView->LogicToPhysic(dot.x,dot.y);
	pos=m_StrokeList.GetHeadPosition();
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if(pStroke->DotRight(dot,pWnd))
		{
			pStroke->SetFlag(TRUE);
			break;
		}
	}
}

void XTracker::SearchDot(CQuad &quad)
{
	CStroke *pStroke = NULL;
	POSITION pos=m_StrokeList.GetHeadPosition();
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		pStroke->SetFlag(FALSE);
	}
	pos=m_StrokeList.GetHeadPosition();
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if(pStroke->DotInQuad(quad))
		{
			pStroke->SetFlag(TRUE);
		}
	}

	return;
}

void XTracker::AddDotHead(CStroke *pStroke,CDot &dotIndex,CWnd *pWnd)
{
	CHLView *pView=(CHLView*)pWnd;
	double dbdata=3;
	pView->ActualLen(dbdata);
	AddDotToHead(pStroke, dotIndex, dbdata);

	return;
}

DWORD XTracker::FillRect(CWnd *pWnd)
{
	if(NULL == m_pDotHead)
	{
		return 0;
	}

	CHLView *pView = (CHLView*)pWnd;
	CDC     *pDC   = pView->GetDC();

	CRect rect;
	CQuad quad;
	CDot  dot;
	DotSelectList *plist = NULL;
	double dbdata = 2;
	DWORD  dwDumpPt = 0;

	pView->ActualLen(dbdata);
	CDot dt;
	dt.x = -9999999999999;
	dt.y = -9999999999999;
	DotHead *pHead = m_pDotHead;
	BOOL bIsFirstDot = FALSE;
	while(pHead)
	{
		plist = pHead->m_pHead;
		while(plist)
		{
			dot         = plist->m_Node;
			
			double dbDs = Distance(dt, dot);
			if (dbDs < g_SysCfg.m_dbDotFilterPrecis)
			{
				dwDumpPt = 1;
			}
			
			dt = dot;
			quad.left   = dot.x-dbdata;
			quad.right  = dot.x+1.75*dbdata;
			quad.top    = dot.y+dbdata;
			quad.bottom = dot.y-1.75*dbdata;

			pView->PhysicToLogic(quad);
			rect = (CRect)quad;
			if (bIsFirstDot)
			{
				bIsFirstDot = FALSE;
				pDC->FillSolidRect(&rect,RED);
			}
			else
			{
				pDC->FillSolidRect(&rect,BLACK);
			}
			plist = plist->m_pNext;
		}
		pHead=pHead->m_pNext;
	}

	pView->ReleaseDC(pDC);

	return dwDumpPt;
}


void XTracker::ClearAndStore(CPoint &point, CWnd *pWnd)
{
	InverDrawDot(pWnd);
	SearchDot(point,pWnd);
	ClearDot();
	GetDot(point,pWnd);
	FillRect(pWnd);
	DataEXChange();
}

BOOL XTracker::HasDot(CQuad &quad)
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if(pStroke->HasDot(quad))
			return TRUE;
	}
	return FALSE;
}

void XTracker::ClearAndStore(CQuad &quad, CWnd *pWnd)
{
	InverDrawDot(pWnd);
	SearchDot(quad);
	ClearDot();
	GetDot(quad);
	FillRect(pWnd);
	DataEXChange();
}

void XTracker::DelRect(CWnd *pWnd)
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		pStroke->DelRect(pWnd);
	}
}

BOOL XTracker::IsOnDot(CPoint &point, CWnd *pWnd)
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if(pStroke->IsOnDot(point,pWnd))
			return TRUE;
	}
	return FALSE;
}

void XTracker::Edit(CPoint &point, CWnd *pWnd)
{
	SearchDot(point,pWnd);
	GetDot(point,pWnd);
	FillRect(pWnd);
	DataEXChange();
}

void XTracker::SetFlag(BOOL bflag)
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		pStroke->SetFlag(bflag);
	}
}

void XTracker::Edit(CQuad &quad, CWnd *pWnd)
{
	SearchDot(quad);
	GetDot(quad);
	if (0 != FillRect(pWnd))
	{
		CString str("");
        str.LoadString(IDS_SELRANGE_HAVEDUMP);
		AfxMessageBox(str);
	}
	DataEXChange();
}

void XTracker::AddRect(CWnd *pWnd)
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		pStroke->AddRect(pWnd);
	}
}

void XTracker::AddPoint(CPoint &point,CWnd *pWnd)
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if(pStroke->AddPoint(point,pWnd))
			break;
	}
}

void XTracker::DelDot()
{
	if(m_pDotHead==NULL)
	{
		return;
	}

	DotHead       *pList    = m_pDotHead;
	DotHead       *pDotNext = NULL;
	DotSelectList *pDotList = NULL;
	CStroke       *pStroke  = NULL;

	int nIndex = 0;
	int nCount = 0;
	while(pList)
	{
		pDotNext = pList->m_pNext;
		pStroke  = (CStroke*)(pList->m_pAdd);
		pDotList = pList->m_pHead;
		nCount   = 0;
		
		while(pDotList)
		{
			nIndex = pDotList->m_nNum;
			pStroke->RemoveDot(nIndex);
			if(DotListSub(&pList, nCount))
			{
				ModifyStrokeList(pStroke);
			}

			if(pList != NULL)
			{
				pDotList = pList->m_pHead;
			}
			else
			{
				break;
			}
		}// end while

		if(pList!=NULL)
		{
			pList=pList->m_pNext;
		}
		else
		{
			pList=pDotNext;
		}
	}// end while

	return;
}


void XTracker::ModifyStrokeList(CStroke *pStroke)
{
	if (NULL == pStroke)
	{
		return;
	}

	POSITION pos      = m_StrokeList.GetHeadPosition();
	CStroke *pTStroke = NULL;
	POSITION pre      = NULL;

	while(pos)
	{
		pre     = pos;
		pTStroke= m_StrokeList.GetNext(pos);
		if(pTStroke == pStroke)
		{
			m_StrokeList.RemoveAt(pre);
		}
	}

	return;
}

BOOL XTracker::HitValidPoint(CPoint &point, CWnd *pWnd)
{
	POSITION pos=m_StrokeList.GetHeadPosition();
	if(pos==NULL)
		return FALSE;
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_StrokeList.GetNext(pos);
		if (TRUE == pStroke->m_bStyle)
		{
			continue;
		}

		if(pStroke->Click(point,pWnd))
			return TRUE;
	}
	return FALSE;
}

void XTracker::ClearAll()
{
	ClearDot();
	ClearStrokeList();
}

void XTracker::SplitPoint(double dbPoint)//断点操作
{
	if(m_pDotHead==NULL)
		return;
	DotHead *pDotHead=m_pDotHead;
	DotSelectList *pList=NULL;
	DotSelectList *pStore=NULL;
	CStroke *pStroke=NULL;
	int nCount;
	while(pDotHead)
	{
		pList=pDotHead->m_pTail;
		pStroke=(CStroke*)(pDotHead->m_pAdd);
		if (NULL == pStroke->GetParent())
		{
			return;
		}
		
		nCount=pStroke->GetSize();
		BOOL bFirst=TRUE;
		CDot dt1,dt2,dt3,dt4;
		int nBegin,nEnd,nTemp;
		while(pList)
		{
			//从后往前进行
			if(pList==pDotHead->m_pTail)
			{
				if(0<pList->m_nNum &&pList->m_nNum<nCount-1)
				{
					dt3=pStroke->m_dotArray[pList->m_nNum];
					dt1=pStroke->m_dotArray[pList->m_nNum-1];
					dt2=pStroke->m_dotArray[pList->m_nNum+1];
					dt1=pStroke->GetDot(dt1,dt3,dbPoint);
					dt2=pStroke->GetDot(dt2,dt3,dbPoint);
					nBegin=pList->m_nNum+1;
					nEnd=pStroke->m_dotArray.GetUpperBound();
					pStroke->SplitDot(nBegin,nEnd,dt2,dt2);
					dt4=dt1;
					nTemp=pList->m_nNum;
					pStore=pList;
					pList=pList->m_pFront;
				}
				else
				{
					pStore=pList->m_pFront;
					if(pStore)
					{
						if(pList->bLeft)
						{
							delete pList->m_pLeft;
							pList->bLeft=FALSE;
						}
						if(pList->bRight)
						{
							delete pList->m_pRight;
							pList->bRight=FALSE;
						}
						delete pList;
						pList=pStore;
						pList->m_pNext=NULL;
						pDotHead->m_pTail=pList;
					}
					else 
					{
						break;
					}
				}
			}
			else
			{
				if(0<pList->m_nNum &&pList->m_nNum<nCount-1)
				{
					dt3=pStroke->m_dotArray[pList->m_nNum];
					dt1=pStroke->m_dotArray[pList->m_nNum-1];
					dt2=pStroke->m_dotArray[pList->m_nNum+1];
					dt1=pStroke->GetDot(dt1,dt3,dbPoint);
					dt2=pStroke->GetDot(dt2,dt3,dbPoint);
					nBegin=pList->m_nNum+1;
					nEnd=nTemp-1;
					pStroke->SplitDot(nBegin,nEnd,dt2,dt4);
					dt4=dt1;
					nTemp=pList->m_nNum;
				}
				pStore=pList;
				pList=pList->m_pFront;
			}	
		}
		if(pStore !=NULL)
		{
			if(pStore==pDotHead->m_pHead)
			{
				pStroke->m_dotArray[nTemp]=dt4;
				pStroke->m_dotArray.SetSize(nTemp+1);
			}
		}
		pDotHead=pDotHead->m_pNext;
	}
}

void XTracker::JoinPoint()//联点操作
{
	if(m_pDotHead==NULL)
	{
		return;
	}
	
	DotHead *pHead=m_pDotHead;
	int nCount=0;
	while(pHead)
	{
		nCount++;
		pHead=pHead->m_pNext;
	}

	if (1 == nCount)
	{
		CStroke *pStroke=(CStroke*)(m_pDotHead->m_pAdd);
		if (pStroke) 
		{
			int nCount = pStroke->m_dotArray.GetSize();
			CDot sdot = (pStroke->m_dotArray)[0];
			CDot edot = (pStroke->m_dotArray)[nCount - 1];
			double dbD = pStroke->Distance(sdot, edot);
			if (dbD > g_SysCfg.m_dbDotFilterPrecis)
			{
				(pStroke->m_dotArray).Add(sdot);
			}
			else
			{
				CString str("");
				str.LoadString(IDS_THETWOPOINTTOOSMALL);
				AfxMessageBox(str);
			}
		}
		return;
	}
	else if (2 == nCount)
	{
		//两点在两个不同的stroke中
		if(!IsDiffStroke())
		{
			return;
		}

		CStroke *pStroke1 = NULL;
		CStroke *pStroke2 = NULL;
		pHead=m_pDotHead;
		pStroke1=(CStroke*)(pHead->m_pAdd);
		pHead=pHead->m_pNext;
		pStroke2=(CStroke*)(pHead->m_pAdd);
		//两个Stroke不为填充线
		if(IsFillStyle(pStroke1)||IsFillStyle(pStroke2))
		{
			return;
		}

		//两点在两个不同的NODE中
		if(!IsSameNode(pStroke1, pStroke2))
		{
			return;
		}
		
		int nJudge1 = 0;
		int nJudge2 = 0;
		pHead=m_pDotHead;
		nJudge1=HeadOrTail(pHead);
		pHead=pHead->m_pNext;
		nJudge2=HeadOrTail(pHead);
		if((0 == nJudge1) && (1 == nJudge2))
		{
			pStroke2->AddStroke(pStroke1);
		}
		if(nJudge2==0 && nJudge1==1)
		{
			pStroke1->AddStroke(pStroke2);
		}
		if(nJudge2==0 && nJudge1==0)
		{
			pStroke1->ConnectHeadToHead(pStroke2);
		}
		if(nJudge2==1 && nJudge1==1)
		{
			pStroke1->ConnectTailToTail(pStroke2);
		}
	}
	else
	{
		return;
	}
	
	return;
}


void XTracker::MoveDot(double MoveX, double MoveY)
{
	if(m_pDotHead==NULL)
		return;
	DotHead *pHead=m_pDotHead;		
	CStroke *pStroke;
	DotSelectList *plist;
	while(pHead)
	{
		plist=pHead->m_pHead;
		pStroke=(CStroke*)(pHead->m_pAdd);
		while(plist)
		{
			pStroke->MoveDot(plist->m_nNum,MoveX,MoveY);
			plist->m_Node.x+=MoveX;
			plist->m_Node.y+=MoveY;
			plist=plist->m_pNext;
		}
		pHead=pHead->m_pNext;
	}
}

CStroke* XTracker::FindSelListFirstDot(int &nNum, int &nCount) 
{
	DotHead *pHead       = m_pDotHead;
	if (!pHead)
	{
		return 0;
	}

	DotSelectList *pList = NULL;

	pList = pHead->m_pHead;
	if (pList)
	{
		nNum   = pList->m_nNum;
		nCount = 1;
		pList = pList->m_pNext;
		while (pList)
		{
			pList = pList->m_pNext;
			nCount ++;
		}
		
		return (CStroke*)(pHead->m_pAdd);
	}

	CString str("");
	str.LoadString(IDS_NO_SEL_EDITPOINT);
	AfxMessageBox(str);

	return 0;
}

void XTracker::KillAngle(CWnd *pWnd)
{
	int nNum   = -1;
	int nCount = 0;
	CStroke *pStroke = FindSelListFirstDot(nNum, nCount);
	if (nCount > 1)
	{
		CString str("");
		str.LoadString(IDS_SELONEDOT_ONLY);
		AfxMessageBox(str);

		return;
	}
	if (-1 == nNum) 
	{
		return;
	}

	InverDrawDot(pWnd);
	ClearDot();

	pStroke->KillAngle(nNum, g_SysCfg.m_dbKillAngleSpace);

	AddStrokeToEditList(pStroke);
	FillRect(pWnd);
	DataEXChange();

	return;
}

BOOL XTracker::IsSameNode(CStroke *pStroke1, CStroke *pStroke2)
{
	CNode *pNode1,*pNode2;
	pNode1=(CNode*)pStroke1->GetParent();
	pNode2=(CNode*)pStroke2->GetParent();
	if(pNode1==pNode2)
		return TRUE;
	return FALSE;
}

void XTracker::InverDrawDot(CWnd *pWnd)
{
	if(m_pDotHead==NULL)
		return;
	DotHead *pHead=m_pDotHead;
	while(pHead)
	{
		pHead->InverDrawDot(pWnd);
		pHead=pHead->m_pNext;
	}
}

// 查找选中点列表中的第一个点
int XTracker::FindSelListFirstPt(CDot &dot) 
{
	DotHead *pHead       = m_pDotHead;
	if (!pHead)
	{
		return 0;
	}
	
	int nCount = 0;
	
	DotSelectList *pList = NULL;
	while(pHead)
	{
		pList = pHead->m_pHead;
		if (pList)
		{
			dot   = pList->m_Node;
			nCount = 1;
			pList = pList->m_pNext;
			while (pList)
			{
				pList = pList->m_pNext;
				nCount ++;
			}
			
			return nCount;
		}
		
		pHead = pHead->m_pNext;
	}
	
	//	CString str("");
	//	str.LoadString(IDS_NO_SEL_EDITPOINT);
	//	AfxMessageBox(str);
	
	return 0;
}

void XTracker::AddStrokeToEditList(CStroke *pStroke)
{
	if (NULL == pStroke)
	{
		return;
	}
	
	//一个Stroke对应一个DotHead
	DotHead *pAdd=new DotHead;
	int nCount=pStroke->m_dotArray.GetSize();
	int i = 0;
	int j = 0;
	pAdd->m_pAdd=pStroke;
	pAdd->m_nCount=nCount;
	for(i=0;i<nCount;i++)
	{
		CDot dot=pStroke->m_dotArray.GetAt(i);
		j=(i==0)? 0:i-1;
		CDot dot1=pStroke->m_dotArray.GetAt(j);//左结点
		j=(i==nCount-1)? nCount-1:i+1;
		CDot dot2=pStroke->m_dotArray.GetAt(j);//右结点
		if(i==0)
			pAdd->AddSelectDot(dot,dot1,dot2,i,1,0);
		else if(i==nCount-1)
			pAdd->AddSelectDot(dot,dot1,dot2,i,0,1);
		else 
			pAdd->AddSelectDot(dot,dot1,dot2,i,0,0);
	}
	
	if(m_pDotHead==NULL)
	{
		m_pDotHead=pAdd;
		m_pDotTail=pAdd;
	}
	else
	{
		m_pDotTail->m_pNext=pAdd;
		m_pDotTail=pAdd;
	}
	
	return;
}

BOOL XTracker::Track(CPoint &point,CWnd *pWnd)//是否选中对象
{
	BOOL bFlag=FALSE;
	
	AfxLockTempMaps();
	pWnd->SetCapture();
	
	if (CWnd::GetCapture() != pWnd)
	{
		return FALSE;
	}
	
	CPoint ptthis    = point;
	m_ptPrev  = point;
	m_ptNow   = m_ptPrev;
	
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
					LastBrush(pWnd,m_ptPrev, ptthis);
				}
				
				m_rectOut.left   = point.x;
				m_rectOut.right  = ptthis.x;
				m_rectOut.top    = point.y;
				m_rectOut.bottom = ptthis.y;
				
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{
				bFlag    = TRUE;
				ptthis.x = (int)(short)LOWORD(msg.lParam);
				ptthis.y = (int)(short)HIWORD(msg.lParam);
				DrawRect(pWnd,m_ptPrev,m_ptNow,ptthis);
				m_ptNow = ptthis;
				
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

void XTracker::LastBrush(CWnd *pWnd, CPoint &ptf, CPoint &ptl)
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

void XTracker::DrawRect(CWnd *pWnd,CPoint &ptf,CPoint &ptl,CPoint &point)
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

BOOL XTracker::IsFillStyle(CStroke *pStroke)
{
	return pStroke->m_bStyle;
}

BOOL XTracker::IsDiffStroke()
{
	DotHead *pHead=m_pDotHead;
	if(pHead->m_pHead!=pHead->m_pTail)
		return FALSE;
	pHead=pHead->m_pNext;
	if(pHead->m_pHead!=pHead->m_pTail)
		return FALSE;
	return TRUE;
}

int XTracker::HeadOrTail(DotHead *pHead)
{
	CStroke *pStroke=(CStroke*)(pHead->m_pAdd);
	DotSelectList *pList=pHead->m_pHead;
	int nNum=pList->m_nNum;
	int nCount=pStroke->GetSize();
	if(nNum==0)//头
		return 0;
	else if(nNum==nCount-1)
		return 1;//尾
	else
		return 2;//其它
}

BOOL XTracker::DotListSub(DotHead **pHead, int &nCount)
{
	DotHead *pDHNext  = NULL;
	DotHead *pDHFront = NULL;
	BOOL bFlag = FALSE;
	DotSelectList *plist     = (*pHead)->m_pHead;
	DotSelectList *plistNext = plist->m_pNext;
	(*pHead)->m_pHead        = plistNext;
	
	if(plist->bLeft)
	{
		delete plist->m_pLeft;
		plist->bLeft=FALSE;
	}
	
	if(plist->bRight)
	{
		delete plist->m_pRight;
		plist->bRight=FALSE;
	}
	
	if (plist)
	{
		delete plist;
		plist = NULL;
	}
	
	plist=(*pHead)->m_pHead;
	nCount++;
	
	while(plist)
	{
		plist->m_nNum--;
		plist=plist->m_pNext;
	}
	
	if((*pHead)->m_pHead==NULL)//分支被删完
	{
		if(nCount==(*pHead)->m_nCount)
		{
			bFlag=TRUE;
		}
		
		if(*pHead!=m_pDotHead)
		{
			pDHFront=m_pDotHead;
			while(pDHFront->m_pNext!=*pHead)
			{
				pDHFront=pDHFront->m_pNext;
			}
			
			pDHNext=(*pHead)->m_pNext;
			pDHFront->m_pNext=pDHNext;
			
			if (*pHead)
			{
				delete *pHead;
				*pHead=NULL;
			}
		}
		else
		{
			m_pDotHead=(*pHead)->m_pNext;
			delete *pHead;
			*pHead=NULL;
		}
		
		if(bFlag)
		{
			return TRUE;
		}
		
		return FALSE;
	}
	
	return FALSE;
}

void XTracker::DataEXChange()
{
	if(m_pDotHead==NULL)
		return;
	DotHead *pHead=m_pDotHead;	
	while(pHead)
	{
		pHead->DataExChange();
		pHead=pHead->m_pNext;
	}
}

// 求两点的距离
double XTracker::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

void XTracker::ReplaceDot()
{
	DotHead *pHead=m_pDotHead;
	CStroke *pStroke;
	DotSelectList *plist;
	CDot dot;
	int i;
	while(pHead)
	{
		plist=pHead->m_pHead;
		pStroke= (pHead->m_pAdd);
		while(plist)
		{
			dot=plist->m_Node;
			i=plist->m_nNum;
			pStroke->m_dotArray.SetAt(i,dot);
			plist=plist->m_pNext;
		}
		pHead=pHead->m_pNext;
	}
}

void XTracker::AddDotHeadB(CStroke *pStroke,CQuad &quad)//往pDHead中加结点
{
	//一个Stroke对应一个DotHead
	DotHead *pAdd=new DotHead;
	CDot dot;
	CDot dot1,dot2;
	int nCount=pStroke->m_dotArray.GetSize();
	int i,j;
	pAdd->m_pAdd=pStroke;
	pAdd->m_nCount=nCount;
	for(i=0;i<nCount;i++)
	{
		dot=pStroke->m_dotArray.GetAt(i);
		if(quad.left<=dot.x && dot.x<=quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
		{
			j=(i==0)? 0:i-1;
			dot1=pStroke->m_dotArray.GetAt(j);//左结点
			j=(i==nCount-1)? nCount-1:i+1;
			dot2=pStroke->m_dotArray.GetAt(j);//右结点
			if(i==0)
				pAdd->AddSelectDot(dot,dot1,dot2,i,1,0);
			else if(i==nCount-1)
				pAdd->AddSelectDot(dot,dot1,dot2,i,0,1);
			else 
				pAdd->AddSelectDot(dot,dot1,dot2,i,0,0);
		}
	}
	
	if(m_pDotHead==NULL)
	{
		m_pDotHead=pAdd;
		m_pDotTail=pAdd;
	}
	else
	{
		m_pDotTail->m_pNext=pAdd;
		m_pDotTail=pAdd;
	}
}

void XTracker::ClearDot()
{
	if(m_pDotHead==NULL)
	{
		return;
	}
	
	DotHead *pHead=m_pDotHead;
	DotHead *pHeadNext;
	while(pHead)
	{
		pHeadNext=pHead->m_pNext;
		pHead->DelSelectDot();
		delete pHead;
		pHead=pHeadNext;
	}
	m_pDotHead=NULL;
	
	return;
}

BOOL XTracker::ListEmpty()
{
	if(m_pDotHead==NULL)
	{
		return TRUE;
	}
	return FALSE;
}

void XTracker::AddDotToHead(CStroke *pStroke,CDot &dotIndex, double dbdata)
{
	if (m_pDotHead) 
	{
		return;
	}
	
	DotHead *pAdd=new DotHead;
	if (NULL == pAdd) 
	{
		return;
	}
	
	CDot dot;
	CDot dot1,dot2,dotFirst,dotEnd;
	int nCount=pStroke->m_dotArray.GetSize();
	int i,j;
	CQuad quad;
	pAdd->m_pAdd=pStroke;
	pAdd->m_nCount=nCount;
	BOOL bflag=FALSE;
	dotFirst=pStroke->m_dotArray.GetAt(0);
	dotEnd=pStroke->m_dotArray.GetAt(nCount-1);
	if(dotFirst==dotEnd)
		bflag=TRUE;
	for(i=0;i<nCount;i++)
	{
		dot=pStroke->m_dotArray.GetAt(i);
		quad.left=dot.x-dbdata;
		quad.right=dot.x+dbdata;
		quad.top=dot.y+dbdata;
		quad.bottom=dot.y-dbdata;
		if(quad.left<=dotIndex.x && dotIndex.x<=quad.right && quad.bottom<=dotIndex.y && dotIndex.y<=quad.top)
		{
			j=(i==0)? nCount-1:i-1;
			dot1=pStroke->m_dotArray.GetAt(j);
			j=(i==nCount-1)? 0:i+1;
			dot2=pStroke->m_dotArray.GetAt(j);
			if(bflag)
			{
				
				pAdd->AddSelectDot(dot,dot1,dot2,i,0,0);
			}
			else 
			{
				if(i==0)
					pAdd->AddSelectDot(dot,dot1,dot2,i,1,0);
				else if(i==nCount-1)
					pAdd->AddSelectDot(dot,dot1,dot2,i,0,1);
				else 
					pAdd->AddSelectDot(dot,dot1,dot2,i,0,0);
			}
			break;
		}
	}
	if(m_pDotHead==NULL)
	{
		m_pDotHead=pAdd;
		m_pDotTail=pAdd;
	}
	else
	{
		m_pDotTail->m_pNext=pAdd;
		m_pDotTail=pAdd;
	}
	
	return;
}

BOOL XTracker::HitSelectDotB(CDot &dotTest, double dbdata)
{
	DotHead *pHead=m_pDotHead;
	DotSelectList *pList = NULL;
	CDot dot;
	CQuad quad;
	while(pHead)
	{
		pList=pHead->m_pHead;
		while(pList)
		{
			dot=pList->m_Node;
			quad.left=dot.x-dbdata;
			quad.right=dot.x+dbdata;
			quad.top=dot.y+dbdata;
			quad.bottom=dot.y-dbdata;
			if(quad.DotIn(dotTest))
				return TRUE;
			pList=pList->m_pNext;
		}
		pHead=pHead->m_pNext;
	}
	return FALSE;
}
