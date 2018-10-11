// NodePolygon.cpp: implementation of the CNodePolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodePolygon.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodePolygon::CNodePolygon()
{
	m_type=NODE_POLYGON;

	m_nPolygon = g_SysCfg.m_nPolygon;
	m_nNum     = g_SysCfg.m_nPolyNum;
	m_nCross   = g_SysCfg.m_nPolyCross;
	m_nStar    = g_SysCfg.m_nPolyStar;
}

CNodePolygon::~CNodePolygon()
{
}

BOOL CNodePolygon::Track(CWnd* pWnd, CPoint ptOrg)
{
	if (::GetCapture() != NULL)
		return FALSE;

	BOOL bResult=TRUE;

	AfxLockTempMaps();

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	CDC* pDC;
	pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);

	CDot dtOrg,dtNow;
	dtOrg = ptOrg;
	((CHLView *)pWnd)->LogicToPhysic(dtOrg.x,dtOrg.y);
	dtNow = dtOrg;
	

	BOOL bMoved=FALSE;

	//消息处理
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CString str,prompt;
	prompt.LoadString(IDS_PROMPT_POLYGON);
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				if(bMoved)
					bResult=TRUE;
				else
					bResult=FALSE;

				goto ExitLoop;
				break;
			}
		case WM_MOUSEMOVE:
			{
				if(!bMoved)
					bMoved=TRUE;

				Adjust(msg,ptOrg);
				CPoint pointNow;
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				dtNow = pointNow;
				((CHLView *)pWnd)->LogicToPhysic(dtNow.x,dtNow.y);
				
				int mode=pDC->SetROP2(R2_NOTXORPEN);
				CQuad quad;
				quad=CQuad(dtOrg,dtNow);
				XDC	*pXDC=((CHLView *)pWnd)->PrepareDC(pDC);
				Draw(pXDC);
				Create(quad);
				Draw(pXDC);
				
				pDC->SetROP2(mode);

				//状态行提示
				quad.Normal();

				str.Format(prompt,quad.Width(),quad.Height());
				pFrame->SendMessage(WM_SETMESSAGESTRING, 0,(LPARAM)(LPCSTR)str);

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
	{
		return FALSE;
	}

	//生成最终对象
	CQuad quad;
	quad=CQuad(dtOrg,dtNow);
	BOOL bRet = Create(quad);

	return bRet;
}

BOOL CNodePolygon::Create(CQuad &rect)
{
	//删除可能存在的数据
	Delete();

	//确定中心，及半径，XY方向缩放比例等
	CStroke *pStroke = NULL;
	CDot center;
	double r(0.0);
	double ratioX(0.0),ratioY(0.0);
	CDot dot;
	double step(0.0);
	step = 360.0 / m_nNum;

	center.x = (rect.left + rect.right) / 2.0;
	center.y = (rect.bottom + rect.top) / 2.0;

	r = min(rect.Height(),rect.Width());
	if(fabs(r) < EPSINON)
	{
		return FALSE;
	}
	
	ratioX = rect.Width()  / r;
	ratioY = rect.Height() / r;
	r = r / 2;

	switch(m_nPolygon)
	{
	case POLYGON_LINE:
		{
			pStroke = new CStroke(m_property);
			for(int i = 0; i <= m_nNum; i++)
			{
				dot.x =  r * cos(PAI * (90 + i * step) / 180) + center.x;
				dot.y =  r * sin(PAI * (90 + i * step) / 180) + center.y;
				pStroke->Add(dot);
			}
			m_list.AddTail(pStroke);
			break;
		}
	case POLYGON_STAR:
		{
			double r1=r*cos(PAI*step/360)*m_nStar/100;
			
			pStroke=new CStroke(m_property);
			for(int i=0;i<m_nNum;i++)
			{
				dot.x = r*cos(PAI*(90+i*step)/180)+center.x;
				dot.y = r*sin(PAI*(90+i*step)/180)+center.y;
				pStroke->Add(dot);
				
				dot.x = r1*cos(PAI*(90+(i+.5)*step)/180)+center.x;
				dot.y = r1*sin(PAI*(90+(i+.5)*step)/180)+center.y;
				pStroke->Add(dot);
			}
			
			dot.x = r * cos(PAI/2)+center.x;
			dot.y = r * sin(PAI/2)+center.y;
			pStroke->Add(dot);


			m_list.AddTail(pStroke);
			break;
		}
	case POLYGON_CROSS:
		{
			BOOL *pList=new BOOL[m_nNum];
			for(int i=0;i<m_nNum;i++)
			{
				pList[i]=TRUE;
			}
			
			for(int i=0;i<m_nNum;i++)
			{
				if(pList[i])
				{
					pStroke=new CStroke(m_property);
					for(int j=i;;j=j+m_nCross+1)
					{
						int k;
						dot.x = r*cos(PAI*(90+j*step)/180)+center.x;
						dot.y = r*sin(PAI*(90+j*step)/180)+center.y;
						pStroke->Add(dot);
						
						k=(j%m_nNum);
						if(pList[k])
							pList[k]=FALSE;
						else
							break;
					}
					m_list.AddTail(pStroke);
				}
			}

			delete []pList;
			break;
		}
	}

	Scale(center,ratioX,ratioY);
	return TRUE;
}

