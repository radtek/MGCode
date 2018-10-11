// CmdHitOtherObj.h: interface for the CCmdHitOtherObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDHITOTHEROBJ_H__97EC4526_DE3C_4287_B8C4_46D964B7A6A8__INCLUDED_)
#define AFX_CMDHITOTHEROBJ_H__97EC4526_DE3C_4287_B8C4_46D964B7A6A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdHitOtherObj :public CCommand
{
public:
	CCmdHitOtherObj(CHLDoc *pDoc,CPoint &point,CWnd *pWnd);
	virtual ~CCmdHitOtherObj();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
	
protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDHITOTHEROBJ_H__97EC4526_DE3C_4287_B8C4_46D964B7A6A8__INCLUDED_)
