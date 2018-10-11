// RedMarkThread.cpp: implementation of the RedMarkThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RedMarkThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRITICAL_SECTION g_LockReadMark;


CRedMarkThread::CRedMarkThread()
{
	m_QuadList.RemoveAll();
	//初始化事件
	char pEventName[256];
	sprintf(pEventName,"H%s","F_RedExitMark");
	m_hEventThreadExit=::CreateEvent(NULL,TRUE,TRUE,pEventName);
	sprintf(pEventName,"H%s","F_RedPreview");
	m_hEventRedPreview = ::CreateEvent(NULL,TRUE,FALSE,pEventName);
	sprintf(pEventName,"H%s","F_DataUpdate");
	m_hEventUpdateArray = ::CreateEvent(NULL,TRUE,FALSE,pEventName);

	m_bIsRed   = TRUE;
	m_bIsArray = FALSE;

	InitializeCriticalSection(&g_LockReadMark);
}

CRedMarkThread::~CRedMarkThread()
{
	//关闭事件
//	StopPreview();//确保已经关闭
	CloseHandle(m_hEventRedPreview);
	CloseHandle(m_hEventThreadExit);
	CloseHandle(m_hEventUpdateArray);
	if (m_hRedThread!=NULL)
	{
		CloseHandle(m_hRedThread);
		m_hRedThread=NULL;
	}

	DeleteCriticalSection(&g_LockReadMark);
}

UINT CRedMarkThread::StartPreview(BOOL bIsRed, BOOL bIsArray)	//启动边框预览
{
	m_bIsRed = bIsRed;
	m_bIsArray = bIsArray;

	SetEvent(m_hEventRedPreview);
	ResetEvent(m_hEventThreadExit);
	if (m_hRedThread!=NULL)
	{
		CloseHandle(m_hRedThread);
		m_hRedThread=NULL;
	}
	CHLDoc* pDoc = GetDocument();
	if (NULL == pDoc) 
	{
		return 0;
	}

	g_DAT.EnterMarkEnv();

	g_DAT.SetMarkPara(pDoc->GetLay());

	if (m_bIsArray)
	{
		m_hRedThread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ArrayMarkThread, this, 0, NULL);
	}
	else
	{
		m_hRedThread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PreviewThread, this, 0, NULL);
	}
	
	if (m_hRedThread==NULL)
	{
		ResetEvent(m_hEventRedPreview);
		SetEvent(m_hEventThreadExit);
	}

	return 0;
}

//停止边框预览 //关闭线程
UINT CRedMarkThread::StopPreview()
{
	ResetEvent(m_hEventRedPreview); //通知停止边框预览
	g_DAT.MarkCmdStop();
	g_DAT.LeaveMarkEnv();
	int i=0;
	while (WAIT_OBJECT_0!=WaitForSingleObject(m_hEventThreadExit,0)) 
	{
		//等待预览线程退出
		i++;
		if (i>1000)
		{
			DSF_TRACE("Can't stop Thread\n");
			if (m_hRedThread!=NULL)
			{
				TerminateThread(m_hRedThread,0);
				DSF_TRACE("Force Terminate Red Thread\n");
			}
			break;
		}
		Sleep(10);
	}
	if (m_hRedThread!=NULL)
	{
		CloseHandle(m_hRedThread);
		m_hRedThread=NULL;
	}

	m_PosArray.RemoveAll();
	m_MarkArray.RemoveAll();
	
	m_bIsRed   = TRUE;
	m_bIsArray = FALSE;
	
	return 0;
}
		
void CRedMarkThread::SetQuadArray(CArray<SELQUAD ,SELQUAD> &QuadList) //在此加上同步
{
	MYTRACE("SELQUAD_Call\r\n");
	RmLock lock;
	m_QuadList.RemoveAll();
	m_QuadList.Copy(QuadList);
	if (WAIT_OBJECT_0==WaitForSingleObject(m_hEventRedPreview,0))
 	{
 		g_DAT.MarkCmdStop();	
 	}

	SetEvent(m_hEventUpdateArray);
	return;
}

//预览打标线程
UINT CRedMarkThread::PreviewThread(LPVOID pParam)
{
	CRedMarkThread* pRedMarkThread=(CRedMarkThread*)pParam;

	pRedMarkThread->GetMarkData();
	
	SELQUAD quad;	
	CStroke stroke;
	CDot	dot;
	g_DAT.MarkStart(pRedMarkThread->m_bIsRed, 0);
	g_DAT.TaskStart(0);

//	DWORD dwCount = 0;
//	char szMsg[64];
	
	while (WAIT_OBJECT_0==WaitForSingleObject(pRedMarkThread->m_hEventRedPreview,0))
	{
		pRedMarkThread->GetMarkData();
		int nCount = pRedMarkThread->m_QuadMarkArray.GetSize();

		for (int i = 0; i < nCount; i++)
		{
			quad=pRedMarkThread->m_QuadMarkArray.GetAt(i);
			stroke.RemoveAllPoint();
			dot.x=quad.m_QD.left;dot.y=quad.m_QD.bottom;
			stroke.Add(dot);
			dot.x=quad.m_QD.right;dot.y=quad.m_QD.bottom;
			stroke.Add(dot);
			dot.x=quad.m_QD.right;dot.y=quad.m_QD.top;
			stroke.Add(dot);
			dot.x=quad.m_QD.left;dot.y=quad.m_QD.top;
			stroke.Add(dot);
			dot.x=quad.m_QD.left;dot.y=quad.m_QD.bottom;
			stroke.Add(dot);

			pRedMarkThread->GetMarkData();
			nCount = pRedMarkThread->m_QuadMarkArray.GetSize();

//			g_DAT.MarkStroke(stroke);
			
		}
		
//		sprintf(szMsg, "Mark Count == %d; x=%f, y=%f\r\n", dwCount, dot.x, dot.y);
//		dwCount++;
//		MYTRACE(szMsg);
	}
	g_DAT.TaskEnd(0);
	g_DAT.MarkEnd();
	
	SetEvent(pRedMarkThread->m_hEventThreadExit);//通知线程退出
	return 0;
}

