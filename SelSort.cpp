// SelSort.cpp: implementation of the CSelSort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "SelSort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelSort::CSelSort()
{

}

CSelSort::~CSelSort()
{

}

DWORD CSelSort::GetSort(CArray<GNODE,GNODE> &ObjList, CStroke *pStroke)
{
	if (NULL == pStroke)
	{
		return -1;
	}

	int nCount   = ObjList.GetSize();

	int nDtCount = pStroke->GetSize();
	if (2 > nDtCount)
	{
		return -1;
	}

	CArray<GNODE,GNODE> SortList;
	CArray<GNODE,GNODE> tempList;

	for (int i = 0; i < nDtCount - 1; i++)
	{
		CDot dt1 = pStroke->GetAt(i);
		CDot dt2 = pStroke->GetAt(i + 1);

		tempList.RemoveAll();
		for (int j = 0; j < nCount; j++)
		{
			BOOL bSort = ObjList[j].m_bIsSort;
			if (!bSort) 
			{
				if (GetRectCrossDot(ObjList[j], dt1, dt2, tempList)) 
				{
					ObjList[j].m_bIsSort  = TRUE;
				}
			}
		}

		SortList.Append(tempList);
	}// end for (int i = 0; i < nDtCount - 2; i++)

	int nLen = SortList.GetSize();
	for (int k = 0; k < nLen; k++)
	{
		SortList[k].m_nNum = k;
		SortList[k].m_bIsSort  = TRUE;
	}

	ObjList.RemoveAll();
	ObjList.Append(SortList);

	return nCount - nLen;
}

BOOL CSelSort::GetRectCrossDot(GNODE &gNode, CDot &dtS, CDot &dtE, CArray<GNODE,GNODE> &ObjList)
{
	CDot dt1, dt2, CrosDot;
	BOOL bRet = FALSE;

	QUADSG quad = gNode.m_rt;
	double dbMin = 99999999;

	dt1.x = quad.left;
	dt1.y = quad.top;
	dt2.x = quad.right;
	dt2.y = quad.top;
	BOOL bIsGet = GetCrossDot(dt1, dt2, dtS, dtE, CrosDot);
	if (bIsGet)
	{
		double dbDs = Distance(CrosDot, dtS);
		if (dbMin > dbDs) 
		{
			dbMin = dbDs;
		}

		bRet = TRUE;
	}

	dt1.x = quad.right;
	dt1.y = quad.top;
	dt2.x = quad.right;
	dt2.y = quad.bottom;
	bIsGet = GetCrossDot(dt1, dt2, dtS, dtE, CrosDot);
	if (bIsGet)
	{
		double dbDs = Distance(CrosDot, dtS);
		if (dbMin > dbDs) 
		{
			dbMin = dbDs;
		}
		bRet = TRUE;
	}

	dt1.x = quad.right;
	dt1.y = quad.bottom;
	dt2.x = quad.left;
	dt2.y = quad.bottom;
	bIsGet = GetCrossDot(dt1, dt2, dtS, dtE, CrosDot);
	if (bIsGet)
	{
		double dbDs = Distance(CrosDot, dtS);
		if (dbMin > dbDs) 
		{
			dbMin = dbDs;
		}
		bRet = TRUE;
	}

	dt1.x = quad.left;
	dt1.y = quad.bottom;
	dt2.x = quad.left;
	dt2.y = quad.top;
	bIsGet = GetCrossDot(dt1, dt2, dtS, dtE, CrosDot);
	if (bIsGet)
	{
		double dbDs = Distance(CrosDot, dtS);
		if (dbMin > dbDs) 
		{
			dbMin = dbDs;
		}

		bRet = TRUE;
	}

	if (bRet)
	{
		BOOL bInsert = FALSE;
		gNode.m_dbDistanc = dbMin;
		int nCount = ObjList.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			GNODE gNd  = ObjList.GetAt(i);
			if (gNd.m_dbDistanc > gNode.m_dbDistanc)
			{
				int nPos = i;
				if (nPos < 0) 
				{
					nPos = 0;
				}
				ObjList.InsertAt(nPos, gNode);
				bInsert = TRUE;
				break;
			}
		}

		if (!bInsert)
		{
			ObjList.Add(gNode);
		}
	}
	
	return bRet;
}

/*
求两直线交点，前提是两条直线必须有交点
在相交的情况下，可以应付各种情况(垂直、系数等)
*/
BOOL CSelSort::GetCrossDot(CDot &p1, CDot &p2, CDot &q1, CDot &q2, CDot &CrosDot)
{
	//必须相交求出的才是线段的交点，但是下面的程序段是通用的
	BOOL bRet = IsCross(p1,p2,q1,q2);
	if (!bRet)
	{
		return FALSE;
	}
	
	/*根据两点式化为标准式，进而求线性方程组*/
	double tempLeft(0.0), tempRight(0.0);

	//求x坐标
	tempLeft  = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
	tempRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) 
		        * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);
    CrosDot.x = tempRight / tempLeft;

	//求y坐标	
	tempLeft  = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
	tempRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x- p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y 
		        * (q1.x - q2.x) * (p2.y - p1.y);

	CrosDot.y = tempRight /  tempLeft;

	return TRUE;
}

