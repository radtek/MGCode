// CmdCopy.h: interface for the CCmdCopy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDCOPY_H__583122A6_46BF_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDCOPY_H__583122A6_46BF_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdCopy : public CCommand  
{
public:
	CCmdCopy(CHLDoc *pDoc);
	virtual ~CCmdCopy();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDCOPY_H__583122A6_46BF_11D4_A483_00105A615525__INCLUDED_)
