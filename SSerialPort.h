// SSerialPort.h: interface for the CSSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSERIALPORT_H__D5FEE7C6_C8A6_405C_9A65_33FA1E652900__INCLUDED_)
#define AFX_SSERIALPORT_H__D5FEE7C6_C8A6_405C_9A65_33FA1E652900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
ON_MESSAGE(WM_COMM_DATA, OnCommData)
afx_msg LRESULT OnCommData(WPARAM wParam, LPARAM lParam);
wParam: nPort
*/

#define WM_COMM_DATA	(WM_APP+101)

class CSSerialPort  
{
public:
	static CSSerialPort *CreateObject();
	void ReleaseObject();

	//Set
	virtual BOOL ShowCommConfigDialog(int nPort, int &nBaudRate, HWND hWnd=NULL)=0;	
	virtual BOOL OpenPort(int nPort=1, int nBaudRate=9600, HWND hwndMsg=NULL)=0;
	
	virtual BOOL IsOpen()=0;
	virtual void ClosePort()=0;
	virtual LPCSTR GetErrInfo()=0;

	virtual DCB  GetDCB()=0;
	virtual void SetDCB(DCB dcb)=0;
	
	//Read/Write
	virtual void SetBufferSize(DWORD dwInBufferSize=4096)=0;
	virtual void SetTimeout(DWORD dwReadTotalTimeout=3000, DWORD dwReadIntervalTimeout=10)=0;
	virtual void ClearBuffer()=0;

	//line functions is only for block mode
	virtual void SetNewLine(LPCSTR szNewLine="\r")=0;	//0x0D
	virtual LPCSTR ReadLine()=0;
	virtual void WriteLine(LPCSTR szString)=0;

	virtual LPCSTR ReadString()=0;
	virtual void WriteString(LPCSTR szString)=0;

	virtual BYTE* ReadData(DWORD *pNumberOfRead)=0;
	virtual void WriteData(BYTE *buffer, DWORD dwNumberToWrite)=0;
};

#endif // !defined(AFX_SSERIALPORT_H__D5FEE7C6_C8A6_405C_9A65_33FA1E652900__INCLUDED_)
