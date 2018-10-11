#include "stdafx.h"
#include "MarkCard.h"


CMarkCard::CMarkCard(void)
{
	m_hModule = NULL;
	m_pInitCard = NULL;
	m_pGetCardSts = NULL;
	m_pGetCardError = NULL;
	m_pRegEvent = NULL;
	m_pUnRegEvent = NULL;
	m_pGetHdVer = NULL;
	m_pGetSwVer = NULL;
	m_pSetCaliParaFile = NULL;
	m_pSetCardParaFile = NULL;
	m_pSetMarkPara = NULL;
	m_pEnterMarkEnv = NULL;
	m_pLeaveMarkEnv = NULL;
	m_pMarkStroke = NULL;
	m_pMarkStart = NULL;
	m_pMarkEnd = NULL;
	m_pTaskStart = NULL;
	m_pTaskEnd = NULL;
	m_pGetMarkTime = NULL;
	m_pSetCurrent = NULL;
	m_pInput = NULL;
	m_pOutput = NULL;
	m_pGetOutput = NULL;
	m_pSetGrayCurrent = NULL;
	m_pGetMarkRange = NULL;
	m_pGetParaInfo = NULL;
	m_pMarkCmdStart = NULL;
	m_pMarkCmdPause = NULL;
	m_pMarkCmdRemark = NULL;
	m_pMarkCmdStop = NULL;
	m_pGetErrInfo = NULL;
	m_pExitCard = NULL;
	m_pGoTo = NULL;
	m_pAutoCal = NULL;
	m_SetAutoCalRefPos = NULL;
	m_pBeamOn  = NULL;
	m_pBeamOff = NULL;
	m_pSetLayerChangeCallback = NULL;

	m_bTouchFrist=TRUE;
	m_nTouchCount=0;

	m_nMarkLayNum = -1;
	m_bIsResMark  = FALSE;

	for (int i=0; i<4; i++)
	{
		memset(&m_paraInfo[i], 0, sizeof(PARA));
	}

	m_evtSet.hReadyEvt = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_evtSet.hTaskStartEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_evtSet.hTaskEndEvt = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hCheckCardEvtContinue = CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hCheckCardEvtExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_pCheckCardThread = NULL;

	m_dbFixZ = NOT_FIX_Z;
}

CMarkCard::~CMarkCard(void)
{
	if (m_pCheckCardThread != NULL)
	{
		ResetEvent(m_hCheckCardEvtContinue);
		SetEvent(m_hCheckCardEvtExit);
		WaitForSingleObject(m_pCheckCardThread->m_hThread, INFINITE);
	}
	CloseHandle(m_evtSet.hReadyEvt);
	CloseHandle(m_evtSet.hTaskStartEvt);
	CloseHandle(m_evtSet.hTaskEndEvt);
	CloseHandle(m_hCheckCardEvtContinue);
	CloseHandle(m_hCheckCardEvtExit);
}

//重复打标时，中间状态处理
void CMarkCard::Between(HANDLE hStop)
{
	//	Sleep(g_sys.m_nDelayBetween);
	DWORD dwStart = timeGetTime();
	for (int i = 0; i < (int)(g_sys.m_nDelayBetween); i++)
	{
		if(::WaitForSingleObject(hStop,0) == WAIT_OBJECT_0)
		{
			break;
		}

		DWORD dwEnd = timeGetTime();
		if ((int)(fabs((double)dwEnd - dwStart)) >= (int)(g_sys.m_nDelayBetween))
		{
			break;
		}

		Sleep(1);
	}

	return;
}

BOOL CMarkCard::Touch()
{
	UINT nInput = 0;
	if(GetInput(nInput))
	{
		GetInputStr(m_strMulSel);
		BOOL result=(nInput>>12)&0x1;
		if(g_sys.m_bMarkOnce)
		{
			//保证在一个对象上只触发一次
			if(m_bTouchFrist==TRUE)
			{
				if(result==TRUE)
				{				
					m_nTouchCount++;
					m_bTouchFrist=FALSE;
					if(m_nTouchCount>=g_sys.m_nTouchTimes)
					{
						m_nTouchCount=0;
						return TRUE;
					}
					else
						return FALSE;
				}
				else
					return FALSE;
			}
			else
			{
				if(result==FALSE) m_bTouchFrist=TRUE;	
				return FALSE;
			}
		}
		else
			return(result);

	}
	return FALSE;
}

