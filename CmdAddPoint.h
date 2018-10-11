// CmdAddPoint.h: interface for the CCmdAddPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDADDPOINT_H__08A1672D_818E_4474_BC23_447E40A7C08C__INCLUDED_)
#define AFX_CMDADDPOINT_H__08A1672D_818E_4474_BC23_447E40A7C08C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdAddPoint : public CCommand  
{
public:
	CCmdAddPoint(CHLDoc *pDoc,CPoint &point,CWnd *pWnd);
	virtual ~CCmdAddPoint();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDADDPOINT_H__08A1672D_818E_4474_BC23_447E40A7C08C__INCLUDED_)
