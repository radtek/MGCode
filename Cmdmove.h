// CmdMove.h: interface for the CCmdMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDMOVE_H__C7A85BA1_4604_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDMOVE_H__C7A85BA1_4604_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdMove : public CCommand  
{
public: 
	CCmdMove(CHLDoc *pDoc,double moveX,double moveY);
	virtual ~CCmdMove();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
	CHLDoc * m_pDoc;
	double m_dbMoveX,m_dbMoveY;
};

#endif // !defined(AFX_CMDMOVE_H__C7A85BA1_4604_11D4_A483_00105A615525__INCLUDED_)