BOOL CMarkCard::GetInputStr(CString &str)
{
	UINT v = 0; 
	if(!GetInput(v))
		return FALSE;

	///////////////////////
	str.Format("%d%d%d%d%d",(v&I_MULSEL_4)?1:0,(v&I_MULSEL_3)?1:0,\
		(v&I_MULSEL_2)?1:0,(v&I_MULSEL_1)?1:0,(v&I_MULSEL_0)?1:0);

	return TRUE;
}

UINT CMarkCard::GetMarkCount(int nLayIndex)
{
	return UINT(m_lay.GetLayer(nLayIndex).m_para.dbCount);
}

CLay *CMarkCard::GetCurLay()
{
	return &m_lay;
}

void CMarkCard::SetMarkLayNum(int nLayNum, BOOL bIsResMark)
{
	m_nMarkLayNum = nLayNum;
	m_bIsResMark  = bIsResMark;
}

BOOL CMarkCard::LoadMarkLib(void)
{
	char pszPath[1024];
	GetExepath(pszPath);
	CString strMarkLib;
	strMarkLib.Format("%s%s", pszPath, g_sys.m_strCardName);
	SetDllDirectory(strMarkLib);

	strMarkLib.Format("%s%s\\device.dll", pszPath, g_sys.m_strCardName);
	m_hModule = LoadLibrary(strMarkLib);
	int nErr = GetLastError();


	if (m_hModule == NULL)
		return FALSE;

	m_pInitCard = (HS_INITCARD)GetProcAddress(m_hModule, (LPSTR)1);
	m_pGetCardSts = (HS_GETCARDSTS)GetProcAddress(m_hModule, (LPSTR)2);
	m_pGetCardError = (HS_GETCARDERROR)GetProcAddress(m_hModule, (LPSTR)3);
	m_pRegEvent = (HS_REGEVENT)GetProcAddress(m_hModule, (LPSTR)4);
	m_pUnRegEvent = (HS_UNREGEVENT)GetProcAddress(m_hModule, (LPSTR)5);
	m_pGetHdVer = (HS_GETHDVER)GetProcAddress(m_hModule, (LPSTR)6);
	m_pGetSwVer = (HS_GETSWVER)GetProcAddress(m_hModule, (LPSTR)7);
	m_pSetCaliParaFile = (HS_SETCALIPARAFILE)GetProcAddress(m_hModule, (LPSTR)8);
	m_pSetCardParaFile = (HS_SETCARDPARAFILE)GetProcAddress(m_hModule, (LPSTR)9);
	m_pSetMarkPara = (HS_SETMARKPARA)GetProcAddress(m_hModule, (LPSTR)10);
	m_pEnterMarkEnv = (HS_ENTERMARKENV)GetProcAddress(m_hModule, (LPSTR)11);
	m_pLeaveMarkEnv = (HS_LEAVEMARKENV)GetProcAddress(m_hModule, (LPSTR)12);
	m_pMarkStroke = (HS_MARKSTROKE)GetProcAddress(m_hModule, (LPSTR)13);
	m_pMarkStart = (HS_MARKSTART)GetProcAddress(m_hModule, (LPSTR)14);
	m_pMarkEnd = (HS_MARKEND)GetProcAddress(m_hModule, (LPSTR)15);
	m_pTaskStart = (HS_TASKSTART)GetProcAddress(m_hModule, (LPSTR)16);
	m_pTaskEnd = (HS_TASKEND)GetProcAddress(m_hModule, (LPSTR)17);
	m_pGetMarkTime = (HS_GETMARKTIME)GetProcAddress(m_hModule, (LPSTR)18);
	m_pSetCurrent = (HS_SETCURRENT)GetProcAddress(m_hModule, (LPSTR)19);
	m_pInput = (HS_INPUT)GetProcAddress(m_hModule, (LPSTR)20);
	m_pOutput = (HS_OUTPUT)GetProcAddress(m_hModule, (LPSTR)21);
	m_pGetOutput = (HS_GETOUTPUT)GetProcAddress(m_hModule, (LPSTR)22);
	m_pSetGrayCurrent = (HS_SETGRAYCURRENT)GetProcAddress(m_hModule, (LPSTR)23);
	m_pGetMarkRange = (HS_GETMARKRANGE)GetProcAddress(m_hModule, (LPSTR)24);
	m_pGetParaInfo = (HS_GETPARAINFO)GetProcAddress(m_hModule, (LPSTR)25);
	m_pMarkCmdStart = (HS_MARKCMDSTART)GetProcAddress(m_hModule, (LPSTR)26);
	m_pMarkCmdPause = (HS_MARKCMDPAUSE)GetProcAddress(m_hModule, (LPSTR)27);
	m_pMarkCmdRemark = (HS_MARKCMDREMARK)GetProcAddress(m_hModule, (LPSTR)28);
	m_pMarkCmdStop = (HS_MARKCMDSTOP)GetProcAddress(m_hModule, (LPSTR)29);
	m_pGetErrInfo = (HS_GETERRINFO)GetProcAddress(m_hModule, (LPSTR)30);
	m_pExitCard = (HS_EXITCARD)GetProcAddress(m_hModule, (LPSTR)31);
	m_pGoTo = (HS_GOTO)GetProcAddress(m_hModule, (LPSTR)32);
	m_pAutoCal = (HS_AUTOCAL)GetProcAddress(m_hModule, (LPSTR)33);
	m_SetAutoCalRefPos = (HS_SetAutoCalRefPos)GetProcAddress(m_hModule,(LPSTR)34);
	m_pBeamOn = (HS_BeamOn)GetProcAddress(m_hModule,(LPSTR)35);
	m_pBeamOff = (HS_BeamOff)GetProcAddress(m_hModule,(LPSTR)36);
	m_pSetLayerChangeCallback = (HS_SET_LAYER_CHANGE_CALLBACK)GetProcAddress(m_hModule, (LPSTR)37);

	return m_pInitCard&&m_pGetCardSts&&m_pGetCardError&&m_pRegEvent&&m_pUnRegEvent&&m_pGetHdVer&&
		m_pGetSwVer&&m_pSetCaliParaFile&&m_pSetCardParaFile&&m_pTaskStart&&m_pTaskEnd&&m_pGetOutput&&
		m_pSetMarkPara&&m_pEnterMarkEnv&&m_pLeaveMarkEnv&&m_pMarkStroke&&m_pMarkStart&&
		m_pMarkEnd&&m_pGetMarkTime&&m_pSetCurrent&&m_pInput&&m_pOutput&&m_pSetGrayCurrent&&
		m_pGetMarkRange&&m_pGetParaInfo&&m_pMarkCmdStart&&m_pMarkCmdPause&&m_pMarkCmdRemark&&
		m_pMarkCmdStop&&m_pGetErrInfo&&m_pExitCard&&m_pGoTo&&m_pAutoCal &&m_SetAutoCalRefPos && m_pBeamOn && m_pBeamOff && m_pSetLayerChangeCallback;
}

