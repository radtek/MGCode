// CmdMAlign.h: interface for the CCmdMAlign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDMALIGN_H__10519F23_5961_4311_8538_997D0A50E29C__INCLUDED_)
#define AFX_CMDMALIGN_H__10519F23_5961_4311_8538_997D0A50E29C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"

class CCmdMAlign : public CCommand  
{
public:
	CCmdMAlign(CHLDoc *pDoc, int nType);
	virtual ~CCmdMAlign();
	virtual BOOL Do(BOOL bRedo);
	virtual BOOL Undo(BOOL bRedo= FALSE);
protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;

};

#endif // !defined(AFX_CMDMALIGN_H__10519F23_5961_4311_8538_997D0A50E29C__INCLUDED_)
