// CmdScale.cpp: implementation of the CCmdScale class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdScale.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdScale::CCmdScale(CHLDoc *pDoc,CDot dot,double ratioX,double ratioY)
{
	commandName.LoadString(IDS_COMMAND_SCALE);
	m_pDoc=pDoc;
	m_dotCenter=dot;
	m_dbRatioX=ratioX;
	m_dbRatioY=ratioY;
}

CCmdScale::~CCmdScale()
{

}

BOOL CCmdScale::Do(BOOL bRedo )
{
	m_pDoc->ScaleCmd(m_dotCenter,m_dbRatioX,m_dbRatioY);
	return TRUE;
}

BOOL CCmdScale::Undo(BOOL bRedo)
{
	m_pDoc->ScaleCmd(m_dotCenter,1/m_dbRatioX,1/m_dbRatioY);
	return TRUE;
}
