// JoinPointCmd.cpp: implementation of the JoinPointCmd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "JoinPointCmd.h"
#include "HLView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JoinPointCmd::JoinPointCmd(CHLDoc *pDoc,CWnd *pWnd)
{
	m_pDoc=pDoc;
	m_chainOld=pDoc->GetChain();
	CHLView *pView=(CHLView*)pWnd;
	pView->JoinPoint();
	m_chainNew=pDoc->GetChain();
	m_bFirst=TRUE;
}

JoinPointCmd::~JoinPointCmd()
{

}

BOOL JoinPointCmd::Do(BOOL bRedo )
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL JoinPointCmd::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}

