// CmdNewDrag.cpp: implementation of the CCmdNewDrag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdNewDrag.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdNewDrag::CCmdNewDrag(CHLDoc *pDoc,CDot &dot,double dbratioX,double dbratioY,double dbmoveX,double dbmoveY)
{
	commandName.LoadString(IDS_NEWDRAG_CMD);

	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	pDoc->DragCmd(dot,dbratioX,dbratioY,dbmoveX,dbmoveY);
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdNewDrag::~CCmdNewDrag()
{

}

BOOL CCmdNewDrag::Do(BOOL bRedo )
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdNewDrag::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}

