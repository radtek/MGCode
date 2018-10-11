// CmdFill.cpp: implementation of the CCmdChangeLay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "Cmdchangelay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdChangeLay::CCmdChangeLay(CHLDoc *pDoc)
{
	commandName.LoadString(IDS_COMMAND_FILL);
	m_pDoc=pDoc;
	m_chainOld=pDoc->GetChain();
	m_pDoc->SetLayerCmd();
	m_chainNew=pDoc->GetChain();
	m_bFirst=TRUE;
}

CCmdChangeLay::~CCmdChangeLay()
{
}
void CCmdChangeLay::SetVerFillPara(double spaceVer,double spaceVerOld)
{
	m_dbFillSpaceVer=spaceVer;
	m_dbOldSpaceVer=spaceVerOld;
}

BOOL CCmdChangeLay::Do(BOOL bRedo)
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdChangeLay::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}
