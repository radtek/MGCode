#include "stdafx.h"
#include "ETCharCTInfo.h"


CETCharCTInfo::CETCharCTInfo(void)
{
	m_nOffset=0;
	m_dwAsciiCode=0;
}

CETCharCTInfo::~CETCharCTInfo(void)
{
	ClearStrokeList();
}

void CETCharCTInfo::ClearStrokeList()
{
	POSITION pre,pos;
	CStroke *pStroke;
	pos=m_StrokeList.GetHeadPosition();
	while(pos)
	{
		pre=pos;
		pStroke=m_StrokeList.GetNext(pos);
		m_StrokeList.RemoveAt(pre);
	}
}