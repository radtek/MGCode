// SortListObj.cpp: implementation of the CSortListObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "SortListObj.h"
#include "HLView.h"
#include "SetSortDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortListObj::CSortListObj()
{
	m_nCurIndex = 0;
}

CSortListObj::~CSortListObj()
{

}

BOOL CSortListObj::InitSortPro()
{
	m_nCurIndex = 0;

	return TRUE;
}

BOOL CSortListObj::Track(CWnd* pView)
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
	
	BOOL bChange = FALSE;

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
			goto ExitLoop;
		}

		if( (msg.message == WM_NCPAINT) ||
			(msg.message == WM_PAINT) )
			DispatchMessage(&msg);

		if(msg.hwnd != pView->GetSafeHwnd())
			continue;

		switch (msg.message)
		{
		case WM_LBUTTONDOWN:
			{
				if (pTree)
				{
					pTree->SetSortStatus(FALSE);
					DrawSRect(pDoc->m_Sort, nIndex, FALSE);
					pTree = NULL;
				}
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				if (IsDotInSortList(pointNow, pView,pDoc->m_Sort, nIndex))
				{
					pTree = (CTree*)((pDoc->m_Sort[nIndex]).m_pTree);
					if (pTree)
					{
						pTree->SetSortStatus(TRUE);
						DrawSRect(pDoc->m_Sort, nIndex, TRUE);
					}
				}
				DispatchMessage(&msg);
				pHLView->Invalidate();
				break;
			}
		case WM_LBUTTONDBLCLK:
			{
				if (pTree)
				{
					pTree->SetSortStatus(FALSE);
					DrawSRect(pDoc->m_Sort,nIndex, FALSE);
					pTree = NULL;
				}
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				if (IsDotInSortList(pointNow, pView,pDoc->m_Sort, nIndex))
				{
					pTree = (CTree*)((pDoc->m_Sort[nIndex]).m_pTree);
					if (pTree)
					{
						pTree->SetSortStatus(TRUE);
					}
					DrawSRect(pDoc->m_Sort, nIndex, TRUE);
					if ((m_nCurIndex -1) != (pDoc->m_Sort[nIndex]).m_nSort) 
					{
						int nCount = pDoc->m_Sort.GetSize();
						SortCurList(pDoc->m_Sort, nIndex);
						(pDoc->m_Sort[nIndex]).m_nSort = m_nCurIndex;
						m_nCurIndex ++;
						m_nCurIndex = (m_nCurIndex % nCount);
						bChange = TRUE;
					}
					pHLView->Invalidate();
				}
				else
				{
					pHLView->Invalidate();
					CString str("");
					str.LoadString(IDS_STRISEXITSORTPRO);
					if (IDOK == AfxMessageBox(str, MB_OKCANCEL)) 
					{
						pHLView->m_nDrawTool = ID_DRAW_CLICK;
						goto ExitLoop;
					}
				}
				break;
			}
		case WM_RBUTTONDOWN:
			{
				DrawSRect(pDoc->m_Sort, nIndex, FALSE);
				if (pTree)
				{
					pTree->SetSortStatus(FALSE);
					pTree = NULL;
				}
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				if (IsDotInSortList(pointNow, pView,pDoc->m_Sort, nIndex))
				{
					pTree = (CTree*)((pDoc->m_Sort[nIndex]).m_pTree);
					if (pTree)
					{
						pTree->SetSortStatus(TRUE);
						DrawSRect(pDoc->m_Sort, nIndex, TRUE);
					}

					CSetSortDlg SortDlg;
					SortDlg.m_nNewNum = (pDoc->m_Sort[nIndex]).m_nSort;
					ReleaseCapture();
					int nRet = SortDlg.DoModal();
					pHLView->SetCapture();
					
					if (IDOK != nRet) 
					{
						break;
					}
					switch(SortDlg.m_nChangeType)
					{
					case 0:
						{
							///////////////////////////
							int nCount = (pDoc->m_Sort).GetSize();
							if (SortDlg.m_nNewNum >= nCount) 
							{
								SortDlg.m_nNewNum = nCount - 1;
							}

							int nOldSort = (pDoc->m_Sort[nIndex]).m_nSort;

							int nDst = abs(SortDlg.m_nNewNum - nOldSort);
							if (SortDlg.m_nNewNum > nOldSort)
							{
								int nJCount = 0;
								for (int i = 0; i < nCount; i++)
								{
									int nI = (pDoc->m_Sort[i]).m_nSort;
									if ((nI > nOldSort) && (nI <= SortDlg.m_nNewNum))
									{
										nDst++;
										(pDoc->m_Sort[i]).m_nSort--;
									}
									if (nJCount == nDst)
									{
										break;
									}
								}// end for
							}
							else if (SortDlg.m_nNewNum < nOldSort)
							{
								int nJCount = 0;
								for (int i = 0; i < nCount; i++)
								{
									int nI = (pDoc->m_Sort[i]).m_nSort;
									if ((nI < nOldSort) && (nI >= SortDlg.m_nNewNum))
									{
										nDst++;
										(pDoc->m_Sort[i]).m_nSort++;
									}
									if (nJCount == nDst)
									{
										break;
									}
								}// end for
							}

							bChange = TRUE;
							(pDoc->m_Sort[nIndex]).m_nSort = SortDlg.m_nNewNum;
							///////////////////////////
							break;
						}
					case 1:
						{
							int nCount = (pDoc->m_Sort).GetSize();
							if (SortDlg.m_nNewNum >= nCount) 
							{
								SortDlg.m_nNewNum = nCount - 1;
							}

							int nOldSort = (pDoc->m_Sort[nIndex]).m_nSort;
							
							for (int i = 0; i < nCount; i++)
							{
								int nI = (pDoc->m_Sort[i]).m_nSort;
								if (nI == SortDlg.m_nNewNum) 
								{
									int nExchng = (pDoc->m_Sort[i]).m_nSort;
									(pDoc->m_Sort[i]).m_nSort = nOldSort;
									(pDoc->m_Sort[nIndex]).m_nSort = nExchng;
									break;
								}
							}
							bChange = TRUE;
							break;
						}
					default:
						{
							break;
						}
					}
					pHLView->Invalidate();
				}
				else
				{
					DispatchMessage(&msg);
				}
				break;
			}
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
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

return bChange;
}


