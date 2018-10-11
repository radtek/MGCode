// CmdMultiSelect.cpp: implementation of the CCmdMultiSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdMultiSelect.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdMultiSelect::CCmdMultiSelect(CHLDoc *pDoc,CQuad &quad)
{
	commandName.LoadString(IDS_MULTISELECT_CMD);
	m_pDoc=pDoc;
	m_chainOld.StoreChain(pDoc->GetChain());
	pDoc->CaptureObjectCmd(quad);
	m_chainNew.StoreChain(pDoc->GetChain());
	CQuad quadNew=pDoc->GetLimit();
	pDoc->SetTrackerQuad(quadNew);
	m_bFirst=TRUE;
}

CCmdMultiSelect::~CCmdMultiSelect()
{
}

BOOL CCmdMultiSelect::Do(BOOL bRedo)
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdMultiSelect::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}