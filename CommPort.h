// CommPort.h: interface for the CCommPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMPORT_H__928D816E_605A_11D8_ABC3_000AE64C79F3__INCLUDED_)
#define AFX_COMMPORT_H__928D816E_605A_11D8_ABC3_000AE64C79F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CCommPort  
{
public:
	CCommPort();
	virtual ~CCommPort();

	HANDLE  m_hReadProc;
	BOOL    m_bBack;
public:
	BOOL   OpenCom(CString strPort,int baud);
	BOOL   CloseCom();
	BOOL   CheckComCable(CString strCheck,int nDelaytime,CString strBack);
	BOOL   WriteComCur(double dbCur,BOOL bWait,int nDelaytime,CString strBack);
	BOOL   WriteComPower(double dbCur,BOOL bWait,int nDelaytime,CString strBack);

public:
	int    ReadCom();
	HANDLE GetComHandle(){return m_hCom;};

private:
	CString m_strReadBack;
	HANDLE m_hCom;							//´®ÐÐ¿Ú
	BOOL m_bFeedBack;

	BOOL   WriteCom(char*pBuf,unsigned  short packNum);
	BOOL   OpenReadThread() ;
	BOOL   CloseReadThread();
	BOOL   ChangePort(CString strPort,int baud);
	BOOL   SetupConnection(CString strPort,int baud);
	BOOL WaitReadBack(int nDelaytime);

	void CompareCurReadBack(char* lpBuf);

public:
	//Updated by AndyF.Shelby on 3 Mar. 2006 for Light Command
	BOOL CCommPort::SendCommand(CString strSend, CString strOK, BOOL bWait, int nDelaytime);
};

#endif // !defined(AFX_COMMPORT_H__928D816E_605A_11D8_ABC3_000AE64C79F3__INCLUDED_)
