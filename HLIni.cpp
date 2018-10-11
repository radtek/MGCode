// HlIni.cpp: implementation of the CHlIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HlIni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHlIni::CHlIni()
{

}

CHlIni::~CHlIni()
{

}
inline CHlIni::CHlIni(LPCTSTR lpszFile)
{
	if (lpszFile!=NULL)
		::_tcscpy(m_szFileName,lpszFile);
}

BOOL CHlIni::SetFileName(LPCTSTR lpszFileName)
{
	::GetModuleFileName(NULL, m_szFileName, MAX_PATH);
	strcpy(strrchr(m_szFileName, '\\')+1, lpszFileName);
	return TRUE;		
}

void CHlIni::WriteDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, double dbKeyValue)
{
	CString strValue;
	strValue.Format(_T("%f"),dbKeyValue);
	WriteString(lpszSection,lpszKey,strValue);
}

void CHlIni::WriteInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nKeyValue)
{
	CString strValue;
	strValue.Format(_T("%d"),nKeyValue);
	WriteString(lpszSection,lpszKey,strValue);
}

void CHlIni::WriteString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	VERIFY(IsInitialized());
	///////////////////////////////
	::WritePrivateProfileString(lpszSection,lpszKey,lpszValue,m_szFileName);
}

LPTSTR CHlIni::ReadString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultStr)
{
	VERIFY(IsInitialized());
	
	TCHAR buff[MAX_PATH];
	DWORD ret = ::GetPrivateProfileString(lpszSection,lpszKey,lpszDefaultStr,buff,MAX_PATH,m_szFileName);
	if (ret<=0)  return (LPTSTR)NULL;
	
	TCHAR *pStr = new TCHAR[ret+1];
	if (pStr==NULL) return (LPTSTR)NULL;
	::ZeroMemory((PVOID)pStr,sizeof(TCHAR)*(ret+1));
	::CopyMemory((PVOID)pStr,(PVOID)buff,sizeof(TCHAR)*ret);
	
	return (LPTSTR)pStr;
}

CString CHlIni::ReadString2(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultStr)
{
	LPTSTR p = ReadString(lpszSection,lpszKey,lpszDefaultStr);
	if ( NULL!= p )
	{
		CString strData(p);
		delete[] p;
		return strData;
	}
	
	return _T("");
}

int CHlIni::ReadInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nDefault) 
{
	int nKeyValue(nDefault);
	
	LPTSTR p=ReadString(lpszSection,lpszKey,NULL);
	if ( NULL!=p )
	{
		nKeyValue = ::_ttoi(p);
		delete[] p;
	}
	
	return nKeyValue;
}

double CHlIni::ReadDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, double dbDefault) 
{
	double dbKeyValue(dbDefault);

	LPTSTR p=ReadString(lpszSection,lpszKey,NULL);
	if ( NULL!=p )
	{
		dbKeyValue = ::atof(p);
		delete[] p;
	}

	return dbKeyValue;
}






