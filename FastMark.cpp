// FastMark.cpp: implementation of the CFastMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FastMark.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFastMark::CFastMark()
{
	m_hThread = NULL;
	m_pDoc = NULL;
	m_bIsStop = TRUE;
	
	char szEvent[256];
	sprintf_s(szEvent, 256, "%s", "FastMark");
    m_hEventFastMark     = ::CreateEvent(NULL,TRUE,FALSE,szEvent);
}

CFastMark::~CFastMark()
{
	CloseHandle(m_hEventFastMark);
	if (m_hThread!=NULL)
	{
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}
}

CHLDoc *CFastMark::GetDocument() 
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

UINT CFastMark::StartFastMark(HWND hWnd)
{
	m_bIsStop = FALSE;

	m_hWnd  = hWnd;
	ResetEvent(hStop);

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

	g_DAT.EnterMarkEnv();
	g_DAT.SetMarkPara(m_pDoc->GetLay());
	m_hThread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MarkThread, this, 0, NULL);
	if (m_hThread==NULL)
	{
		SetEvent(hStop);
	}

	return 0;
}

//Í£Ö¹±ß¿òÔ¤ÀÀ //¹Ø±ÕÏß³Ì
UINT CFastMark::StopFastMark()
{
	SetEvent(hStop);
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
		::PostMessage(m_hWnd, WM_FASTMARK_END, 0, 0);
	}

	return 0;
}

UINT CFastMark::MarkThread(LPVOID pParam)
{
	CFastMark* pFastMark =(CFastMark*)pParam;
	if (NULL == pFastMark) 
	{
		return 0;
	}

	g_DAT.MarkStart(0, 0);	
	g_DAT.TaskStart(0);
	HWND hWnd = pFastMark->m_hWnd;
	pFastMark->m_pDoc->m_chain.FastMark(hWnd);
	g_DAT.TaskEnd(0);
	g_DAT.MarkEnd();

	SetEvent(hStop);
	pFastMark->m_bIsStop = TRUE;

	::PostMessage(hWnd, WM_FASTMARK_END, 0, 0);

	ExitThread(0);
	
	return 0;
}