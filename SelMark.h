// SelMark.h: interface for the CSelMark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELMARK_H__284F5FCE_CCBA_4D8E_BDA7_1011B60B6731__INCLUDED_)
#define AFX_SELMARK_H__284F5FCE_CCBA_4D8E_BDA7_1011B60B6731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HLView.h"

class CSelMark  
{
public:
	CSelMark();
	virtual ~CSelMark();

public:
	CHLDoc *GetDocument();
	static UINT MarkThread(LPVOID pParam);

	UINT StartSelMark(HWND hWnd, SUBOBJMARKSET &stSub, int nMarkMode = ENU_MARKSEL);
	UINT StopSelMark();

public:
	HANDLE m_hThread;
	HANDLE m_hEventSelMark;
//	HANDLE m_hEventSelMarkExit;

	HWND m_hWnd;

	SUBOBJMARKSET m_stSub;

	int    m_nMarkMode;
	double m_dbLineLen;
	
private:
	BOOL m_bIsStop;
	CHLDoc *m_pDoc;

};

#endif // !defined(AFX_SELMARK_H__284F5FCE_CCBA_4D8E_BDA7_1011B60B6731__INCLUDED_)
