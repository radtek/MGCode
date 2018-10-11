// TextSet.h: interface for the CTextSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTSET_H__A553193B_8AED_4831_897F_E5E325567A50__INCLUDED_)
#define AFX_TEXTSET_H__A553193B_8AED_4831_897F_E5E325567A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NodeText.h"
#include "Text.h"

class CTextSet : public CObject 
{
public:
	CTextSet();
	virtual ~CTextSet();

public:
	void DeleteNodeText();
	void operator=(const CTextSet& src);
	void Delete();
	CNodeText* m_pNodeText;
	CText* m_pText;
};

#endif // !defined(AFX_TEXTSET_H__A553193B_8AED_4831_897F_E5E325567A50__INCLUDED_)
