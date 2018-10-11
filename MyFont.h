// MyFont.h: interface for the CMyFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFONT_H__C65D93E3_80A8_443F_B488_C0A156B52FF3__INCLUDED_)
#define AFX_MYFONT_H__C65D93E3_80A8_443F_B488_C0A156B52FF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyFont : public CFont
{
public:
	CMyFont();
	virtual ~CMyFont();

	void SetHeight(long nHeight);
	void SetFaceName(LPCTSTR szName);

	LOGFONT GetLogFont();
	void SetLogFont(LOGFONT &font);	

private:
	LOGFONT m_logfont;
};

#endif // !defined(AFX_MYFONT_H__C65D93E3_80A8_443F_B488_C0A156B52FF3__INCLUDED_)
