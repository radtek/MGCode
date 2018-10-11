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

	void    ReadSysCfg();       // 加载系统配置信息
	void    WriteSysCfg();      // 写系统配置信息
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

	double m_dbBoxX;			//操作范围大小
	double m_dbBoxY;

	BOOL m_bStopPreview;		//是否停止预览


	static CMarkCard* m_pCard;
	static BOOL m_bSystemOk;	//系统是否准备就绪
	
	int m_nDibSampleMode;		//图像采样方式

	//灰度非线性补偿数组
	BYTE m_GrayCompensate[256];

	//触发信号设置
	//脚踏开关参数
	UINT m_nTouchTimes;					//每多少信号触发一次
	BOOL m_bMarkOnce;					//不在同一物体上打多次
	UINT m_nDelayBefore;				//触发后延时
	UINT m_nDelayAfter;					//打标完成后延时
	UINT m_nDelayBetween;				//每次打标中间间隔
	UINT m_nDelayEnd;					//结束信号
	UINT m_nMarkRepeat;					//整体重复打标次数

	CString m_strCardName;

	UINT m_nFullTaskCount;				//任务满的数量

	BOOL m_bEnableCardLog;				//启用打标卡日志
	LANGID m_langID;
private:
	char m_pFileName[256];			//系统配置文件名称
	char m_szExePath[256];		//EXE文件运行目录

	CRightMng m_RightMng;

public:
	//////////////////////////////////////////////////////////////////////////////////
	//Wafer
	int m_nRefPointType;  //参考点类型: 0表示以(0,0)点为中心点, 1表示以(0,0)点为左上角点

	int m_nCViewPort;

	int m_nLighterPort;
	int m_nLaserControllerPort;
	int m_nPowerMeterPort;
	int m_nPowerMeterType; //0表示相干功率,1表示大族功率计
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
	int m_nWriteDelay;                  //写频率延时
	char m_szOPCIP[20];                 //OPC服务器IP地址

	int m_dbProductSize;

	//RunLog
	int m_nLogLevel;	

	//Workbench
	double m_dbWorkbenchMaxPosErr;
};

extern CSystem g_sys;

#endif // !defined(AFX_SYSTEMPARA_H__D552C92C_31A0_4511_A6A0_F2E1DABE4517__INCLUDED_)





















