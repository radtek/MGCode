// CmdArray.cpp: implementation of the CCmdArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD g_dwJumpCnt =-1;
CCmdArray::CCmdArray(CHLDoc *pDoc, CArray<OBJPOS, OBJPOS> &dtArray,BOOL flag)
{
	commandName.LoadString(IDS_COMMAND_ARRAY);
	m_pDoc=pDoc;
//	m_chainOld=pDoc->GetChain();
	m_chainOld.StoreChain(pDoc->GetChain());

	if (flag)
	{
		g_dwJumpCnt = 0;
	}
	else
	{
		g_dwJumpCnt = -1;
	}
	
	m_pDoc->ArrayCmd(dtArray);
	g_dwJumpCnt =-1;

//	m_chainNew=pDoc->GetChain();
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}

CCmdArray::~CCmdArray()
{

}


BOOL CCmdArray::Do(BOOL bRedo)
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetChain(m_chainNew);
	return TRUE;
}

BOOL CCmdArray::Undo(BOOL bRedo)
{
	m_pDoc->SetChain(m_chainOld);
	return TRUE;
}
