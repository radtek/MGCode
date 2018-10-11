// System.cpp: implementation of the CSystemPara class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "System.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSystem g_sys;
BOOL CSystem::m_bSystemOk = FALSE;
CMarkCard* CSystem::m_pCard = NULL;

extern SYSCFG g_SysCfg;
extern CArray<USERINFO, USERINFO> g_RightInfoList; // 用户权限列表


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystem::CSystem()
{
	m_pCard = NULL;
	m_nDibSampleMode=1;	
	m_bEnableCardLog = 0;
	m_nFullTaskCount = 4;
	m_langID = 0x804;
}

CSystem::~CSystem()
{
	if(m_pCard)
	{
		delete m_pCard;
	}
}

void CSystem::ReadSysCfg()
{
	char szPath[256];
	GetExepath(szPath);
	char szFileName[256];
	sprintf(szFileName, "%s%s", szPath, "SystemSet.ini");

	g_SysCfg.m_dbDotFilterPrecis = ReadIniDouble(szFileName, "SysCfg", "PointFilterPrecis",   "0.1");
    g_SysCfg.m_dbDotFilterAngle = ReadIniDouble(szFileName, "SysCfg", "PointFilterAngle",   "45");
	g_SysCfg.m_dbDotMoveStepLen  = ReadIniDouble(szFileName, "SysCfg", "PointPrecisMoveStep", "1");
	g_SysCfg.m_dbMicro           = ReadIniDouble(szFileName, "SysCfg", "MICRO",       "2");
	g_SysCfg.m_dbClick			 = ReadIniDouble(szFileName, "SysCfg", "CLICK",       "5");
	g_SysCfg.m_nUndoCount        = ReadIniInt(szFileName,    "SysCfg", "UNDO",        "10");	
	g_SysCfg.m_dbOneToTwo		 = ReadIniDouble(szFileName, "SysCfg", "ONETOTWO",    "0.1");
	g_SysCfg.m_dbExtendSpace     = ReadIniDouble(szFileName, "SysCfg", "ExtendSpace", "-0.1");
    g_SysCfg.m_dbKillAngleSpace  = ReadIniDouble(szFileName, "SysCfg", "KillAngleSpace", "0.01");
	g_SysCfg.m_dbGraphImportPrics  = ReadIniDouble(szFileName, "SysCfg", "GraphImportPrics", "0.01");
	g_SysCfg.m_dbJoinDistance	=  ReadIniDouble(szFileName,  _T("EditCfg"), _T("JoinDistance"),"0.01");	//曲线合并时的距离限制

	// 绘图参数
    g_SysCfg.m_dbDrawEllipSet  = ReadIniDouble(szFileName, "SysCfg", "DrawEllipSet", "0.5");
	g_SysCfg.m_nGridCol        = ReadIniInt(szFileName, "SysCfg", "DrawGridCOL", "3");
	g_SysCfg.m_nGridRow        = ReadIniInt(szFileName, "SysCfg", "DrawGridRow", "3");

	g_SysCfg.m_nPolygon        = ReadIniInt(szFileName, "SysCfg", "POLYGON", "0");
	g_SysCfg.m_nPolyNum        = ReadIniInt(szFileName, "SysCfg", "POLYNUM", "5");
	g_SysCfg.m_nPolyCross      = ReadIniInt(szFileName, "SysCfg", "POLYCROSS", "1");
	g_SysCfg.m_nPolyStar       = ReadIniInt(szFileName, "SysCfg", "POLYSTAR", "50");

    g_SysCfg.m_nLayerCnt       = ReadIniInt(szFileName, "SysCfg", "LayerCnt", "10");
	if (g_SysCfg.m_nLayerCnt > LAYERCOUNT) 
	{
		g_SysCfg.m_nLayerCnt = LAYERCOUNT;
	}

	// 预览鼠标缩放比例
	g_SysCfg.m_dbPreViewZoomScale  = ReadIniDouble(szFileName, "SysCfg", "PreViewZoomScale", "1.2");



	return;
}

