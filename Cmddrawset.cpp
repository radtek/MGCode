// CmdDrawSet.cpp: implementation of the CCmdDrawSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdDrawSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDrawSet::CCmdDrawSet(CHLDoc *pDoc)
{
	commandName.LoadString(IDS_COMMAND_DRAWSET);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	m_pDoc->DrawsetCmd();
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdDrawSet::~CCmdDrawSet()
{

}

BOOL CCmdDrawSet::Do(BOOL bRedo )
{	
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdDrawSet::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}