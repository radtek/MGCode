// MgrText.h: interface for the CMgrText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MGRTEXT_H__A0FF7157_D1AA_4973_80BF_4D7BCE1763A2__INCLUDED_)
#define AFX_MGRTEXT_H__A0FF7157_D1AA_4973_80BF_4D7BCE1763A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Text.h"

//////////////////////////////////////////////////////////////////////////
//此类专门用来处理文本内容的数据表
//在遍例时通过ID找到相关的TEXT数据
//////////////////////////////////////////////////////////////////////////
class CMgrText
{
public:
	CMgrText();
	virtual ~CMgrText();

public:
	void Clear();
	void FlashText(UINT nTextID,DWORD nStyle);
	BOOL FlashNextText(DWORD nStyle);
	void SetUse(BOOL bFlag);
	void Add(CMgrText* pMgrText);
	void SetBaseID(UINT nID);
	BOOL GetCopySts();
	UINT GetBaseID();
	void SetCopySts(BOOL bFlag);
	UINT GetLastID();
	void SaveText(FILE* fp,BOOL bFlag);
	BOOL IsExist(UINT nIndex);
	CText* GetAt(UINT nIndex);
	void Delete();
	void SetNextID(UINT nID);
	//nStyle=0x00时不进行下一个号的处理，即MARKTIMES不加1
	BOOL IsOverMark(UINT nID,DWORD nStyle=0x00);
	void GetString(UINT nID,CString& strInfo,DWORD nStyle=0x00);
	UINT GetNextID();
	void AddText(CText* pText);

	void DeleteByID(UINT nTextID);

	void operator =(const CMgrText &src);
		
	CTypedPtrList<CObList,CText*>m_list;		//文本数据列表

private:
	UINT m_nTextNextID;

	UINT m_nTextBase;
	BOOL m_bIsCopy;

public:
	BOOL ChangeText(CString strTxt,CString ModelNo);
	BOOL ChangeText(CString strTxt,UINT textid);
};
extern CMgrText* g_MgrText;

#endif // !defined(AFX_MGRTEXT_H__A0FF7157_D1AA_4973_80BF_4D7BCE1763A2__INCLUDED_)
