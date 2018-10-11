// NodeEllipse.cpp: implementation of the CNodeEllipse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeEllipse.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double ARC=0.5;
CNodeEllipse::CNodeEllipse()
{
	m_type  =NODE_ELLIPSE;
	m_nCount=0;
	ARC     = g_SysCfg.m_dbDrawEllipSet;
	m_pStrkTemp = NULL;
}

CNodeEllipse::~CNodeEllipse()
{
}


BOOL CNodeEllipse::Track(CWnd* pWnd, CPoint ptOrg)
{
	if (m_pStrkTemp)
	{
		delete m_pStrkTemp;
		m_pStrkTemp = NULL;
	}
	
	if (::GetCapture() != NULL) //A NULL return value means the current thread has not captured the mouse. However, it is possible that another thread or process has captured the mouse.
		return FALSE;

	CHLView *pHLView=(CHLView*)pWnd;
	
	BOOL bResult=TRUE;

	AfxLockTempMaps();

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	CDC* pDC;
	pDC = pWnd->GetDC();
	XDC *pXDC=pHLView->PrepareDC(pDC);
	ASSERT_VALID(pDC);

	CPoint pointNow;
	pointNow=ptOrg;
	BOOL bMoved=FALSE;

	//消息处理
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CString str,prompt;
	prompt.LoadString(IDS_PROMPT_RECT);
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			if(bMoved)
				bResult=TRUE;
			else
				bResult=FALSE;
			goto ExitLoop;
			break;
		case WM_MOUSEMOVE:
			{
				if(!bMoved)
					bMoved=TRUE;
				
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);

				int mode=pDC->SetROP2(R2_NOTXORPEN);
				Draw(pXDC);
				CDot dot1,dot2;
				dot1.x=ptOrg.x;
				dot1.y=ptOrg.y;
				dot2.x=pointNow.x;
				dot2.y=pointNow.y;
				pHLView->LogicToPhysic(dot1.x,dot1.y);
				pHLView->LogicToPhysic(dot2.x,dot2.y);
				DOT Center;
				Center.x=(dot1.x+dot2.x)/2;
				Center.y=(dot1.y+dot2.y)/2;
				Create(dot1,dot2,Center,msg);
				Draw(pXDC);
				pDC->SetROP2(mode);
				break;
			}
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:

	pWnd->ReleaseDC(pDC);
	ReleaseCapture();
	AfxUnlockTempMaps(FALSE);

	if(!bResult)
		return FALSE;

	return TRUE;
}


void CNodeEllipse::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;
	
	if (bFlag)
	{
		fs<<"NEPS_B";
		m_property.Serialize(fp,bFlag);
		//实际坐标转为13个控点
		PointToCtrPoint();
		SerializePoint(fp,bFlag);
		fs<<"NEPS_E";
	}
	else
	{
		while(fs>>str)
		{
			if(str== "PRO_B")
			{
				m_property.Serialize(fp,bFlag);
				continue;
			}
			if(str == "EPS_B")
			{
				SerializePoint(fp,bFlag);
				//由13个控点生成实际点
				CtrlPointToPoint();
			}
			if(str == "NEPS_E")
				break;	
		}
	}
}

//void CNodeEllipse::MarkPreview(CDC *pDC, const int &nPreviewSpeed)
//{
//	PreprocessMark(0);
//
//	POSITION pos=m_list.GetHeadPosition();
//	CStroke *pStroke;
//	while(pos!=NULL)
//	{	
//		pStroke=m_list.GetNext(pos);
//		pStroke->MarkPreview(pDC, nPreviewSpeed);
//	}
//}

void CNodeEllipse::SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->SetLayColor(nLayer, color);
	}
	m_property.m_color  = color;
	m_property.m_nLayer  = nLayer;
}

void CNodeEllipse::Mark(HWND hWnd, int nFocusType)
{
	PreprocessMark(0x00);
	if(m_list.GetCount()<=0)
		return;

	CStroke *pStroke;
	int n =  g_DAT.GetMarkCount(m_property.m_nLayer);
	
	BOOL bStop=FALSE;
	for(int i= 0;i<n;i++)
	{
		POSITION pos=m_list.GetHeadPosition();
		while(pos!=NULL)	
		{	
			pStroke=m_list.GetNext(pos);
			bStop = !(g_DAT.MarkStroke(*pStroke));
		}
		if(bStop)
			break;
	}
}

