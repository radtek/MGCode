// CmdConvert.h: interface for the CCmdConvert class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDCONVERT_H__D77B07EA_608D_442D_A2FC_3243EB834C9C__INCLUDED_)
#define AFX_CMDCONVERT_H__D77B07EA_608D_442D_A2FC_3243EB834C9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"
class CCmdConvert : public CCommand  
{
public:
	CCmdConvert(CHLDoc *pDoc, BOOL bPopMsg = TRUE);
	virtual ~CCmdConvert();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
	BOOL m_bPopMsg;
};

#endif // !defined(AFX_CMDCONVERT_H__D77B07EA_608D_442D_A2FC_3243EB834C9C__INCLUDED_)
