// TextSet.cpp: implementation of the CTextSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "TextSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextSet::CTextSet()
{
 	m_pNodeText = new CNodeText();
 	m_pText = new CText(1);
}

CTextSet::~CTextSet()
{
}

void CTextSet::Delete()
{
 	delete m_pNodeText;
 	delete m_pText;
}

void CTextSet::operator=(const CTextSet& src)
{
	Delete();
	m_pNodeText = (CNodeText*)(src.m_pNodeText->Copy(0x01));
	m_pText = new CText(1);
	*m_pText = *(src.m_pText);
}

void CTextSet::DeleteNodeText()
{
	delete m_pNodeText;
}
