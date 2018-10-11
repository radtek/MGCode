// CfgMng.cpp: implementation of the CCfgMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CfgMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCfgMng::CCfgMng()
{

}

CCfgMng::~CCfgMng()
{

}

// Open the specified file
BOOL CCfgMng::OpenFile(LPCTSTR lpszFileName, UINT nFlags)
{
	try
	{
		if (!m_file.Open(lpszFileName, nFlags))
		{
			return FALSE;
		}
		m_strFileName = lpszFileName;
		return TRUE;
	}
	catch (...) 
	{
		return FALSE;
	}
}


// Close the current file
void CCfgMng::CloseFile()
{
	try
	{
		m_file.Close();
	}
	catch (...)
	{
	}
	
	return;
}

// Move to the start of the file
void CCfgMng::MoveStart()
{
	m_file.SeekToBegin();
	return;
}

// Move the end of the file
void CCfgMng::MoveEnd()
{
	m_file.SeekToEnd();
	return;
}


BOOL CCfgMng::IsFileExist(CString &strFileName)
{
	CFileFind ofilefind;
	if (!ofilefind.FindFile(strFileName))
	{
		ofilefind.Close();

		return FALSE;
	}

	ofilefind.Close();

	return TRUE;
}

BOOL CCfgMng::ReadWToC(CArray<WTOC, WTOC> &WToCList)
{
	char szPath[256];
	GetExepath(szPath);
	CString strFileName("");
	strFileName.Format("%s%s", szPath, "WToC.w2c");

	if (!IsFileExist(strFileName)) 
	{
		return FALSE;
	}

	if (!OpenFile(strFileName, OPEN_MODE_READ))
	{
		CloseFile();
		return FALSE;
	}

	CString strFindTemp;
	MoveStart();

	while(m_file.ReadString(strFindTemp))
	{
		CString temp = strFindTemp;

		int nFirstPost = -1;

		int nEndPost   = -1;

		if (";" == temp.Left(1))
		{
			continue;
		}

		nFirstPost = temp.FindOneOf("@");
		if (-1 == nFirstPost)
		{
			continue;
		}

		WTOC wtoc;

		// Key÷µ
		CString strValue = temp.Left(nFirstPost);
		strValue.TrimLeft();
		strValue.TrimRight();
		wtoc.dbW = atof(strValue);

		// ƒ¨»œ÷µ
		strValue   = temp.Mid((nFirstPost + 1), temp.GetLength());
		strValue.TrimLeft();
		strValue.TrimLeft("=");
		strValue.TrimRight();
		wtoc.dbC = atof(strValue);

		WToCList.Add(wtoc);
	}
	
	CloseFile();

	return TRUE;
}
