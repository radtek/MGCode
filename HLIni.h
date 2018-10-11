// HlIni.h: interface for the CHlIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLINI_H__46314F82_02E6_11D8_8AEF_000AE6A39D2F__INCLUDED_)
#define AFX_HLINI_H__46314F82_02E6_11D8_8AEF_000AE6A39D2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHlIni : public CObject  
{
public:
	CHlIni();
	CHlIni(LPCTSTR lpszFile);
	virtual ~CHlIni();

public:
	BOOL  SetFileName(LPCTSTR lpszFileName);
	BOOL  IsInitialized(){ return ((m_szFileName==NULL)?FALSE:TRUE); };
	CString ReadString2(LPCTSTR lpszSection,LPCTSTR lpszKey, LPCTSTR lpszDefaultStr = NULL);
	LPTSTR	ReadString(LPCTSTR lpszSection,LPCTSTR lpszKey, LPCTSTR lpszDefaultStr = NULL);
	int		ReadInt(LPCTSTR lpszSection,LPCTSTR lpszKey, int  nDefault = 0);
	double	ReadDouble(LPCTSTR lpszSection,LPCTSTR lpszKey,double dbDefault = 0);
	void	WriteString(LPCTSTR lpszSection,LPCTSTR lpszKey,LPCTSTR lpszValue);
	void	WriteInt(LPCTSTR lpszSection,LPCTSTR lpszKey,int nKeyValue);
	void	WriteDouble(LPCTSTR lpszSection,LPCTSTR lpszKey,double dbKeyValue);

protected:
	TCHAR m_szFileName[MAX_PATH];

private:

};

#endif // !defined(AFX_HLINI_H__46314F82_02E6_11D8_8AEF_000AE6A39D2F__INCLUDED_)
