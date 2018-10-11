// RedMarkThread.h: interface for the RedMarkThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REDMARKTHREAD_H__7934462C_1C1B_4D0D_BA49_5437A14DCCD1__INCLUDED_)
#define AFX_REDMARKTHREAD_H__7934462C_1C1B_4D0D_BA49_5437A14DCCD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HLView.h"

extern CRITICAL_SECTION g_LockReadMark;
class RmLock
{
public:
	RmLock(){EnterCriticalSection(&g_LockReadMark);};
	~RmLock(){LeaveCriticalSection(&g_LockReadMark);};
};


class CRedMarkThread  
{
public:
	CRedMarkThread(); 
	virtual ~CRedMarkThread(); 

	UINT StartPreview(BOOL bIsRed = TRUE, BOOL bIsArray = FALSE);	//启动边框预览
	UINT StopPreview();		//停止边框预览 //关闭线程
	void SetQuadArray(CArray<SELQUAD ,SELQUAD> &QuadList); //在此加上同步
	void SetMarkStrk(CArray<ARRAYMARK, ARRAYMARK> &ArrayMark); //在此加上同步

	DWORD GetMarkData();
	DWORD GetArrayMarkData();

	CHLDoc *GetDocument();

	static UINT PreviewThread(LPVOID pParam);				// 预览打标线程
    static UINT ArrayMarkThread(LPVOID pParam);				// 阵列预览打标线程
private:
	CArray<SELQUAD ,SELQUAD> m_QuadList; //要边框预览的数组	
	CArray<SELQUAD ,SELQUAD> m_QuadMarkArray; //打标流程中的边框预览数据
	HANDLE m_hEventRedPreview; //边框预览通知
	HANDLE m_hEventThreadExit; //边框预览线程退出
	HANDLE m_hEventUpdateArray; //打标中要求更新数据
	HANDLE m_hRedThread;	//红光预览线程句柄

	BOOL   m_bIsRed;
	BOOL   m_bIsArray;

	CArray<ARRAYMARK, ARRAYMARK> m_PosArray;
	CArray<ARRAYMARK, ARRAYMARK> m_MarkArray;
	
};

#endif // !defined(AFX_REDMARKTHREAD_H__7934462C_1C1B_4D0D_BA49_5437A14DCCD1__INCLUDED_)
