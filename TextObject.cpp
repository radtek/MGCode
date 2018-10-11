// TextObject.cpp: implementation of the CTextObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "TextObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextObject::CTextObject()
{
}

CTextObject::~CTextObject()
{
	CTextSet* pTextSet;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pTextSet=m_list.GetNext(pos);
		delete pTextSet;
	}
	
	m_list.RemoveAll();
}

void CTextObject::AddText(CTextSet* pTextSet)
{
	m_list.AddTail(pTextSet);
}

//删除列表中的了对象
void CTextObject::DeleteItem(UINT nIndex)
{
	CTextSet* pTextSet;

	UINT nCurIndex = 0;
	POSITION prv;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		prv=pos;
		pTextSet=m_list.GetNext(pos);
		if(nCurIndex == nIndex)
		{
			pTextSet->Delete();
			delete pTextSet;
			m_list.RemoveAt(prv);
			break;
		}
		nCurIndex ++;
	}
}

void CTextObject::Delete()
{
	CTextSet* pTextSet;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pTextSet=m_list.GetNext(pos);
		pTextSet->Delete();
		delete pTextSet;
	}

	m_list.RemoveAll();
}

CTextSet* CTextObject::GetItem(UINT nIndex)
{
	CTextSet* pTextSet;
	
	UINT nCurIndex = 0;
	POSITION prv;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		prv=pos;
		pTextSet=m_list.GetNext(pos);
		if(nCurIndex == nIndex)
		{
			return pTextSet;
			break;
		}
		nCurIndex ++;
	}

	return NULL;
}
