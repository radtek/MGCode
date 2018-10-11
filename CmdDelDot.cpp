// CmdDelDot.cpp: implementation of the CCmdDelDot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdDelDot.h"
#include "HLView.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDelDot::CCmdDelDot(CHLDoc *pDoc,CWnd *pWnd)
{
	commandName.LoadString(IDS_DELDOT_CMD);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	CHLView *pView=(CHLView*)pWnd;
	pView->DelDot();
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdDelDot::~CCmdDelDot()
{
}

BOOL CCmdDelDot::Do(BOOL bRedo)
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdDelDot::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}