void CNodePolygon::Serialize(FILE* fp,BOOL bFlag)
{
	//把它当成PLT图存笔画信息
	CMySerialize fs(fp);
	CString str;

	if (bFlag)
	{
		//写入头信息
		fs<<"NPLT_B";
		m_property.Serialize(fp,bFlag);
		Serialize_list(fp,bFlag);
		//写入尾信息
		fs<<"NPLT_E";
	}
}


void CNodePolygon::AddRect(CWnd *pWnd)
{
	CHLView *pView=(CHLView*)pWnd;
	CDC *pDC=pView->GetDC();
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke=m_list.GetNext(pos);
	int nCount=pStroke->m_dotArray.GetSize();
	nCount--;
	CQuad quad;
	CDot dot;
	double dbR=3;
	pView->ActualLen(dbR);
	CPen RedPen(PS_SOLID,1,RED);
	CPen *OldPen=pDC->SelectObject(&RedPen);
	CRect rect;
	for(int i=0;i<nCount;i++)
	{
		dot=pStroke->m_dotArray[i];
		quad.left=dot.x-dbR;
		quad.right=dot.x+dbR;
		quad.bottom=dot.y-dbR;
		quad.top=dot.y+dbR;
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);
		rect=(CRect)quad;
		//pDC->Ellipse(&rect);
		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);
	}
	pDC->SelectObject(OldPen);
	RedPen.DeleteObject();
	pView->ReleaseDC(pDC);
}

void CNodePolygon::DelRect(CWnd *pWnd)
{
	CHLView *pView=(CHLView*)pWnd;
	CDC *pDC=pView->GetDC();
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke=m_list.GetNext(pos);
	int nCount=pStroke->m_dotArray.GetSize();
	nCount--;
	CQuad quad;
	CDot dot;
	double dbR=3;
	pView->ActualLen(dbR);
	CPen RedPen(PS_SOLID,1,RED);
	CPen *OldPen=pDC->SelectObject(&RedPen);
	int mode=pDC->SetROP2(R2_NOTXORPEN);
	CRect rect;
	for(int i=0;i<nCount;i++)
	{
		dot=pStroke->m_dotArray[i];
		quad.left=dot.x-dbR;
		quad.right=dot.x+dbR;
		quad.bottom=dot.y-dbR;
		quad.top=dot.y+dbR;
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);
		rect=(CRect)quad;
	//	pDC->Ellipse(&rect);
		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);
	}
	pDC->SetROP2(mode);
	pDC->SelectObject(OldPen);
	RedPen.DeleteObject();
	pView->ReleaseDC(pDC);
}

//DEL void CNodePolygon::GetDot(CPoint &point, myNode *pHead,CWnd *pWnd)
//DEL {
//DEL POSITION pos;
//DEL 	CStroke *pStroke;
//DEL 	int nCount;
//DEL 	CHLView *pView=(CHLView*)pWnd;
//DEL 	double dbdata=3;
//DEL 	pView->ActualLen(dbdata);
//DEL 	CDot dot;
//DEL 	int j;
//DEL 	CDot dot1,dot2;
//DEL 	CQuad quad;
//DEL 	CDot dotTemp;
//DEL 	dotTemp.x=point.x;
//DEL 	dotTemp.y=point.y;
//DEL 	pView->LogicToPhysic(dotTemp.x,dotTemp.y);
//DEL 	quad.top=dotTemp.y+dbdata;
//DEL 	quad.bottom=dotTemp.y-dbdata;
//DEL 	quad.left=dotTemp.x-dbdata;
//DEL 	quad.right=dotTemp.x+dbdata;
//DEL 	pos=m_list.GetHeadPosition();
//DEL 	BOOL bFlag;
//DEL 	int t=-1;
//DEL 	int z=0;
//DEL 	while(pos)
//DEL 	{
//DEL 		t++;
//DEL 		pStroke=m_list.GetNext(pos);
//DEL 		nCount=pStroke->m_dotArray.GetSize();
//DEL 		nCount--;
//DEL 		bFlag=FALSE;
//DEL 		for(int i=0;i<nCount;i++)
//DEL 		{
//DEL 			dot=pStroke->m_dotArray.GetAt(i);
//DEL 			if(quad.left<=dot.x && dot.x<quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
//DEL 			{
//DEL 				bFlag=TRUE;
//DEL 				pHead->m_pmyStroke=new myStroke[1];
//DEL 				j=(i==0)? nCount-1:i-1;
//DEL 				dot1=pStroke->m_dotArray.GetAt(j);
//DEL 				j=(i==nCount-1)? 0:i+1;
//DEL 				dot2=pStroke->m_dotArray.GetAt(j);
//DEL 				(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,0,0,t);
//DEL 				(pHead->m_pmyStroke[z]).m_nCount=nCount;
//DEL 				g_bReady=FALSE;
//DEL 				pHead->m_nCount=1;
//DEL 				break;
//DEL 			}
//DEL 		}
//DEL 		if(bFlag)
//DEL 			break;
//DEL 	}
//DEL }

