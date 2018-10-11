// CmdDelete.h: interface for the CCmdDelete class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDELETE_H__583122A3_46BF_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDDELETE_H__583122A3_46BF_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdDelete: public CCommand  
{
public:
	CCmdDelete(CHLDoc *pDoc);
	virtual ~CCmdDelete();
	virtual BOOL Do(BOOL bRedo);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDDELETE_H__583122A3_46BF_11D4_A483_00105A615525__INCLUDED_)
