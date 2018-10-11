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
HANDLE hMark = ::CreateEvent(NULL,TRUE,FALSE,"Mark"); //��ʼ��꣬�·�����
HANDLE hStop = ::CreateEvent(NULL,TRUE,FALSE,"Stop");
HANDLE hMarkSts = ::CreateEvent(NULL,TRUE,FALSE,"MarkSts"); //���ڴ�������Signal������No Signal
HANDLE hThread = ::CreateEvent(NULL,TRUE,FALSE,"Thread");
HANDLE hStopAfterTouch = ::CreateEvent(NULL,TRUE,FALSE,"StopAfterTouch");   //��̤������������ֹ ZY

UINT Mark_Process(LPVOID lpPara)
{
	InitHLExceptionManager();

//	g_DAT.EnterMarkEnv();
	SetEvent(hThread);
	
	while(1)
	{
		Sleep(1);

		if(WaitForSingleObject(hThread,0) != WAIT_OBJECT_0){
			//����߳��źű��ⲿ�ͷ����˳�
			break;
		}
		if((CMarkMgr::Touch(lpPara) == 0)){
			//��̤�źŴ���
			HWND hWnd = (HWND)lpPara;
			ResetEvent(hMark);
			ResetEvent(hStopAfterTouch);
			//�����̷߳���Ϣ
			PostMessage(hWnd,WM_MARK_START,0,0);
		}
		else if(WaitForSingleObject(hMark,0)==WAIT_OBJECT_0)
		{
			//�˻������Ľ����괥��
		}
		else
		{
			//�����̤�ź�ĩ��������CONTINUE
			continue;
		}
		
		HANDLE hEvents[2];
		hEvents[0]=hStopAfterTouch;
		hEvents[1]=hMark;
		//ʵ�ʴ������
		//	WaitForSingleObject(hMark,INFINITE);
		
		DWORD dwWaitResult = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);  
		if(dwWaitResult==WAIT_OBJECT_0 )   //��̤������������ֹ ��ȫ�� �ĵ��źŲ�ƥ�� ��...
		{
			continue; 
		}

		if(WaitForSingleObject(hThread,0) != WAIT_OBJECT_0)
		{
			//����߳��źű��ⲿ�ͷ����˳�
			break;
		}

		{
			//��������״̬���ñ����˳�ʱ�Զ��������
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
��̤��������ֵ���壺
	0-������ʾ��ʼ���
	9-����״̬
*/
UINT CMarkMgr::Touch(LPVOID lPara)
{
	UINT nRtn=9;
//	if(g_DAT.Touch())	//FPC��ʹ������ɵĽ�̤���̣�ͨ��CDlgMarkNormalPane�Ķ�ʱ����ѯIOʵ�֡����⣬����У���ļ�ʱ������߳������߳���RTC����ʹ���ϻ��ͻ��
//		nRtn = 0;
	return nRtn;
}

UINT CMarkMgr::PreProcMark(LPVOID lPara)
{
	if(m_pChain)
	{
		//ֻ������Ԥ����
		m_pChain->PreprocessMark(0x00);
	}

	return 0;
}

/*
��꺯������ֵ���壺
	0-�����������˳����˺�Ҫ����PreprocessMark����
	1-�����ֹ�˳����˺�ֻ������ź��Ƿ��д���
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
					
					//���δ�����
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
					
					//���δ�����
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
					//���û�е�ֹͣʱ���������һ�����ݵĴ���
					m_pChain->PreprocessMark(0x01);
				}
			}
		}
		else
		{
			//�߿�Ԥ��
			g_DAT.MarkStart(1, 0);
			g_DAT.TaskStart(0);
			m_pChain->PreTrack();
			g_DAT.TaskEnd(0);
			g_DAT.MarkEnd();
		}
		DWORD dwID = GetCurrentThreadId();
		//���������
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
	//�ȴ����ڴ��״̬ʱ�˳�
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
	//���ڴ������з���TRUE�����򷵻�FALSE
	return (WaitForSingleObject(hMarkSts,0)==WAIT_OBJECT_0);
}

void CMarkMgr::SetLoopMarkInfo(BOOL bLoopMark, int nIntervalTime)
{
	m_bLoopMark     = bLoopMark;
    m_nIntervalTime = nIntervalTime;
}
