// stdafx.cpp : source file that includes just the standard includes
//	HL.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "Quad.h"

SYSCFG g_SysCfg;
//SYSTEMPARAM g_SysPara;
BOOL   g_bIsSort       = FALSE;
BOOL   g_bIsRedPreview = FALSE;
BOOL   g_bIsSelMark    = FALSE;
BOOL   g_bIsFastMark   = FALSE;

DWORD64 g_OBJID  = 0;     // CTree对象ID
LANGID  g_LangID = 2052;

DWORD   g_dwVersion = 520000;

USERINFO g_CurUserInfo; // 当前用户信息
CArray<USERINFO, USERINFO> g_RightInfoList; // 权限列表

LPLOADDIB LoadDib = NULL;
LPPROCDIB ProcDib = NULL;
LPPROCGRAYDIB ProcGrayDib = NULL;
LPDELDIB DelDib = NULL;

pINITAUTOPARASET InitAutoParaSet = NULL;
pDESTORYAUTOSET  DestoryAutoSet  = NULL;
pWRITEAUTOPARA   WriteAutoPara   = NULL;
pREADAUTOPARA    ReadAutoPara    = NULL;


void GetExepath(char *pPath)  //获取执行目录
{
	char pExePath[256];
	memset(pExePath, 0, 256);
	GetModuleFileName(NULL,pExePath,256);
	char* pEnd = strrchr(pExePath,'\\');
	*(pEnd+1) = NULL;
	if(pPath)
	{
		strcpy(pPath, pExePath);
	}
	return;
}

double MyPrecision(double val,int digit)
{
	double k;
	k=pow(double(10),digit);
	if(val>0)
		val=(_int64)(val*k+0.5)/k;
	else
		val=(_int64)(val*k-0.5)/k;

	return val;
}

double MyLogToTrue(double val)
{
	CQuad quad(0,0,val,val);
	quad.LogToTrue();
	return quad.Width();
}


double MyTrueToLog(double val)
{
	return val;
}

double MyDegToRad(double val)
{
	return val*PAI/180;
}

double MyRadToDeg(double val)
{
	return val*180/PAI;
}


double ReadIniDouble(char *pszFileName, LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue)
{
	TCHAR szbuff[MAX_PATH];
	memset(szbuff, 0, MAX_PATH);
	
	DWORD dwRet=::GetPrivateProfileString(lpszSection,lpszKey,lpszDefaultValue,szbuff, MAX_PATH, pszFileName);
	if( 0 >= dwRet )
	{
	    return	FALSE;
	}

	double duValue = atof(szbuff);

	return duValue;
}

void WriteIniDouble(char *pszFileName, LPCTSTR lpszSection, LPCTSTR lpszKey, double dbValue)
{
	TCHAR szbuff[MAX_PATH];
	memset(szbuff, 0, MAX_PATH);
	sprintf_s(szbuff, MAX_PATH, "%f", dbValue);
	
	::WritePrivateProfileString(lpszSection, lpszKey, szbuff, pszFileName);

	return;
}

int ReadIniInt(char *pszFileName, LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue)
{
	TCHAR szbuff[MAX_PATH];
	memset(szbuff, 0, MAX_PATH);
	
	DWORD dwRet=::GetPrivateProfileString(lpszSection,lpszKey,lpszDefaultValue,szbuff, MAX_PATH, pszFileName);
	if( 0 >= dwRet )
	{
	    return	FALSE;
	}

	int nValue = atoi(szbuff);

	return nValue;
}

void WriteIniInt(char *pszFileName, LPCTSTR lpszSection, LPCTSTR lpszKey, int nValue)
{
	TCHAR szbuff[MAX_PATH];
	memset(szbuff, 0, MAX_PATH);
	sprintf_s(szbuff, MAX_PATH, "%d", nValue);
	
	::WritePrivateProfileString(lpszSection, lpszKey, szbuff, pszFileName);

	return;
}

void WriteRunLog(LPCSTR lpszLog, int nLevel)
{
	if (nLevel > g_sys.m_nLogLevel)
		return;

	CString strLogDir, strLogFile, strDelFile, strNewLine;

	char AppDir[MAX_PATH];
	::GetModuleFileName(::GetModuleHandle(NULL), AppDir, MAX_PATH);
	*strrchr(AppDir, '.')=0;
	LPCTSTR strAppName = strrchr(AppDir,'\\')+1;
	strLogDir.Format("D:\\%sRunLog", strAppName);

	CTimeSpan timeSpan(30,0,0,0);	//Ä¬ÈÏ±£Áô30ÌìµÄÈÕÖ¾
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeDel = timeCur -timeSpan;
	strLogFile.Format("%s\\%s_%s.txt", strLogDir, strAppName, timeCur.Format("%Y%m%d"));
	strDelFile.Format("%s\\%s_%s.txt", strLogDir, strAppName, timeDel.Format("%Y%m%d"));
	if (timeCur.Format("%Y%m%d").IsEmpty())	//LZQ,20130121
		AfxMessageBox("WriteRunLog: system time error!");

	SYSTEMTIME time;
	::GetLocalTime(&time);
	strNewLine.Format("%4d-%02d-%02d %02d:%02d:%02d.%03d %s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, lpszLog);

	static CRITICAL_SECTION csWriteLog;
	static BOOL bFirstLog = TRUE;	
	if (bFirstLog)
	{
		::InitializeCriticalSection(&csWriteLog);
		bFirstLog = FALSE;
	}
	::EnterCriticalSection(&csWriteLog);

	CFileStatus status;
	if (!CFile::GetStatus(strLogDir, status))
		::CreateDirectory(strLogDir, NULL);

	if (CFile::GetStatus(strDelFile, status))
		::DeleteFile(strDelFile);

	CStdioFile file;
	if (file.Open(strLogFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
	{
		file.SeekToEnd();		
		file.WriteString(strNewLine);
		file.Close();
	}

	::LeaveCriticalSection(&csWriteLog);
}
