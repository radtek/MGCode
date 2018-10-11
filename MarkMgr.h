// MarkMgr.h: interface for the CMarkMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MARKMGR_H__785718E1_79B0_44C5_9918_F78FB6E8B536__INCLUDED_)
#define AFX_MARKMGR_H__785718E1_79B0_44C5_9918_F78FB6E8B536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HLDoc.h"

#define MARK_TEND	100
#define MARK_SEND	101
#define MARK_PREE	102
#define MARK_INFO	103

#define MARK_LASER	0
#define MARK_RED	1
#define MARK_FRAME	2

extern CRITICAL_SECTION g_Lock;

class Lock
{
public:
	Lock()
	{
		::EnterCriticalSection(&g_Lock);
	};
	~Lock()
	{
		::LeaveCriticalSection(&g_Lock);
	};
};

extern HANDLE hMark;
extern HANDLE hStop;
extern HANDLE hMarkSts;
extern HANDLE hThread;
extern HANDLE hStopAfterTouch;

class CMarkMgr
{
public:
	CMarkMgr();
	virtual ~CMarkMgr();

public:
	BOOL MarkIsOver();
	void SetMarkType(DWORD nType);
	void SetLoopMarkInfo(BOOL bLoopMark, int nIntervalTime);
	void SetLay(const CLay &lay);
	BOOL GetRedPreview();
	void StopMark();
	void ReMark();
	void PauseMark();
	void StartMark();
	DWORD GetSingleMarkTime();
	BOOL SetChain(CHLDoc* pDoc);
	void StopThread();
	void RunThread(LPVOID lPara);
	static UINT Touch(LPVOID lPara);			//脚踏函数
	static UINT PreProcMark(LPVOID lPara);		//数据预处理函数
	static UINT Mark(LPVOID lPara);				//打标处理

	static CChain* m_pChain;
	static DWORD m_dwSingle;		//单个打标时间
	static DWORD m_dwMarkCount;		//标识总单元数
	static DWORD m_dwCurPos;		//标识当前打标位置，与上一个参数形成百分比
	static DWORD m_nMarkType;		//打标方式

	static BOOL  m_bLoopMark;
	static int   m_nIntervalTime;

	static CHLDoc *m_pDoc;
};

#endif // !defined(AFX_MARKMGR_H__785718E1_79B0_44C5_9918_F78FB6E8B536__INCLUDED_)
