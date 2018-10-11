// NodeRect.cpp: implementation of the CNodeRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "NodeRect.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double g_Randon=0.5;
CNodeRect::CNodeRect()
{
	m_type=NODE_RECT;
}

CNodeRect::~CNodeRect()
{
}

BOOL CNodeRect::Track(CWnd* pWnd, CPoint ptOrg)
{
	// A NULL return value means the current thread has not captured the mouse. However,
	// it is possible that another thread or process has captured the mouse.
	if (::GetCapture() != NULL)
	{
		return FALSE;
	}

	CHLView *pHLView=(CHLView*)pWnd;
	
	BOOL bResult=TRUE;

	AfxLockTempMaps();

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	CDC* pDC;
	pDC       = pWnd->GetDC();
	XDC *pXDC = pHLView->PrepareDC(pDC);
	ASSERT_VALID(pDC);

	CPoint pointNow;
	
	pointNow    = ptOrg;
	BOOL bMoved = FALSE;

	//消息处理
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CString str,prompt;
	prompt.LoadString(IDS_PROMPT_RECT);

	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
		{
			break;
		}

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				if(bMoved)
				{
					bResult=TRUE;
				}
				else
				{
					bResult=FALSE;
				}
				goto ExitLoop;
				
				break;
			}
		case WM_MOUSEMOVE:
			{
				if(!bMoved)
				{
					bMoved=TRUE;
				}

				Adjust(msg,ptOrg);
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);

				int mode=pDC->SetROP2(R2_NOTXORPEN);
				CDot dot1,dot2;
				dot1.x=ptOrg.x;
				dot1.y=ptOrg.y;
				dot2.x=pointNow.x;
				dot2.y=pointNow.y;

				pHLView->LogicToPhysic(dot1.x,dot1.y);
				pHLView->LogicToPhysic(dot2.x,dot2.y);

				Draw(pXDC);
				Create(dot1,dot2);
				Draw(pXDC);
				pDC->SetROP2(mode);

				break;
			}
		default:
			{
				DispatchMessage(&msg);
		        break;
			}
		}// end switch
	}// end for

ExitLoop:

	pWnd->ReleaseDC(pDC);
	ReleaseCapture();
	AfxUnlockTempMaps(FALSE);

	if(!bResult)
	{
		return FALSE;
	}

return TRUE;
}


BOOL CNodeRect::Create(CDot dot1,CDot dot2)
{
	Delete();
	CStroke *pStroke = NULL;
	CDot     dot;
	
	pStroke=new CStroke;
	if (pStroke == NULL)
	{
		return FALSE;
	}
	pStroke->SetProperty(PTY_INIT, m_property);

	dot=dot1;
	
	pStroke->Add(dot);
	dot.x=dot2.x;
	dot.y=dot1.y;

	pStroke->Add(dot);
	dot=dot2;

	pStroke->Add(dot);
	dot.x=dot1.x;
	dot.y=dot2.y;

	pStroke->Add(dot);
	dot=dot1;

	pStroke->Add(dot);

	m_list.AddTail(pStroke);

	return TRUE;
}

