#pragma once
#include "Stroke.h"

typedef CTypedPtrList<CObList,CStroke*> STROKELIST;

class CETCharCTInfo : public CObject
{
public:
	CETCharCTInfo(void);
	~CETCharCTInfo(void);
public:
	STROKELIST m_StrokeList;
	UINT m_nOffset;
	DWORD m_dwAsciiCode;
	void ClearStrokeList();
};