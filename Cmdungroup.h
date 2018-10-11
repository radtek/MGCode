// CmdUngroup.h: interface for the CCmdUngroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDUNGROUP_H__2C141602_46F6_11D4_B789_E25FABB88F0F__INCLUDED_)
#define AFX_CMDUNGROUP_H__2C141602_46F6_11D4_B789_E25FABB88F0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdUngroup : public CCommand  
{
public:
	CCmdUngroup(CHLDoc *pDoc);
	virtual ~CCmdUngroup();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDUNGROUP_H__2C141602_46F6_11D4_B789_E25FABB88F0F__INCLUDED_)
