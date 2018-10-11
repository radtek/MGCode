// ObjMark.h: interface for the CObjMark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJMARK_H__B7403567_379A_4928_95C5_C9582B4EBEF5__INCLUDED_)
#define AFX_OBJMARK_H__B7403567_379A_4928_95C5_C9582B4EBEF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjMark  
{
public:
	CObjMark();
	virtual ~CObjMark();

public:
	CHLDoc *GetDocument();
	static UINT MarkThread(LPVOID pParam);
	
	UINT StartFastMark(HWND hWnd);
	UINT StopFastMark();
	
public:
	HANDLE m_hThread;
	HANDLE m_hEventFastMark;
	
	HWND m_hWnd;
	
private:
	BOOL m_bIsStop;
	CHLDoc *m_pDoc;

};

#endif // !defined(AFX_OBJMARK_H__B7403567_379A_4928_95C5_C9582B4EBEF5__INCLUDED_)
