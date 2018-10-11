// SplitPointCmd.cpp: implementation of the SplitPointCmd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "SplitPointCmd.h"
#include "HLView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SplitPointCmd::SplitPointCmd(CHLDoc *pDoc,CWnd *pWnd,double dbPoint)
{
	m_pDoc=pDoc;
	m_chainOld=pDoc->GetChain();
	CHLView *pView=(CHLView*)pWnd;
	pView->SplitPoint(dbPoint);
	m_chainNew=pDoc->GetChain();
	m_bFirst=TRUE;
}

SplitPointCmd::~SplitPointCmd()
{

}

BOOL SplitPointCmd::Do(BOOL bRedo )
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL SplitPointCmd::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}
