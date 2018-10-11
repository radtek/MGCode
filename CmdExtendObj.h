// CmdExtendObj.h: interface for the CCmdExtendObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDEXTENDOBJ_H__4417E5B3_7C8F_474A_8C53_6BC4EDAB789E__INCLUDED_)
#define AFX_CMDEXTENDOBJ_H__4417E5B3_7C8F_474A_8C53_6BC4EDAB789E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdExtendObj  : public CCommand
{
public:
	CCmdExtendObj(CHLDoc *pDoc, double dbSpace,BOOL bIsKillOldObj);
	virtual ~CCmdExtendObj();

public:
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
protected:
	CChain   m_chainOld;
	CChain   m_chainNew;
	double   m_dbSpace;
	BOOL     m_bFirst;
    BOOL     m_bIsKillOldObj;
	CHLDoc * m_pDoc;

};

#endif // !defined(AFX_CMDEXTENDOBJ_H__4417E5B3_7C8F_474A_8C53_6BC4EDAB789E__INCLUDED_)
