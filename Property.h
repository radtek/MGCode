// Property.h: interface for the CProperty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTY_H__0C449176_5909_4EEC_BADE_CAE5B9E6D0E2__INCLUDED_)
#define AFX_PROPERTY_H__0C449176_5909_4EEC_BADE_CAE5B9E6D0E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CProperty : public CObject  
{
	DECLARE_SERIAL(CProperty)
		
public:
	CProperty();
	virtual ~CProperty();

public:
	void operator=(const CProperty& src);
	BOOL operator==(const CProperty& src);
	CProperty(const CProperty& src);
	virtual void Serialize(FILE* fp,BOOL bFlag);
	void Set(int mode, CProperty &src);

public:
	int	 m_nLayer;
	BOOL m_bLockFlag;
	COLORREF m_color;

};

#endif // !defined(AFX_PROPERTY_H__0C449176_5909_4EEC_BADE_CAE5B9E6D0E2__INCLUDED_)
