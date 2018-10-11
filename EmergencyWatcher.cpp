#include "stdafx.h"
#include "EmergencyWatcher.h"
#include "HL.H"
#include "DlgMarkNormalPane.h"
#include "MarkMgr.h"

CEmergencyWatcher g_EmergencyWatcher;

CEmergencyWatcher::CEmergencyWatcher(void)
{
	m_hWndNotify = NULL;
	m_pThread = NULL;
	m_bThreadContinue = TRUE;
}


CEmergencyWatcher::~CEmergencyWatcher(void)
{
	if (m_pThread)
		delete m_pThread;
}

void CEmergencyWatcher::RunWatcher(HWND hWnd)
{
	m_hWndNotify = hWnd;
	m_bThreadContinue = TRUE;

	if (m_pThread==NULL)
	{
		m_pThread = AfxBeginThread(CEmergencyWatcher::WatchThreadFun, (LPVOID)this);

		SetThreadPriority(m_pThread,THREAD_PRIORITY_ABOVE_NORMAL);

		m_pThread->m_bAutoDelete = FALSE;
	}
}

void CEmergencyWatcher::StopWatcher()
{
	m_bThreadContinue = FALSE;
	WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	delete m_pThread;
	m_pThread = NULL;
}

UINT CEmergencyWatcher::WatchThreadFun(LPVOID pParam)
{
	CEmergencyWatcher *pWatcher = (CEmergencyWatcher*)pParam;

	while(pWatcher->m_bThreadContinue)
	{
		Sleep(10);
				
		//安全门检查:在线时通过流水线自动上下料，安全门要一直有效；离线时必须要开门上下料，安全门只能在打标时有效；
		if (g_sys.m_bUseSafeDoor)
		{
			if (g_sys.m_bPLCOnline || g_pDlgMarkNormalPane->m_bMarking)
			{			
				BOOL bFirstDoorOpen=FALSE;
				if (pWatcher->SafeDoorCheck(bFirstDoorOpen))
					pWatcher->SafeDoorProc(bFirstDoorOpen);
			}
		}

		//其它检查

	}
	return 0;
}

BOOL CEmergencyWatcher::SafeDoorCheck(BOOL &bFirst)
{
	bFirst = FALSE;

	static BOOL bLastDoorOpen = FALSE;	//上一次检查时安全门的状态
	BOOL bDoorOpen = g_IO.INPUT_SAFEDOOR();
	if (bDoorOpen)
	{
		if (!bLastDoorOpen)
			bFirst = TRUE;	
	}
	bLastDoorOpen = bDoorOpen;
	return bDoorOpen;
}

void CEmergencyWatcher::SafeDoorProc(BOOL bFirst)
{
	//立即执行
	SetEvent(g_hEventMainThreadEmergency);
	SetEvent(hStop);

	g_DAT.MarkCmdStop();
	g_pMotion->Stop();

	if (g_IO.INPUT_WORKBENCH_DISABLE())
		g_pMotion->Disable();

	//窗口提示
	//if (bFirst)
		PostMessage(m_hWndNotify, WM_EMERGENCY_EVENT, 1, NULL);

}
