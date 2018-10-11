// DogCheck.cpp: implementation of the CDogCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "hl.h"
#include "DogCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDogCheck g_DogCheck;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define FDICH_DLL "Fdich.dll"
#else
#define FDICH_DLL "Fdich.dll"
#endif

CDogCheck::CDogCheck()
{
	memset(m_pszDogID, 0, 256);
	memset(m_pszLmtVerifyID, 0, 256);
	m_bInit = FALSE;
	m_hModule = NULL;
	m_hModule = LoadLibrary(FDICH_DLL);
	if (m_hModule == NULL)
	{
		m_bInit = FALSE;
		return;
	}
	m_pCheckAll = (CheckAll)GetProcAddress(m_hModule, (LPCSTR)0x0001);
	m_pCheckDogIsExist = (CheckDogIsExist)GetProcAddress(m_hModule, (LPCSTR)0x0002);
	m_pGetDogIdAndLmtVerifyCode = (GetDogIdAndLmtVerifyCode)GetProcAddress(m_hModule, (LPCSTR)0x0004);
	m_pSetDogRemoveProc = (SetDogRemoveProc)GetProcAddress(m_hModule, (LPCSTR)0x0005);
	m_pGetTypeVerifyCode = (GetTypeVerifyCode)GetProcAddress(m_hModule, (LPCTSTR)0x0006);
	m_pGetDogVer = (GetDogVer)GetProcAddress(m_hModule, (LPCTSTR)0x0007);
	m_pGetDogTimeInfo = (GetDogTimeInfo)GetProcAddress(m_hModule, (LPCTSTR)0x0008);
	m_pTimeCountStart = (TimeCountStart)GetProcAddress(m_hModule, (LPCTSTR)0x0009);
	m_pTimeCountSaveCur = (TimeCountSaveCur)GetProcAddress(m_hModule, (LPCTSTR)10);
	if (m_pCheckAll&&m_pCheckDogIsExist&&m_pGetDogIdAndLmtVerifyCode&&
		m_pSetDogRemoveProc&&m_pGetTypeVerifyCode&&m_pGetDogVer&&
		m_pGetDogTimeInfo&&m_pTimeCountStart&&m_pTimeCountSaveCur)
	{
		m_bInit = TRUE;
	}
	else
	{
		m_bInit = FALSE;
	}
}

CDogCheck::~CDogCheck()
{
	if (m_hModule != NULL)
	{
		FreeLibrary(m_hModule);
	}
}

BOOL CDogCheck::__CheckAll(DWORD dwSoftType, DWORD dwCardType)
{
	if (!m_bInit)
	{
		AfxMessageBox("Not find Fdich.dll or Version is not correct");
		return FALSE;
	}
	DWORD dwCode;
	char pszMsg[1024];
	m_pCheckAll(dwSoftType, pszMsg, 1024, dwCardType, dwCode);
	BOOL bFlag = __CheckCode(dwCode, pszMsg);
	return bFlag;
}
BOOL CDogCheck::__CheckDogIsExist(long nStyle)
{
	DWORD dwCode;
	m_pCheckDogIsExist(nStyle, dwCode);
	BOOL bFlag = (dwCode==FRC_SUCCESS);//__CheckCode(dwCode, "");
	return bFlag;
}
void CDogCheck::__GetDogIdAndLmtVerifyCode(char *pszDogID, DWORD nDogIDSize, 
								char *pszLmtVerifyCode, DWORD nLmtVerifySize)
{
	m_pGetDogIdAndLmtVerifyCode(pszDogID, nDogIDSize, pszLmtVerifyCode, nLmtVerifySize);
}
void CDogCheck::__SetDogRemoveProc(pOnDogRemove dogRemoveProc, LPVOID lpContext)
{
	if(m_pSetDogRemoveProc != NULL)
	{
		m_pSetDogRemoveProc(dogRemoveProc, lpContext);
	}
}