void CSystem::WriteSysCfg()
{
	char szPath[256];
	GetExepath(szPath);
	char szFileName[256];
	sprintf(szFileName, "%s%s", szPath, "SystemSet.ini");

	WriteIniDouble(szFileName, "SysCfg", "PointFilterPrecis",   g_SysCfg.m_dbDotFilterPrecis);
    WriteIniDouble(szFileName, "SysCfg", "PointFilterAngle",   g_SysCfg.m_dbDotFilterAngle);
	WriteIniDouble(szFileName, "SysCfg", "PointPrecisMoveStep", g_SysCfg.m_dbDotMoveStepLen);
	WriteIniDouble(szFileName, "SysCfg", "MICRO",      g_SysCfg.m_dbMicro);
	WriteIniDouble(szFileName, "SysCfg", "CLICK",      g_SysCfg.m_dbClick);
	WriteIniInt(szFileName,    "SysCfg", "UNDO",       g_SysCfg.m_nUndoCount);	
	WriteIniDouble(szFileName, "SysCfg", "ONETOTWO",   g_SysCfg.m_dbOneToTwo);
	WriteIniDouble(szFileName, "SysCfg", "ExtendSpace",g_SysCfg.m_dbExtendSpace);
    WriteIniDouble(szFileName, "SysCfg", "KillAngleSpace",g_SysCfg.m_dbKillAngleSpace);
	WriteIniDouble(szFileName, "EditCfg", "JoinDistance",g_SysCfg.m_dbJoinDistance);

	// 绘图参数
    WriteIniDouble(szFileName, "SysCfg", "DrawEllipSet",g_SysCfg.m_dbDrawEllipSet);
    WriteIniInt(szFileName, "SysCfg", "DrawGridCOL",g_SysCfg.m_nGridCol);
    WriteIniInt(szFileName, "SysCfg", "DrawGridRow",g_SysCfg.m_nGridRow);

	WriteIniInt(szFileName, "SysCfg", "POLYGON",  g_SysCfg.m_nPolygon);
    WriteIniInt(szFileName, "SysCfg", "POLYNUM",  g_SysCfg.m_nPolyNum);
	WriteIniInt(szFileName, "SysCfg", "POLYCROSS",g_SysCfg.m_nPolyCross);
    WriteIniInt(szFileName, "SysCfg", "POLYSTAR", g_SysCfg.m_nPolyStar);
	
	// View缩放
	WriteIniDouble(szFileName, "SysCfg", "PreViewZoomScale", g_SysCfg.m_dbPreViewZoomScale);



	WriteIniInt(szFileName,"SysCfg", "dbProductSize",m_dbProductSize);
	

	return;
}

void CSystem::WriteLang()
{
	char szPath[256];
	GetExepath(szPath);
	char szFileName[256];
	sprintf(szFileName, "%s%s", szPath, "SystemSet.ini");	
	strcpy(m_pFileName, szFileName);

	WriteIniInt(szFileName, "LANG", "LANG", m_langID);
}

CString CSystem::GetProfileSting(char *pSec, char *pKey, LPCTSTR strDefault)
{
	char szRtn[256];
	::GetPrivateProfileString(pSec,pKey,strDefault,szRtn,256,m_pFileName);
	::WritePrivateProfileString(pSec,pKey,szRtn, m_pFileName);
	return CString(szRtn);
}

void CSystem::WriteProfileSting(char *pSec, char *pKey, LPCSTR strValue)
{
	::WritePrivateProfileString(pSec, pKey, strValue, m_pFileName);
}


int CSystem::GetProfileInt(char *pSec, char *pKey, int nDefault)
{
	char szRtn[256];
	char szSrc[256];
	sprintf(szSrc,"%d",nDefault);
	
	::GetPrivateProfileString(pSec,pKey,szSrc,szRtn,256,m_pFileName);
	::WritePrivateProfileString(pSec,pKey,szRtn, m_pFileName);
	return atoi(szRtn);
}