DWORD CRedMarkThread::GetMarkData() //在此加上同步
{
	if (WAIT_OBJECT_0==WaitForSingleObject(m_hEventUpdateArray,0))
	{
		g_DAT.MarkCmdStop();
		RmLock lock;
		m_QuadMarkArray.RemoveAll();
		m_QuadMarkArray.Copy(m_QuadList);
		g_DAT.MarkStart(m_bIsRed, 0);
		g_DAT.TaskStart(0);
		ResetEvent(m_hEventUpdateArray);
	}
	
	return 0;
}

CHLDoc *CRedMarkThread::GetDocument() 
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


void CRedMarkThread::SetMarkStrk(CArray<ARRAYMARK, ARRAYMARK> &ArrayMark) //在此加上同步
{
	RmLock lock;
	m_PosArray.RemoveAll();
	m_PosArray.Append(ArrayMark);

	if (WAIT_OBJECT_0==WaitForSingleObject(m_hEventRedPreview,0))
 	{
 		g_DAT.MarkCmdStop();	
 	}

	SetEvent(m_hEventUpdateArray);
	return;
}

DWORD CRedMarkThread::GetArrayMarkData() //在此加上同步
{
	if (WAIT_OBJECT_0==WaitForSingleObject(m_hEventUpdateArray,0))
	{
		g_DAT.MarkCmdStop();
		RmLock lock;

		m_MarkArray.RemoveAll();
		m_MarkArray.Append(m_PosArray);
	
		g_DAT.MarkStart(m_bIsRed, 0);
		g_DAT.TaskStart(0);
		ResetEvent(m_hEventUpdateArray);
	}
	
	return 0;
}

//预览打标线程
UINT CRedMarkThread::ArrayMarkThread(LPVOID pParam)
{
	CRedMarkThread* pRedMarkThread=(CRedMarkThread*)pParam;

	pRedMarkThread->GetArrayMarkData();
	
	SELQUAD quad;
	CStroke stroke;
	CDot	dot;	
	g_DAT.MarkStart(pRedMarkThread->m_bIsRed, 0);
	g_DAT.TaskStart(0);

//	DWORD dwCount = 0;
//	char szMsg[64];
	
	while (WAIT_OBJECT_0==WaitForSingleObject(pRedMarkThread->m_hEventRedPreview,0))
	{
		pRedMarkThread->GetArrayMarkData();
		int nCount = pRedMarkThread->m_MarkArray.GetSize();

		for (int i = 0; i < nCount; i++)
		{
			ARRAYMARK AryMark =pRedMarkThread->m_MarkArray.GetAt(i);

			stroke.RemoveAllPoint();
			dot.x=AryMark.qd.left;dot.y=AryMark.qd.bottom;
			stroke.Add(dot);
			dot.x=AryMark.qd.right;dot.y=AryMark.qd.bottom;
			stroke.Add(dot);
			dot.x=AryMark.qd.right;dot.y=AryMark.qd.top;
			stroke.Add(dot);
			dot.x=AryMark.qd.left;dot.y=AryMark.qd.top;
			stroke.Add(dot);
			dot.x=AryMark.qd.left;dot.y=AryMark.qd.bottom;
			stroke.Add(dot);

			stroke.Rotate(AryMark.qd.Center(), AryMark.rota);

#ifdef _DOUBLE_HEAD_MARK
			DOT6 dot;
			dot.x = AryMark.Lf.x;
			dot.y = AryMark.Lf.y;
			dot.z = AryMark.Rf.x;
			dot.i = AryMark.Rf.y;
			g_DAT.SetMarkOffset(dot);
#endif
			pRedMarkThread->GetArrayMarkData();
			nCount = pRedMarkThread->m_MarkArray.GetSize();

//			g_DAT.MarkStroke(stroke);
			
		}
		
//		sprintf(szMsg, "Mark Count == %d; x=%f, y=%f\r\n", dwCount, dot.x, dot.y);
//		dwCount++;
//		MYTRACE(szMsg);
	}
	
	g_DAT.TaskEnd(0);
	g_DAT.MarkEnd();
	
	SetEvent(pRedMarkThread->m_hEventThreadExit);//通知线程退出
	return 0;
}