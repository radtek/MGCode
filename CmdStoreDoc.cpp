// CmdStoreDoc.cpp: implementation of the CCmdStoreDoc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdStoreDoc.h"
#include "resource.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdStoreDoc::CCmdStoreDoc(CHLDoc *pDoc)
{
	commandName.LoadString(IDS_STOREDOC);

	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	pDoc->m_chain.SetSelect();
	pDoc->SetTrackerQuad(pDoc->GetAllLimit());
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdStoreDoc::~CCmdStoreDoc()
{

}

BOOL CCmdStoreDoc::Do(BOOL bRedo )
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdStoreDoc::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}