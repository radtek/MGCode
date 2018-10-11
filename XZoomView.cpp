// XZoomView.cpp : implementation file
//

#include "stdafx.h"
#include "XZoomView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const DWORD dw_TIMEERID_VIEW = 0xFF0FF002;

UINT MarkPreview(LPVOID lpPara)
{
	XZoomView* pView = (XZoomView*)lpPara;
	CStroke* pStroke;

	while(pView->m_bRun)
	{
		POSITION pos = pView->m_strokelist.GetHeadPosition();
		while(pos)
		{
			pStroke = pView->m_strokelist.GetNext(pos);
			pView->CalCurPos(pStroke);
			if(!pView->m_bRun)
			{
				break;
			}
		}
	}

	pView->m_bRun = FALSE;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// XZoomView

IMPLEMENT_DYNCREATE(XZoomView, CZoomView)

XZoomView::XZoomView()
{
	m_dbViewX = 100;
	m_dbViewY = 100;
	m_dbPreviewSpeed = 1;
	m_pThread = NULL;
	m_bRun    = FALSE;
	InitializeCriticalSection(&m_cri);
}

XZoomView::~XZoomView()
{
	DeleteCriticalSection(&m_cri);
}


BEGIN_MESSAGE_MAP(XZoomView, CZoomView)
	//{{AFX_MSG_MAP(XZoomView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XZoomView drawing

void XZoomView::OnDraw(CDC* pDC)
{
	XDC *pXDC=PrepareDC(pDC);
	if (NULL == pXDC)
	{
		return;
	}

	DrawBox(pXDC);

	CStroke* pStroke = NULL;
	POSITION pos = m_strokelist.GetHeadPosition();
	while(pos)
	{
		pStroke = m_strokelist.GetNext(pos);
		pStroke->Draw(pXDC);
	}

	DrawSelQuadList();

	return;
}

void XZoomView::DrawBox(XDC *pDC)
{
	double width=m_dbViewX;
	double height=m_dbViewY;
	
	pDC->MoveTo(-width/2,-height/2);
	pDC->LineTo(-width/2,height/2);
	pDC->LineTo(width/2,height/2);
	pDC->LineTo(width/2,-height/2);
	pDC->LineTo(-width/2,-height/2);
}

/////////////////////////////////////////////////////////////////////////////
// XZoomView diagnostics

#ifdef _DEBUG
void XZoomView::AssertValid() const
{
	CView::AssertValid();
}

void XZoomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void XZoomView::Lock()
{
	EnterCriticalSection(&m_cri);
}

void XZoomView::UnLock()
{
	LeaveCriticalSection(&m_cri);
}
/////////////////////////////////////////////////////////////////////////////
// XZoomView message handlers
void XZoomView::OnTimer(UINT nIDEvent)
{
	m_niIndex++;
	if(m_bRun)
	{
		DrawBall();
	}
	else
	{
		KillTimer(dw_TIMEERID_VIEW);
		Invalidate();
	}
	CZoomView::OnTimer(nIDEvent);
}

BOOL XZoomView::MouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CZoomView::OnMouseWheel(nFlags,zDelta,pt);
}

void XZoomView::OnInitialUpdate() 
{
	CZoomView::OnInitialUpdate();
	ZoomPage();
}

void XZoomView::ZoomPage()
{
	double width=m_dbViewX;
	double height=m_dbViewY;

	CQuad quad;
	quad=CQuad(CDot(0,0),CDot(width,height));
	width=quad.Width()*1.2;
	height=quad.Height()*1.2;
	DoZoomIn(width,height);
}

void XZoomView::SetViewPage(double x, double y)
{
	m_dbViewX = x;
	m_dbViewY = y;
}

void XZoomView::SetStrokeList(StrokeList* plist)
{
	m_strokelist.RemoveAll();

	CStroke* pStroke;
	POSITION pos = plist->GetHeadPosition();
	while (pos) {
		pStroke = plist->GetNext(pos);
		m_strokelist.AddTail(pStroke);
	}
}

