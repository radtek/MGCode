// NodeLine.cpp: implementation of the CNodeLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "NodeLine.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodeLine::CNodeLine()
{
	m_type=NODE_LINE;
}

CNodeLine::~CNodeLine()
{
}

CNodeLine::CNodeLine(CStroke *pStroke)
{
	m_type=NODE_LINE;
	Delete();
	m_list.AddTail(pStroke);
	m_property = pStroke->m_property;
	CalLimit();
}

void CNodeLine::Adjust(MSG &msg,const CPoint &ptOrg)
{
	//CTRL控制
	if(MK_CONTROL&msg.wParam)
	{
		CPoint pointNow;
		pointNow.x = (int)(short)LOWORD(msg.lParam);
		pointNow.y = (int)(short)HIWORD(msg.lParam);

		int x,y;
		x=pointNow.x-ptOrg.x;
		y=pointNow.y-ptOrg.y;
		if(abs(x)>abs(y))
			y=0;
		else
			x=0;

		msg.lParam=ptOrg.y+y;
		msg.lParam<<=16;
		msg.lParam+=(ptOrg.x+x);
	}

}
void CNodeLine::Adjust(MSG msg, CPoint pointOld,CPoint &pointNew)
{
	int cx,cy;
	if(MK_CONTROL&msg.wParam)
	{
		cx=pointNew.x-pointOld.x;
		cy=pointNew.y-pointOld.y;
		if(abs(cx)>abs(cy))
			pointNew.y=pointOld.y;
		else
			pointNew.x=pointOld.x;
	}
	
}

BOOL CNodeLine::Track(CWnd* pWnd, CPoint ptOrg)
{
	if (::GetCapture() != NULL)// 如果已经有窗口捕获了鼠标则退出
		return FALSE;
	AfxLockTempMaps();  // 在循环过程中保持映射模式

	//捕获鼠标
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	CHLView* pView = (CHLView*)pWnd;

	// 取设备上下文
	CDC* pDC;
	pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);
	CPen pen(PS_SOLID,1,RGB(0,0,0));  
	CPen *oldPen=pDC->SelectObject(&pen);

	CPoint pointNow;
	CPoint pointOld;
	CPoint pointTemp;
	pointNow=ptOrg;
	BOOL bLine=TRUE; 
	CStroke *pStroke=new CStroke(m_property);
	WORD wStatu; 
	BOOL bEnd=FALSE;
	BOOL bLButtonDownPrev(FALSE);
	int mode;

	//取消息，直到焦点丢失或结束绘图过程,存储的坐标都是逻辑坐标
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	while (!bEnd)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
		case WM_MOUSEWHEEL:
 		case WM_LBUTTONDBLCLK: //双击退出至选择状态
 			break;
		case WM_LBUTTONDOWN:
			
			bLButtonDownPrev=TRUE;
			break;
		case WM_LBUTTONUP:
			if(bLine)
			{
				CDot dt=(CDot)pointNow;
				pView->LogicToPhysic(dt.x,dt.y);
				pStroke->Add(dt);
				pointOld=pointNow;
			}
			else
			{ 
				CDot dt=(CDot)pointNow;
				pView->LogicToPhysic(dt.x,dt.y);
				pStroke->Add(dt);
				pointOld=pointNow;
				bLine=TRUE;
			}
			break;
		case WM_RBUTTONDOWN:
			  bEnd=TRUE; 
			  mode=pDC->SetROP2(R2_NOTXORPEN);
			  pDC->MoveTo(pointOld);
			  pDC->LineTo(pointNow); 
			  pDC->SetROP2(mode);
			  break;
		case WM_MOUSEMOVE:
			wStatu=msg.wParam; 
			if(wStatu&MK_LBUTTON)
			{
				if(bLButtonDownPrev||(!bLButtonDownPrev&&bLine))
				{
					CDot dt=(CDot)pointNow;
					pView->LogicToPhysic(dt.x,dt.y);
					pStroke->Add(dt); 
					bLButtonDownPrev=FALSE;
				}
				bLine=FALSE;
				pointOld=pointNow;
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				
				pDC->MoveTo(pointOld);
				pDC->LineTo(pointNow);
				
				CDot dt=(CDot)pointNow;
				pView->LogicToPhysic(dt.x,dt.y);
				pStroke->Add(dt); 
			}
			else
			{
				mode=pDC->SetROP2(R2_NOTXORPEN);
				pDC->MoveTo(pointOld);
				pDC->LineTo(pointNow); 

				bLine=TRUE;
							
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				
				Adjust(msg,pointOld,pointNow);
				pDC->MoveTo(pointOld);
				pDC->LineTo(pointNow); 
				
				pDC->SetROP2(mode);
				
			}
			DispatchMessage(&msg);
			break;

		default:
			DispatchMessage(&msg);
			break;
		}
	}

	pDC->SelectObject(oldPen);
	pen.DeleteObject();

	pWnd->ReleaseDC(pDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	int nCount = pStroke->GetSize();
	if (0 >= nCount)
	{
		return FALSE;
	}
	else
	{
		m_list.AddTail(pStroke);
	}
	
	return TRUE;
}

 void CNodeLine::Serialize(FILE* fp,BOOL bFlag)
 {
 	CMySerialize fs(fp);
	CString str;

	if (bFlag)
	{
		//写入头信息
		fs<<"NLIN_B";
		m_property.Serialize(fp,bFlag);
		Serialize_list(fp,bFlag);
		//写入尾信息
		fs<<"NLIN_E";
	}
	else
	{
		while(fs>>str)
		{
			if(str.Left(5) == "PRO_B")
			{
				m_property.Serialize(fp,bFlag);
				continue;
			}
			if(str.Left(4) == "SL_B")
			{
				Serialize_list(fp,bFlag);
				continue;
			}
			if(str.Left(6) == "NLIN_E")
				break;
		}
	}
 }