BOOL CDogCheck::__CheckCode(DWORD dwCode, char *pszMsg)
{
	CString strTemp;
	strTemp.Format("%d", dwCode);
	OutputDebugString(strTemp);
	switch(dwCode)
	{
	case FRC_SUCCESS://返回成功
		return TRUE;
	case FRC_SUCESS_WITH_USERID://带工号返回成功
		return TRUE;
	case FRC_SUCCESS_WITH_TIMESHOW://成功但限时使用，并提醒使用剩余时间
		//软件授权还允许运行%d天，需要联系大族客服人员，重新申请与(狗ID=%s, 校验码=%s)匹配的许可授权补丁，以便到期后能继续使用
	case FRC_SUCCESS_WITH_INSPECT://成功但巡检提醒
		//设备已连续运行一年，需要联系大族客服人员，对设备进行维护检修，以确保安全正常运行，
		//并申请与(狗ID=%s, 校验码=%s)匹配的软件授权检修确认补丁
		strTemp = pszMsg;
		AfxMessageBox(strTemp);
		return TRUE;
	case FRC_FAIL_DOG_LIB_LOAD://载入软件狗操作动态库失败
	case FRC_FAIL_DOG_LIB_GUID://操作失败，HansLmtH.dll模块库类型不匹配
	case FRC_FAIL_DOG_LIB_VERSION://操作失败，HansLmtH.dll模块库版本至少是V1.1.0
		//模块库载入失败或模块库版本不正确，请检测软件安装是否正确
	case FRC_FAIL_DOG_EXECUTE://将初始狗转换成运行狗失败
		//软件狗操作异常，请检测狗连接是否正常，驱动是否安装正确
	case FRC_FAIL_DOG_TIMEOUT://试用时间已经用完
		//当前软件授权已到期，需要联系大族客服人员，重新申请与(狗ID=%s, 校验码=%s)匹配的许可授权补丁，以便继续使用
	case FRC_FAIL_DOG_WRONG_TYPE:
		//!=0x00软件狗类型不匹配，请确认软件狗是否是该设备配套的软件狗，否则请联系大族客服人员
		//!=0x01软件狗类型不匹配，请确认软件狗是否是该设备配套的软件狗，否则，请确认软件，控制卡及软件狗是否需要升级
	case FRC_FAIL_DOG_CMP_CARD://与打标卡不对应
	case FRC_FAIL_DOG_FUNC_UNKNOW://狗操作中发生未知错误
	case FRC_FAIL_DOG_FUNC_NODOG://没有检测到软件狗
	case FRC_FAIL_DOG_FUNC_WRONGDOG://检测到软件狗，但不是合法的软件狗
		
	case FRC_FAIL_DOG_FUNC_CHECK_WRONG_BEFORE_EXECUTE://在Execute函数调用前没有成功调用过Check函数
	case FRC_FAIL_DOG_FUNC_LIB_L_VERSION://HansLmtL.dll模块库版本至少V1.1.0
		//模块库载入失败或模块库版本不正确，请检测软件安装是否正确
		
	case FRC_FAIL_DOG_FUNC_MOREDOG://插入的软件狗多于1个
	case FRC_FAIL_DOG_FUNC_WRONG_PARAM://函数传入参数不支持
		
	case FRC_FAIL_PARA_UNKNOW_CARDTYPE://未知的控制卡类型
	case FRC_FAIL_PARA_UNKONW_DOGTYPE://未知的狗类型
		
	case FRC_FAIL_CARD_LIB_LOAD://控制卡库加载失败
	case FRC_FAIL_CARD_FUNC_GET://取控制卡库接口函数失败
	case FRC_FAIL_CARD_NONE://无控制卡
	case FRC_FAIL_CARD_WRITE_NEW://写新卡失败
		//写卡失败，请检测控制卡的连接或坏损
		strTemp = pszMsg;
		AfxMessageBox(strTemp);
		return FALSE;
	case FRC_FAIL_CANCEL:
	default:
		return FALSE;
	}
}

BOOL CDogCheck::__Dog()
{
	DWORD dwCardType = CT_PCI3000;
	BOOL bFlag = __CheckAll(0x02 ,dwCardType);
	if (bFlag)
	{
		__RegRemoveProc();
	}
	return bFlag;
}

CString CDogCheck::GetExePath()
{
	//之前通过装载动态库时来确定当前程序是否运行在安装目录下即可
	//找到自已的目录，即为安装目录。否则运行错误
	char pExePath[256];
	
	GetModuleFileName(NULL,pExePath,256); //If this parameter is NULL, GetModuleFileName returns the path for the file containing the current process. 
	char* pEnd = strrchr(pExePath,'\\');  //Scan a string for the last occurrence of a character.	
	*(pEnd+1) = NULL;
	
	CString strTemp;
	strTemp.Format("%s", pExePath);
	return strTemp;
}

DWORD WINAPI OnDogRemove(LPVOID lpContext)
{
	CDogCheck *pDogCheck = (CDogCheck*)lpContext;
	do 
	{
		AfxMessageBox("SofDog has been removed, Please insert the sofDog!");
		AfxGetMainWnd()->PostMessage(WM_QUIT, 0, 0);
		break;
	}
	while (!pDogCheck->__CheckDogIsExist(2));
	return 0;
}

void CDogCheck::__RegRemoveProc()
{
	__GetDogIdAndLmtVerifyCode(m_pszDogID, 256, m_pszLmtVerifyID, 256);
	m_pGetTypeVerifyCode(m_pszTypeVerifyID, 256);
	m_pGetDogVer(m_dwDogVerMain, m_dwDogVerSub);
	m_pGetDogTimeInfo(m_bTmSupport, m_pszTmEsVerifyID, 256, m_bTmEnable, m_dwTmUsed);
	__SetDogRemoveProc(&OnDogRemove, this);
}

BOOL CDogCheck::DogTimeCountStart()
{
	return m_pTimeCountStart()==FRC_SUCCESS;
}

BOOL CDogCheck::DogTimeCountSaveCur()
{
	return m_pTimeCountSaveCur()==FRC_SUCCESS;
}
