#include "stdafx.h"
#include "ETFont.h"


CETFont::CETFont(void)
{
	FontInfo.StrFontName="ETFonts";
	FontInfo.StrFontVer="Ver1.0";
	FontInfo.nCharNum=256;
	New();
}

CETFont::~CETFont(void)
{
	Destroy();
}

void CETFont::New()
{
	for(UINT iCounter=0;iCounter<FontInfo.nCharNum;iCounter++)
	{
		CETCharCTInfo *pETCharCTInfoIns=new CETCharCTInfo();
		pETCharCTInfoIns->m_dwAsciiCode=iCounter;
		m_ETChar_list.AddTail(pETCharCTInfoIns);
	}
}

void CETFont::ReNew()
{
	POSITION pos;
	CETCharCTInfo *pETCharCTInfoIns;
	pos=m_ETChar_list.GetHeadPosition();
	for(UINT iCounter=0;iCounter<FontInfo.nCharNum;iCounter++)
	{
		pETCharCTInfoIns=m_ETChar_list.GetNext(pos);
		pETCharCTInfoIns->m_nOffset=0;
		pETCharCTInfoIns->ClearStrokeList();
	}
}

void CETFont::Destroy()
{
	POSITION pos,pre;
	CETCharCTInfo *pETCharCTInfoIns;
	pos=m_ETChar_list.GetHeadPosition();
	for(UINT iCounter=0;iCounter<FontInfo.nCharNum;iCounter++)
	{
		pre=pos;
		pETCharCTInfoIns=m_ETChar_list.GetNext(pos);
		m_ETChar_list.RemoveAt(pre);
		delete pETCharCTInfoIns;
	}
}