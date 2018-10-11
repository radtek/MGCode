// FntTTF.h: interface for the CFntTTF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FNTTTF_H__759CA421_FF30_11D4_A486_00105A615525__INCLUDED_)
#define AFX_FNTTTF_H__759CA421_FF30_11D4_A486_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Fnt.h"

class CFntTTF : public CFnt  
{
	DECLARE_SERIAL(CFntTTF)

public:
	CFntTTF();
	virtual ~CFntTTF();
	virtual void Serialize( CArchive& ar );

public:
	LOGFONT m_lf;		//字体
	virtual BOOL MakeStroke();
	BOOL MakeFixStroke();		//固定间距
	BOOL MakeOldStroke();		//老方式，不等间距
	
};

#endif // !defined(AFX_FNTTTF_H__759CA421_FF30_11D4_A486_00105A615525__INCLUDED_)
