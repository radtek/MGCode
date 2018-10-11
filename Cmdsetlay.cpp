// CmdSetLay.cpp: implementation of the CCmdSetLay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdSetLay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
#define PTY_LAYER	1
#define PTY_PARA	2
#define PTY_ADD		6
*/
CCmdSetLay::CCmdSetLay(CHLDoc *pDoc,int mode,CLayer ly)
{
	if(mode==PTY_OUTLINE || mode==PTY_FILL || mode==PTY_DEL||mode==PTY_LAYER)
	{
		m_bFlag=TRUE;
	}
	else
	{
		m_bFlag=FALSE;
	}

	commandName.LoadString(IDS_COMMAND_SETLAY);
	m_pDoc=pDoc;

	m_layerOld=pDoc->GetLayer(ly.m_nLayer);
	if(m_bFlag)
	{
//		m_chainOld=pDoc->GetChain();
		m_chainOld.StoreChain(pDoc->GetChain());
	}

	m_pDoc->SetLayCmd(mode,ly);
	m_layerNew=pDoc->GetLayer(ly.m_nLayer);
	if(m_bFlag)
	{
//		m_chainNew=pDoc->GetChain();
		m_chainNew.StoreChain(pDoc->GetChain());
	}

	m_bFirst=TRUE;

	return;
}

CCmdSetLay::~CCmdSetLay()
{

}


BOOL CCmdSetLay::Do(BOOL bRedo )
{
	if(m_bFirst)
	{
		m_bFirst=FALSE;
	}
	else
	{
		m_pDoc->SetLayer(m_layerNew);

		if(m_bFlag)    m_pDoc->SetChain(m_chainNew);
	}

	return TRUE;
}

BOOL CCmdSetLay::Undo(BOOL bRedo)
{
	m_pDoc->SetLayer(m_layerOld);
	
	if(m_bFlag)    m_pDoc->SetChain(m_chainOld);

	return TRUE;
}
