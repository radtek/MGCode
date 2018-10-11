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
	case FRC_SUCCESS://���سɹ�
		return TRUE;
	case FRC_SUCESS_WITH_USERID://�����ŷ��سɹ�
		return TRUE;
	case FRC_SUCCESS_WITH_TIMESHOW://�ɹ�����ʱʹ�ã�������ʹ��ʣ��ʱ��
		//�����Ȩ����������%d�죬��Ҫ��ϵ����ͷ���Ա������������(��ID=%s, У����=%s)ƥ��������Ȩ�������Ա㵽�ں��ܼ���ʹ��
	case FRC_SUCCESS_WITH_INSPECT://�ɹ���Ѳ������
		//�豸����������һ�꣬��Ҫ��ϵ����ͷ���Ա�����豸����ά�����ޣ���ȷ����ȫ�������У�
		//��������(��ID=%s, У����=%s)ƥ��������Ȩ����ȷ�ϲ���
		strTemp = pszMsg;
		AfxMessageBox(strTemp);
		return TRUE;
	case FRC_FAIL_DOG_LIB_LOAD://���������������̬��ʧ��
	case FRC_FAIL_DOG_LIB_GUID://����ʧ�ܣ�HansLmtH.dllģ������Ͳ�ƥ��
	case FRC_FAIL_DOG_LIB_VERSION://����ʧ�ܣ�HansLmtH.dllģ���汾������V1.1.0
		//ģ�������ʧ�ܻ�ģ���汾����ȷ�����������װ�Ƿ���ȷ
	case FRC_FAIL_DOG_EXECUTE://����ʼ��ת�������й�ʧ��
		//����������쳣�����⹷�����Ƿ������������Ƿ�װ��ȷ
	case FRC_FAIL_DOG_TIMEOUT://����ʱ���Ѿ�����
		//��ǰ�����Ȩ�ѵ��ڣ���Ҫ��ϵ����ͷ���Ա������������(��ID=%s, У����=%s)ƥ��������Ȩ�������Ա����ʹ��
	case FRC_FAIL_DOG_WRONG_TYPE:
		//!=0x00��������Ͳ�ƥ�䣬��ȷ��������Ƿ��Ǹ��豸���׵����������������ϵ����ͷ���Ա
		//!=0x01��������Ͳ�ƥ�䣬��ȷ��������Ƿ��Ǹ��豸���׵��������������ȷ����������ƿ���������Ƿ���Ҫ����
	case FRC_FAIL_DOG_CMP_CARD://���꿨����Ӧ
	case FRC_FAIL_DOG_FUNC_UNKNOW://�������з���δ֪����
	case FRC_FAIL_DOG_FUNC_NODOG://û�м�⵽�����
	case FRC_FAIL_DOG_FUNC_WRONGDOG://��⵽������������ǺϷ��������
		
	case FRC_FAIL_DOG_FUNC_CHECK_WRONG_BEFORE_EXECUTE://��Execute��������ǰû�гɹ����ù�Check����
	case FRC_FAIL_DOG_FUNC_LIB_L_VERSION://HansLmtL.dllģ���汾����V1.1.0
		//ģ�������ʧ�ܻ�ģ���汾����ȷ�����������װ�Ƿ���ȷ
		
	case FRC_FAIL_DOG_FUNC_MOREDOG://��������������1��
	case FRC_FAIL_DOG_FUNC_WRONG_PARAM://�������������֧��
		
	case FRC_FAIL_PARA_UNKNOW_CARDTYPE://δ֪�Ŀ��ƿ�����
	case FRC_FAIL_PARA_UNKONW_DOGTYPE://δ֪�Ĺ�����
		
	case FRC_FAIL_CARD_LIB_LOAD://���ƿ������ʧ��
	case FRC_FAIL_CARD_FUNC_GET://ȡ���ƿ���ӿں���ʧ��
	case FRC_FAIL_CARD_NONE://�޿��ƿ�
	case FRC_FAIL_CARD_WRITE_NEW://д�¿�ʧ��
		//д��ʧ�ܣ�������ƿ������ӻ���
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
	//֮ǰͨ��װ�ض�̬��ʱ��ȷ����ǰ�����Ƿ������ڰ�װĿ¼�¼���
	//�ҵ����ѵ�Ŀ¼����Ϊ��װĿ¼���������д���
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