void CMarkCard::FreeMarkLib(void)
{
	if(m_hModule)
	{
		FreeLibrary(m_hModule);
	}
	m_hModule = NULL;
	m_pInitCard = NULL;
	m_pGetCardSts = NULL;
	m_pGetCardError = NULL;
	m_pRegEvent = NULL;
	m_pUnRegEvent = NULL;
	m_pGetHdVer = NULL;
	m_pGetSwVer = NULL;
	m_pSetCaliParaFile = NULL;
	m_pSetCardParaFile = NULL;
	m_pSetMarkPara = NULL;
	m_pEnterMarkEnv = NULL;
	m_pLeaveMarkEnv = NULL;
	m_pMarkStroke = NULL;
	m_pMarkStart = NULL;
	m_pMarkEnd = NULL;
	m_pTaskStart = NULL;
	m_pTaskEnd = NULL;
	m_pGetMarkTime = NULL;
	m_pSetCurrent = NULL;
	m_pInput = NULL;
	m_pOutput = NULL;
	m_pGetOutput = NULL;
	m_pSetGrayCurrent = NULL;
	m_pGetMarkRange = NULL;
	m_pGetParaInfo = NULL;
	m_pMarkCmdStart = NULL;
	m_pMarkCmdPause = NULL;
	m_pMarkCmdRemark = NULL;
	m_pMarkCmdStop = NULL;
	m_pGetErrInfo = NULL;
	m_pExitCard = NULL;
	m_pGoTo = NULL;
	m_pAutoCal = NULL;
	m_SetAutoCalRefPos = NULL;
	m_pBeamOn = NULL;
	m_pBeamOff = NULL;
	m_pSetLayerChangeCallback = NULL;
}

