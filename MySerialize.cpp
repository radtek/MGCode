// MySerialize.cpp: implementation of the CMySerialize class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "MySerialize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySerialize::CMySerialize(FILE* fp)
{
	m_pFile = NULL;
	m_pFile = new CStdioFile(fp);
}

CMySerialize::~CMySerialize()
{
	delete m_pFile;
}

BOOL CMySerialize::WriteString(CString& strInfo)
{
	if(m_pFile)
	{
		strInfo += "\r\n";
		m_pFile->WriteString(strInfo);
	}

	return TRUE;
}

BOOL CMySerialize::operator <<(double d)
{
	if(m_pFile)
	{
		CString str;
		str.Format("%.6f",d);
		WriteString(str);
	}
	
	return TRUE;
}

BOOL CMySerialize::operator<<(DWORD n)
{
	if(m_pFile)
	{
		CString str;
		str.Format("%d",n);
		WriteString(str);
	}
	
	return TRUE;
}

BOOL CMySerialize::operator<<(int n)
{
	if(m_pFile)
	{
		CString str;
		str.Format("%d",n);
		WriteString(str);
	}
	
	return TRUE;
}

BOOL CMySerialize::operator<<(UINT n)
{
	if(m_pFile)
	{
		CString str;
		str.Format("%d",n);
		WriteString(str);
	}
	
	return TRUE;
}

BOOL CMySerialize::operator<<(LONG n)
{
	if(m_pFile)
	{
		CString str;
		str.Format("%d",n);
		WriteString(str);
	}
	
	return TRUE;
}

BOOL CMySerialize::operator<<(WORD n)
{
	if(m_pFile)
	{
		CString str;
		str.Format("%d",n);
		WriteString(str);
	}
	
	return TRUE;
}

BOOL CMySerialize::operator<<(float f)
{
	if(m_pFile)
	{
		CString str;
		str.Format("%.6f",f);
		WriteString(str);
	}
	
	return TRUE;
}

BOOL CMySerialize::operator<<(CString str)
{
	if(m_pFile)
		WriteString(str);
	
	return TRUE;
}

BOOL CMySerialize::operator>>(double& d)
{
	BOOL bFlag = FALSE;

	if(m_pFile)
	{
		CString str;
		bFlag = m_pFile->ReadString(str);
		d = atof(str);
	}

	return bFlag;
}

BOOL CMySerialize::operator>>(DWORD& n)
{
	BOOL bFlag = FALSE;

	if(m_pFile)
	{
		CString str;
		bFlag = m_pFile->ReadString(str);
		n = atoi(str);
	}

	return bFlag;
}

BOOL CMySerialize::operator>>(int& n)
{
	BOOL bFlag = FALSE;
	
	if(m_pFile)
	{
		CString str;
		bFlag = m_pFile->ReadString(str);
		n = atoi(str);
	}
	
	return bFlag;
}

BOOL CMySerialize::operator>>(UINT& n)
{
	BOOL bFlag = FALSE;
	if(m_pFile)
	{
		CString str;
		bFlag = m_pFile->ReadString(str);
		n = atoi(str);
	}
	return bFlag;
}

BOOL CMySerialize::operator>>(LONG& n)
{
	BOOL bFlag = FALSE;
	if(m_pFile)
	{
		CString str;
		bFlag = m_pFile->ReadString(str);
		n = atoi(str);
	}
	return bFlag;
}

BOOL CMySerialize::operator>>(WORD& n)
{
	BOOL bFlag = FALSE;
	if(m_pFile)
	{
		CString str;
		bFlag = m_pFile->ReadString(str);
		n = atoi(str);
	}
	return bFlag;
}

BOOL CMySerialize::operator>>(float& f)
{
	BOOL bFlag = FALSE;
	if(m_pFile)
	{
		CString str;
		bFlag = m_pFile->ReadString(str);
		f = (float)(atof(str));
	}
	return bFlag;
}

BOOL CMySerialize::operator>>(CString& str)
{
	BOOL bFlag = FALSE;
	if(m_pFile)
	{
		bFlag = m_pFile->ReadString(str);
		str.TrimRight("\r\n");
	}
	return bFlag;
}
