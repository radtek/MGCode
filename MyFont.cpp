// MyFont.cpp: implementation of the CMyFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyFont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyFont::CMyFont()
{
	m_logfont.lfHeight = 9; 
	m_logfont.lfWidth = 0; 
	m_logfont.lfEscapement = 0; 
	m_logfont.lfOrientation = 0; 
	m_logfont.lfWeight = FW_NORMAL; 
	m_logfont.lfItalic = FALSE; 
	m_logfont.lfUnderline = FALSE; 
	m_logfont.lfStrikeOut = FALSE; 
	m_logfont.lfCharSet = DEFAULT_CHARSET; 
	m_logfont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	m_logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	m_logfont.lfQuality = DEFAULT_QUALITY; 
	m_logfont.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(m_logfont.lfFaceName, "Tohama");
}

CMyFont::~CMyFont()
{
	
}

void CMyFont::SetHeight(long nHeight)
{
	DeleteObject();

	m_logfont.lfHeight = nHeight;
	CreateFontIndirect(&m_logfont);
}

void CMyFont::SetFaceName(LPCTSTR szName)
{
	DeleteObject();
	
	strcpy(m_logfont.lfFaceName, szName);
	CreateFontIndirect(&m_logfont);	
}

LOGFONT CMyFont::GetLogFont()
{
	return m_logfont;
}

void CMyFont::SetLogFont(LOGFONT &font)
{
	DeleteObject();

	memcpy(&m_logfont, &font, sizeof(m_logfont));
	CreateFontIndirect(&m_logfont);	
}