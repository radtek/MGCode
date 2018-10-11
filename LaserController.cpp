// LaserController.cpp: implementation of the CLaserController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "LaserController.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLaserController::CLaserController()
{
	m_nType = 0;
	m_pSerialPort = CSSerialPort::CreateObject();
}

CLaserController::~CLaserController()
{
	m_pSerialPort->ReleaseObject();	
}

BOOL CLaserController::IsOpen()
{
	return m_pSerialPort->IsOpen();
}

BOOL CLaserController::SetThermaTrack(int nThermaTrack)
{
	return TRUE;
}

//Hans
CHansLaserController::CHansLaserController()
{
	m_nType = 0;
}

BOOL CHansLaserController::Open(int nCommPort)
{
	m_pSerialPort->SetNewLine("\r\n");
	m_pSerialPort->SetTimeout(300);
	if (!m_pSerialPort->OpenPort(nCommPort, 38400))
	{
		CString strTip;
		strTip.Format("打开Hans激光控制器串口(COM%d)失败!", nCommPort);
		WriteRunLog(strTip);
		AfxMessageBox(strTip);	
		return FALSE;
	}
	return TRUE;
}

BOOL CHansLaserController::SetCurrent(double dbCurrent)
{
	CString strCMD, strInput, strMsg;
	
	if (m_pSerialPort->IsOpen())
	{
		strCMD = "GC";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strInput = m_pSerialPort->ReadLine();
		if (strInput.IsEmpty())
		{
			strMsg.Format("Send:%s\nRec:%s", strCMD, strInput);
			WriteRunLog(strMsg);
			AfxMessageBox("Set the current error! Unable to get the current!");
			return FALSE;
		}
	
		dbCurrent*=10;
		double dbOldCurrent = atoi(strInput);
		if (dbOldCurrent!=dbCurrent)
		{
			strCMD.Format("CU %.0f$", dbCurrent);
			m_pSerialPort->ClearBuffer();
			m_pSerialPort->WriteLine(strCMD);
			strInput = m_pSerialPort->ReadLine();
			if (strInput.IsEmpty() || strInput.Left(2)!="OK")
			{
				strMsg.Format("Send:%s\nRec:%s", strCMD, strInput);
				WriteRunLog(strMsg);
				AfxMessageBox("Set the current error!");
				return FALSE;
			}
		}	
	}
	return TRUE;
}

BOOL CHansLaserController::OpenBeam()
{
	CString strCMD, strRec, strTip, strLog;
	try
	{	
		//打开光闸(SHUTTER)
		strCMD = "SF 1";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strRec = m_pSerialPort->ReadLine();
		if (strRec.Left(2)!="OK") throw 2;
		Sleep(100);
		
		//Q驱控制模式(TRIGGER MODE)
		strCMD = "TM=2";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strRec = m_pSerialPort->ReadLine();
		if (strRec.Left(2)!="OK") throw 3;
		Sleep(100);
		
		//Q驱控制(BEAM ON)
		strCMD = "SH 1";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strRec = m_pSerialPort->ReadLine();
		if (strRec.Left(2)!="OK") throw 4;
	}
	catch (int e)
	{
		switch(e)
		{
		case 2:
			strTip = "Fail Open Shutter !";
			break;
		case 3:
			strTip = "Fail  Change TRIGGER MODE !";
			break;
		case 4:
			strTip = "Fail Open PULSE ON!";
			break;
		}
		strLog.Format("%s,Send(%s),Rec(%s)", strTip, strCMD, strRec);
		WriteRunLog(strLog);
		AfxMessageBox(strTip);
		return FALSE;
	}
	return TRUE;
}

BOOL CHansLaserController::CloseBeam()
{
	CString strTip, strCMD, strRec;
	
	//Q驱控制(PULSE ON)
	strCMD = "PC=0";
	m_pSerialPort->ClearBuffer();
	m_pSerialPort->WriteLine(strCMD);
	strRec = m_pSerialPort->ReadLine();
	if (strRec!=strCMD)
	{		
		strTip.Format("Send(%s),Rec(%s)", strCMD, strRec);
		WriteRunLog(strTip);
		
		strTip = "Close the PULSE ON failed, please ON the laser panel manually!";
		WriteRunLog(strTip);
		AfxMessageBox(strTip);
		return FALSE;
	}
	
	Sleep(200);
	
	//Q驱控制模式(TRIGGER MODE)
	strCMD = "TM=1";
	m_pSerialPort->ClearBuffer();
	m_pSerialPort->WriteLine(strCMD);
	strRec = m_pSerialPort->ReadLine();
	if (strRec!=strCMD)
	{		
		strTip.Format("Send(%s),Rec(%s)", strCMD, strRec);
		WriteRunLog(strTip);
		
		strTip = "Change the TRIGGER MODE failure, please on the laser panel manually!";
		WriteRunLog(strTip);
		AfxMessageBox(strTip);
		return FALSE;
	}
	return TRUE;
}

//Coherent
CCoherentLaserController::CCoherentLaserController()
{
	m_nType = 1;
}

