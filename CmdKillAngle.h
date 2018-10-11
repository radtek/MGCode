// CmdKillAngle.h: interface for the CCmdKillAngle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDKILLANGLE_H__7FB52FB4_5B8D_48A9_BA77_3F8756B49933__INCLUDED_)
#define AFX_CMDKILLANGLE_H__7FB52FB4_5B8D_48A9_BA77_3F8756B49933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdKillAngle  : public CCommand
{
public:
	CCmdKillAngle(CHLDoc *pDoc);
	virtual ~CCmdKillAngle();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;

};

#endif // !defined(AFX_CMDKILLANGLE_H__7FB52FB4_5B8D_48A9_BA77_3F8756B49933__INCLUDED_)