void CNodeEllipse::SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsResMark)
{
	PreprocessMark(0x00);
	if(m_list.GetCount()<=0)
	{
		return;
	}

	CStroke *pStroke = NULL;
	BOOL bStop       = FALSE;
	POSITION pos = m_list.GetHeadPosition();
	while(pos!=NULL)	
	{	
		if(::WaitForSingleObject(hSelStop,0)==WAIT_OBJECT_0)
		{
			return;
		}
		pStroke=m_list.GetNext(pos);
		bStop = !(g_DAT.MarkStroke(*pStroke, nLayer, bIsResMark));
	}

	return;
}

void CNodeEllipse::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	if(m_list.GetCount()<=0)
		return;
	CStroke *pStroke;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->ExportPlt(file,bUnExportFill,bScall100);
	}
	
}

BOOL CNodeEllipse::Create(CDot dt1,CDot dt2,DOT Center,MSG msg)
{
//	Delete();
	if (m_pStrkTemp)
	{
		m_list.RemoveAll();
		delete m_pStrkTemp;
		m_pStrkTemp = NULL;
	}
	
	double dbLen=fabs(dt2.x-dt1.x)/2;
	double dbWid=fabs(dt2.y-dt1.y)/2;
	if(msg.wParam & MK_CONTROL)
	{
		dbLen=dbWid=max(dbLen,dbWid);
	}

	if(dbLen>=dbWid)
	{
		CStroke *pStroke=new CStroke;
		if (NULL == pStroke)
		{
			return FALSE;
		}
		m_pStrkTemp = pStroke;
		pStroke->SetProperty(PTY_INIT, m_property);

		double r(0.0);
		if(2*dbLen>ARC)
		{
			double dbTemp=(2*dbLen*dbLen-ARC*ARC)/(2*dbLen*dbLen);
			double dbAraf=acos(dbTemp);
			m_nCount=int(2*PAI/(dbAraf));
			2*PAI/dbAraf-m_nCount>=0.5 ? m_nCount+=1:m_nCount=m_nCount;
			dbAraf=2*PAI/m_nCount;
			CDot dot;
			double dbArc;
			int i;
			for(i=0;i<m_nCount;i++)
			{
				dbArc=i*dbAraf;
				r=sqrt((dbLen*dbLen*dbWid*dbWid)/(dbLen*dbLen*sin(dbArc)*sin(dbArc)+dbWid*dbWid*cos(dbArc)*cos(dbArc)));
				dot.x=r*cos(dbArc);
				dot.y=r*sin(dbArc);
				dot.x+=Center.x;
				dot.y+=Center.y;
				pStroke->Add(dot);
			}
			dot=pStroke->m_dotArray[0];
			pStroke->Add(dot);
			m_list.AddTail(m_pStrkTemp);
//			m_list.AddTail(pStroke);
		}
	}
	else
	{
		CStroke *pStroke = new CStroke;
		if (NULL == pStroke)
		{
			return FALSE;
		}
		m_pStrkTemp = pStroke;
		pStroke->SetProperty(PTY_INIT, m_property);
		
		double r;
		r=dbLen;
		dbLen=dbWid;
		dbWid=r;
		if(2*dbLen>ARC)
		{
			double  dbAraf=acos((2*dbLen*dbLen-ARC*ARC)/(2*dbLen*dbLen));
			m_nCount=int(2*PAI/(dbAraf));
			2*PAI/dbAraf-m_nCount>=0.5 ? m_nCount+=1:m_nCount=m_nCount;
			dbAraf=2*PAI/m_nCount;
			CDot dot;
			double dbArc;
			for(int i=0;i<m_nCount;i++)
			{
				dbArc=i*dbAraf;
				r=sqrt((dbLen*dbLen*dbWid*dbWid)/(dbLen*dbLen*cos(dbArc)*cos(dbArc)+dbWid*dbWid*sin(dbArc)*sin(dbArc)));
				dot.x=r*cos(dbArc);
				dot.y=r*sin(dbArc);
				dot.x+=Center.x;
				dot.y+=Center.y;
				pStroke->Add(dot);
			}
			dot=pStroke->m_dotArray[0];
			pStroke->Add(dot);
			m_list.AddTail(m_pStrkTemp);
//			m_list.AddTail(pStroke);
		}
	}
	return TRUE;
}

void CNodeEllipse::SerializePoint(FILE *fp, BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;

	if(bFlag)
	{
		fs<<"EPS_B";
		for(int i=0;i<13;i++)
		{
			fs<<130;
			fs<<m_dotArray[i].x;
			fs<<131;
			fs<<m_dotArray[i].y;
		}
		fs<<"EPS_E";
	}
	else
	{
		for(int i=0;i<13;i++)
		{
			fs>>str;
			if(130 == atoi(str))
				fs>>m_dotArray[i].x;

			fs>>str;
			if(131 == atoi(str))
				fs>>m_dotArray[i].y;
		}

		while(fs>>str)
		{
			if(str == "EPS_E")
				break;
		}
	}
}

