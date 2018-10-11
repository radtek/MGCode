// KeyBind.h: interface for the CKeyBind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBIND_H__5659B25E_456C_4DA9_BC2A_BF60983C26A5__INCLUDED_)
#define AFX_KEYBIND_H__5659B25E_456C_4DA9_BC2A_BF60983C26A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KeyBind/KeyBindObj.h"
#include "KeyBind/BindSetArchive.h"
#include <afxtempl.h>

class CKeyBind  
{
public:
	void RefreshBindSet();
	int DoModal();
	CKeyBind();
	virtual ~CKeyBind();
public:
	CTypedPtrList<CObList,CKeyBindObj*> m_keyBindSet;
	HACCEL m_hAccel;
};

#endif // !defined(AFX_KEYBIND_H__5659B25E_456C_4DA9_BC2A_BF60983C26A5__INCLUDED_)
