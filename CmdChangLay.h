// CmdChangLay.h: interface for the CCmdChangLay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDCHANGLAY_H__D6326A24_FA9C_11D8_ABC5_000AE64C79F3__INCLUDED_)
#define AFX_CMDCHANGLAY_H__D6326A24_FA9C_11D8_ABC5_000AE64C79F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdChangLay : public CCommand  
{
public:
	CCmdChangLay(CHLDoc *pDoc);
	virtual ~CCmdChangLay();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
protected:
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
	CHLDoc * m_pDoc;

};

#endif // !defined(AFX_CMDCHANGLAY_H__D6326A24_FA9C_11D8_ABC5_000AE64C79F3__INCLUDED_)
