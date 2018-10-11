// MarkMgr.cpp: implementation of the CMarkMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "MarkMgr.h"
#include "Error.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CHLDoc *CMarkMgr::m_pDoc=NULL;
CChain* CMarkMgr::m_pChain = NULL;
DWORD CMarkMgr::m_dwSingle = 0;
DWORD CMarkMgr::m_dwCurPos = 0;
DWORD CMarkMgr::m_dwMarkCount = 0;
DWORD CMarkMgr::m_nMarkType = MARK_LASER;

BOOL  CMarkMgr::m_bLoopMark = FALSE;
int   CMarkMgr::m_nIntervalTime = 0;

CRITICAL_SECTION g_Lock;
HANDLE hMark = ::CreateEvent(NULL,TRUE,FALSE,"Mark"); //开始打标，下发数据
HANDLE hStop = ::CreateEvent(NULL,TRUE,FALSE,"Stop");
HANDLE hMarkSts = ::CreateEvent(NULL,TRUE,FALSE,"MarkSts"); //处于打标过程中Signal，否则No Signal
HANDLE hThread = ::CreateEvent(NULL,TRUE,FALSE,"Thread");
HANDLE hStopAfterTouch = ::CreateEvent(NULL,TRUE,FALSE,"StopAfterTouch");   //脚踏触发后意外中止 ZY

UINT Mark_Process(LPVOID lpPara)
{
	InitHLExceptionManager();

//	g_DAT.EnterMarkEnv();
	SetEvent(hThread);
	
	while(1)
	{
		Sleep(1);

		if(WaitForSingleObject(hThread,0) != WAIT_OBJECT_0){
			//如果线程信号被外部释放则退出
			break;
		}
		if((CMarkMgr::Touch(lpPara) == 0)){
			//脚踏信号触发
			HWND hWnd = (HWND)lpPara;
			ResetEvent(hMark);
			ResetEvent(hStopAfterTouch);
			//给主线程发消息
			PostMessage(hWnd,WM_MARK_START,0,0);
		}
		else if(WaitForSingleObject(hMark,0)==WAIT_OBJECT_0)
		{
			//人机交互的界面打标触发
		}
		else
		{
			//如果脚踏信号末被触发则CONTINUE
			continue;
		}
		
		HANDLE hEvents[2];
		hEvents[0]=hStopAfterTouch;
		hEvents[1]=hMark;
		//实际打标流程
		//	WaitForSingleObject(hMark,INFINITE);
		
		DWORD dwWaitResult = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);  
		if(dwWaitResult==WAIT_OBJECT_0 )   //脚踏触发后意外中止 安全门 文档信号不匹配 等...
		{
			continue; 
		}

		if(WaitForSingleObject(hThread,0) != WAIT_OBJECT_0)
		{
			//如果线程信号被外部释放则退出
			break;
		}

		{
			//进入锁定状态，该变量退出时自动解除锁定
			DSF_TRACE("Lock in Mark ... !!!\n");
			Lock tmp;
			SetEvent(hMarkSts);
			CMarkMgr::Mark(lpPara);
			CMarkMgr::m_dwCurPos = 0;
			DSF_TRACE("Lock out Mark ... !!!\n");
		}
	}
//	g_DAT.LeaveMarkEnv();

	DSF_TRACE("Thread Exit ...\n");
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarkMgr::CMarkMgr()
{
	::InitializeCriticalSection(&g_Lock);
}

CMarkMgr::~CMarkMgr()
{
	::DeleteCriticalSection(&g_Lock);	
}

void CMarkMgr::RunThread(LPVOID lPara)
{
	ResetEvent(hThread);
	ResetEvent(hMark);
	ResetEvent(hMarkSts);
	ResetEvent(hStop);

	m_pChain = NULL;
	m_dwSingle = 0;

	AfxBeginThread(Mark_Process,lPara);
}

void CMarkMgr::StopThread()
{
	for(;;)
	{
		if(WaitForSingleObject(hMarkSts,0) != WAIT_OBJECT_0)
			break;
		Sleep(20);
		DSF_TRACE("Wait Mark Stop ... \n");
	}
	
	ResetEvent(hThread);
	SetEvent(hMark);
	Sleep(100);
	m_pChain = NULL;
	DSF_TRACE("Stop Thread Cmd ...\n");
}

BOOL CMarkMgr::SetChain(CHLDoc* pDoc)
{
	Lock tmp;
	m_pDoc = pDoc;
	if (m_pDoc)
	{
		m_pChain = &m_pDoc->m_chain;
		m_dwMarkCount = 1;
		if(m_pChain)
			m_dwMarkCount = m_pChain->GetCount();
	}
	return TRUE;
}

/*
脚踏函数返回值定义：
	0-正常提示开始打标
	9-常规状态
*/
UINT CMarkMgr::Touch(LPVOID lPara)
{
	UINT nRtn=9;
//	if(g_DAT.Touch())	//FPC不使用这个旧的脚踏流程，通过CDlgMarkNormalPane的定时器查询IO实现。另外，设置校正文件时，这个线程与主线程在RTC卡的使用上会冲突；
//		nRtn = 0;
	return nRtn;
}

UINT CMarkMgr::PreProcMark(LPVOID lPara)
{
	if(m_pChain)
	{
		//只是在做预处理
		m_pChain->PreprocessMark(0x00);
	}

	return 0;
}