void XZoomView::PreViewStart()
{
	m_viewx = 0;
	m_viewy = 0;
	m_viewcolor=RGB(255,255,255);
	m_curxpos = 0;
	m_curypos = 0;
 	SetTimer(dw_TIMEERID_VIEW,40,NULL);
	m_bRun = TRUE;
	m_niIndex = 0;
	if (m_pThread)
	{
		TerminateThread(m_pThread->m_hThread, -1);
		delete m_pThread;
		m_pThread = NULL;
	}
	m_pThread = AfxBeginThread(MarkPreview,this);
}

void XZoomView::PreviewStop()
{
	m_bRun = FALSE;
	Sleep(200);
	KillTimer(dw_TIMEERID_VIEW);
//	WaitForSingleObject(m_pThread->m_hThread,INFINITE);
	TerminateThread(m_pThread->m_hThread, -1);
	if (m_pThread)
	{
		TerminateThread(m_pThread->m_hThread, -1);
//		delete m_pThread;
		m_pThread = NULL;
	}
	Invalidate();
}

void XZoomView::SetPreviewSpeed(double dbSpeed)
{
	m_dbPreviewSpeed = dbSpeed;
}

void XZoomView::DrawBall()
{
	//画当前位置
	if(m_niIndex>1)
		DrawCurBall(m_viewx,m_viewy,m_viewcolor,m_niIndex-1);
	//数据交换
	Lock();
	m_viewx = m_curxpos;
	m_viewy = m_curypos;
	m_viewcolor = m_color;
	UnLock();
	//画最后当前位置
	DrawCurBall(m_viewx,m_viewy,m_viewcolor,m_niIndex);
}

void XZoomView::DrawCurBall(double x, double y, COLORREF col, int iCount)
{
	CDC* pDC = GetDC();
	int mode=pDC->SetROP2(R2_NOTXORPEN);
	XDC* pXDC = PrepareDC(pDC);

	CPen pen(PS_SOLID,1,col);
	CBrush brush(RGB(0,0,0));
	CPen* oldpen = pDC->SelectObject(&pen);
	CBrush* oldbrush = pDC->SelectObject(&brush);
	pXDC->PhysicToLogic(x,y);

	CRect rt;
	rt.left = (long)x-5;
	rt.top = (long)y-5;
	rt.right = (long)x+5;
	rt.bottom = (long)y+5;
	pDC->Ellipse(&rt);

	pDC->SetROP2(mode);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	pen.DeleteObject();
	brush.DeleteObject();
	ReleaseDC(pDC);
}

void XZoomView::CalCurPos(CStroke *pStroke)
{
	CDot   dotStart,dotEnd;			     //起点,终点
	double count(0.0);					 //微矢量划分次数
	double distanceX(0.0),distanceY(0.0);//X,Y向距离
	double distance(0.0);				 //直线距离
	double angle(0.0);					 //角度
	double x(0.0),y(0.0);				 //微矢量位置
	double step(0.0);					 //10ms移动量
	double stepX(0.0),stepY(0.0);		 //X,Y向步长
	
	Lock();
	m_color    = pStroke->m_property.m_color;
	dotStart.x = m_curxpos;
	dotStart.y = m_curypos;
	UnLock();

	//跳转
	dotEnd    = pStroke->m_dotArray[0];
	distanceX = dotEnd.x-dotStart.x;
	distanceY = dotEnd.y-dotStart.y;
	distance  = sqrt(distanceX*distanceX+distanceY*distanceY);

	step  = (m_dbPreviewSpeed*0.01);
	angle = atan2(distanceY,distanceX);
	count = distance/step;

	stepX = step*cos(angle);
	stepY = step*sin(angle);

	x=dotStart.x;
	y=dotStart.y;

	for(int j = 0; j < count-1; j++)
	{	
		x += stepX;
		y += stepY;

		Lock();
		m_curxpos = x;
		m_curypos = y;
		UnLock();
		Sleep(5);

		if(!m_bRun)
		{
			return;
		}
	}		


	//有效矢量
	for(int i=0; i < pStroke->m_dotArray.GetSize()-1; i++)
	{
		dotStart= pStroke->m_dotArray[i];	
		dotEnd  = pStroke->m_dotArray[i+1];	

		distanceX = dotEnd.x-dotStart.x;
		distanceY = dotEnd.y-dotStart.y;
		distance  = sqrt(distanceX*distanceX+distanceY*distanceY);

		step = (m_dbPreviewSpeed*0.01);
		angle = atan2(distanceY,distanceX);
		count = distance/step;
		
		stepX = step*cos(angle);
		stepY = step*sin(angle);
				
		x=dotStart.x;
		y=dotStart.y;

		for(int j = 0; j < count-1; j++)
		{	
			x += stepX;
			y += stepY;

			Lock();
			m_curxpos = x;
			m_curypos = y;
			UnLock();
			Sleep(5);

			if(!m_bRun)
			{
				return;
			}
		}		
		Lock();
		m_curxpos = dotEnd.x;
		m_curypos = dotEnd.y;
		UnLock();
	}
}


