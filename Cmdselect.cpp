// CmdSelect.cpp: implementation of the CCmdSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdSelect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdSelect::CCmdSelect(CHLDoc *pDoc,BOOL *pFlag1,BOOL *pFlag2,int count)
{
	commandName.LoadString(IDS_COMMAND_SELECT);
	m_pDoc=pDoc;

	//内存分配采用谁申请谁释放的原则
	m_pFlag1=new BOOL[count];
	m_pFlag2=new BOOL[count];

	memcpy(m_pFlag1,pFlag1,sizeof(BOOL)*count);
	memcpy(m_pFlag2,pFlag2,sizeof(BOOL)*count);
	
	m_bFirst=TRUE;
}


CCmdSelect::~CCmdSelect()
{
	delete []m_pFlag1;
	delete []m_pFlag2;
}

BOOL CCmdSelect::Do(BOOL bRedo )
{
	if(m_bFirst)
		m_bFirst=FALSE;
	else
		m_pDoc->SetSelectFlag(m_pFlag2);
	return TRUE;
}

BOOL CCmdSelect::Undo(BOOL bRedo)
{
	m_pDoc->SetSelectFlag(m_pFlag1);
	return TRUE;
}
