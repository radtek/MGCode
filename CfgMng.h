// CfgMng.h: interface for the CCfgMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFGMNG_H__FFC97B7E_3480_4EE9_8207_0E4D2C416752__INCLUDED_)
#define AFX_CFGMNG_H__FFC97B7E_3480_4EE9_8207_0E4D2C416752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FILE_MAX_LENGTH			FILENAME_MAX
#define CREATE_MODE_REBUILD		CFile::modeCreate
#define CREATE_MODE_NOREBUILD	CFile::modeNoTruncate | CFile::modeCreate

#define OPEN_MODE_READ			CFile::modeRead
#define OPEN_MODE_WRITE			CFile::modeReadWrite
#define OPEN_MODE_READWRITE		CFile::modeReadWrite

class CCfgMng  
{
public:
	CCfgMng();
	virtual ~CCfgMng();

public:
	void CloseFile();
	BOOL OpenFile(LPCTSTR lpszFileName, UINT nFlags);
	void MoveStart();
	void MoveEnd();
	BOOL IsFileExist(CString &strFileName);

public:
	BOOL ReadWToC(CArray<WTOC, WTOC> &WToCList);

private:
	CString m_strFileName;
	CStdioFile m_file;

};

#endif // !defined(AFX_CFGMNG_H__FFC97B7E_3480_4EE9_8207_0E4D2C416752__INCLUDED_)