/*
打标函数返回值定义：
	0-正常打标完成退出，此后要进行PreprocessMark处理
	1-打标中止退出，此后只检测打标信号是否有处理
*/
UINT CMarkMgr::Mark(LPVOID lPara)
{
	/*
	DSF_TRACE("Mark Init... !!!\n");
	UINT nRtn=0;

	if(m_pChain)
	{
		UINT nMarkType = 1;
		switch(m_nMarkType)
		{
		case MARK_LASER:
			nMarkType = 0;
			break;
		case MARK_RED:
			nMarkType = 1;
			break;
		}
		
		HWND hWnd = (HWND)lPara;

		if(m_nMarkType != MARK_FRAME)
		{
			if (m_bLoopMark) 
			{
				for(;m_bLoopMark;)
				{
					DSF_TRACE("Mark Start...\n");
					g_DAT.MarkStart(nMarkType, 0); 
					g_DAT.TaskStart(0);
					CMarkMgr::m_dwCurPos = 0;
					m_pChain->Mark(hWnd);
					g_DAT.TaskEnd(0);
					g_DAT.MarkEnd();
					DSF_TRACE("Mark End...\n");
					
					//单次打标结束
					m_dwSingle = g_DAT.GetMarkTime();
					if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0){
						break;
					}

					DWORD dwStart = timeGetTime();
					for (int i = 0; i < m_nIntervalTime; i++)
					{
						if (!m_bLoopMark)
						{
							break;
						}

						DWORD dwEnd = timeGetTime();
						if ((int)(fabs((double)(dwEnd - dwStart))) >= m_nIntervalTime)
						{
							break;
						}

						Sleep(1);
					}

					SendMessage(hWnd,WM_MARK_END,MARK_SEND,0);
					
				}// end for(;m_bLoopMark;)
			}
			else
			{
				for(int i=0; i < (int)g_sys.m_nMarkRepeat; i++)
				{
					DSF_TRACE("Mark Start...!!!\n");
					g_DAT.MarkStart(nMarkType, 0); 
					g_DAT.TaskStart(0);
				//	g_DAT.MarkStrokePause();
					CMarkMgr::m_dwCurPos = 0;
					DSF_TRACE("m_pChain->Mark(hWnd); !!!\n");
					m_pChain->Mark(hWnd);
					g_DAT.TaskEnd(0);
					DSF_TRACE("g_DAT.MarkEnd(); !!!\n");
					g_DAT.MarkEnd();
					DSF_TRACE("Mark End...!!!\n");
					
					//单次打标结束
					m_dwSingle = g_DAT.GetMarkTime();
					if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0){
						break;
					}
					if(i< (int)g_sys.m_nMarkRepeat-1){
						SendMessage(hWnd,WM_MARK_END,MARK_SEND,0);
						g_DAT.Between(hStop);
					}
				}
			}
			
			if(MARK_LASER == m_nMarkType){
				if(::WaitForSingleObject(hStop,0)!=WAIT_OBJECT_0){
					//如果没有点停止时，则进行下一次数据的处理
					m_pChain->PreprocessMark(0x01);
				}
			}
		}
		else
		{
			//边框预览
			g_DAT.MarkStart(1, 0);
			g_DAT.TaskStart(0);
			m_pChain->PreTrack();
			g_DAT.TaskEnd(0);
			g_DAT.MarkEnd();
		}
		DWORD dwID = GetCurrentThreadId();
		//总体打标结束
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0){
			nRtn = 1;
		}
		ResetEvent(hMark);
		ResetEvent(hStop);
		ResetEvent(hMarkSts);
		DSF_TRACE("Mark Exit... hMarkSts !!!\n");
		SendMessage(hWnd,WM_MARK_END,MARK_TEND,0);
		DSF_TRACE("Mark Exit... Send_Mark_End !!!\n");
	}
	DSF_TRACE("Mark Exit...\n");
	return nRtn;
	*/

	HWND hWnd = (HWND)lPara;
	m_pDoc->GuardCustomMark();
	ResetEvent(hMark);
	ResetEvent(hStop);
	ResetEvent(hMarkSts);
	PostMessage(hWnd,WM_MARK_END,MARK_TEND,0);

	return 0;

}

DWORD CMarkMgr::GetSingleMarkTime()
{
	return m_dwSingle;
}

void CMarkMgr::StartMark()
{
	ResetEvent(hStop);
	SetEvent(hMark);
	//等待处于打标状态时退出
//	DSF_TRACE("CMD START MARK !!!");
//	WaitForSingleObject(hMarkSts,INFINITE);
}

void CMarkMgr::PauseMark()
{
	ResetEvent(hMark);
	g_DAT.MarkCmdPause();
}

void CMarkMgr::ReMark()
{
	SetEvent(hMark);
	g_DAT.MarkCmdRemark();
}

void CMarkMgr::StopMark()
{
	SetEvent(hStop);
	g_DAT.MarkCmdStop();
	DSF_TRACE("CMD STOP MARK !!!");
}

BOOL CMarkMgr::GetRedPreview()
{
	return m_nMarkType!=MARK_LASER;
}

void CMarkMgr::SetLay(const CLay &lay)
{
	CLay lay1 = lay;
	g_DAT.SetMarkPara(lay1);
}

void CMarkMgr::SetMarkType(DWORD nType)
{
	m_nMarkType = nType;
}

BOOL CMarkMgr::MarkIsOver()
{
	//处于打标过程中返回TRUE，否则返回FALSE
	return (WaitForSingleObject(hMarkSts,0)==WAIT_OBJECT_0);
}

void CMarkMgr::SetLoopMarkInfo(BOOL bLoopMark, int nIntervalTime)
{
	m_bLoopMark     = bLoopMark;
    m_nIntervalTime = nIntervalTime;
}
