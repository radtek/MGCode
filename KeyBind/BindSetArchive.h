// BindSetArchive.h: interface for the CBindSetArchive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINDSETARCHIVE_H__C8D281AC_26A1_40ED_A728_EC714932A565__INCLUDED_)
#define AFX_BINDSETARCHIVE_H__C8D281AC_26A1_40ED_A728_EC714932A565__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBindSetArchive
{
public:
	CBindSetArchive(UINT nMode);
	virtual ~CBindSetArchive();
public:
	CArchive	*m_pArchive;
	CFile		m_file;
};


#endif // !defined(AFX_BINDSETARCHIVE_H__C8D281AC_26A1_40ED_A728_EC714932A565__INCLUDED_)
