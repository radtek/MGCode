// AutoSortListObj.cpp: implementation of the CAutoSortListObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "AutoSortListObj.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRITICAL_SECTION g_scri;
void Run(LPVOID pPara,int nga)
{
	CAutoSortListObj* pSortObj = (CAutoSortListObj*)pPara;

	int nSize  = pSortObj->m_nSize;
	double dbDistance = 0;

	int *pnRetSort = new int[nSize];
	if (NULL == pnRetSort)
	{
		return;
	}
	CString str("");
	memset(pnRetSort, 0, sizeof(int)*nSize);
	while(pSortObj->m_bRun)
	{
		pSortObj->RunGA(nga, pnRetSort, &dbDistance, nSize);

		{
			SLock t;
			pSortObj->m_nCount++;
			if(dbDistance < pSortObj->m_dbMinDistance)
			{
				pSortObj->m_dbMinDistance = dbDistance;
				memcpy(pSortObj->m_pRetSortList, pnRetSort, sizeof(int)*nSize);
				((CHLView*)(pSortObj->m_pView))->Invalidate();
			}
			str.Format("Count:%d\r\nolddis:%.3f\r\nmindis:%.3f\r\neff:%.3f", 
				pSortObj->m_nCount,
				pSortObj->m_dbOldDistance, 
				pSortObj->m_dbMinDistance,
				(pSortObj->m_dbOldDistance - pSortObj->m_dbMinDistance) * 100 / pSortObj->m_dbOldDistance);
			str += "%";
			pSortObj->m_Msg.SetPromptMessage(str);
		}
	}// end while

	if (pnRetSort)
	{
		delete[] pnRetSort;
		pnRetSort = NULL;
	}

	return;
}

UINT RUNTSP0(LPVOID pPara)
{
	Run(pPara,0);
	return 1;
}

UINT RUNTSP1(LPVOID pPara)
{
	Run(pPara,1);
	return 1;
}

UINT RUNTSP2(LPVOID pPara)
{
	Run(pPara,2);
	return 1;
}

UINT RUNTSP3(LPVOID pPara)
{
	Run(pPara,3);
	return 1;
}

UINT RUNTSP4(LPVOID pPara)
{
	Run(pPara,4);
	return 1;
}

UINT RUNTSP5(LPVOID pPara)
{
	Run(pPara,5);
	return 1;
}

UINT RUNTSP6(LPVOID pPara)
{
	Run(pPara,6);
	return 1;
}

UINT RUNTSP7(LPVOID pPara)
{
	Run(pPara,7);
	return 1;
}

UINT RUNTSP8(LPVOID pPara)
{
	Run(pPara,8);
	return 1;
}

CAutoSortListObj::CAutoSortListObj()
{
	InitializeCriticalSection(&g_scri);
	InitGA    = NULL;
	RunGA     = NULL;
	m_hModule = NULL;
	m_pView   = NULL;
	m_nSize   = 0;
	m_pRetSortList = NULL;
	m_strDispMsg = "";

	m_dwSelObj = 0;
}

CAutoSortListObj::~CAutoSortListObj()
{
	DeleteCriticalSection(&g_scri);
	m_hModule = NULL;
}

