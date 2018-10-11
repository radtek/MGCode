// CmdSelGroupByNum.cpp: implementation of the CCmdSelGroupByNum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdSelGroupByNum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdSelGroupByNum::CCmdSelGroupByNum(CHLDoc *pDoc,int nNum, CWnd *pWnd)
{
	commandName.LoadString(IDS_SELGROUPBYNUM_CMD);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	pDoc->SelGroupByNumCmd(nNum, pWnd);
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;

}

CCmdSelGroupByNum::~CCmdSelGroupByNum()
{

}


BOOL CCmdSelGroupByNum::Do(BOOL bRedo )
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdSelGroupByNum::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}
