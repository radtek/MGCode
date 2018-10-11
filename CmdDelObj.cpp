// CmdDelObj.cpp: implementation of the CCmdDelObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdDelObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDelObj::CCmdDelObj(CHLDoc *pDoc, DWORD dwPara)
{
	commandName.LoadString(IDS_COMMAND_DELETE);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	m_pDoc->DelObjCmd(dwPara);
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdDelObj::~CCmdDelObj()
{

}

BOOL CCmdDelObj::Do(BOOL bRedo)
{	
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdDelObj::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}


CCmdDelDsfObj::CCmdDelDsfObj(CHLDoc *pDoc, DWORD dwPara)
{
	commandName.LoadString(IDS_COMMAND_DELETE);
	m_pDoc=pDoc;
	//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	m_pDoc->DelObjCmd(dwPara);
	//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdDelDsfObj::~CCmdDelDsfObj()
{

}

BOOL CCmdDelDsfObj::Do(BOOL bRedo)
{	
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdDelDsfObj::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}