double CSystem::GetProfileDouble(char* pSec,char* pKey,double dbDefault)
{
	char szRtn[256];
	char szSrc[256];
	sprintf(szSrc,"%.3f",dbDefault);
	
	::GetPrivateProfileString(pSec,pKey,szSrc,szRtn,256,m_pFileName);
	::WritePrivateProfileString(pSec,pKey,szRtn, m_pFileName);
	return atof(szRtn);
}

void CSystem::WriteProfileInt(char* pSec,char* pKey, int nValue)
{
	  WriteIniInt(m_pFileName, pSec, pKey, nValue);
}

void CSystem::WriteProfileDouble(char* pSec,char* pKey, int dbValue)
{
	WriteIniDouble(m_pFileName, pSec, pKey, dbValue);
}

void CSystem::GetExepath(char *pPath)  //获取执行目录
{
	//之前通过装载动态库时来确定当前程序是否运行在安装目录下即可
	//找到自已的目录，即为安装目录。否则运行错误
	char pExePath[256];

	GetModuleFileName(NULL,pExePath,256); //If this parameter is NULL, GetModuleFileName returns the path for the file containing the current process. 
	char* pEnd = strrchr(pExePath,'\\');  //Scan a string for the last occurrence of a character.	
	*(pEnd+1) = NULL;

	if(pPath)
		strcpy(pPath,pExePath);
}

CString CSystem::GetInstallPath()
{
	GetExepath(m_szExePath);
	CString str("");
	str.Format("%s",m_szExePath);
	return str;
}

BOOL CSystem::Create()
{
	m_pCard = new CMarkCard;
	if(m_pCard){
		m_bSystemOk = m_pCard->LoadMarkLib();
		if (!m_bSystemOk)
		{
			AfxMessageBox(IDS_LOADMARKLIBFAIL);
		}
		else
		{
			m_bSystemOk = m_pCard->InitCard();
			if (!m_bSystemOk)
			{
				AfxMessageBox(IDS_INITCARDFAIL);
			}
		}
		SetDllDirectory(NULL);
	}
	return m_bSystemOk;
}

CMarkCard* CSystem::GetCardHandle()
{
	return m_pCard;
}

