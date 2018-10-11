// CmdDelDot.h: interface for the CCmdDelDot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDELDOT_H__23CA8E55_8604_4D0B_984D_7214878144BF__INCLUDED_)
#define AFX_CMDDELDOT_H__23CA8E55_8604_4D0B_984D_7214878144BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdDelDot : public CCommand  
{
public:
	CCmdDelDot(CHLDoc *pDoc,CWnd *pWnd);
	virtual ~CCmdDelDot();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
	
protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDDELDOT_H__23CA8E55_8604_4D0B_984D_7214878144BF__INCLUDED_)
