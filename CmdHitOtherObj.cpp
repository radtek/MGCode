// CmdHitOtherObj.cpp: implementation of the CCmdHitOtherObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdHitOtherObj.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdHitOtherObj::CCmdHitOtherObj(CHLDoc *pDoc,CPoint &point,CWnd *pWnd)
{
	commandName.LoadString(IDS_HITOTHEROBJ_CMD);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	pDoc->ChangeQuadCmd(point,pWnd);
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdHitOtherObj::~CCmdHitOtherObj()
{

}

BOOL CCmdHitOtherObj::Do(BOOL bRedo )
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdHitOtherObj::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}
