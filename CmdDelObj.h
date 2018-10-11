// CmdDelObj.h: interface for the CCmdDelObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDELOBJ_H__AD1E1253_4EE5_47CC_A05C_761E7B848466__INCLUDED_)
#define AFX_CMDDELOBJ_H__AD1E1253_4EE5_47CC_A05C_761E7B848466__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"

class CCmdDelObj : public CCommand  
{
public:
	CCmdDelObj(CHLDoc *pDoc, DWORD dwTree);
	virtual ~CCmdDelObj();
	virtual BOOL Do(BOOL bRedo);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;

};

class CCmdDelDsfObj : public CCommand  
{
public:
	CCmdDelDsfObj(CHLDoc *pDoc, DWORD dwTree);
	virtual ~CCmdDelDsfObj();
	virtual BOOL Do(BOOL bRedo);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;

};

#endif // !defined(AFX_CMDDELOBJ_H__AD1E1253_4EE5_47CC_A05C_761E7B848466__INCLUDED_)
