#pragma once

#include "AdvParaItem.h"

typedef CList<CString, CString&> StringList;

const TCHAR c_strParaIni[] = _T("MarkPara.ini");
const TCHAR c_strParaDescIni[] = _T("MarkParaDesc.ini");

class CManageLayPara
{
	enum{
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_ENUM
	};
	typedef struct ItemInfo
	{
		int nSectionIndex;
		int nItemIndex;
		int nItemType;
		int nEnumType;
		int bShow;
	}ItemInfo;
public:
	CManageLayPara(void);
	~CManageLayPara(void);
	void InitPropertyGrid(CMFCPropertyGridCtrl &propertyGrid);
	void GetPara(BYTE *pBUFF, int nSize, CMFCPropertyGridCtrl &propertyGrid);
	void SetPara(BYTE *pBUFF, int nSize, CMFCPropertyGridCtrl &propertyGrid);
	void LoadParaInfo();
	void LoadDefaultPara();
	void ClearArray();
	CString GetLayItemName(int index);
	CString GetLayItemValue(int index, BYTE *pBuff, int nSize);
	void LoadAdvParaLib();
private:
	CArray<CMFCPropertyGridProperty*, CMFCPropertyGridProperty*> m_propertyItemList;
	CArray<ItemInfo, ItemInfo&> m_arrayParaType;
	CArray<CString, CString&> m_arrayParaName;
	CArray<CString, CString&> m_arraySectionName;
	CArray<StringList*, StringList*> m_arrayEnum;
	int m_nShowListIndex[3];
	BYTE m_pDefaultPara[sizeof(PARA)];
	int m_nCount;
//	CString m_strAdvSectionName;
//	CString m_strAdvItemName;
//	CAdvParaItem *m_pAdvItem;

	HMODULE m_hAdvParaModule;
	LPVOID m_pShowAdvParaDlg;
};
