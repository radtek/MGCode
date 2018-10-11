// AutoSortListObj.h: interface for the CAutoSortListObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOSORTLISTOBJ_H__70B0002B_6862_4E87_AAED_756A271F70FB__INCLUDED_)
#define AFX_AUTOSORTLISTOBJ_H__70B0002B_6862_4E87_AAED_756A271F70FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MsgWnd.h"

struct POS
{
	POS()
	{
		Clear();
	}

	void Clear()
	{
		memset(this,0,sizeof(this));
	}
	double sx;
	double sy;
	double ex;
	double ey;
};

typedef struct INIGA{
	INIGA()
	{
		dbpc = 0.3;
		dbpm = 0.1;
		dbeval = 0.5;
		nNodeRate =5;
		nPCMode = 2;
		nPMMode = 1;
	}
	void operator=(const INIGA& src)
	{
		dbpc = src.dbpc;
		dbpm = src.dbpm;
		dbeval = src.dbeval;
		nPCMode = src.nPCMode;
		nPMMode = src.nPMMode;
		nNodeRate = src.nNodeRate;
	}
	double dbpc;
	double dbpm;
	double dbeval;
	UINT nPCMode;
	UINT nPMMode;
	int nNodeRate;
}INIGA;

typedef BOOL (WINAPI *INITGA)(int,POS*,int);
typedef void (WINAPI *RUNGA)(int,int*,double*,int);

extern CRITICAL_SECTION g_scri;
class SLock
{
public:
	SLock(){EnterCriticalSection(&g_scri);};
	~SLock(){LeaveCriticalSection(&g_scri);};
};

class CAutoSortListObj  
{
public:
	CAutoSortListObj();
	virtual ~CAutoSortListObj();

public:
	BOOL Track(CWnd* pView);
	BOOL TrackSelPt(CWnd* pView, CArray<ASNODE,ASNODE> &Sort);
	BOOL InitSort();
	BOOL DestroySort();

	BOOL SetSortResult(int *pRetList, int nSize);
	double Distance(CDot &sDot, CDot &eDot);

	void GetResultMsg();

	BOOL RunSort(CWnd *pWnd);
	BOOL ExitSort();

public:
	CWnd   *m_pView;
	HMODULE m_hModule;
	INITGA  InitGA;
	RUNGA   RunGA;

	double m_dbMinDistance;
	double m_dbOldDistance;
	
	int    m_nSize;
	BOOL   m_bRun;
	int    m_nCount;
	CString m_strDispMsg;

	int*   m_pRetSortList;

	DWORD  m_dwSelObj;

	//消息处理
	CMsgWnd m_Msg;
};

#endif // !defined(AFX_AUTOSORTLISTOBJ_H__70B0002B_6862_4E87_AAED_756A271F70FB__INCLUDED_)
