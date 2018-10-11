// CmdAddNode.cpp: implementation of the CCmdAddNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdAddNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdAddNode::CCmdAddNode(CHLDoc *pDoc,CTree *pTree)
{
	commandName.LoadString(IDS_COMMAND_ADDNODE);
	m_pDoc=pDoc;
	m_chainOld=pDoc->GetChain();
	m_pDoc->AddNodeCmd(pTree);
	m_chainNew=pDoc->GetChain();
	m_bFirst=TRUE;
}

CCmdAddNode::CCmdAddNode(CHLDoc *pDoc,CNode *pNode)
{
	commandName.LoadString(IDS_COMMAND_ADDNODE);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());
	m_pDoc->AddNodeCmd(pNode);
//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdAddNode::~CCmdAddNode()
{
}

BOOL CCmdAddNode::Do(BOOL bRedo)
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdAddNode::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}
