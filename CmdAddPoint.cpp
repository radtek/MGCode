// CmdAddPoint.cpp: implementation of the CCmdAddPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdAddPoint.h"
#include "resource.h"
#include "HLView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdAddPoint::CCmdAddPoint(CHLDoc *pDoc,CPoint &point,CWnd *pWnd)
{
	commandName.LoadString(IDS_ADDPOINT_CMD);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	CHLView *pView=(CHLView*)pWnd;
	pView->AddPoint(point);
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdAddPoint::~CCmdAddPoint()
{

}

BOOL CCmdAddPoint::Do(BOOL bRedo )
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdAddPoint::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}
