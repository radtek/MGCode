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

//200�����أ�1600*1200
//500�����أ�2432*2050

/////����ģʽ
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
	BOOL   m_bCCDMatch;         //У���Ƿ�ɹ�
	double m_dbCaliX;			//У�����������ϵ�µ�X
	double m_dbCaliY;			//У�����������ϵ�µ�Y
	double m_dbCaliScale;		//У�����������ϵ�µ�ScaleX

	//ÿ�δ�����OK��CCD����
	double m_dbTrigX;//X������������
	double m_dbTrigY;//Y������������
	double m_dbTrigAngle;//��ת�Ƕ�

	double m_dbCurWorkX;	//����ʱ����̨����x
	double m_dbCurWorkY;	//����ʱ����̨����y	
	double m_dbCurWorkZ;	//����ʱ����̨����z	
	
	int m_nErrorCount;//CCDͼ��ƥ��,ʧ���ܴ���


public:	
	BOOL InitCCD();

	void ClearCCDBuffer();
	void SendCCDCmd(LPCTSTR szCmd);
	CString RecCCDReply();

	BOOL SetLighter(int nLight1, int nLight2);
	BOOL SetCurLighter(int Cur ,int nLight);
	//�õ�����̨����(�ѵ�ǰ�Ѿ�ִ�в���Ķ�λ�㣬�ƶ�����λ��ʱ����λ������λ��CCD����)
	void GetWorkPosPointToCCDCenter(BOOL bCCD1, double &x, double &y,double& degree);
	void GetTrigMM(BOOL bCCD1, double &x, double &y,double& degree);

	void GetWorkPos(double &x,double &y);
	void GetTrigOK(double &dbX, double &dbY, double &dbAngle);
	
	BOOL SetWorkProgNo(int nProgId=-1); //���ó����ţ�-1��ʾ����
	
	void SetWorkMode(int nMode=1);//�趨����ģʽ����̣�����
	BOOL CCDLocate();//��ʼУ��,

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