BOOL CMarkCard::GetCardSts(UINT &nSts)
{
	if (m_pGetCardSts != NULL)
	{
		UINT nRet = m_pGetCardSts(&nSts);
		if (CheckReutrnValue(nRet, "GetCardSts"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::GetCardError(UINT &nErrCode)
{
	if (m_pGetCardError != NULL)
	{
		UINT nRet = m_pGetCardError(&nErrCode);
		if (CheckReutrnValue(nRet, "GetCardError"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::RegEvent()
{
	if (m_pRegEvent != NULL)
	{
		UINT nRet = m_pRegEvent(m_evtSet);
		if (CheckReutrnValue(nRet, "RegEvent"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::UnRegEvent()
{
	if (m_pUnRegEvent != NULL)
	{
		UINT nRet = m_pUnRegEvent();
		if (CheckReutrnValue(nRet, "UnRegEvent"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::TaskStart(UINT nTaskIndex)
{
	if (m_pTaskStart != NULL)
	{
		UINT nRet = m_pTaskStart(nTaskIndex);
		if (CheckReutrnValue(nRet, "TaskStart"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::TaskEnd(UINT nTaskIndex)
{
	if (m_pTaskEnd != NULL)
	{
		UINT nRet = m_pTaskEnd(nTaskIndex);
		if (CheckReutrnValue(nRet, "TaskEnd"))
		{
			return TRUE;
		}
	}
	return FALSE;
}

//主版本号,次版本号，定制版本号，次次版本号
void CMarkCard::GetHdVer(UINT &nMainVer, UINT &nSubVer, UINT &nSSubVer, UINT &nCustomVer)
{
	if (m_pGetHdVer != NULL)
	{
		UINT dwVer;
		UINT nRet = m_pGetHdVer(&dwVer);
		if (CheckReutrnValue(nRet, "GetHdVer"))
		{
			nMainVer = (dwVer&0xFF000000)>>24;
			nSubVer = (dwVer&0x00FF0000)>>16;
			nSSubVer = (dwVer&0x0000FF00)>>8;
			nCustomVer = (dwVer&0x000000FF);
		}
	}
}
void CMarkCard::GetSwVer(UINT &nMainVer, UINT &nSubVer, UINT &nSSubVer, UINT &nCustomVer)
{
	if (m_pGetSwVer != NULL)
	{
		UINT dwVer;
		UINT nRet = m_pGetSwVer(&dwVer);
		if (CheckReutrnValue(nRet, "GetSwVer"))
		{
			nMainVer = (dwVer&0xFF000000)>>24;
			nSubVer = (dwVer&0x00FF0000)>>16;
			nSSubVer = (dwVer&0x0000FF00)>>8;
			nCustomVer = (dwVer&0x000000FF);
		}
	}
}
BOOL CMarkCard::InitCard()
{
	if (m_pInitCard != NULL)
	{
		UINT nRet =m_pInitCard();
		if (CheckReutrnValue(nRet, "InitCard"))
		{
			TCHAR pszTemp[512];
			GetExepath(pszTemp);
			CString strCaliFile,strCardFile;
			strCaliFile.Format("%s%s\\%s", pszTemp, g_sys.m_strCardName, c_califile);
			strCardFile.Format("%s%s\\%s", pszTemp, g_sys.m_strCardName, c_cardfile);
			SetCardParaFile((const char *)strCardFile);
			SetCaliParaFile((const char *)strCaliFile);
			UINT nRetParaInfo = m_pGetParaInfo(m_paraInfo, 4);
			CheckReutrnValue(nRetParaInfo, "GetParaInfo");
			RegEvent();
//			m_pCheckCardThread = AfxBeginThread(CheckCardProc, this);
			return TRUE;
		}
	}
	return FALSE;
}
void CMarkCard::ExitCard()
{
	if (m_pExitCard != NULL)
	{
		UnRegEvent();
		UINT nRet = m_pExitCard();
		CheckReutrnValue(nRet, "ExitCard");
	}
}
BOOL CMarkCard::EnterMarkEnv(UINT nFlag)
{
	if (m_pEnterMarkEnv != NULL)
	{
		UINT nRet = m_pEnterMarkEnv(nFlag);
		if (CheckReutrnValue(nRet, "EnterMarkEnv"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
void CMarkCard::LeaveMarkEnv()
{
	if (m_pLeaveMarkEnv != NULL)
	{
		UINT nRet = m_pLeaveMarkEnv();
		CheckReutrnValue(nRet, "LeaveMarkEnv");
	}
}
BOOL CMarkCard::MarkStart(UINT nType, UINT nSts)
{
	if (m_pMarkStart != NULL)
	{
		ResetEvent(m_hCheckCardEvtContinue);
		HANDLE hMod = GetCurrentThread();
		SetThreadPriority(hMod,THREAD_PRIORITY_ABOVE_NORMAL);
		UINT nRet = m_pMarkStart(nType, nSts);
		if (CheckReutrnValue(nRet, "MarkStart"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
void CMarkCard::MarkEnd()
{
	if (m_pMarkEnd != NULL)
	{
		UINT nRet = m_pMarkEnd();
		CheckReutrnValue(nRet, "MarkEnd");
		SetEvent(m_hCheckCardEvtContinue);
	}
}

BOOL CMarkCard::MarkStroke(const CStroke &stroke)
{
	if (m_pMarkStroke != NULL)
	{
		if (stroke.m_dotArray.IsEmpty())
		{
			if (stroke.m_nCountDealy)
			{
				DOT6 dot;
				dot.x = stroke.m_nCountDealy;
				dot.y = dot.z = 0;
				m_pMarkStroke(1, &dot, 1, stroke.m_property.m_nLayer);
				return TRUE;
			}
			return TRUE;
		}

		int nCount = stroke.m_dotArray.GetSize();
		DOT6 *pDotArray = new DOT6[nCount];
		memset(pDotArray, 0, nCount*sizeof(DOT6));
		if(m_bIsResMark)
		{
			for (int i=nCount-1; i>=0; i--)
			{
				
				pDotArray[nCount-1-i].x = static_cast<float>(stroke.m_dotArray[i].x);
				pDotArray[nCount-1-i].y = static_cast<float>(stroke.m_dotArray[i].y);
				pDotArray[nCount-1-i].z = static_cast<float>((m_dbFixZ==NOT_FIX_Z) ? stroke.m_dotArray[i].z : m_dbFixZ);
			}
		}
		else
		{
			for (int i=0; i<nCount; i++)
			{
				pDotArray[i].x = static_cast<float>(stroke.m_dotArray[i].x);
				pDotArray[i].y = static_cast<float>(stroke.m_dotArray[i].y);
				pDotArray[i].z = static_cast<float>((m_dbFixZ==NOT_FIX_Z) ? stroke.m_dotArray[i].z : m_dbFixZ);
			}
		}
		int nLay = stroke.m_property.m_nLayer;
		if (m_nMarkLayNum >= 0)
		{
			if (g_SysCfg.m_nLayerCnt < m_nMarkLayNum) 
			{
				m_nMarkLayNum = g_SysCfg.m_nLayerCnt;
			}
			nLay = m_nMarkLayNum;
		}
		else
		{
			if (g_SysCfg.m_nLayerCnt < nLay)
			{
				nLay = g_SysCfg.m_nLayerCnt;
			}
		}
		UINT nRet = m_pMarkStroke(0, pDotArray, nCount, nLay);
		delete[] pDotArray;
		if (CheckReutrnValue(nRet, "MarkStroke"))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMarkCard::MarkStroke(const CStroke &stroke, int nLay, BOOL bIsResMark)
{
	if (m_pMarkStroke != NULL)
	{
		int nCount = stroke.m_dotArray.GetSize();
		DOT6 *pDotArray = new DOT6[nCount];
		memset(pDotArray, 0, nCount*sizeof(DOT6));
		if(bIsResMark)
		{
			for (int i=nCount-1; i>=0; i--)
			{
				pDotArray[nCount-1-i].x = static_cast<float>(stroke.m_dotArray[i].x);
				pDotArray[nCount-1-i].y = static_cast<float>(stroke.m_dotArray[i].y);
				pDotArray[nCount-1-i].z = static_cast<float>((m_dbFixZ==NOT_FIX_Z) ? stroke.m_dotArray[i].z : m_dbFixZ);
			}
		}
		else
		{
			for (int i=0; i<nCount; i++)
			{
				pDotArray[i].x = static_cast<float>(stroke.m_dotArray[i].x);
				pDotArray[i].y = static_cast<float>(stroke.m_dotArray[i].y);
				pDotArray[i].z = static_cast<float>((m_dbFixZ==NOT_FIX_Z) ? stroke.m_dotArray[i].z : m_dbFixZ);
			}
		}
		if (g_SysCfg.m_nLayerCnt < nLay) 
		{
			nLay = g_SysCfg.m_nLayerCnt;
		}
		UINT nRet = m_pMarkStroke(0, pDotArray, nCount, nLay);
		delete[] pDotArray;
		if (CheckReutrnValue(nRet, "MarkStroke"))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMarkCard::MarkStroke(UINT nType, LPVOID pInfo, int nNum, UINT nLayIndex)
{
	if(m_pMarkStroke != NULL)
	{
		int nLay = nLayIndex;
		if (g_SysCfg.m_nLayerCnt < nLay) 
		{
			nLay = g_SysCfg.m_nLayerCnt;
		}
		UINT nRet = m_pMarkStroke(nType, pInfo, nNum, nLay);
		if (CheckReutrnValue(nRet, "MarkStroke"))
		{
			return TRUE;
		}
	}
	return FALSE;
}

//返回ms数
DWORD CMarkCard::GetMarkTime()
{
	if (m_pGetMarkTime != NULL)
	{
		UINT us = 1;
		UINT nRet = m_pGetMarkTime(&us);
		if (CheckReutrnValue(nRet, "GetMarkTime"))
		{
			return us;
		}
	}
	return 0;
}
BOOL CMarkCard::SetCurrent(double dbCurrent)
{
	if (m_pSetCurrent != NULL)
	{
		UINT nRet = m_pSetCurrent(dbCurrent);
		if (CheckReutrnValue(nRet, "SetCurrent"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::GetInput(UINT &nInput)
{
	if (m_pInput != NULL)
	{
		UINT nRet = m_pInput(&nInput);
		if (CheckReutrnValue(nRet, "GetInput"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::SetOutput(UINT dwOutput, UINT nMask)
{
	if(m_pOutput != NULL)
	{
		UINT nRet = m_pOutput(dwOutput, nMask);
		if (CheckReutrnValue(nRet, "SetOutput"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::GetOutput(UINT &dwOutput)
{
	if(m_pOutput != NULL)
	{
		UINT nRet = m_pGetOutput(&dwOutput);
		if (CheckReutrnValue(nRet, "GetOutput"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::GetMarkRange(double &dbX, double &dbY)
{
	if (m_pGetMarkRange != NULL)
	{
		UINT nRet = m_pGetMarkRange(&dbX, &dbY);
	}
	else
	{
		dbX=100;
		dbY=100;
	}
	return TRUE;
}
BOOL CMarkCard::MarkCmdStart()
{
	if (m_pMarkCmdStart != NULL)
	{
		UINT nRet = m_pMarkCmdStart();
		if (CheckReutrnValue(nRet, "MarkCmdStart"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::MarkCmdPause()
{
	if (m_pMarkCmdPause != NULL)
	{
		UINT nRet = m_pMarkCmdPause();
		if (CheckReutrnValue(nRet, "MarkCmdPause"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::MarkCmdRemark()
{
	if (m_pMarkCmdRemark != NULL)
	{
		UINT nRet = m_pMarkCmdRemark();
		if (CheckReutrnValue(nRet, "MarkCmdRemark"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::MarkCmdStop()
{
	if (m_pMarkCmdStop != NULL)
	{
		UINT nRet = m_pMarkCmdStop();
		if (CheckReutrnValue(nRet, "MarkCmdStop"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
void CMarkCard::SetCaliParaFile(const char *pszFile)
{
	if (m_pSetCaliParaFile != NULL)
	{
		UINT nRet = m_pSetCaliParaFile(pszFile);
		CheckReutrnValue(nRet, "SetCaliParaFile");
	}
}
void CMarkCard::SetCardParaFile(const char *pszFile)
{
	if (m_pSetCardParaFile != NULL)
	{
		UINT nRet = m_pSetCardParaFile(pszFile);
		CheckReutrnValue(nRet, "SetCardParaFile");
	}
}

BOOL CMarkCard::SetMarkPara(CLay &lay)
{
	m_lay = lay;
	if (m_pSetMarkPara != NULL)
	{
		int nCount = lay.GetSize();
		PARA* pPara = new PARA[nCount];
		for(int i=0;i<nCount;i++)
			pPara[i] = lay.GetLayer(i).m_para;

		UINT nRet = m_pSetMarkPara(pPara, nCount);
		delete[] pPara;
		if (CheckReutrnValue(nRet, "SetMarkPara"))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMarkCard::SetGrayCurrent(double *pdbList, int nNum)
{
	if (m_pSetGrayCurrent != NULL)
	{
		UINT nRet = m_pSetGrayCurrent(pdbList, nNum);
		if (CheckReutrnValue(nRet, "SetGrayCurrent"))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMarkCard::GetParaInfo(LPVOID pInfo, int nSize)
{
	if (m_pGetParaInfo != NULL)
	{
		UINT nRet = m_pGetParaInfo(pInfo, nSize);
		if (CheckReutrnValue(nRet, "GetParaInfo"))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMarkCard::CheckReutrnValue(UINT nRet, const char *pFunc)
{
	char pszInfo[512];
	if (LOWORD(nRet)!=0)
	{
		if (m_pGetErrInfo != NULL)
		{
			m_pGetErrInfo(nRet, pszInfo, 512);
			CTime time = CTime::GetCurrentTime();
			CString strLine;
			strLine.Format("%d:%d:%d error %s return %08X:%s\r\n", 
				time.GetHour(),
				time.GetMinute(),
				time.GetSecond(),
				pFunc, 
				nRet,
				pszInfo);
			this->Log(strLine);
		}
		return FALSE;
	}
	if (HIWORD(nRet)!=0)
	{
		if (m_pGetErrInfo != NULL)
		{
			m_pGetErrInfo(nRet, pszInfo, 512);
			CTime time = CTime::GetCurrentTime();
			CString strLine;
			strLine.Format("%d:%d:%d warning %s return %08X:%s\r\n", 
				time.GetHour(),
				time.GetMinute(),
				time.GetSecond(),
				pFunc, 
				nRet,
				pszInfo);
			this->Log(strLine);
		}
	}
	return TRUE;
}
BOOL CMarkCard::OpenLogFile()
{
	if (!g_sys.m_bEnableCardLog)
	{
		return FALSE;
	}
	char pszTemp[1024];
	GetExepath(pszTemp);
	CString strFile;
	CTime time = CTime::GetCurrentTime();
	strFile.Format("%slog\\%d-%d-%d.txt", pszTemp, time.GetYear(), time.GetMonth(), time.GetDay());
	if (m_logFile.Open(strFile, CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		return TRUE;
	}
	return FALSE;
}
void CMarkCard::CloseLogFile()
{
	if (m_logFile.m_hFile != CFile::hFileNull)
	{
		m_logFile.Close();
	}
}
void CMarkCard::Log(const CString &strLog)
{
	if (!g_sys.m_bEnableCardLog)
	{
		return;
	}
	m_logFile.WriteString(strLog);
}

UINT CMarkCard::CheckCardProc(LPVOID lpContext)
{
	CMarkCard *pCard = (CMarkCard*)lpContext;
	HANDLE handels[2];
	handels[0] = pCard->m_hCheckCardEvtContinue;
	handels[1] = pCard->m_hCheckCardEvtExit;
	while(TRUE)
	{
		DWORD dwRet = WaitForMultipleObjects(2, handels, FALSE, INFINITE);
		switch(dwRet)
		{
		case WAIT_OBJECT_0:
			{
				Sleep(1000);
				UINT nCode = 0;
				pCard->GetCardError(nCode);
				if (!pCard->CheckReutrnValue(nCode, "GetCardError"))
				{
					TCHAR pszTemp[1024];
					pCard->m_pGetErrInfo(nCode, pszTemp, 1024);
					AfxMessageBox(pszTemp);
					AfxGetMainWnd()->PostMessage(WM_QUIT, 0, 0);
				}
			}
			break;
		case WAIT_OBJECT_0+1:
			{
				AfxEndThread(0);
			}
			break;
		}
	}
	return 0;
}

BOOL CMarkCard::BuffTime(UINT nTime)
{
	if (m_pMarkStroke != NULL)
	{
		UINT nRet = m_pMarkStroke(3, &nTime, 1, 0);
		if (CheckReutrnValue(nRet, "BuffTime"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::BuffOutput(UINT nValue, UINT nMask)
{
	if (m_pMarkStroke != NULL)
	{
		UINT buff[2];
		buff[0] = nValue;
		buff[1] = nMask;
		UINT nRet = m_pMarkStroke(4, buff, 2, 0);
		if (CheckReutrnValue(nRet, "BuffOutput"))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMarkCard::BuffInput(UINT nValue, UINT nMask)
{
	if (m_pMarkStroke != NULL)
	{
		UINT buff[2];
		buff[0] = nValue;
		buff[1] = nMask;
		UINT nRet = m_pMarkStroke(5, buff, 2, 0);
		if (CheckReutrnValue(nRet, "BuffInput"))
		{
			return TRUE;
		}
	}
	return FALSE;
}	

void CMarkCard::GoTo(double dbX, double dbY, double dbZ)
{
	if (m_pGoTo != NULL)
	{
		UINT nRet = m_pGoTo(dbX, dbY, dbZ);
		if (CheckReutrnValue(nRet, "BuffInput"))
		{
			return;
		}
	}
}

void CMarkCard::AutoCal()
{
	if (m_pAutoCal != NULL)
	{
		UINT nRet = m_pAutoCal();
	}
}

void CMarkCard::SetAutoCalRefPos()
{
	if (m_SetAutoCalRefPos != NULL)
	{
		UINT nRet = m_SetAutoCalRefPos();
	}
}

void CMarkCard::BeamOn()
{
	if (m_pBeamOn != NULL)
	{
		UINT nRet = m_pBeamOn();
		if (CheckReutrnValue(nRet, "BeamOn"))
			return;
	}
}

void CMarkCard::BeamOff()
{
	if (m_pBeamOff != NULL)
	{
		UINT nRet = m_pBeamOff();
		if (CheckReutrnValue(nRet, "BeamOff"))
			return;
	}
}

void CMarkCard::SetLayerChangeCallback(HS_LAYER_CHANGE_CALLBACK pCallback)
{
	if (m_pSetLayerChangeCallback != NULL)
	{
		UINT nRet = m_pSetLayerChangeCallback(pCallback);
		if (CheckReutrnValue(nRet, "SetLayerChangeCallback"))
			return;
	}
}