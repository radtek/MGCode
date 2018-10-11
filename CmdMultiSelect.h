// CmdMultiSelect.h: interface for the CCmdMultiSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDMULTISELECT_H__BCBC6CD5_8DF2_4CBA_B37F_40C8FB26222D__INCLUDED_)
#define AFX_CMDMULTISELECT_H__BCBC6CD5_8DF2_4CBA_B37F_40C8FB26222D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdMultiSelect : public CCommand  
{
public:
	CCmdMultiSelect(CHLDoc *pDoc,CQuad &quad);
	virtual ~CCmdMultiSelect();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
	
protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDMULTISELECT_H__BCBC6CD5_8DF2_4CBA_B37F_40C8FB26222D__INCLUDED_)
