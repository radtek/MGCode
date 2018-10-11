// CCDCalibrate.h: interface for the CCCDCalibrate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCDCALIBRATE_H__6348CC82_5F09_11D9_82F7_00FE53C15554__INCLUDED_)
#define AFX_CCDCALIBRATE_H__6348CC82_5F09_11D9_82F7_00FE53C15554__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "HLIni.h"
#include "resource.h" 
#include "Dot.h"
#include "SSerialPort.h"
#include "SocketClient.h"

//200万像素，1600*1200
//500万像素，2432*2050

/////工作模式
enum
{
	eProgMode=0,eRunMode
};


class CCCDCalibrate : public CHlIni  
{
public:
	CCCDCalibrate();
	virtual ~CCCDCalibrate();

public:
	BOOL   m_bCCDMatch;         //校正是否成功
	double m_dbCaliX;			//校正后打标机坐标系下的X
	double m_dbCaliY;			//校正后打标机坐标系下的Y
	double m_dbCaliScale;		//校正后打标机坐标系下的ScaleX

	//每次触发后OK的CCD参数
	double m_dbTrigX;//X方向像素坐标
	double m_dbTrigY;//Y方向像素坐标
	double m_dbTrigAngle;//旋转角度

	double m_dbCurWorkX;	//触发时工作台坐标x
	double m_dbCurWorkY;	//触发时工作台坐标y	
	double m_dbCurWorkZ;	//触发时工作台坐标z	
	
	int m_nErrorCount;//CCD图像不匹配,失败总次数


public:	
	BOOL InitCCD();

	void ClearCCDBuffer();
	void SendCCDCmd(LPCTSTR szCmd);
	CString RecCCDReply();

	BOOL SetLighter(int nLight1, int nLight2);
	BOOL SetCurLighter(int Cur ,int nLight);
	//得到工作台坐标(把当前已经执行捕获的定位点，移动到此位置时，定位点正好位于CCD中心)
	void GetWorkPosPointToCCDCenter(BOOL bCCD1, double &x, double &y,double& degree);
	void GetTrigMM(BOOL bCCD1, double &x, double &y,double& degree);

	void GetWorkPos(double &x,double &y);
	void GetTrigOK(double &dbX, double &dbY, double &dbAngle);
	
	BOOL SetWorkProgNo(int nProgId=-1); //设置程序编号，-1表示不变
	
	void SetWorkMode(int nMode=1);//设定工作模式：编程，运行
	BOOL CCDLocate();//开始校正,

	BOOL SaveCCDScreen(LPCTSTR szInfo);
	void DoEvent();

public:
	BOOL    m_bInit;
	CSSerialPort *m_pCommCCD;
	CSSerialPort *m_pCommLighter;
	CSocketClient m_socketCCD;

public:		
	
	double GetCaptPrecision(BOOL bCCD1);
	void MoveToCCDCenter(BOOL bCCD1,  double& xWorkPosMM, double& yWorkPosMM);
	void CCD2ToCCD1Center(double &xWorkPosMM, double &yWorkPosMM);

	void MoveToCCDCenter(BOOL bCCD1, double xPixel,double yPixel,double& xWorkMM, double& yWorkMM);
	int m_nCCDType;
};


extern CCCDCalibrate g_ccd;
extern HANDLE g_hEventCCDRec;
extern HWND g_hWndView;
#endif // !defined(AFX_CCDCALIBRATE_H__6348CC82_5F09_11D9_82F7_00FE53C15554__INCLUDED_)

