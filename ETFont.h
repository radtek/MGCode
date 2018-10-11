#pragma once

#include "ETCharCTInfo.h"

class CETFont
{
public:
	CETFont(void);
	~CETFont(void);
	struct
	{
		CString StrFontName;
		CString StrFontVer;
		UINT nCharNum;
	} FontInfo;
	CTypedPtrList<CObList,CETCharCTInfo *> m_ETChar_list;
private:
	void New();
	void Destroy();
public:
	void ReNew();
};

