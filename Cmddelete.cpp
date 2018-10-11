// CmdDelete.cpp: implementation of the CCmdDelete class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdDelete.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDelete::CCmdDelete(CHLDoc *pDoc)
{
	commandName.LoadString(IDS_COMMAND_DELETE);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	m_pDoc->DeleteCmd();
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdDelete::~CCmdDelete()
{

}

BOOL CCmdDelete::Do(BOOL bRedo)
{	
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdDelete::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}