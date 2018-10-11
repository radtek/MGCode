// JoinPointCmd.h: interface for the JoinPointCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOINPOINTCMD_H__926BEDA5_9CF1_40A5_9FF8_E760815BFD0D__INCLUDED_)
#define AFX_JOINPOINTCMD_H__926BEDA5_9CF1_40A5_9FF8_E760815BFD0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"
class JoinPointCmd : public CCommand  
{
public:
	JoinPointCmd(CHLDoc *pDoc,CWnd *pWnd);
	virtual ~JoinPointCmd();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
};

#endif // !defined(AFX_JOINPOINTCMD_H__926BEDA5_9CF1_40A5_9FF8_E760815BFD0D__INCLUDED_)
