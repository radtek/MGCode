// ObjMark.cpp: implementation of the CObjMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "ObjMark.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CObjMark::CObjMark()
{
	m_hThread = NULL;
	m_pDoc = NULL;
	m_bIsStop = TRUE;
	
	char szEvent[256];
	sprintf(szEvent,"%s","FastMark");
    m_hEventFastMark     = ::CreateEvent(NULL,TRUE,FALSE,szEvent);
}

CObjMark::~CObjMark()
{
	CloseHandle(m_hEventFastMark);
	if (m_hThread!=NULL)
	{
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}
}

CHLDoc *CObjMark::GetDocument() 
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

UINT CObjMark::StartFastMark(HWND hWnd)
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

	g_DAT.SetLay(m_pDoc->GetLay());
	g_DAT.SetRedPreview(FALSE);
	g_DAT.EnterMarkEnv();
	m_hThread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MarkThread, this, 0, NULL);
	if (m_hThread==NULL)
	{
		SetEvent(hStop);
	}

	return 0;
}

//Í£Ö¹±ß¿òÔ¤ÀÀ //¹Ø±ÕÏß³Ì
UINT CObjMark::StopFastMark()
{
	SetEvent(hStop);
	g_DAT.StopMark();
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

UINT CObjMark::MarkThread(LPVOID pParam)
{
	CObjMark* pFastMark =(CObjMark*)pParam;
	if (NULL == pFastMark) 
	{
		return 0;
	}

	g_DAT.MarkStart(FALSE);	
		
	HWND hWnd = pFastMark->m_hWnd;
	pFastMark->m_pDoc->m_chain.FastMark(hWnd);
	
	g_DAT.MarkEnd();

	SetEvent(hStop);
	pFastMark->m_bIsStop = TRUE;

	::PostMessage(hWnd, WM_FASTMARK_END, 0, 0);

	ExitThread(0);
	
	return 0;
}