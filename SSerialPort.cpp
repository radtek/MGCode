// SSerialPort.cpp: implementation of the CSSerialPort class.
//
//////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "SSerialPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ASSERT_RTN(rtn)		if(!(rtn)) ReportError(__LINE__);

#define NEWLINE_MAXLEN	8
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CSSerialPortImp: public CSSerialPort
{
public:
	CSSerialPortImp();
	virtual ~CSSerialPortImp();
	
public:
	virtual BOOL ShowCommConfigDialog(int nPort, int &nBaudRate, HWND hWnd=NULL);	
	virtual BOOL OpenPort(int nPort=1, int nBaudRate=9600, HWND hwndMsg=NULL);
	
	virtual BOOL IsOpen();
	virtual void ClosePort();
	virtual LPCSTR GetErrInfo();
	
	virtual DCB  GetDCB();
	virtual void SetDCB(DCB dcb);
		
	//Read/Write
	virtual void SetBufferSize(DWORD dwInBufferSize=4096);
	virtual void SetTimeout(DWORD dwReadTotalTimeout=3000, DWORD dwReadIntervalTimeout=10);
	virtual void ClearBuffer();
	
	virtual void SetNewLine(LPCSTR szNewLine="\r");	//0x0D
	virtual LPCSTR ReadLine();
	virtual void WriteLine(LPCSTR szString);
	
	virtual LPCSTR ReadString();
	virtual void WriteString(LPCSTR szString);
	
	virtual BYTE* ReadData(DWORD *pNumberOfRead);
	virtual void WriteData(BYTE *buffer, DWORD dwNumberToWrite);
	
protected:
	DWORD m_dwPort;
	HANDLE m_hComm;
	HWND m_hwndMsg;
	BYTE m_bufferNewLine[NEWLINE_MAXLEN];

	DWORD m_dwReadTotalTimeout;
	DWORD m_dwReadIntervalTimeout;	

	BYTE *m_bufferReceive;
	DWORD m_dwSizeOfReceiveBuffer;
	
	//thread
	CWinThread *m_hWinThreadComm;
	static UINT	CommThread(LPVOID pParam);
	
	//For overlapped
	HANDLE m_hStopThreadEvent;
	OVERLAPPED m_overlappedWait;
	OVERLAPPED m_overlappedWrite;
	OVERLAPPED m_overlappedRead;
	OVERLAPPED m_overlappedReceive;
	
	char m_szErrInfo[255];
	static void ReportError(int nLine);
	void BuildDCB(int nBaudRate, DCB &dcb);
};

CSSerialPortImp::CSSerialPortImp()
{
	m_dwPort = 0;
	m_hComm = INVALID_HANDLE_VALUE;
	m_hwndMsg = NULL;

	SetNewLine();	
	SetTimeout();

	m_dwSizeOfReceiveBuffer = 4096;
	m_bufferReceive = new BYTE[m_dwSizeOfReceiveBuffer+2];
	memset(m_bufferReceive, 0, m_dwSizeOfReceiveBuffer+2);
	
	m_hWinThreadComm = NULL;

	m_hStopThreadEvent = NULL;
	
	memset(&m_overlappedWait, 0, sizeof(OVERLAPPED));
	memset(&m_overlappedRead, 0, sizeof(OVERLAPPED));
	memset(&m_overlappedWrite, 0, sizeof(OVERLAPPED));
	memset(&m_overlappedReceive, 0, sizeof(OVERLAPPED));
	
	ASSERT_RTN(m_hStopThreadEvent=::CreateEvent(NULL, TRUE, FALSE, NULL));
	ASSERT_RTN(m_overlappedWait.hEvent=::CreateEvent(NULL, TRUE, FALSE, NULL));	
	ASSERT_RTN(m_overlappedRead.hEvent=::CreateEvent(NULL, TRUE, FALSE, NULL));
	ASSERT_RTN(m_overlappedWrite.hEvent=::CreateEvent(NULL, TRUE, FALSE, NULL));
}

CSSerialPortImp::~CSSerialPortImp()
{
	ClosePort();

	delete[] m_bufferReceive;
	m_bufferReceive=NULL;
	
	ASSERT_RTN(::CloseHandle(m_hStopThreadEvent));
	ASSERT_RTN(::CloseHandle(m_overlappedWait.hEvent));
	ASSERT_RTN(::CloseHandle(m_overlappedRead.hEvent));
	ASSERT_RTN(::CloseHandle(m_overlappedWrite.hEvent));
}


