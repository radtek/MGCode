// CmdExtendObj.cpp: implementation of the CCmdExtendObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdExtendObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdExtendObj::CCmdExtendObj(CHLDoc *pDoc, double dbSpace, BOOL bIsKillOldObj)
{
	commandName.LoadString(IDS_EXTENDOBJ_CMD);
	
	m_pDoc     = pDoc;
//	m_chainOld = pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	pDoc->ExtendObjCmd(dbSpace, bIsKillOldObj);
	m_dbSpace  = dbSpace;
	m_bIsKillOldObj  = bIsKillOldObj;
//	m_chainNew = pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdExtendObj::~CCmdExtendObj()
{

}


BOOL CCmdExtendObj::Do(BOOL bRedo)
{
	if(m_bFirst)
	{
		m_bFirst=FALSE;
	}
	else
	{
		m_pDoc->SetChain(m_chainNew);
	}
	
	return TRUE;
}

BOOL CCmdExtendObj::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}