BOOL CSortListObj::IsDotInSortList(CPoint &point, CWnd *pWnd, CArray<SORTNODE,SORTNODE> &slist, int &nRetIndex)
{
	int nCount= slist.GetSize();
	if(nCount <= 0)
	{
		return FALSE;
	}

	CHLView *pView=(CHLView*)pWnd;
	CQuad quad;
	SORTNODE   sNode;
	double dbdata = 3;
	pView->ActualLen(dbdata);
	CRect rect;
	for(int i = 0; i< nCount; i++)
	{
		sNode       = slist.GetAt(i);
		quad.left   = sNode.m_SPt.x;
		quad.top    = sNode.m_SPt.y;
		quad.right  = sNode.m_SPt.x;
		quad.bottom = sNode.m_SPt.y;
		
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);
		quad.top    = quad.top - 10;
		quad.right  = quad.right + 20;
		rect = (CRect)quad;

		int nOff = sNode.m_MNum * 21;

		if((rect.left + nOff)<=point.x && point.x<=(rect.right  + nOff) && rect.top<=point.y && point.y<=rect.bottom)
		{
			nRetIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CSortListObj::SortCurList(CArray<SORTNODE,SORTNODE> &slist, int nIndex)
{
	int nCount = slist.GetSize();
	if (nCount <= 0) 
	{
		return FALSE;
	}
	
	int nOldSort = slist[nIndex].m_nSort;
	for (int i = 0; i < nCount; i++)
	{
		int nI = slist[i].m_nSort;
		if (m_nCurIndex == nI) 
		{
			slist[i].m_nSort = nOldSort;
			break;
		}
	}

	return TRUE;
}

BOOL CSortListObj::DrawSRect(CArray<SORTNODE,SORTNODE> &slist, int nIndex, BOOL bDraw)
{
	int nCount = slist.GetSize();
	if ((nCount <= 0) || ((nCount - 1) < nIndex))
	{
		return FALSE;
	}
	
	if (bDraw)
	{
		slist[nIndex].m_Color = RGB(0, 0, 81);
	}
	else
	{
		slist[nIndex].m_Color = RGB(128, 128, 128);
	}

	return TRUE;
}