BOOL CCoherentLaserController::Open(int nCommPort)
{
	if (!m_pSerialPort->OpenPort(nCommPort, 19200))
	{
		CString strTip;
		strTip.Format("Open the coherent laser controller serial port (COM%d) failure!", nCommPort);
		WriteRunLog(strTip);
		AfxMessageBox(strTip);	
		return FALSE;
	}
	return TRUE;
}

BOOL CCoherentLaserController::SetCurrent(double dbCurrent)
{
	CString strCMD, strInput, strMsg;

	try
	{		
		m_pSerialPort->SetNewLine("\r");
		Sleep(100);
		
		strCMD = "E=1";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strInput = m_pSerialPort->ReadLine();
		if (strInput.IsEmpty())	throw 2;
		Sleep(100);
		
		strCMD = ">=0";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strInput = m_pSerialPort->ReadLine();
		if (strInput.IsEmpty()) throw 3;
		Sleep(100);
		
		strCMD.Format("C=%3.1f", dbCurrent);
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strInput = m_pSerialPort->ReadLine();
		if (strInput!=strCMD) throw 4;				
	}
	catch (int e)
	{
		CString strTip;
		switch(e)
		{
		case 2:
			strTip.Format("Initialize the laser communication (step 2) failure!");
			break;
		case 3:
			strTip.Format("Initialize the laser communication (step 3) failure!");
			break;
		case 4:
			strTip = "Set the current error!";
			break;
		}
					
		strMsg.Format("Send:%s\nRec:%s", strCMD, strInput);
		WriteRunLog(strMsg);					
		AfxMessageBox(strTip);
		return FALSE;
	}	
	return TRUE;
}

BOOL CCoherentLaserController::OpenBeam()
{
	CString strCMD, strRec, strTip, strLog;
	try
	{	
		//打开光闸(SHUTTER):S=1
		strCMD = "S=1";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strRec = m_pSerialPort->ReadLine();
		if (strRec!=strCMD) throw 2;
		Sleep(100);
		
		//Q驱控制模式(TRIGGER MODE)
		strCMD = "TM=2";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strRec = m_pSerialPort->ReadLine();
		if (strRec!=strCMD) throw 3;
		Sleep(100);
		
		//Q驱控制(PULSE ON)
		strCMD = "PC=1";
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine(strCMD);
		strRec = m_pSerialPort->ReadLine();
		if (strRec!=strCMD) throw 4;
	}
	catch (int e)
	{
		switch(e)
		{
		case 2:
			strTip = "Fail Open Shutter!";
			break;
		case 3:
			strTip = "Fail Change TRIGGER MODE!";
			break;
		case 4:
			strTip = "Fail Open PULSE ON!";
			break;
		}
		strLog.Format("%s,Send(%s),Rec(%s)", strTip, strCMD, strRec);
		WriteRunLog(strLog);
		AfxMessageBox(strTip);
		return FALSE;
	}
	return TRUE;
}

BOOL CCoherentLaserController::CloseBeam()
{
	CString strTip, strCMD, strRec;
	
	//Q驱控制(PULSE ON)
	strCMD = "PC=0";
	m_pSerialPort->ClearBuffer();
	m_pSerialPort->WriteLine(strCMD);
	strRec = m_pSerialPort->ReadLine();
	if (strRec!=strCMD)
	{		
		strTip.Format("Send(%s),Rec(%s)", strCMD, strRec);
		WriteRunLog(strTip);

		strTip = "Close the PULSE ON failed, please ON the laser panel manually!";
		WriteRunLog(strTip);
		AfxMessageBox(strTip);
		return FALSE;
	}
	
	Sleep(200);
	
	//Q驱控制模式(TRIGGER MODE)
	strCMD = "TM=1";
	m_pSerialPort->ClearBuffer();
	m_pSerialPort->WriteLine(strCMD);
	strRec = m_pSerialPort->ReadLine();
	if (strRec!=strCMD)
	{		
		strTip.Format("Send(%s),Rec(%s)", strCMD, strRec);
		WriteRunLog(strTip);
		
		strTip = "Change the TRIGGER MODE failure, please on the laser panel manually!";
		WriteRunLog(strTip);
		AfxMessageBox(strTip);
		return FALSE;
	}
	return TRUE;
}

BOOL CCoherentLaserController::SetThermaTrack(int nThermaTrack)
{
	CString strTip,strLog;
	if (nThermaTrack>6150 || nThermaTrack<100)
	{
		strTip.Format("ThermaTrack值(%d)超范围!", nThermaTrack);
		WriteRunLog(strTip);
		AfxMessageBox(strTip);
		return FALSE;
	}
	
	CString strCMD, strRec;
	strCMD.Format("MD=%04d", nThermaTrack);
	m_pSerialPort->ClearBuffer();
	m_pSerialPort->WriteLine(strCMD);
	strRec=m_pSerialPort->ReadLine();
	if (strCMD!=strRec)
	{
		strTip = "设置ThermaTrack值失败!";
		strLog.Format("%s,Send(%s),Rec(%s)", strTip, strCMD, strRec);
		WriteRunLog(strLog);
		AfxMessageBox(strTip);
		return FALSE;
	}
	return TRUE;
}