void XZoomView::DrawArray(CQuad &quad, CArray<OBJPOS, OBJPOS> &dtArray, CArray<SELQUAD, SELQUAD> &QuadList)
{
	DelAllStrk();

	QuadList.RemoveAll();

	int nCount = dtArray.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CStroke *pStrk = new CStroke();
		if (NULL == pStrk)
		{
			continue;
		}

		CDot cDt   = quad.Center();
		double dbx = dtArray[i].x - cDt.x;
		double dby = dtArray[i].y - cDt.y;
		quad.SetOffset(dbx, dby);

		CDot dt1(quad.left, quad.top);
		CDot dt2(quad.right, quad.top);
		CDot dt3(quad.right, quad.bottom);
		CDot dt4(quad.left, quad.bottom);
		CDot dt5(quad.left, quad.top);
		pStrk->Add(dt1);
		pStrk->Add(dt2);
		pStrk->Add(dt3);
		pStrk->Add(dt4);
		pStrk->Add(dt5);

		pStrk->Rotate(quad.Center(), dtArray[i].z);

		SELQUAD selQuad;
		selQuad.m_QD   = pStrk->CalLimit();
		selQuad.m_bSel = FALSE;
		selQuad.m_pGroupObj = (DWORD)pStrk;

		QuadList.Add(selQuad);
		
		m_strokelist.AddTail(pStrk);
	}

	return;
}


void XZoomView::UpdateDrawArray(CQuad &quad, CArray<OBJPOS, OBJPOS> &dtArray, CArray<SELQUAD, SELQUAD> &QuadList)
{
	DelAllStrk();

	int nCount = dtArray.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CStroke *pStrk = new CStroke();
		if (NULL == pStrk)
		{
			continue;
		}

		CDot cDt   = quad.Center();
		double dbx = dtArray[i].x - cDt.x;
		double dby = dtArray[i].y - cDt.y;
		quad.SetOffset(dbx, dby);

		CDot dt1(quad.left, quad.top);
		CDot dt2(quad.right, quad.top);
		CDot dt3(quad.right, quad.bottom);
		CDot dt4(quad.left, quad.bottom);
		CDot dt5(quad.left, quad.top);
		pStrk->Add(dt1);
		pStrk->Add(dt2);
		pStrk->Add(dt3);
		pStrk->Add(dt4);
		pStrk->Add(dt5);

		pStrk->Rotate(quad.Center(), dtArray[i].z);
		QuadList[i].m_QD  = pStrk->CalLimit();
		
		m_strokelist.AddTail(pStrk);
	}

	return;
}

void XZoomView::DelAllStrk()
{
	CStroke* pStroke = NULL;
	POSITION prev    = NULL;
	POSITION pos = m_strokelist.GetHeadPosition();
	while(pos)
	{
		prev = pos;
		pStroke = m_strokelist.GetNext(pos);
		if (pStroke)
		{
			m_strokelist.RemoveAt(prev);
			delete pStroke;
			pStroke = NULL;
		}
	}

	m_strokelist.RemoveAll();

	return;
}