void CNodeEllipse::PointToCtrPoint()
{
	CQuad quad=CalLimit();
	quad.TrueToLog();
	CDot center;
	center.x=(quad.left+quad.right)/2;
	center.y=(quad.top+quad.bottom)/2;
	double EtoBConst;
	EtoBConst=2.0/3*(sqrt((double)2)-1);
	double offsetX,offsetY;
	offsetX=quad.Width()*EtoBConst;
	offsetY=quad.Height()*EtoBConst;
	
	//////////////////////////////
	m_dotArray[0].x=
	m_dotArray[1].x=
	m_dotArray[11].x=
	m_dotArray[12].x=quad.left;

	m_dotArray[5].x=
	m_dotArray[6].x=
	m_dotArray[7].x=quad.right;

	m_dotArray[2].x=
	m_dotArray[10].x=center.x-offsetX;

	m_dotArray[4].x=
	m_dotArray[8].x=center.x+offsetX;

	m_dotArray[3].x=
	m_dotArray[9].x=center.x;

	/////////////////////////////
	m_dotArray[2].y=
	m_dotArray[3].y=
	m_dotArray[4].y=quad.bottom;
	
	m_dotArray[8].y=
	m_dotArray[9].y=
	m_dotArray[10].y=quad.top;

	m_dotArray[7].y=
	m_dotArray[11].y=center.y+offsetY;

	m_dotArray[1].y=
	m_dotArray[5].y=center.y-offsetY;

	m_dotArray[0].y=
	m_dotArray[12].y=
	m_dotArray[6].y=center.y;
}

void CNodeEllipse::CtrlPointToPoint()
{
	CDot dt1,dt2,Center;
	dt1.x=m_dotArray[0].x;
	dt1.y=m_dotArray[10].y;
	dt2.x=m_dotArray[5].x;
	dt2.y=m_dotArray[3].y;
	Center.x=(dt1.x+dt2.x)/2;
	Center.y=(dt1.y+dt2.y)/2;
	dt1.LogToTrue();
	dt2.LogToTrue();
	Center.LogToTrue();
	double dbLen=fabs(dt2.x-dt1.x)/2;
	double dbWid=fabs(dt2.y-dt1.y)/2;
	if(dbLen>=dbWid)
	{
		CStroke *pStroke=new CStroke;
		double r;
		double dbAraf=acos((2*dbLen*dbLen-ARC*ARC)/(2*dbLen*dbLen));
		m_nCount=int(2*PAI/(dbAraf));
		2*PAI/dbAraf-m_nCount>=0.5 ? m_nCount+=1:m_nCount=m_nCount;
		dbAraf=2*PAI/m_nCount;
		CDot dot;
		double dbArc;
		int i;
		for(i=0;i<m_nCount;i++)
		{
			dbArc=i*dbAraf;
			r=sqrt((dbLen*dbLen*dbWid*dbWid)/(dbLen*dbLen*sin(dbArc)*sin(dbArc)+dbWid*dbWid*cos(dbArc)*cos(dbArc)));
			dot.x=r*cos(dbArc);
			dot.y=r*sin(dbArc);
			dot.x+=Center.x;
			dot.y+=Center.y;
			pStroke->Add(dot);
		}
		dot=pStroke->m_dotArray[0];
		pStroke->Add(dot);
		pStroke->SetProperty(PTY_INIT, m_property);
		m_list.AddTail(pStroke);
	}
	else
	{
		CStroke *pStroke=new CStroke;
		double r;
		r=dbLen;
		dbLen=dbWid;
		dbWid=r;
		double  dbAraf=acos((2*dbLen*dbLen-ARC*ARC)/(2*dbLen*dbLen));
		m_nCount=int(2*PAI/(dbAraf));
		2*PAI/dbAraf-m_nCount>=0.5 ? m_nCount+=1:m_nCount=m_nCount;
		dbAraf=2*PAI/m_nCount;
		CDot dot;
		double dbArc;
		for(int i=0;i<m_nCount;i++)
		{
			dbArc=i*dbAraf;
			r=sqrt((dbLen*dbLen*dbWid*dbWid)/(dbLen*dbLen*cos(dbArc)*cos(dbArc)+dbWid*dbWid*sin(dbArc)*sin(dbArc)));
			dot.x=r*cos(dbArc);
			dot.y=r*sin(dbArc);
			dot.x+=Center.x;
			dot.y+=Center.y;
			pStroke->Add(dot);
		}
		dot=pStroke->m_dotArray[0];
		pStroke->Add(dot);
		pStroke->SetProperty(PTY_INIT, m_property);
		m_list.AddTail(pStroke);
	}

	return;
}