void CNodeLine::AddRect(CWnd *pWnd)
{
	CHLView *pView=(CHLView*)pWnd;
	CDC *pDC=pView->GetDC();
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke=m_list.GetNext(pos);
	int nCount=pStroke->m_dotArray.GetSize();
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
		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);
	}
	pDC->SelectObject(OldPen);
	pView->ReleaseDC(pDC);
}

void CNodeLine::DelRect(CWnd *pWnd)
{
	CHLView *pView=(CHLView*)pWnd;
	CDC *pDC=pView->GetDC();
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke=m_list.GetNext(pos);
	int nCount=pStroke->m_dotArray.GetSize();
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
	pView->ReleaseDC(pDC);
}

//DEL void CNodeLine::GetDot(CPoint &point,myNode*pHead, CWnd *pWnd)
//DEL {
//DEL 	POSITION pos;
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
//DEL 		bFlag=FALSE;
//DEL 		for(int i=0;i<nCount;i++)
//DEL 		{
//DEL 			dot=pStroke->m_dotArray.GetAt(i);
//DEL 			if(quad.left<=dot.x && dot.x<quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
//DEL 			{
//DEL 				bFlag=TRUE;
//DEL 				pHead->m_pmyStroke=new myStroke[1];
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

//DEL void CNodeLine::ReplaceDot(myNode *pHead)
//DEL {
//DEL 	int i,j,k,nNum;
//DEL 	j=-1;
//DEL 	POSITION pos=m_list.GetHeadPosition();
//DEL 	if(pos==NULL)
//DEL 		return;
//DEL 	CStroke *pStroke;
//DEL 	DotSelectList *pList;
//DEL 	CDot dot;
//DEL 	for(i=0;i<pHead->m_nCount;i++)
//DEL 	{
//DEL 		k=(pHead->m_pmyStroke[i]).m_nRow;
//DEL 		while(j!=k)
//DEL 		{
//DEL 			pStroke=m_list.GetNext(pos);
//DEL 			j++;
//DEL 		}
//DEL 		pList=(pHead->m_pmyStroke[i]).pHead;
//DEL 		while(pList)
//DEL 		{
//DEL 			dot=pList->m_Node;
//DEL 			nNum=pList->m_nNum;
//DEL 			pStroke->m_dotArray.SetAt(nNum,dot);
//DEL 			pList=pList->m_pNext;
//DEL 		}
//DEL 	}
//DEL }

//DEL void CNodeLine::GetDot(CQuad &quad, myNode *pHead,CWnd *pWnd)
//DEL {
//DEL 	POSITION pos=m_list.GetHeadPosition();
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