LPCSTR CSSerialPortImp::GetErrInfo()
{
	return m_szErrInfo; 
}

BOOL CSSerialPortImp::ShowCommConfigDialog(int nPort, int &nBaudRate, HWND hWnd)
{
	COMMCONFIG commconfig;
	commconfig.dwSize = sizeof(COMMCONFIG);
	BuildDCB(nBaudRate, commconfig.dcb);

	CString strComm;
	strComm.Format(_T("COM%d"), nPort);
	if (CommConfigDialog(strComm, hWnd, &commconfig))
	{
		nBaudRate = commconfig.dcb.BaudRate;
		return TRUE;
	}
	return FALSE;
}

void CSSerialPortImp::BuildDCB(int nBaudRate, DCB &dcb)
{
	dcb.DCBlength = sizeof(DCB);
	dcb.fBinary = TRUE;              // Bianry must be TRUE in Win32 
	dcb.EofChar = '\xA0';
	dcb.fNull = FALSE;
	
	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.StopBits = 0;
	
	//no Parity
	dcb.fParity = FALSE;
	dcb.Parity = 0;
	dcb.fErrorChar = FALSE;
	dcb.ErrorChar = 0x00;
	
	//no RTS/CTS flow control
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fOutxCtsFlow = FALSE;
		
	//no DTR/DSR flow control
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutxDsrFlow = FALSE;

	//no  Xon/Xoff flow control
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fTXContinueOnXoff = FALSE;
	
	//PC ready signal
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fDsrSensitivity = FALSE;
		
	dcb.fAbortOnError = FALSE;
	dcb.fDummy2 = 0;
	dcb.wReserved = 0;
	dcb.XonLim = 2048;
	dcb.XoffLim = 512;
	dcb.XonChar=0x11;
	dcb.XoffChar=0x13;
	dcb.EvtChar=0;
	dcb.wReserved1=0;

	//Read Settings "9600,n,8,1"
	dcb.BaudRate = nBaudRate;
}

BOOL CSSerialPortImp::IsOpen()
{
	return m_hComm!=INVALID_HANDLE_VALUE;
}

BOOL CSSerialPortImp::OpenPort(int nPort, int nBaudRate, HWND hwndMsg)
{
	// get a handle to the port
	DCB dcb;
	BuildDCB(nBaudRate, dcb);

	CString strPort;
	strPort.Format(_T("\\\\.\\COM%d"), nPort);	
	m_hComm = ::CreateFile(strPort, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (m_hComm==INVALID_HANDLE_VALUE)
		return FALSE;

	ASSERT_RTN(::SetCommState(m_hComm, &dcb));
	ASSERT_RTN(::PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT));
	
	if (hwndMsg)
	{
		ASSERT_RTN(::ResetEvent(m_hStopThreadEvent));
		ASSERT_RTN(m_hWinThreadComm = AfxBeginThread(CommThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED));
		m_hWinThreadComm->m_bAutoDelete = FALSE;
		m_hWinThreadComm->ResumeThread();
	}

	m_dwPort = nPort;
	m_hwndMsg = hwndMsg;
	return TRUE;
}

void CSSerialPortImp::ClosePort()
{
	if (m_hComm==INVALID_HANDLE_VALUE)
		return;

	if (m_hWinThreadComm)
	{
		::SetEvent(m_hStopThreadEvent);		
		::WaitForSingleObject(m_hWinThreadComm->m_hThread, 5000);

		delete m_hWinThreadComm;
		m_hWinThreadComm = NULL;
	}

	ASSERT_RTN(::CloseHandle(m_hComm));
	m_hComm = INVALID_HANDLE_VALUE;
}

DCB  CSSerialPortImp::GetDCB()
{
	DCB dcb;
	if (m_hComm!=INVALID_HANDLE_VALUE)
	{
		ASSERT_RTN(::GetCommState(m_hComm, &dcb));
	}

	return dcb;
}

void CSSerialPortImp::SetDCB(DCB dcb)
{
	if (m_hComm==INVALID_HANDLE_VALUE)
		return;

	ASSERT_RTN(::SetCommState(m_hComm, &dcb));
}

void CSSerialPortImp::SetBufferSize(DWORD dwInBufferSize)
{
	m_dwSizeOfReceiveBuffer = dwInBufferSize;

	delete m_bufferReceive;
	m_bufferReceive = new BYTE[dwInBufferSize];
}