BOOL CAutoSortListObj::InitSort()
{
	if (m_hModule)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}

	char szPath[256];
	GetExepath(szPath);
	char szFileName[256];
	sprintf_s(szFileName, 256, "%s%s", szPath, "XGA.dll");
	m_hModule = LoadLibrary(szFileName);
	if(m_hModule)
	{
		InitGA = (INITGA)GetProcAddress(m_hModule,"InitGA");
		RunGA  = (RUNGA)GetProcAddress(m_hModule,"RunGA");
	}
	else
	{
		AfxMessageBox(IDS_LOADLIB_ERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL CAutoSortListObj::DestroySort()
{
	if (m_hModule)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}

	return TRUE;
}

double CAutoSortListObj::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

BOOL CAutoSortListObj::RunSort(CWnd *pWnd)
{
	m_Msg.CreateMsgWindow();
	m_Msg.ShowWindow(SW_SHOW);
	m_Msg.UpdateWindow();

	m_Msg.SetViewTimer(200);
	m_Msg.SetPromptMessage("Start Run Auto Sort ...");
	
	if (NULL == pWnd)
	{
		return FALSE;
	}

	if (!InitSort())
	{
		return FALSE;
	}

	CHLView *pView = (CHLView*)pWnd;
	m_pView = pView;
	CHLDoc  *pDoc  = (CHLDoc*)(pView->GetDocument());

	m_dbMinDistance = 9999999999999999999.0;
	m_nCount = 0;

	CString str("");
	m_nSize = (pDoc->m_ASort).GetSize();
	if(m_nSize < 6)
	{
		m_Msg.SetPromptMessage("Err:Object < 6!!!\r\nPlease exit the function");
		return FALSE;
	}
	if(m_nSize>3000)
	{
		m_Msg.SetPromptMessage("Err:Object > 3000!!!\r\nPlease exit the function");
		return FALSE;
	}
	m_pRetSortList = new int[m_nSize];
	memset(m_pRetSortList, 0, sizeof(int)*m_nSize);
	if (!m_pRetSortList)
	{
		return FALSE;
	}

	POS* pPos = new POS[m_nSize];
	for(int i=0; i< m_nSize; i++)
	{
		ASNODE nd = pDoc->m_ASort.GetAt(i);
		pPos[i].sx = (double)nd.m_SDt.x;
		pPos[i].sy = (double)nd.m_SDt.y;
		pPos[i].ex = (double)nd.m_EDt.x;
		pPos[i].ey = (double)nd.m_EDt.y;
	}

	m_dbOldDistance = 0;
	for (int j = 0; j < m_nSize; j++)
	{
		int s = j;
		int e = (j+1)>=m_nSize?j+1-m_nSize:j+1;
		ASNODE ns = pDoc->m_ASort.GetAt(s);
		ASNODE ne = pDoc->m_ASort.GetAt(e);

		m_dbOldDistance += Distance(ns.m_EDt,ne.m_SDt);
	}

	InitGA(0, pPos, m_nSize);
	InitGA(1, pPos, m_nSize);
	InitGA(2, pPos, m_nSize);
	InitGA(3, pPos, m_nSize);
	InitGA(4, pPos, m_nSize);
	InitGA(5, pPos, m_nSize);
	InitGA(6, pPos, m_nSize);
	InitGA(7, pPos, m_nSize);
	InitGA(8, pPos, m_nSize);

	if (pPos)
	{
		delete[] pPos;
		pPos = NULL;
	}
	
	m_bRun = TRUE;
	AfxBeginThread(RUNTSP0, this);
	AfxBeginThread(RUNTSP1, this);
	AfxBeginThread(RUNTSP2, this);
	AfxBeginThread(RUNTSP3, this);
	AfxBeginThread(RUNTSP4, this);
	AfxBeginThread(RUNTSP5, this);
	AfxBeginThread(RUNTSP6, this);
	AfxBeginThread(RUNTSP7, this);
	AfxBeginThread(RUNTSP8, this);

	return TRUE;
}

BOOL CAutoSortListObj::ExitSort()
{
	m_bRun = FALSE;
	m_Msg.SetViewTimer(0);
	m_Msg.Close();
	Sleep(1000);

	if (m_pRetSortList) 
	{
		delete[] m_pRetSortList;
		m_pRetSortList = NULL;
	}

	m_pView = NULL;

	DestroySort();
	return TRUE;
}

BOOL CAutoSortListObj::SetSortResult(int *pRetList, int nSize)
{
	if (!m_pView)
	{
		return FALSE;
	}
	CHLDoc* pDoc = (CHLDoc*)(((CHLView*)m_pView)->GetDocument());

	int m_nSize = (pDoc->m_ASort).GetSize();
	if (m_nSize <= 0)
	{
		return FALSE;
	}

	for(int i=0; i< m_nSize; i++)
	{
		(pDoc->m_ASort)[i].m_nSort = pRetList[i] - 1;
	}
	
	return TRUE;
}

BOOL CAutoSortListObj::Track(CWnd* pView)
{
	if ((NULL != pView) && (CWnd::GetCapture() != NULL))
	{
		return FALSE;
	}

	CHLView *pHLView=(CHLView*)pView;

	CHLDoc *pDoc = (CHLDoc*)(pHLView->GetDocument());
	if (NULL == pDoc)
	{
		return FALSE;
	}
	
	BOOL bRet = FALSE;

	AfxLockTempMaps();

	pView->SetCapture();
	pView->UpdateWindow();

	CPoint pointNow;
	
	CTree *pTree  = NULL;
	int    nIndex = 0;

	//消息处理
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CString str,prompt;
	prompt.LoadString(IDS_PROMPT_RECT);
	for (;;)
	{
		MSG msg;
		BOOL bRm = ::GetMessage(&msg, NULL, 0, 0);
		if (!bRm)
		{
			bRet = FALSE;
			goto ExitLoop;
		}
		if( (msg.message == WM_NCPAINT) ||
			(msg.message == WM_PAINT))
			DispatchMessage(&msg);

		if((msg.hwnd != pView->GetSafeHwnd())&&(msg.hwnd!=m_Msg.GetSafeHwnd()))
			continue;

		switch (msg.message)
		{
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
			{
				DispatchMessage(&msg);
				break;
			}
		case WM_LBUTTONDBLCLK:
			{
				if (IDOK ==AfxMessageBox(IDS_STRISEXITSORTPRO, MB_OKCANCEL)) 
				{
					pHLView->m_nDrawTool = ID_DRAW_CLICK;
					bRet = TRUE;
					goto ExitLoop;
				}
				
				break;
			}
		case WM_RBUTTONDOWN:
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_TIMER:
		case WM_COMMAND:
		case WM_SYSCOMMAND:
			{
				DispatchMessage(&msg);
				break;
			}
		case WM_SIZING:
			{
				DispatchMessage(&msg);
				break;
			}
		default:
			{
//				DispatchMessage(&msg);
		        break;
			}
		}// end switch
	}// end for

ExitLoop:
	ReleaseCapture();
	AfxUnlockTempMaps(FALSE);

return bRet;
}


BOOL CAutoSortListObj::TrackSelPt(CWnd* pView, CArray<ASNODE,ASNODE> &Sort)
{
	if ((NULL != pView) && (CWnd::GetCapture() != NULL))
	{
		return FALSE;
	}

	CHLView *pHLView=(CHLView*)pView;

	CHLDoc *pDoc = (CHLDoc*)(pHLView->GetDocument());
	if (NULL == pDoc)
	{
		return FALSE;
	}
	
	BOOL bRet = FALSE;

	AfxLockTempMaps();

	pView->SetCapture();
	pView->UpdateWindow();

	CPoint pointNow;
	
	CTree *pTree  = NULL;
	int    nIndex = 0;

	

	//消息处理
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CString str,prompt;
	prompt.LoadString(IDS_PROMPT_RECT);
 
	for (;;)
	{
		MSG msg;
		BOOL bRm = ::GetMessage(&msg, NULL, 0, 0);
		if (!bRm)
		{
			bRet = FALSE;
			goto ExitLoop;
		}
		if( (msg.message == WM_NCPAINT) ||
			(msg.message == WM_PAINT) )
		{
			DispatchMessage(&msg);
		}

		if((msg.hwnd != pView->GetSafeHwnd())&&(msg.hwnd!=m_Msg.GetSafeHwnd()))
			continue;

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				DispatchMessage(&msg);
				break;
			}
		case WM_LBUTTONDOWN:
			{
				pointNow.x = (long)(short)LOWORD(msg.lParam);
				pointNow.y = (long)(short)HIWORD(msg.lParam);

				int nCount = Sort.GetSize();
				for (int i = 0; i < nCount; i++) 
				{
					ASNODE nd = Sort.GetAt(i);
					double sx = (double)nd.m_SDt.x;
					double sy = (double)nd.m_SDt.y;
					pHLView->PhysicToLogic(sx, sy);
					CRect rts((long)sx - 3, (long)sy - 3,(long)sx + 3, (long)sy + 3);
					
					double ex = (double)nd.m_EDt.x;
					double ey = (double)nd.m_EDt.y;
					pHLView->PhysicToLogic(ex, ey);
					CRect rte((long)ex - 3, (long)ey - 3,(long)ex + 3, (long)ey + 3);

					if ((rts.PtInRect(pointNow)) || (rte.PtInRect(pointNow)))
					{
						m_dwSelObj = nd.m_pTree;
						bRet = TRUE;
						CArray<ASNODE,ASNODE> Sn;
						Sn.RemoveAll();
						for (int j = i; j < nCount; j++)
						{
							ASNODE nd = Sort.GetAt(j);
							Sn.Add(nd);
						}
						for (int k = 0; k < i; k++)
						{
							ASNODE nd = Sort.GetAt(k);
							Sn.Add(nd);
						}
						Sort.RemoveAll();
						Sort.Append(Sn);
						Sn.RemoveAll();

						goto ExitLoop;
					}
				}// end for (int i = 0;
				DispatchMessage(&msg);
				break;
			}
		case WM_LBUTTONDBLCLK:
			{
				if (IDOK ==AfxMessageBox(IDS_STRISEXITSORTPRO, MB_OKCANCEL)) 
				{
					pHLView->m_nDrawTool = ID_DRAW_CLICK;
					bRet = FALSE;
					goto ExitLoop;
				}
				
				break;
			}
		case WM_RBUTTONDOWN:
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
		case CN_UPDATE_COMMAND_UI:
			{
				DispatchMessage(&msg);
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
	ReleaseCapture();
	AfxUnlockTempMaps(FALSE);

return bRet;
}

void CAutoSortListObj::GetResultMsg()
{
	SLock t;
	if ((!m_pView) || (!m_pRetSortList))
	{
		return;
	}
	
	CHLDoc* pDoc = (CHLDoc*)(((CHLView*)m_pView)->GetDocument());

	int m_nSize = (pDoc->m_ASort).GetSize();
	if (m_nSize <= 0)
	{
		return;
	}

	for(int i=0; i< m_nSize; i++)
	{
		(pDoc->m_ASort)[i].m_nSort = m_pRetSortList[i] - 1;
	}

	((CHLView*)(m_pView))->m_strAutoSortInfo = m_strDispMsg;
	
	return;
}

