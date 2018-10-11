// CmdStoreDoc.h: interface for the CCmdStoreDoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSTOREDOC_H__0A84213F_45B9_4058_8FD0_3B373761A5FE__INCLUDED_)
#define AFX_CMDSTOREDOC_H__0A84213F_45B9_4058_8FD0_3B373761A5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"
class CCmdStoreDoc : public CCommand  
{
public:
	CCmdStoreDoc(CHLDoc *pDoc);
	virtual ~CCmdStoreDoc();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
	
protected:
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
	CHLDoc * m_pDoc;
};

#endif // !defined(AFX_CMDSTOREDOC_H__0A84213F_45B9_4058_8FD0_3B373761A5FE__INCLUDED_)
