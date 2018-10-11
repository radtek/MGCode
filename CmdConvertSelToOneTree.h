// CmdConvertSelToOneTree.h: interface for the CCmdConvertSelToOneTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDCONVERTSELTOONETREE_H__3B2418BD_7C4E_4BF6_9C55_B8091D1C8BF3__INCLUDED_)
#define AFX_CMDCONVERTSELTOONETREE_H__3B2418BD_7C4E_4BF6_9C55_B8091D1C8BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdConvertSelToOneTree : public CCommand  
{
public:
	CCmdConvertSelToOneTree(CHLDoc *pDoc);
	virtual ~CCmdConvertSelToOneTree();
	
public:
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;

};

#endif // !defined(AFX_CMDCONVERTSELTOONETREE_H__3B2418BD_7C4E_4BF6_9C55_B8091D1C8BF3__INCLUDED_)
