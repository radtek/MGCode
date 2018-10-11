// PowerMeter.h: interface for the CPowerMeter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWERMETER_H__1DB65341_9CD7_4C11_B57E_BBD4ED999B3F__INCLUDED_)
#define AFX_POWERMETER_H__1DB65341_9CD7_4C11_B57E_BBD4ED999B3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SSerialPort.h"

class CPowerMeter  
{
public:
	CPowerMeter();
	virtual ~CPowerMeter();

	BOOL IsOpen();
	void Close();

	virtual BOOL Open(int nCommPort)=0;	
	virtual BOOL QueryPower(double &dbPower)=0;

	CString m_strErrInfo;

protected:
	int m_nType;	//0表示大族功率计,1表示相干功率计
	CSSerialPort *m_pSerialPort;
};

class CHansPowerMeter : public CPowerMeter
{
public:
	CHansPowerMeter();
	virtual BOOL Open(int nCommPort);
	virtual BOOL QueryPower(double &dbPower);
};

class CCoherentPowerMeter : public CPowerMeter
{
public:
	CCoherentPowerMeter();
	virtual BOOL Open(int nCommPort);
	virtual BOOL QueryPower(double &dbPower);
};

#endif // !defined(AFX_POWERMETER_H__1DB65341_9CD7_4C11_B57E_BBD4ED999B3F__INCLUDED_)
