// PowerMeter.cpp: implementation of the CPowerMeter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "PowerMeter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPowerMeter::CPowerMeter()
{
	m_nType = 0;
	m_pSerialPort = CSSerialPort::CreateObject();
}

CPowerMeter::~CPowerMeter()
{
	m_pSerialPort->ReleaseObject();	
}

BOOL CPowerMeter::IsOpen()
{
	return m_pSerialPort->IsOpen();
}

void CPowerMeter::Close()
{
	if (m_pSerialPort->IsOpen())
		m_pSerialPort->ClosePort();
}

//Hans
CHansPowerMeter::CHansPowerMeter()
{
	m_nType = 0;
}



BOOL CHansPowerMeter::Open(int nCommPort)
{
	m_pSerialPort->SetNewLine("\r\n");
	if (!m_pSerialPort->OpenPort(nCommPort, 38400))
	{
		m_strErrInfo.Format("Open Hans Powermeter (COM%d) Fail!", nCommPort);
		WriteRunLog(m_strErrInfo);
		return FALSE;
	}
	return TRUE;
}

BOOL CHansPowerMeter::QueryPower(double &dbPower)
{

	m_pSerialPort->SetNewLine("\r\n");
	if (!m_pSerialPort->OpenPort(g_sys.m_nPowerMeterPort, 38400))
	{
		return FALSE;
	}
	try
	{
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteLine("RCD");
		CString strRec = m_pSerialPort->ReadLine();
		if (!strRec.IsEmpty())
		{
			dbPower = atof(strRec);
			Close();
			return TRUE;
		}
		return FALSE;
	}
	catch (CException*)
	{
		Close();
		return FALSE;
	}
}

//Coherent
CCoherentPowerMeter::CCoherentPowerMeter()
{
	m_nType = 1;
}


BOOL CCoherentPowerMeter::Open(int nCommPort)
{
	m_pSerialPort->SetNewLine("\r");
	if (!m_pSerialPort->OpenPort(nCommPort, 9600))
	{
		CString strTip;
		strTip.Format("打开相干功率计(COM%d)失败!", nCommPort);
		WriteRunLog(strTip);
		AfxMessageBox(strTip);	
		return FALSE;
	}
	return TRUE;
}

BOOL CCoherentPowerMeter::QueryPower(double &dbPower)
{
	m_pSerialPort->SetNewLine("\r");
	if (!m_pSerialPort->OpenPort(g_sys.m_nPowerMeterPort, 9600))
	{
		return FALSE;
	}
	try
	{
		//m_pSerialPort->SetNewLine("\r\n");
		m_pSerialPort->ClearBuffer();
		m_pSerialPort->WriteString("PW?\r\n");
		CString strRec = m_pSerialPort->ReadLine();
		OutputDebugString(strRec);
		if (!strRec.IsEmpty())
		{
			dbPower = atof(strRec);
			Close();
			return TRUE;
		}
	}
	catch (CException*)
	{
		Close();
		return FALSE;
	}
	return TRUE;
}