UINT CSystem::ReadSysPara()
{
	char szPath[256];
	GetExepath(szPath);
	char szFileName[256];
	sprintf(szFileName, "%s%s", szPath, "SystemSet.ini");	
	strcpy(m_pFileName, szFileName);

	//打标范围
	m_dbBoxX			= GetProfileDouble("HARDWARE","BOX_X",100);
	m_dbBoxY			= GetProfileDouble("HARDWARE","BOX_Y",100);
	
	m_nDibSampleMode	= GetProfileInt("Picture","Sample_Mode",1);
	if (m_nDibSampleMode>=3||m_nDibSampleMode<0)
	{
		m_nDibSampleMode=0;
	}

	m_nFullTaskCount = ReadIniInt(szFileName, "Advance", "FullTaskCount", "4");
	m_bEnableCardLog = ReadIniInt(szFileName, "Advance", "EnableCardLog", "0");
	m_langID = ReadIniInt(szFileName, "LANG", "LANG", "2052");

	char pszTemp[1024];
	GetPrivateProfileString("CARD", "CARD_NAME", "CARD_NULL", pszTemp, 1024, szFileName);
	m_strCardName = pszTemp;

//接口信号配置
	m_nTouchTimes	=ReadIniInt(szFileName, "TOUCH", "CYCLE","1");
	m_bMarkOnce	    =ReadIniInt(szFileName, "TOUCH", "B_MARK_ONCE","1");
	m_nDelayBefore	=ReadIniInt(szFileName, "TOUCH", "DELAY","0");
	m_nMarkRepeat	=ReadIniInt(szFileName, "TOUCH", "MARK_CNT","1");
	m_nDelayAfter	=ReadIniInt(szFileName, "TOUCH", "MARK_END_DELAY","0");
	m_nDelayBetween	=ReadIniInt(szFileName, "TOUCH", "DLY_BETWEEN_MARK","0");
	m_nDelayEnd	    =ReadIniInt(szFileName, "TOUCH", "MARK_END_PULSE_W","0");
	
	//CUSTOM
	m_nCCDType = GetProfileInt("HARDWARE", "CCD_TYPE", 1);
	m_nCCDPort = GetProfileInt("HARDWARE", "CCD_PORT", 1);
	m_nCCDTransport = GetProfileInt("CCD", "Transport", 2);
	m_strCCD_IP    =  GetProfileSting("CCD", "IP", "192.168.0.10");
	m_nCCD_IP_Port  = GetProfileInt("CCD", "IP_PORT", 8500); 
	m_nLighterPort = GetProfileInt("HARDWARE", "LIGHTER_PORT", 2);
	m_nLaserControllerPort = GetProfileInt("HARDWARE", "LASER_CONTROLLER_PROT", 3);
	m_nPowerMeterPort = GetProfileInt("HARDWARE", "POWER_METER_PORT", 4);
	m_nPowerMeterType = GetProfileInt("HARDWARE", "POWER_METER_TYPE", 0);
	m_nBeamExpanderPort = GetProfileInt("HARDWARE", "BEAM_EXPANDER_PORT", 5);
	m_bPLCOnline = GetProfileInt( _T("HARDWARE"), _T("PLC_ONLINE"), 0);
	m_bUseSafeDoor = GetProfileInt( _T("HARDWARE"), _T("USE_SAFE_DOOR"), 1);
	m_bDEV_3030B = GetProfileInt( _T("HARDWARE"), _T("DEV_3030B"), 0);

	//LASER
	m_nLaserCurrentMode = GetProfileInt("LASER", "CurrentMode", 0);
	m_nLaserCurrentMin = GetProfileInt("LASER", "CurrentMin", 0);
	m_nLaserCurrentMax = GetProfileInt("LASER", "CurrentMax", 45);
	m_nLaserCurrentKeep = GetProfileInt("LASER", "CurrentKeep", 0);
	m_nLaserLayDelay = GetProfileInt("LASER", "LayDelay", 1);
	
	m_nCCDNum = GetProfileInt("CCD", "CCDNum", 2);
	m_nCCD1PixelX = GetProfileInt("CCD", "CCD1PixelX", 2432);
	m_nCCD1PixelY = GetProfileInt("CCD", "CCD1PixelY", 2050);
	m_nCCD2PixelX = GetProfileInt("CCD", "CCD2PixelX", 1600);
	m_nCCD2PixelY = GetProfileInt("CCD", "CCD2PixelY", 1200);
	m_bHandPosOnCCDFail = GetProfileInt("CCD", "HandPosOnCCDFail", 0);

	m_dbProductSize = GetProfileInt("SysCfg", "dbProductSize",600);


	//OPCLaser
	m_bUseOpcLaser = GetProfileInt( _T("OPCLaser"), _T("bUseOpcLaser"), 0);
	m_nWriteDelay = GetProfileInt( _T("OPCLaser"), _T("OPCWriteFreqDelay"), 30);
	TCHAR szOPCIP[20]={0};
	DWORD dwRet = ::GetPrivateProfileString(_T("OPCLaser"), _T("OPCIP"), _T("192.0.2.1"), szOPCIP, 20, szFileName);
	if ( dwRet > 0 ) _tcsncpy(m_szOPCIP, szOPCIP, 20);


	m_nLogLevel = GetProfileInt( _T("RunLog"), _T("LogLevel"), eLogTime);

	m_dbWorkbenchMaxPosErr = GetProfileDouble("Workbench","MaxPosErr",0.010);
	WriteIniDouble(m_pFileName, "Workbench","MaxPosErr",m_dbWorkbenchMaxPosErr);

	return 0;
}

void CSystem::WriteRightInfo(CString &strFileName)
{
	m_RightMng.WriteRightInfo(g_RightInfoList, strFileName);
	return;
}

void CSystem::WriteSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFileName)
{
	m_RightMng.WriteSpMarkInfo(objMarkSet, strFileName);
	return;
}

void CSystem::ReadRightInfo(CString &strFileName)
{
	m_RightMng.ReadRightInfo(g_RightInfoList, strFileName);
	return;
}

