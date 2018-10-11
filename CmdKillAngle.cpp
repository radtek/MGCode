// CmdKillAngle.cpp: implementation of the CCmdKillAngle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdKillAngle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdKillAngle::CCmdKillAngle(CHLDoc *pDoc)
{
	commandName.LoadString(IDS_HITOTHEROBJ_CMD);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	pDoc->KillAngleCmd();
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdKillAngle::~CCmdKillAngle()
{

}

BOOL CCmdKillAngle::Do(BOOL bRedo )
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

BOOL CCmdKillAngle::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}