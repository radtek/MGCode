// KeyBindObj.h: interface for the CKeyBindObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBINDOBJ_H__5C8B6A6B_E415_4775_8153_E205043CC0A1__INCLUDED_)
#define AFX_KEYBINDOBJ_H__5C8B6A6B_E415_4775_8153_E205043CC0A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyBindObj : public CObject  
{
	DECLARE_SERIAL(CKeyBindObj)
public:
	CKeyBindObj();
	virtual ~CKeyBindObj();
	void Serialize( CArchive& archive );
public:
	DWORD m_dwCmdId;
	WORD m_wVirtualKeyCode;
	WORD m_wModifiers;
	CString m_strDescriptor;
};

#endif // !defined(AFX_KEYBINDOBJ_H__5C8B6A6B_E415_4775_8153_E205043CC0A1__INCLUDED_)
