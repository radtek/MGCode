// CmdMAlign.cpp: implementation of the CCmdMAlign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdMAlign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdMAlign::CCmdMAlign(CHLDoc *pDoc, int nType)
{
	commandName.LoadString(IDS_MALIGN);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	pDoc->MAlignCmd(nType);
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	CQuad quadNew=m_pDoc->GetLimit();
	m_pDoc->SetTrackerQuad(quadNew);
	m_bFirst=TRUE;
}

CCmdMAlign::~CCmdMAlign()
{

}

BOOL CCmdMAlign::Do(BOOL bRedo )
{	
	if(m_bFirst)
		m_bFirst=FALSE;
	else
	{
		m_pDoc->SetChain(m_chainNew);
		m_chainNew.RefreshSelectQuadList(m_pDoc->m_QuadList);
		CQuad quadNew=m_pDoc->GetLimit();
		m_pDoc->SetTrackerQuad(quadNew);
	}
	return TRUE;
}

BOOL CCmdMAlign::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	m_chainOld.RefreshSelectQuadList(m_pDoc->m_QuadList);
	CQuad quadNew=m_pDoc->GetLimit();
	m_pDoc->SetTrackerQuad(quadNew);
	return TRUE;
}