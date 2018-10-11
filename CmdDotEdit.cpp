// CmdDotEdit.cpp: implementation of the CCmdDotEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdDotEdit.h"
#include "resource.h"
#include "HLView.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDotEdit::CCmdDotEdit(CHLDoc *pDoc,CPoint &point,CWnd *pWnd)
{
	commandName.LoadString(IDS_DOTEDIT_CMD);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	CHLView *pView=(CHLView*)pWnd;
	pView->m_track.SingleModify(point,pWnd);
	pView->ReplaceDot();
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdDotEdit::~CCmdDotEdit()
{
}

BOOL CCmdDotEdit::Do(BOOL bRedo)
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdDotEdit::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}
