// System.h: interface for the CSystemPara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMPARA_H__D552C92C_31A0_4511_A6A0_F2E1DABE4517__INCLUDED_)
#define AFX_SYSTEMPARA_H__D552C92C_31A0_4511_A6A0_F2E1DABE4517__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Card.h"
#include "MarkCard.h"
#include "RightMng.h"

#define g_DAT	(*(CSystem::m_pCard))

class CSystem
{	
public:
	CSystem();
	virtual ~CSystem();

public:
	static CMarkCard* GetCardHandle();
	BOOL    Create();
	CString GetInstallPath();
	void    GetExepath(char* pPath);
	CString GetProfileSting(char *pSec, char *pKey, LPCTSTR strDefault);
	void	WriteProfileSting(char *pSec, char *pKey, LPCSTR strValue);
	int     GetProfileInt(char* pSec,char* pKey,int nDefault);
	double  GetProfileDouble(char* pSec,char* pKey,double dbDefault);
	void    WriteProfileInt(char* pSec,char* pKey, int nValue);
	void    WriteProfileDouble(char* pSec,char* pKey, int dbValue);

	void    ReadSysCfg();       // ����ϵͳ������Ϣ
	void    WriteSysCfg();      // дϵͳ������Ϣ
	void	WriteLang();    

	void    WriteRightInfo(CString &strFileName);
	void    ReadRightInfo(CString &strFileName);
	BOOL    AddUser(USERINFO &stUserInfo);
	BOOL    DelUser(CString &strUserName);
	void    EditUserInfo(USERINFO &stUserInfo);
	BOOL    GetUserInfoByName(CString strUserName, USERINFO &stUserInfo);
	void    EditUserInfoByUserName(USERINFO &stUserInfo, CString &strUserName);
	int     GetIndexByName(CString strUserName);
	BOOL    OnSetDefautUser(CString &strUserName,  BOOL bSel);
	BOOL    GetDefaultUserInfo(USERINFO &stUserInfo);

	BOOL    IsAvailUser(CString &strUserName, CString &strPassWord);

	void    WriteSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFileName);
	void    ReadSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFileName);

	UINT    ReadSysPara();

	double m_dbBoxX;			//������Χ��С
	double m_dbBoxY;

	BOOL m_bStopPreview;		//�Ƿ�ֹͣԤ��


	static CMarkCard* m_pCard;
	static BOOL m_bSystemOk;	//ϵͳ�Ƿ�׼������
	
	int m_nDibSampleMode;		//ͼ�������ʽ

	//�Ҷȷ����Բ�������
	BYTE m_GrayCompensate[256];

	//�����ź�����
	//��̤���ز���
	UINT m_nTouchTimes;					//ÿ�����źŴ���һ��
	BOOL m_bMarkOnce;					//����ͬһ�����ϴ���
	UINT m_nDelayBefore;				//��������ʱ
	UINT m_nDelayAfter;					//�����ɺ���ʱ
	UINT m_nDelayBetween;				//ÿ�δ���м���
	UINT m_nDelayEnd;					//�����ź�
	UINT m_nMarkRepeat;					//�����ظ�������

	CString m_strCardName;

	UINT m_nFullTaskCount;				//������������

	BOOL m_bEnableCardLog;				//���ô�꿨��־
	LANGID m_langID;
private:
	char m_pFileName[256];			//ϵͳ�����ļ�����
	char m_szExePath[256];		//EXE�ļ�����Ŀ¼

	CRightMng m_RightMng;

public:
	//////////////////////////////////////////////////////////////////////////////////
	//Wafer
	int m_nRefPointType;  //�ο�������: 0��ʾ��(0,0)��Ϊ���ĵ�, 1��ʾ��(0,0)��Ϊ���Ͻǵ�

	int m_nCViewPort;

	int m_nLighterPort;
	int m_nLaserControllerPort;
	int m_nPowerMeterPort;
	int m_nPowerMeterType; //0��ʾ��ɹ���,1��ʾ���幦�ʼ�
	int m_nBeamExpanderPort;
	BOOL m_bPLCOnline;
	BOOL m_bUseSafeDoor;
	BOOL m_bDEV_3030B;

	//LASER
	int m_nLaserCurrentMode;
	int m_nLaserCurrentMin;
	int m_nLaserCurrentMax;
	int m_nLaserCurrentKeep;
	int m_nLaserLayDelay;

	int m_nCCDType;
	int m_nCCDTransport;
	int m_nCCDPort;
	int m_nCCD_IP_Port;
	CString m_strCCD_IP;
	int m_nCCDNum;
	int m_nCCD1PixelX;
	int m_nCCD1PixelY;
	int m_nCCD2PixelX;
	int m_nCCD2PixelY;
	BOOL m_bHandPosOnCCDFail;

	//OPCLaser
	int m_bUseOpcLaser;
	int m_nWriteDelay;                  //дƵ����ʱ
	char m_szOPCIP[20];                 //OPC������IP��ַ

	int m_dbProductSize;

	//RunLog
	int m_nLogLevel;	

	//Workbench
	double m_dbWorkbenchMaxPosErr;
};

extern CSystem g_sys;

#endif // !defined(AFX_SYSTEMPARA_H__D552C92C_31A0_4511_A6A0_F2E1DABE4517__INCLUDED_)





