/*
BOOL CNodeRect::Create(CRect &rect,int nMode,double dbValue,BOOL bPreview)
{
	Delete();

	//生成四个点
	CStroke *pStroke=new CStroke(m_property);
	CDot dot0,dot1,dot2,dot3,dot4,dot5,dot6,dot7;

	switch(nMode)
	{

	case 1: //倒圆
		if(rect.Width()>=2*dbValue&&rect.Height()>=2*dbValue)
		{
            dot0.x=rect.left+dbValue;
			dot0.y=rect.bottom;
			dot1.x=rect.right-dbValue;
			dot1.y=rect.bottom;
			pStroke->Add(dot0);
			pStroke->Add(dot1);

			AddCircleStroke(pStroke,CDot(rect.right-dbValue,rect.bottom-dbValue),dbValue,0.5*PAI,0,bPreview);

			dot2.x=rect.right;
			dot2.y=rect.bottom-dbValue;
			dot3.x=rect.right;
			dot3.y=rect.top+dbValue;
			pStroke->Add(dot2);
			pStroke->Add(dot3);

			AddCircleStroke(pStroke,CDot(rect.right-dbValue,rect.top+dbValue),dbValue,0,-0.5*PAI,bPreview);

			dot4.x=rect.right-dbValue;
			dot4.y=rect.top;
			dot5.x=rect.left+dbValue;
			dot5.y=rect.top;
			pStroke->Add(dot4);
			pStroke->Add(dot5);

			AddCircleStroke(pStroke,CDot(rect.left+dbValue,rect.top+dbValue),dbValue,-0.5*PAI,-1.0*PAI,bPreview);

			dot6.x=rect.left;
			dot6.y=rect.top+dbValue;
			dot7.x=rect.left;
			dot7.y=rect.bottom-dbValue;
			pStroke->Add(dot6);
			pStroke->Add(dot7);

			AddCircleStroke(pStroke,CDot(rect.left+dbValue,rect.bottom-dbValue),dbValue,-1.0*PAI,-1.5*PAI,bPreview);

			pStroke->Add(dot0);

		}
		else
		{
            dot0.x=rect.left;
			dot0.y=rect.bottom;
			dot1.x=rect.right;
			dot1.y=rect.bottom;
			dot2.x=rect.right;
			dot2.y=rect.top;
			dot3.x=rect.left;
			dot3.y=rect.top;
			pStroke->Add(dot0);
			pStroke->Add(dot1);
			pStroke->Add(dot2);
			pStroke->Add(dot3);
			pStroke->Add(dot0);
		}
		break;
	case 2://倒角
		if(rect.Width()>=2*dbValue&&rect.Height()>=2*dbValue)
		{

			dot0.x=rect.left+dbValue;
			dot0.y=rect.bottom;
			dot1.x=rect.right-dbValue;
			dot1.y=rect.bottom;
			dot2.x=rect.right;
			dot2.y=rect.bottom-dbValue;
			dot3.x=rect.right;
			dot3.y=rect.top+dbValue;
			dot4.x=rect.right-dbValue;
			dot4.y=rect.top;
			dot5.x=rect.left+dbValue;
			dot5.y=rect.top;
			dot6.x=rect.left;
			dot6.y=rect.top+dbValue;
			dot7.x=rect.left;
			dot7.y=rect.bottom-dbValue;
			pStroke->Add(dot0);
			pStroke->Add(dot1);
			pStroke->Add(dot2);
			pStroke->Add(dot3);
			pStroke->Add(dot4);
			pStroke->Add(dot5);
			pStroke->Add(dot6);
			pStroke->Add(dot7);
			pStroke->Add(dot0);

		}
		else
		{
            dot0.x=rect.left;
			dot0.y=rect.bottom;
			dot1.x=rect.right;
			dot1.y=rect.bottom;
			dot2.x=rect.right;
			dot2.y=rect.top;
			dot3.x=rect.left;
			dot3.y=rect.top;
			pStroke->Add(dot0);
			pStroke->Add(dot1);
			pStroke->Add(dot2);
			pStroke->Add(dot3);
			pStroke->Add(dot0);
		}
		
        break;
	default:
		dot0.x=rect.left;
		dot0.y=rect.bottom;
		dot1.x=rect.right;
		dot1.y=rect.bottom;
		dot2.x=rect.right;
		dot2.y=rect.top;
		dot3.x=rect.left;
		dot3.y=rect.top;
		
		pStroke->Add(dot0);
		pStroke->Add(dot1);
		pStroke->Add(dot2);
		pStroke->Add(dot3);
		pStroke->Add(dot0);

	}
	

	m_list.AddTail(pStroke);
	return TRUE;
}
*/

void CNodeRect::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;

	if (bFlag)
	{
		//写入头信息
		fs<<"NRCT_B";
		m_property.Serialize(fp,bFlag);
		Serialize_list(fp,bFlag);
		//写入尾信息
		fs<<"NRCT_E";
	}
	else
	{
		while(fs>>str)
		{
			if(str == "PRO_B")
			{
				m_property.Serialize(fp,bFlag);
				continue;
			}
			if(str == "SL_B")
			{
				Serialize_list(fp,bFlag);
				continue;
			}
			if(str == "NRCT_E")
				break;
		}
	}

	return;
}
void CNodeRect::AddCircleStroke(CStroke *pStroke, CDot CenterDot, double dbR, double dbStartAngle, double dbEndAngle, BOOL bPreview)
{
	double dbAngleStep = 0.1;
	double dbCurAngle  = dbStartAngle;
	CDot dot;

	if(!bPreview)
	{
		//如果不是绘制时的预览拖动状态，按弦长0.2毫米来生成圆弧
       double dbTemp= 0.2;

	   dbTemp       = MyTrueToLog(dbTemp);
	   dbAngleStep  = 2.0 * asin(dbTemp/2.0/dbR);
	}

	if(dbStartAngle<dbEndAngle)
	{
		for(int i = 0; dbCurAngle < dbEndAngle; i++)
		{
			dot.x = CenterDot.x + dbR * cos(dbCurAngle);
			dot.y = CenterDot.y + dbR * sin(dbCurAngle);

			pStroke->Add(dot);
			dbCurAngle+=dbAngleStep;
		}
	}
	else
	{
		for(int i = 0; dbCurAngle > dbEndAngle; i++)
		{
			dot.x = CenterDot.x + dbR * cos(dbCurAngle);
			dot.y = CenterDot.y + dbR * sin(dbCurAngle);

			pStroke->Add(dot);
			dbCurAngle-=dbAngleStep;
		}
	}
	
    dot.x = CenterDot.x + dbR * cos(dbEndAngle);
	dot.y = CenterDot.y + dbR * sin(dbEndAngle);
	pStroke->Add(dot);

	return;
}



void CNodeRect::TranslateDot(double &x,double &y,double a,double x0,double y0)
{
	double dbx,dby;

	dbx = x - x0;
	dby = y - y0;
	x   = dbx * cos(a) + dby * sin(a);
	y   = dby * cos(a) - dbx * sin(a);

	return;
}

