// CmdSetLay.h: interface for the CCmdSetLay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSETLAY_H__F9C2F981_87FC_11D4_A484_00105A615525__INCLUDED_)
#define AFX_CMDSETLAY_H__F9C2F981_87FC_11D4_A484_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdSetLay : public CCommand  
{
public:
	CCmdSetLay(CHLDoc *pDoc,int mode,CLayer ly);
	virtual ~CCmdSetLay();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
//	CLay m_layOld;
//	CLay m_layNew;
	CLayer m_layerOld;
	CLayer m_layerNew;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
	BOOL m_bFlag;
};

#endif // !defined(AFX_CMDSETLAY_H__F9C2F981_87FC_11D4_A484_00105A615525__INCLUDED_)
