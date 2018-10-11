// RightMng.cpp: implementation of the CRightMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "RightMng.h"
#include "InnerMsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRightMng::CRightMng()
{

}

CRightMng::~CRightMng()
{

}

BOOL CRightMng::OpenDataFile(CString &FileName)
{
	try
	{
		if(!m_fDataFile.Open(FileName, CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone | CFile::typeBinary | CFile::modeNoTruncate))
		{
			return FALSE;
		}
	}
	catch (const CFileException& ex)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CRightMng::CloseDataFile()
{
	try
	{
		m_fDataFile.Close();
	}
	catch (const CFileException& ex)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CRightMng::ReadDataFile(CString &FileName, char *pszBuffer, DWORD &dwBufferLen)
{
	CFileFind findFile;
	BOOL bRet = findFile.FindFile(FileName);
	if (!bRet)
	{
		return FALSE;
	}

	if (!OpenDataFile(FileName))
	{
		return FALSE;
	}

	DWORD dwFileLen;
	
	try
	{
		dwFileLen = (DWORD)m_fDataFile.GetLength();
		m_fDataFile.Read(pszBuffer, dwFileLen);

		dwBufferLen = ((RIGHTINFO*)(pszBuffer))->dwLength;
	}
    catch(...)
	{
	
	}
	
	if (!CloseDataFile())
	{
		return FALSE;
	}

	if (dwFileLen < (((RIGHTINFO*)(pszBuffer))->dwLength))
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CRightMng::WriteDataFile(CString &FileName, char *pszBuffer, DWORD dwBufferLen)
{
	if (!OpenDataFile(FileName))
	{
		return FALSE;
	}

	try
	{
		m_fDataFile.Write(pszBuffer, dwBufferLen);
	}
    catch(...)
	{
	}

	if (!CloseDataFile())
	{
		return FALSE;
	}

	return TRUE;
}


void CRightMng::ReadRightInfo(CArray<USERINFO, USERINFO> &RightInfo, CString &strRightFile)
{
	CString strFileName("");
	if (0 == strRightFile.CompareNoCase("")) 
	{
		char szPath[256];
		GetExepath(szPath);
		strFileName.Format("%s%s", szPath, "RigthInfo.dat");
	}
	else
	{
		strFileName = strRightFile;
	}

    char *pszRigthInfo = new char[1024 * 500];
	if (NULL == pszRigthInfo)
	{
		return ;
	}

	DWORD dwLenght = 0;
	BOOL bRet = ReadDataFile(strFileName, pszRigthInfo, dwLenght);
	if (!bRet)
	{
		if (pszRigthInfo) 
		{
			delete[] pszRigthInfo;
			pszRigthInfo = NULL;
		}
		
		return;
	}

	RightInfo.RemoveAll();

	int nCount = ((RIGHTINFO*)(pszRigthInfo))->nUserCount;
	char *pUInfo = (pszRigthInfo + sizeof(RIGHTINFO));
	for (int i = 0; i < nCount; i++)
	{
		USERINFO UserInfo;
		USERINFO* pstUInfo = ((USERINFO*)(pUInfo + i * sizeof(USERINFO)));
		UserInfo.dwRight1 = pstUInfo->dwRight1;
		UserInfo.dwRight2 = pstUInfo->dwRight2;
		UserInfo.bIsDefaultUser = pstUInfo->bIsDefaultUser;
		strncpy(UserInfo.szUserName, pstUInfo->szUserName, 64);
		strncpy(UserInfo.szPassWord, pstUInfo->szPassWord, 64);
		RightInfo.Add(UserInfo);
	}

	if (pszRigthInfo) 
	{
		delete[] pszRigthInfo;
		pszRigthInfo = NULL;
	}

	return;
}

void CRightMng::WriteRightInfo(CArray<USERINFO, USERINFO> &RightInfo, CString &strRightFile)
{
	CString strFileName("");
	if (0 == strRightFile.CompareNoCase("")) 
	{
		char szPath[256];
		GetExepath(szPath);
		strFileName.Format("%s%s", szPath, "RigthInfo.dat");
	}
	else
	{
		strFileName = strRightFile;
	}

	char *pszRigthInfo = new char[1024 * 500];
	if (NULL == pszRigthInfo)
	{
		return ;
	}

	int nUserCount = RightInfo.GetSize();

	RIGHTINFO stRightInfo;
	stRightInfo.dwLength   = (nUserCount* sizeof(USERINFO)) + sizeof(RIGHTINFO);
	stRightInfo.nUserCount = nUserCount;

	memcpy(pszRigthInfo, &stRightInfo, sizeof(RIGHTINFO));
	int nMemIndex = sizeof(RIGHTINFO);

	for (int i = 0; i < nUserCount; i++)
	{
		USERINFO UserInfo = RightInfo.GetAt(i);
		USERINFO *pstUserInfo = ((USERINFO*)(pszRigthInfo + nMemIndex));
		memcpy(pstUserInfo, &UserInfo, sizeof(USERINFO));
		nMemIndex += sizeof(USERINFO);
	}

	WriteDataFile(strFileName, pszRigthInfo, stRightInfo.dwLength);

	if (pszRigthInfo) 
	{
		delete[] pszRigthInfo;
		pszRigthInfo = NULL;
	}
	
	return;
}

void CRightMng::WriteSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFile)
{
	if (0 == strFile.CompareNoCase("")) 
	{
		return;
	}

	char *pszMarkSet = new char[sizeof(OBJMARKSET)];
	if (NULL == pszMarkSet)
	{
		return ;
	}
	
	memcpy(pszMarkSet, &objMarkSet, sizeof(OBJMARKSET));
	WriteDataFile(strFile, pszMarkSet, sizeof(OBJMARKSET));

	if (pszMarkSet) 
	{
		delete[] pszMarkSet;
		pszMarkSet = NULL;
	}
	
	return;
}

void CRightMng::ReadSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFile)
{
	if (0 == strFile.CompareNoCase("")) 
	{
		return;
	}
	
    char *pszMarkSet = new char[1024];
	if (NULL == pszMarkSet)
	{
		return ;
	}
	
	DWORD dwLenght = 0;
	BOOL bRet = ReadDataFile(strFile, pszMarkSet, dwLenght);
	if (!bRet)
	{
		if (pszMarkSet) 
		{
			delete[] pszMarkSet;
			pszMarkSet = NULL;
		}
		
		return;
	}
	
	memcpy(&objMarkSet, pszMarkSet, sizeof(OBJMARKSET));
	if (pszMarkSet) 
	{
		delete[] pszMarkSet;
		pszMarkSet = NULL;
	}
	
	return;
}
