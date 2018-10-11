// SplitPointCmd.h: interface for the SplitPointCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLITPOINTCMD_H__886B98EB_6242_46DA_AC69_B33FFC1206F3__INCLUDED_)
#define AFX_SPLITPOINTCMD_H__886B98EB_6242_46DA_AC69_B33FFC1206F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"
class SplitPointCmd : public CCommand  
{
public:
	SplitPointCmd(CHLDoc *pDoc,CWnd *pWnd,double dbPoint);
	virtual ~SplitPointCmd();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_SPLITPOINTCMD_H__886B98EB_6242_46DA_AC69_B33FFC1206F3__INCLUDED_)
