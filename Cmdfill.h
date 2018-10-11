// CmdFill.h: interface for the CCmdFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdFill : public CCommand  
{
public:
	CCmdFill(CHLDoc *pDoc,FILLDATA fillnew,FILLDATA fillold);
	virtual ~CCmdFill();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
protected:
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
	CHLDoc * m_pDoc;

	FILLDATA m_fillnew;
	FILLDATA m_fillold;
};

#endif // !defined(AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_)
