// CmdConvertSelToOneTree.cpp: implementation of the CCmdConvertSelToOneTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdConvertSelToOneTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdConvertSelToOneTree::CCmdConvertSelToOneTree(CHLDoc *pDoc)
{
	commandName.LoadString(IDS_COMMAND_COMTOTREE);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	m_pDoc->ComToOneTreeCmd();
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdConvertSelToOneTree::~CCmdConvertSelToOneTree()
{

}

BOOL CCmdConvertSelToOneTree::Do(BOOL bRedo)
{	
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdConvertSelToOneTree::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}