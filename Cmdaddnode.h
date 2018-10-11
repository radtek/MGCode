// CmdAddNode.h: interface for the CCmdAddNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDADDNODE_H__580BA901_46AE_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDADDNODE_H__580BA901_46AE_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdAddNode : public CCommand  
{
public:
	CCmdAddNode(CHLDoc *pDoc,CTree *pTree);
	CCmdAddNode(CHLDoc *pDoc,CNode *pNode);
	virtual ~CCmdAddNode();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDADDNODE_H__580BA901_46AE_11D4_A483_00105A615525__INCLUDED_)
