// SelMark.cpp: implementation of the CSelMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "SelMark.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelMark::CSelMark()
{
	m_hThread = NULL;
	m_pDoc = NULL;
	m_bIsStop = TRUE;

	m_nMarkMode = ENU_MARKSEL;
	m_dbLineLen = 0.0;
	
	char szEvent[256];
	sprintf(szEvent,"%s","SelMark");
    m_hEventSelMark     = ::CreateEvent(NULL,TRUE,FALSE,szEvent);
}

CSelMark::~CSelMark()
{
	CloseHandle(m_hEventSelMark);
	if (m_hThread!=NULL)
	{
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}
}

CHLDoc *CSelMark::GetDocument() 
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return NULL;
	}

	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{	
		return NULL;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return NULL;
	}

	return pDoc;
}

UINT CSelMark::StartSelMark(HWND hWnd , SUBOBJMARKSET &stSub, int nMarkMode)
{
	m_bIsStop = FALSE;

	m_hWnd  = hWnd;
	m_stSub = stSub;
	m_nMarkMode = nMarkMode;
	ResetEvent(m_hEventSelMark);

	if (m_hThread != NULL)
	{
		TerminateThread(m_hThread,0);
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}

	m_pDoc = NULL;
	m_pDoc = GetDocument();
	if (NULL == m_pDoc) 
	{
		return 0;
	}
	
	if ((ENU_MARKFASTFPKNORMALL != m_nMarkMode) && (ENU_MARKFASTFPKLINE != m_nMarkMode))
	{
		g_DAT.SetMarkPara(m_pDoc->GetLay());
	}
	
	g_DAT.EnterMarkEnv();
	m_hThread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MarkThread, this, 0, NULL);
	if (m_hThread==NULL)
	{
		SetEvent(m_hEventSelMark);
	}

	return 0;
}

//Í£Ö¹±ß¿òÔ¤ÀÀ //¹Ø±ÕÏß³Ì
UINT CSelMark::StopSelMark()
{
	SetEvent(m_hEventSelMark);
	g_DAT.MarkCmdStop();
	int i=0;
	while (!m_bIsStop) 
	{
		if (i > 1000)
		{
			if (m_hThread != NULL)
			{
				TerminateThread(m_hThread,1);
			}
			break;
		}
		Sleep(10);
		i++;
	}
	if (m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	m_bIsStop = TRUE;
	m_pDoc    = NULL;

	if (m_hWnd != NULL) 
	{
		::SendMessage(m_hWnd, WM_SELMARK_END, 0, 0);
	}

	return 0;
}

UINT CSelMark::MarkThread(LPVOID pParam)
{
	CSelMark* pSelMark =(CSelMark*)pParam;
	if (NULL == pSelMark) 
	{
		return 0;
	}

	g_DAT.MarkStart(0, 0);	
	g_DAT.TaskStart(0);
		
	HWND hWnd = pSelMark->m_hWnd;

	if (ENU_MARKFASTFPKLINE == pSelMark->m_nMarkMode)
	{
		CStroke strk;
		double dbLen = pSelMark->m_dbLineLen / 2;
		CDot sDt(-dbLen, 0);
		CDot eDt(dbLen, 0);
		strk.Add(sDt);
		strk.Add(eDt);
		g_DAT.MarkStroke(strk, 0, FALSE);
	}
	else
	{
		pSelMark->m_pDoc->m_chain.SelMark(hWnd, pSelMark->m_hEventSelMark, pSelMark->m_stSub);
	}
	
	g_DAT.TaskEnd(0);
	g_DAT.MarkEnd();

	SetEvent(pSelMark->m_hEventSelMark);
	pSelMark->m_bIsStop = TRUE;

	::SendMessage(hWnd, WM_SELMARK_END, 0, 0);

	ExitThread(0);
	
	return 0;
}			