double CSelSort::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

BOOL CSelSort::IsCross(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 )
{
	double dblMulti = 0;
	double dblTmp1  = 0;
	double dblTmp2  = 0;

	// 顶点相交
	if ( p1 == p3 || p1==p4 || p2==p3 || p2==p4 )
	{
		return TRUE;
	}

    // 判断两条线段外接矩形是否相交
	if ( min(p1.x,p2.x) > max(p3.x,p4.x) || max(p1.x,p2.x)
		< min(p3.x,p4.x) || min(p1.y,p2.y) > max(p3.y,p4.y)
		|| max(p1.y,p2.y) < min(p3.y,p4.y))
	{
		return FALSE;
	}

    //计算叉积  
	dblTmp1 = (double)((p1.x - p3.x)*(p4.y-p3.y) - (p4.x-p3.x)*(p1.y - p3.y));
	dblTmp2 = (double)((p4.x -p3.x)*(p2.y - p3.y) - (p2.x-p3.x)*(p4.y-p3.y));
	dblMulti = dblTmp1 * dblTmp2;
	
	if ( dblMulti >= 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

void CSelSort::Adjust(MSG msg, CPoint pointOld,CPoint &pointNew)
{
	int cx,cy;
	if(MK_CONTROL&msg.wParam)
	{
		cx=pointNew.x-pointOld.x;
		cy=pointNew.y-pointOld.y;
		if(abs(cx)>abs(cy))
		{
			pointNew.y=pointOld.y;
		}
		else
		{
			pointNew.x=pointOld.x;
		}
	}

	return;
}


BOOL CSelSort::Track(CWnd* pWnd, CArray<GNODE,GNODE> &ObjList)
{
	// 如果已经有窗口捕获了鼠标则退出
	if (::GetCapture() != NULL)
	{
		return FALSE;
	}

	// 在循环过程中保持映射模式
	AfxLockTempMaps();

	//捕获鼠标
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	CHLView* pView = (CHLView*)pWnd;
	if (!pView)
	{
		return FALSE;
	}

	// 取设备上下文
	CDC* pDC;
	pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);
	CPen pen(PS_SOLID,1,RGB(255,0,0));  
	CPen *oldPen=pDC->SelectObject(&pen);

	CPoint pointNow;
	CPoint pointOld;
	CPoint pointTemp;
	BOOL bLine=TRUE; 

	BOOL bRet   = FALSE; 
	BOOL bFirst = TRUE;

	CProperty propty;
	propty.m_color = RGB(255, 0, 0);
	CStroke *pStroke=new CStroke(propty);
	WORD wStatu; 
	BOOL bEnd=FALSE;
	BOOL bLButtonDownPrev(FALSE);
	int mode;

	CString strMsg;
	strMsg.LoadString(IDS_SELSORTMSG);
	CString strPopMsg;

	//取消息，直到焦点丢失或结束绘图过程,存储的坐标都是逻辑坐标
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	while (!bEnd)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if (CWnd::GetCapture() != pWnd)
		{
			bRet = FALSE;
			break;
		}

		switch (msg.message)
		{
		case WM_MOUSEWHEEL:
 		case WM_LBUTTONDBLCLK: //双击退出至选择状态
			bRet = TRUE;
 			break;
		case WM_LBUTTONDOWN:
			bLButtonDownPrev=TRUE;
			if (bFirst) 
			{
				pointNow.x=(int)(short)LOWORD(msg.lParam);
				pointNow.y=(int)(short)HIWORD(msg.lParam);
				bFirst = FALSE;
			}

			bRet = TRUE;
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
			bRet = TRUE;
			break;
		case WM_RBUTTONDOWN:
			{
				  bEnd=TRUE; 
				  mode=pDC->SetROP2(R2_NOTXORPEN);
				  pDC->MoveTo(pointOld);
				  pDC->LineTo(pointNow); 
				  pDC->SetROP2(mode);

				  int nCunt = ObjList.GetSize();
				  DWORD dwCount = GetSort(ObjList, pStroke);

				  if (0 == dwCount)
				  {
					  bRet = TRUE;
				  }
				  else
				  {
					  strPopMsg.Format(strMsg, nCunt, nCunt - (int)dwCount);
					  if (IDYES == AfxMessageBox(strPopMsg, MB_YESNO)) 
					  {
						  bRet = TRUE;
					  }
					  else
					  {
						  bRet = FALSE;
					  }
				  }

				  break;
			}
		case WM_MOUSEMOVE:

			if (!bFirst) 
			{
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
			}// end if (!bFirst) 
			
			DispatchMessage(&msg);
			bRet = TRUE;
			break;

		default:
			DispatchMessage(&msg);
			bRet = TRUE;
			break;
		}
	}

	pDC->SelectObject(oldPen);
	pen.DeleteObject();

	pWnd->ReleaseDC(pDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	if (NULL != pStroke) 
	{
		pStroke->RemoveAllPoint();
		delete pStroke;
		pStroke = NULL;
	}

	return bRet;
}
