// MySerialize.h: interface for the CMySerialize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSERIALIZE_H__2E4B1930_B263_4090_B8C4_1E133F82F460__INCLUDED_)
#define AFX_MYSERIALIZE_H__2E4B1930_B263_4090_B8C4_1E133F82F460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySerialize  
{
public:
	CMySerialize(FILE* fp);
	virtual ~CMySerialize();

public:
	//–¥»Î
	BOOL operator<<(double d);
	BOOL operator<<(DWORD n);
	BOOL operator<<(int n);
	BOOL operator<<(LONG n);
	BOOL operator<<(UINT n);
	BOOL operator<<(WORD n);
	BOOL operator<<(float f);
	BOOL operator<<(CString str);
	//∂¡≥ˆ
	BOOL operator>>(double& d);
	BOOL operator>>(DWORD& n);
	BOOL operator>>(int& n);
	BOOL operator>>(LONG& n);
	BOOL operator>>(UINT& n);
	BOOL operator>>(WORD& n);
	BOOL operator>>(float& f);
	BOOL operator>>(CString& str);

private:
	CStdioFile* m_pFile;
	BOOL WriteString(CString& strInfo);
};

#endif // !defined(AFX_MYSERIALIZE_H__2E4B1930_B263_4090_B8C4_1E133F82F460__INCLUDED_)