void CSystem::ReadSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFileName)
{
	m_RightMng.ReadSpMarkInfo(objMarkSet, strFileName);
	return;
}

BOOL CSystem::IsAvailUser(CString &strUserName, CString &strPassWord)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUserInfo = g_RightInfoList.GetAt(i);
		if (   0 == strUserName.CompareNoCase(stUserInfo.szUserName)
			&& 0 == strPassWord.CompareNoCase(stUserInfo.szPassWord)
		   )
		{
			g_CurUserInfo = stUserInfo;

			return TRUE;
		}
	}
	
	return FALSE;
}


BOOL CSystem::AddUser(USERINFO &stUserInfo)
{
	BOOL bFindUser = FALSE;
	
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strncmp(stUserInfo.szUserName, stUInfo.szUserName, 64))
		{
			bFindUser = TRUE;
			break;
		}
	}
	
	if (bFindUser) 
	{
		AfxMessageBox(IDS_USERISINLIST);
		return FALSE;
	}
	else
	{
		g_RightInfoList.Add(stUserInfo);
		return TRUE;
	}

	return FALSE;
}

BOOL CSystem::DelUser(CString &strUserName)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strUserName.CompareNoCase(stUInfo.szUserName))
		{
			g_RightInfoList.RemoveAt(i);
			break;
		}
	}
	
	return TRUE;
}

void CSystem::EditUserInfo(USERINFO &stUserInfo)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strncmp(stUserInfo.szUserName, stUInfo.szUserName, 64))
		{
			strncpy(g_RightInfoList[i].szUserName, stUserInfo.szUserName, 64);
			strncpy(g_RightInfoList[i].szPassWord, stUserInfo.szPassWord, 64);
			g_RightInfoList[i].bIsDefaultUser = stUserInfo.bIsDefaultUser;
			g_RightInfoList[i].dwRight1 = stUserInfo.dwRight1;
			g_RightInfoList[i].dwRight2 = stUserInfo.dwRight2;
			break;
		}
	}
	
	return;
}

void CSystem::EditUserInfoByUserName(USERINFO &stUserInfo, CString &strUserName)
{
	BOOL bFindUser = FALSE;
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strncmp(stUInfo.szUserName, strUserName, 64))
		{
			strncpy(g_RightInfoList[i].szUserName, stUserInfo.szUserName, 64);
			strncpy(g_RightInfoList[i].szPassWord, stUserInfo.szPassWord, 64);
			g_RightInfoList[i].bIsDefaultUser = stUserInfo.bIsDefaultUser;
			g_RightInfoList[i].dwRight1 = stUserInfo.dwRight1;
			g_RightInfoList[i].dwRight2 = stUserInfo.dwRight2;
			break;
		}
	}
	
	return;
}

BOOL CSystem::OnSetDefautUser(CString &strUserName, BOOL bSel)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strUserName.CompareNoCase(stUInfo.szUserName)) 
		{
			g_RightInfoList[i].bIsDefaultUser = bSel;
		}
		else
		{
			g_RightInfoList[i].bIsDefaultUser = FALSE;
		}
	}

	if (0 == strUserName.CompareNoCase(g_CurUserInfo.szUserName)) 
	{
		g_CurUserInfo.bIsDefaultUser = bSel;
	}
	else
	{
		g_CurUserInfo.bIsDefaultUser = FALSE;
	}
	
	return FALSE;
}

BOOL CSystem::GetUserInfoByName(CString strUserName, USERINFO &stUserInfo)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strUserName.CompareNoCase(stUInfo.szUserName))
		{
			stUserInfo = stUInfo;
			return TRUE;
		}
	}
	
	return FALSE;
}

int CSystem::GetIndexByName(CString strUserName)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strUserName.CompareNoCase(stUInfo.szUserName))
		{
			return i;
		}
	}
	
	return 0;
}

BOOL CSystem::GetDefaultUserInfo(USERINFO &stUserInfo)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (stUInfo.bIsDefaultUser)
		{
			stUserInfo = stUInfo;
			return TRUE;
		}
	}
	
	return FALSE;
}