//DEL void CNodePolygon::ReplaceDot(myNode *pHead)
//DEL {
//DEL int i,j,k,nNum;
//DEL 	j=-1;
//DEL 	POSITION pos=m_list.GetHeadPosition();
//DEL 	if(pos==NULL)
//DEL 		return;
//DEL 	CStroke *pStroke;
//DEL 	DotSelectList *pList;
//DEL 	CDot dot;
//DEL 	int nCount;
//DEL 	for(i=0;i<pHead->m_nCount;i++)
//DEL 	{
//DEL 		k=(pHead->m_pmyStroke[i]).m_nRow;
//DEL 		while(j!=k)
//DEL 		{
//DEL 			pStroke=m_list.GetNext(pos);
//DEL 			j++;
//DEL 		}
//DEL 		nCount=pStroke->m_dotArray.GetSize();
//DEL 		nCount--;
//DEL 		pList=(pHead->m_pmyStroke[i]).pHead;
//DEL 		while(pList)
//DEL 		{
//DEL 			dot=pList->m_Node;
//DEL 			nNum=pList->m_nNum;
//DEL 			pStroke->m_dotArray.SetAt(nNum,dot);
//DEL 			if(nNum==0)
//DEL 				pStroke->m_dotArray.SetAt(nCount,dot);
//DEL 			pList=pList->m_pNext;
//DEL 		}
//DEL 	}
//DEL }

//DEL void CNodePolygon::GetDot(CQuad &quad,myNode*pHead, CWnd *pWnd)
//DEL {
//DEL POSITION pos=m_list.GetHeadPosition();
//DEL 	CStroke *pStroke;
//DEL 	int nCount;
//DEL 	CHLView *pView=(CHLView*)pWnd;
//DEL 	double dbdata=3;
//DEL 	pView->ActualLen(dbdata);
//DEL 	CDot dot;
//DEL 	int j;
//DEL 	CDot dot1,dot2;
//DEL 	pHead->m_nCount=0;
//DEL 	while(pos)
//DEL 	{
//DEL 		pStroke=m_list.GetNext(pos);
//DEL 		nCount=pStroke->m_dotArray.GetSize();
//DEL 		nCount--;
//DEL 		for(int i=0;i<nCount;i++)
//DEL 		{
//DEL 			dot=pStroke->m_dotArray.GetAt(i);
//DEL 			if(quad.left<=dot.x && dot.x<=quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
//DEL 			{
//DEL 				pHead->m_nCount++;
//DEL 				break;
//DEL 			}
//DEL 		}
//DEL 	}
//DEL 	pHead->m_pmyStroke=new myStroke[pHead->m_nCount];
//DEL 	pos=m_list.GetHeadPosition();
//DEL 	BOOL bFlag;
//DEL 	int t=-1;
//DEL 	int z=0;
//DEL 	while(pos)
//DEL 	{
//DEL 		t++;
//DEL 		pStroke=m_list.GetNext(pos);
//DEL 		nCount=pStroke->m_dotArray.GetSize();
//DEL 		bFlag=FALSE;
//DEL 		for(int i=0;i<nCount;i++)
//DEL 		{
//DEL 			dot=pStroke->m_dotArray.GetAt(i);
//DEL 			if(quad.left<=dot.x && dot.x<=quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
//DEL 			{
//DEL 				bFlag=TRUE;
//DEL 				j=(i==0)? 0:i-1;
//DEL 				dot1=pStroke->m_dotArray.GetAt(j);
//DEL 				j=(i==nCount-1)? nCount-1:i+1;
//DEL 				dot2=pStroke->m_dotArray.GetAt(j);
//DEL 				if(i==0)
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,1,0,t);
//DEL 				else if(i==nCount-1)
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,0,1,t);
//DEL 				else 
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,0,0,t);
//DEL 					(pHead->m_pmyStroke[z]).m_nCount=nCount;
//DEL 			}
//DEL 		}
//DEL 		if(bFlag)
//DEL 			z++;
//DEL 	}
//DEL }






