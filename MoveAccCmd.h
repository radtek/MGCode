// MoveAccCmd.h: interface for the MoveAccCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEACCCMD_H__C6164DCC_DF34_4AA9_810D_D37A0EE27EEA__INCLUDED_)
#define AFX_MOVEACCCMD_H__C6164DCC_DF34_4AA9_810D_D37A0EE27EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"
class MoveAccCmd : public CCommand  
{
public:
	MoveAccCmd(CHLDoc *pDoc,XTracker *pTrack,double MoveX,double MoveY);
	virtual ~MoveAccCmd();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_MOVEACCCMD_H__C6164DCC_DF34_4AA9_810D_D37A0EE27EEA__INCLUDED_)
