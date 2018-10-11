// HLException.cpp: implementation of the CHLException class.
// Coder:liuzq106425
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HLException.h"
#include <eh.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define EXCEPTION_CODE_HL_ERROR 0xE0011000

//VC++ 5.0 _MSC_VER = 1100;  MS VC++ 6.0 _MSC_VER = 1200;  MS VC++ 10.0 _MSC_VER = 1600

#if _MSC_VER >= 1600
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
void CreateDumpFile(struct _EXCEPTION_POINTERS *pExp, char szFileName[MAX_PATH])
{	
	time_t ltime;
	time(&ltime);
	if (ltime!=-1)
		strftime(szFileName, 256, "C:\\HL_%Y%m%d_%H%M%S.dmp", localtime(&ltime));

	HANDLE hDumpFile = CreateFileA(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);  
	if (hDumpFile!=INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION miniDumpExceptionInfo;
		miniDumpExceptionInfo.ExceptionPointers = pExp;  
		miniDumpExceptionInfo.ThreadId = GetCurrentThreadId(); 
		miniDumpExceptionInfo.ClientPointers = TRUE;
		MINIDUMP_TYPE MiniDumpType = MINIDUMP_TYPE(MiniDumpNormal|MiniDumpWithDataSegs|MiniDumpWithFullMemory|MiniDumpWithHandleData|MiniDumpFilterMemory|MiniDumpScanMemory);
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpType, &miniDumpExceptionInfo, NULL, NULL); 
		CloseHandle(hDumpFile);
	}
}
#endif

DWORD WINAPI HLExceptionThreadFunc(LPVOID lpParam) 
{ 
	LPCSTR strText = (LPCSTR)lpParam;
	::MessageBoxA(NULL, strText, "HL Unhandled Exception", MB_OK|MB_ICONERROR|MB_TOPMOST);
	return 0; 
} 

LPTOP_LEVEL_EXCEPTION_FILTER g_pLastTopLevelExceptionFilter=NULL;
LONG WINAPI HLUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	char szFileName[MAX_PATH] = "C:\\HL.dmp";

#if _MSC_VER >= 1600
	CreateDumpFile(ExceptionInfo, szFileName);
#endif

	char strText[1152] = "";
	if (ExceptionInfo->ExceptionRecord->ExceptionCode==0xE06D7363)
	{
		CHLException *pE = *(CHLException **)ExceptionInfo->ExceptionRecord->ExceptionInformation[1];
		if (IsBadWritePtr(pE,4)==0 && pE->m_nSEHFlag==EXCEPTION_CODE_HL_ERROR)
			sprintf(strText, "%s\n%s\n%s", pE->m_strMessage, pE->m_strSource, szFileName);		
		else
			sprintf(strText, "%s\n%s", "C++ Exception!", szFileName);
	}
	else
	{
		sprintf(strText, "SEH Exception: 0x%X\n%s", ExceptionInfo->ExceptionRecord->ExceptionCode, szFileName);	
	}
	
	HANDLE hThread = CreateThread(NULL, 0, HLExceptionThreadFunc, (LPVOID)strText, 0,  NULL);
	WaitForSingleObject(hThread, INFINITE);

	_exit(0);
	return EXCEPTION_CONTINUE_SEARCH;
}

void InitHLExceptionManager()
{
	SetUnhandledExceptionFilter(HLUnhandledExceptionFilter);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void GetCurrentModuleName(char fileName[MAX_PATH])
{
	HMODULE hModule=NULL;
	::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)&GetCurrentModuleName, &hModule);
	::GetModuleFileName(hModule, fileName, MAX_PATH);
}

CHLException::CHLException(LPCSTR strErrorInfo):m_nSEHFlag(EXCEPTION_CODE_HL_ERROR)
{
	char szFile[MAX_PATH];	
	GetCurrentModuleName(szFile);
	strcpy(m_strSource, strrchr(szFile, '\\')+1);
	
	if (strErrorInfo)
		sprintf_s(m_strMessage, "%s", strErrorInfo);
}

CHLException::~CHLException()
{

}

void CHLException::Delete()
{
	delete this;
}
