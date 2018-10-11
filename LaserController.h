// LaserController.h: interface for the CLaserController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LASERCONTROLLER_H__723F0C60_D4CB_491B_8D59_84F2CC15E94C__INCLUDED_)
#define AFX_LASERCONTROLLER_H__723F0C60_D4CB_491B_8D59_84F2CC15E94C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SSerialPort.h"

class CLaserController  
{
public:
	CLaserController();
	virtual ~CLaserController();

	BOOL IsOpen();
	virtual BOOL Open(int nCommPort)=0;
	virtual BOOL SetCurrent(double dbCurrent)=0;
	virtual BOOL OpenBeam()=0;
	virtual BOOL CloseBeam()=0;

	virtual BOOL SetThermaTrack(int nThermaTrack);

protected:
	int m_nType;	//0表示大族激光器,1表示相干激光器
	CSSerialPort *m_pSerialPort;
};

class CHansLaserController : public CLaserController
{
public:
	CHansLaserController();
	
	BOOL IsOpen();
	virtual BOOL Open(int nCommPort);
	virtual BOOL SetCurrent(double dbCurrent);
	virtual BOOL OpenBeam();
	virtual BOOL CloseBeam();
};

class CCoherentLaserController : public CLaserController
{
public:
	CCoherentLaserController();
	
	BOOL IsOpen();
	virtual BOOL Open(int nCommPort);
	virtual BOOL SetCurrent(double dbCurrent);
	virtual BOOL OpenBeam();
	virtual BOOL CloseBeam();
	virtual BOOL SetThermaTrack(int nThermaTrack);
};

#endif // !defined(AFX_LASERCONTROLLER_H__723F0C60_D4CB_491B_8D59_84F2CC15E94C__INCLUDED_)
