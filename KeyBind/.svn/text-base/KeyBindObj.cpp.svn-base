// KeyBindObj.cpp: implementation of the CKeyBindObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\hl.h"
#include "KeyBindObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CKeyBindObj, CObject, VERSIONABLE_SCHEMA | 2 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyBindObj::CKeyBindObj()
{
	m_dwCmdId = 0;
	m_wVirtualKeyCode = 0;
	m_wModifiers = 0;
	m_strDescriptor = "";
}

CKeyBindObj::~CKeyBindObj()
{

}

void CKeyBindObj::Serialize( CArchive& archive )
{
	if (archive.IsStoring())
	{
		archive << m_dwCmdId;
		archive << m_wVirtualKeyCode;
		archive << m_wModifiers;
		archive << m_strDescriptor;
	}
	else
	{
		archive >> m_dwCmdId;
		archive >> m_wVirtualKeyCode;
		archive >> m_wModifiers;
		archive >> m_strDescriptor;
	}
}