void CSSerialPortImp::ClearBuffer()
{
	if (m_hComm==INVALID_HANDLE_VALUE)
		return;

	ASSERT_RTN(::PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT));
}

void CSSerialPortImp::SetTimeout(DWORD dwReadTotalTimeout, DWORD dwReadIntervalTimeout)
{
	m_dwReadTotalTimeout = dwReadTotalTimeout;
	m_dwReadIntervalTimeout = dwReadIntervalTimeout;
}

void CSSerialPortImp::SetNewLine(LPCSTR szNewLine)
{
	ZeroMemory(m_bufferNewLine, NEWLINE_MAXLEN);
	for (int i=0; i<7; i++)
	{
		if ((m_bufferNewLine[i]=szNewLine[i])==0)
			break;
	}
}

LPCSTR CSSerialPortImp::ReadLine()
{
	if (m_hComm==INVALID_HANDLE_VALUE)
	{
		m_bufferReceive[0] = 0;		
		return (LPCSTR)m_bufferReceive;
	}

	COMMTIMEOUTS commTimeOuts;
	commTimeOuts.ReadIntervalTimeout = m_dwReadIntervalTimeout;
	commTimeOuts.ReadTotalTimeoutMultiplier = 0;
	commTimeOuts.ReadTotalTimeoutConstant = 0;
	commTimeOuts.WriteTotalTimeoutConstant = 0;
	commTimeOuts.WriteTotalTimeoutMultiplier = 0;

	DWORD dwNumberRead=0, dwNumberTotalRead=0;
	DWORD dwFirstReadTickCount = ::GetTickCount(), dwUsedTime = 0;
	while(TRUE)
	{
		commTimeOuts.ReadTotalTimeoutConstant = m_dwReadTotalTimeout - dwUsedTime;	
		ASSERT_RTN(::SetCommTimeouts(m_hComm, &commTimeOuts));

		dwNumberRead = 0;
		m_overlappedRead.Offset = 0;
		m_overlappedRead.OffsetHigh = 0;
		if (!::ReadFile(m_hComm, m_bufferReceive+dwNumberTotalRead, m_dwSizeOfReceiveBuffer-dwNumberTotalRead, &dwNumberRead, &m_overlappedRead))
		{
			ASSERT_RTN(::GetLastError()==ERROR_IO_PENDING);
			WaitForSingleObject(m_overlappedRead.hEvent, INFINITE);
			dwNumberRead = m_overlappedRead.InternalHigh;
		}

		if (dwNumberRead>0)
		{
			dwNumberTotalRead += dwNumberRead;
			m_bufferReceive[dwNumberTotalRead] = 0; //for strcmp

			BYTE *pNewLine = m_bufferReceive + (dwNumberTotalRead - strlen((LPCSTR)m_bufferNewLine));
			if (pNewLine>=m_bufferReceive && strcmp((LPCSTR)pNewLine, (LPCSTR)m_bufferNewLine)==0)
			{
				dwNumberTotalRead = pNewLine - m_bufferReceive;
				break;
			}
		}	
		
		dwUsedTime = GetTickCount() - dwFirstReadTickCount;
		if (dwUsedTime >= m_dwReadTotalTimeout)
			break;
	}
	
	m_bufferReceive[dwNumberTotalRead] = 0;
	return (LPCSTR)m_bufferReceive;
}

LPCSTR CSSerialPortImp::ReadString()
{
	return (LPCSTR)ReadData(NULL);
}

BYTE* CSSerialPortImp::ReadData(DWORD *pNumberOfRead)
{
	if (m_hComm==INVALID_HANDLE_VALUE)
	{
		if (pNumberOfRead!=NULL) 
			*pNumberOfRead=0;

		m_bufferReceive[0] = 0;		
		return m_bufferReceive;
	}

	COMMTIMEOUTS commTimeOuts;
	commTimeOuts.ReadIntervalTimeout = m_dwReadIntervalTimeout;
	commTimeOuts.ReadTotalTimeoutMultiplier = 0;
	commTimeOuts.ReadTotalTimeoutConstant = m_dwReadTotalTimeout;
	commTimeOuts.WriteTotalTimeoutConstant = 0;
	commTimeOuts.WriteTotalTimeoutMultiplier = 0;
	ASSERT_RTN(::SetCommTimeouts(m_hComm, &commTimeOuts));

	DWORD dwNumberRead = 0;
	m_overlappedRead.Offset = 0;
	m_overlappedRead.OffsetHigh = 0;
	if (!::ReadFile(m_hComm, m_bufferReceive, m_dwSizeOfReceiveBuffer, &dwNumberRead, &m_overlappedRead))
	{
		ASSERT_RTN(::GetLastError()==ERROR_IO_PENDING);
		WaitForSingleObject(m_overlappedRead.hEvent, INFINITE);
		dwNumberRead = m_overlappedRead.InternalHigh;
	}
	
	if (pNumberOfRead!=NULL) 
		*pNumberOfRead=dwNumberRead;

	m_bufferReceive[dwNumberRead] = 0;	
	return m_bufferReceive;
}

