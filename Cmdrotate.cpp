// CmdRotate.cpp: implementation of the CCmdRotate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdRotate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdRotate::CCmdRotate(CHLDoc *pDoc,CDot dot,double angle )
{
	commandName.LoadString(IDS_COMMAND_ROTATE);
	m_pDoc=pDoc;
	m_dotCenter=dot;
	m_dbAngle=angle;
}

CCmdRotate::~CCmdRotate()
{

}

BOOL CCmdRotate::Do(BOOL bRedo )
{
	m_pDoc->RotateCmd(m_dotCenter,m_dbAngle);
	return TRUE;
}

BOOL CCmdRotate::Undo(BOOL bRedo)
{
	m_pDoc->RotateCmd(m_dotCenter,-m_dbAngle);
	return TRUE;
}