void CSSerialPortImp::WriteLine(LPCSTR szString)
{
	int nLen = strlen(szString);

	char *buffer = new char[nLen+NEWLINE_MAXLEN];
	memcpy(buffer, szString, nLen);
	memcpy(buffer+nLen, m_bufferNewLine, NEWLINE_MAXLEN);
	
	WriteString(buffer);
	delete buffer;
}

void CSSerialPortImp::WriteString(LPCSTR szString)
{
	DWORD dwNumber = strlen((char*)szString);
	WriteData((BYTE *)szString, dwNumber);
	OutputDebugString(szString);
}

void CSSerialPortImp::WriteData(BYTE *buffer, DWORD dwNumberToWrite)
{
	if (m_hComm==INVALID_HANDLE_VALUE)
		return;

	DWORD dwNumberWritten = 0;
	if (!::WriteFile(m_hComm, buffer, dwNumberToWrite, &dwNumberWritten, &m_overlappedWrite))
	{
		ASSERT_RTN(::GetLastError()==ERROR_IO_PENDING);
		ASSERT_RTN(::GetOverlappedResult(m_hComm, &m_overlappedWrite, &dwNumberWritten, TRUE));
	}
	
	ASSERT_RTN(dwNumberWritten==dwNumberToWrite);
}

UINT CSSerialPortImp::CommThread(LPVOID pParam)
{
	CSSerialPortImp *pSerialPort = (CSSerialPortImp*)pParam;

	DWORD dwMask= 0|EV_RXCHAR|EV_ERR;
	ASSERT_RTN(::SetCommMask(pSerialPort->m_hComm, dwMask));

	HANDLE hEventArray[2] = {pSerialPort->m_hStopThreadEvent, pSerialPort->m_overlappedWait.hEvent};
	while(TRUE)
	{ 
		DWORD dwCommEvent=0, dwNumberOfBytesTransferred=0;
		if (!::WaitCommEvent(pSerialPort->m_hComm, &dwCommEvent, &pSerialPort->m_overlappedWait))
		{
			ASSERT_RTN(::GetLastError()==ERROR_IO_PENDING);

			DWORD dwWaitRtn = ::WaitForMultipleObjects(2, hEventArray, FALSE, INFINITE);
			if (dwWaitRtn==0) 
				return 1;
			if (dwWaitRtn==WAIT_FAILED)
				continue;
		}
		
		COMSTAT ComStat;
		DWORD dwCommError=0;
		ASSERT_RTN(::ClearCommError(pSerialPort->m_hComm, &dwCommError, &ComStat));		
		if(dwCommError & CE_RXPARITY)	//“奇偶校验”被启用且发生时
		{
			static char szError[256];
			sprintf(szError, "Serial Port(%d) Parity Error!", pSerialPort->m_dwPort);
			::FatalAppExit(0, szError);
		}

		if (dwCommEvent & EV_RXCHAR)
		{
			if (ComStat.cbInQue)
				::SendMessage(pSerialPort->m_hwndMsg, WM_COMM_DATA, WPARAM(pSerialPort->m_dwPort), 0);
		}
	}
	return 0;
}

void CSSerialPortImp::ReportError(int nLine)
{
	static char szError[1024];
	sprintf(szError, "CSSerialPort Error! Line(%d)!", nLine);
	::FatalAppExit(0, szError);
}

CSSerialPort *CSSerialPort::CreateObject()
{
	return new CSSerialPortImp;
}

void CSSerialPort::ReleaseObject()
{
	CSSerialPortImp *pTureObject = (CSSerialPortImp*)this;
	delete pTureObject;
}
