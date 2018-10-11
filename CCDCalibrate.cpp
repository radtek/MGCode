// CCDCalibrate.cpp: implementation of the CCCDCalibrate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CCDCalibrate.h"
#include "hl.h"
#include "Marknormaldlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CCCDCalibrate g_ccd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCCDCalibrate::CCCDCalibrate()
{
	m_nCCDType  = 1;

	m_dbCaliX = 1;
	m_dbCaliY = 1;
	m_dbCaliScale=1.0;	

	m_dbTrigX=0.0; 
	m_dbTrigY=0.0; 
	m_dbTrigAngle=0.0; 	
	m_nErrorCount=0;	

	m_bInit = FALSE;
	m_pCommCCD = CSSerialPort::CreateObject();
	m_pCommLighter = CSSerialPort::CreateObject();
}

CCCDCalibrate::~CCCDCalibrate()
{
	m_pCommCCD->ReleaseObject();
	m_pCommLighter->ReleaseObject();
}

BOOL CCCDCalibrate::InitCCD()
{
	m_bInit = FALSE;

	CString strTip;
	if (g_sys.m_nCCDTransport==1)
	{
		if (!m_pCommCCD->IsOpen())
		{
			m_pCommCCD->SetNewLine("\r");
			m_pCommCCD->SetBufferSize(1024*1024*3);
			m_pCommCCD->SetTimeout(4000);
			if (!m_pCommCCD->OpenPort(g_sys.m_nCCDPort,9600))
			{
				strTip.Format("Open CCD (COM%d) Fail!", g_sys.m_nCCDPort);
				AfxMessageBox(strTip);
				return m_bInit;
			}
		}
	}
	else
	{
		if (!m_socketCCD.IsConnected())
		{
			m_socketCCD.SetSocket(1, g_sys.m_strCCD_IP, g_sys.m_nCCD_IP_Port);		
			if (!m_socketCCD.Connet())
			{
				strTip.Format("Open CCD (%s:%d) Fail!", g_sys.m_strCCD_IP, g_sys.m_nCCD_IP_Port);
				AfxMessageBox(strTip);
				return m_bInit;
			}		
		}	
	}

	m_bInit = TRUE;
	
	if (g_sys.m_nLighterPort)
	{
		if (!m_pCommLighter->IsOpen())
		{
			m_pCommLighter->SetTimeout(1000);
			if (!m_pCommLighter->OpenPort(g_sys.m_nLighterPort))
			{
				strTip.Format("Open Light (COM%d) Fail!", g_sys.m_nLighterPort);
				AfxMessageBox(strTip);
			}
		}
	}
	

	return m_bInit;
}

void CCCDCalibrate::ClearCCDBuffer()
{
	m_pCommCCD->ClearBuffer();
}

void CCCDCalibrate::SendCCDCmd(LPCTSTR szCmd)
{
	CString strLog;

	if (g_sys.m_nCCDTransport==1)
	{
		m_pCommCCD->ClearBuffer();
		m_pCommCCD->WriteLine(szCmd);

		strLog.Format("CCD(COM%d) Send: %s", g_sys.m_nCCDPort, szCmd);
	}
	else
	{
		CString strCommand = szCmd;
		strCommand += "\r";

		if (m_socketCCD.IsConnected())
		{
			m_socketCCD.ClearBuffer();
			m_socketCCD.Send(strCommand);

			strLog.Format("CCD(TCP) Send: %s", szCmd);
		}
		else
		{
			strLog = "CCD(TCP) Send: network is disconnect!";
		}
	}

	WriteRunLog(strLog);
}

CString CCCDCalibrate::RecCCDReply()
{
	//CV-5000型的读到格式：T1,0,+0067.900,+0276.380,-129.910(0x0d)
	//CV-X170F型的读取格式：T1(0x0d)0,+0067.900,+0276.380,-129.910(0x0d)

	CString strRec, strTemp;
	if (g_sys.m_nCCDTransport==1)
	{
		strRec = m_pCommCCD->ReadLine();

		if (strRec.GetLength()>0)
		{
			if (strRec[0]=='T')	//定位命令
			{
				if (strRec.GetLength()==2 || strRec.GetLength()==3) //方诚相机会返回3个字符，从一个逗号
				{
					strRec = strRec.Left(2);
					strRec += ",";
					strRec += m_pCommCCD->ReadLine();
				}
				else if (strRec.GetLength()>3)
				{
					if (strRec[3]=='\r')	//方诚相机返回值：T1,(0x0d)0,+0067.900,+0276.380,-129.910(0x0d)
						strRec.Delete(3);

					if (strRec[2]=='\r')   //基恩仕CCD
						strRec.SetAt(2, ',');
				}
			}
		}

		strTemp.Format("CCD(COM%d) Rec: %s", g_sys.m_nCCDPort,  strRec);
	}
	else
	{
		if (m_socketCCD.IsConnected())
		{
			strRec = m_socketCCD.Receive();
			strRec.TrimRight(); //去除\r

			if (strRec.GetLength()>0)
			{
				if (strRec[0]=='T')
				{
					if (strRec.GetLength()==2)
					{
						strRec += ",";
						strRec += m_socketCCD.Receive();
						strRec.TrimRight();
					}
				}
			}

			strTemp.Format("CCD(TCP) Rec: %s", strRec);
		}
		else
		{
			strTemp = "CCD(TCP) Rec: network is disconnect!";
		}
	}

	WriteRunLog(strTemp);
	return strRec;
}

BOOL CCCDCalibrate::SetLighter(int nLight1, int nLight2)
{
	CString strCMD1, strCMD2, strInput1="$", strInput2="$", strTmp1, strTmp2;
	if (m_pCommLighter->IsOpen())
	{		
		strCMD1.Format("$340%2X", nLight1);
		strCMD2.Format("$310%2X", nLight2);	
		
		WORD wCheck1=0, wCheck2=0;
		for (int i=0; i<6; i++)
		{
			wCheck1^=strCMD1[i];		
			wCheck2^=strCMD2[i];
		}
		strTmp1.Format("%2X", wCheck1);
		strTmp2.Format("%2X", wCheck2);
		
		strCMD1 += strTmp1;
		strCMD2 += strTmp2;
		
		strCMD1.Replace(' ', '0');
		strCMD2.Replace(' ', '0');
		
		if (nLight1>0)
		{
			m_pCommLighter->ClearBuffer();
			m_pCommLighter->WriteString(strCMD1);
			strInput1 = m_pCommLighter->ReadString();
		}
		if (nLight2>0)
		{
			m_pCommLighter->WriteString(strCMD2);
			strInput2 = m_pCommLighter->ReadString();
		}			
	
		if (strInput1!="$" || strInput2!="$")
		{
			strTmp1.Format("设置光源1和2失败!(%s,%s),(%s,%s)", strCMD1, strInput1, strCMD2, strInput2);
			WriteRunLog(strTmp1);
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}


BOOL CCCDCalibrate::SetCurLighter(int Cur ,int nLight)
{
	CString strCMD, strInput, strCheck;
	if (g_ccd.m_pCommLighter->IsOpen())
	{
		g_ccd.m_pCommLighter->ClearBuffer();
		strCMD.Format("$3%d0%02X", Cur, nLight);
		WORD wCheck=0;
		for (int i=0; i<6; i++)	wCheck^=strCMD[i];

		strCheck.Format("%02X", wCheck);
		strCMD += strCheck;

		g_ccd.m_pCommLighter->WriteString(strCMD);
		strInput = g_ccd.m_pCommLighter->ReadString();

		if (strInput!="$")
		{
			CString strMsg;
			strMsg.Format("设置光源(%d)失败!(%s,%s)", Cur, strCMD, strInput);
			WriteRunLog(strMsg);
			return FALSE;
		}
	}
	return TRUE;
}



BOOL CCCDCalibrate::CCDLocate()
{
	m_dbCurWorkX = g_pMotion->GetPos(eMaskX);
	m_dbCurWorkY = g_pMotion->GetPos(eMaskY);
		
	SendCCDCmd("T1");
	CString	strRec = RecCCDReply();
		
	//读取格式：T1,0,+0067.900,+0276.380,-129.910(0x0d)
	//T1后面的是综合判断值，0表示成功，1表示失败
	//T1后面不能是个数，因为有时捕捉到1个相似度很低的对象时，也会传回个数和位置	
	BOOL bJudge;
	CString strMsg;
	BOOL bRecFormat;
	bRecFormat = (sscanf(strRec, "T1,%d,%lf,%lf,%lf", &bJudge, &m_dbTrigX,&m_dbTrigY,&m_dbTrigAngle)==4);
	if (bRecFormat && bJudge==0)
	{
		strMsg.Format(" CCD自动定位OK,图象位置(%f,%f),  旋转角=%.3f。",m_dbTrigX,m_dbTrigY,m_dbTrigAngle);
		m_bCCDMatch=TRUE;		
	}
	else
	{
		m_nErrorCount++;			
		strMsg.Format("CCD系统自动定位失败,可能是图像不匹配，请重试！", m_nErrorCount);
		m_bCCDMatch = FALSE;
	}
	WriteRunLog(strMsg);		
	return m_bCCDMatch;
}

void CCCDCalibrate::SetWorkMode(int nMode)
{
	CString strCmd;
	if (nMode==eProgMode)
		strCmd.Format(_T("%s%c"),"O0",0x0D);
	else if (nMode==eRunMode)
		strCmd.Format(_T("%s%c"),"R0",0x0D);

	SendCCDCmd(strCmd);
}

BOOL CCCDCalibrate::SetWorkProgNo(int nProgId)
{
	if (nProgId==-1) //-1表示尚未做ＣＣＤ模块，只是正常的编辑图形
		return TRUE; 

	CString strCmd, strReceive, strTemp;

	if (g_sys.m_nCCDType==0)
		strCmd.Format(_T("%s,IN,%.3d"),"PW",nProgId);//改变程序编号CV3000,CV5000
	else
		strCmd.Format(_T("%s,%d,%.3d"),"PW",1,nProgId);//改变程序编号CV-X170F
	
	SendCCDCmd(strCmd);
	strReceive = RecCCDReply();
		
	BOOL bRet;	
	if (strReceive=="PW")
	{
		strTemp.Format("切换CCD程序%d成功!", nProgId);
		bRet = TRUE;
	}
	else
	{
		strTemp.Format("切换CCD程序%d失败! 发送(%s), 接收(%s)", nProgId, strCmd, strReceive);
		bRet = FALSE;
	}
	WriteRunLog(strTemp);
	return bRet;
}


void CCCDCalibrate::GetTrigOK(double &dbX, double &dbY, double &dbAngle)
{
	dbX=m_dbTrigX; 
	dbY=m_dbTrigY;
	dbAngle=m_dbTrigAngle; 
}

void CCCDCalibrate::GetWorkPos(double &x, double &y)
{	
	x=m_dbCurWorkX;
	y=m_dbCurWorkY;
}

void CCCDCalibrate::GetTrigMM(BOOL bCCD1, double &x, double &y,double& degree)
{	
	double dbXmm,dbYmm,dbDegree1;
	if (bCCD1)
	{
		dbXmm=(m_dbTrigX - g_sys.m_nCCD1PixelX/2)*g_ParaIni.m_dbXMMPerPixel;
		dbYmm=(g_sys.m_nCCD1PixelY/2 - m_dbTrigY)*g_ParaIni.m_dbYMMPerPixel;//打标切割对比：切割符号取反
	}
	else
	{
		dbXmm=(m_dbTrigX - g_sys.m_nCCD2PixelX/2)*g_ParaIni.m_dbXMMPerPixel2;
		dbYmm=(g_sys.m_nCCD2PixelY/2 -m_dbTrigY)*g_ParaIni.m_dbYMMPerPixel2;//打标切割对比：切割符号取反
	}
	
	x=dbXmm;
	y=dbYmm;
	dbDegree1=m_dbTrigAngle;	
}


void CCCDCalibrate::GetWorkPosPointToCCDCenter(BOOL bCCD1, double &x, double &y,double& degree)
{	
	double dbXmm,dbYmm,dbDegree1;	
	if (bCCD1)
	{
		dbXmm=(g_sys.m_nCCD1PixelX/2 - m_dbTrigX)*g_ParaIni.m_dbXMMPerPixel;
		dbYmm=-(g_sys.m_nCCD1PixelY/2 - m_dbTrigY)*g_ParaIni.m_dbYMMPerPixel;//打标切割对比：切割符号取反
	}
	else
	{
		dbXmm=(g_sys.m_nCCD2PixelX/2 - m_dbTrigX)*g_ParaIni.m_dbXMMPerPixel2;
		dbYmm=-(g_sys.m_nCCD2PixelY/2 - m_dbTrigY)*g_ParaIni.m_dbYMMPerPixel2;//打标切割对比：切割符号取反
	}
	
	//计算CCD中心在工作台坐标系中的坐标：
	x=m_dbCurWorkX-dbXmm;
	y=m_dbCurWorkY-dbYmm;
	dbDegree1=m_dbTrigAngle;	
}

void CCCDCalibrate::MoveToCCDCenter(BOOL bCCD1, double xPixel, double yPixel, double& xWorkMM, double& yWorkMM)
{
	double dbMoveXLength,dbMoveYLength;
	if (bCCD1)
	{
		dbMoveXLength=(g_sys.m_nCCD1PixelX/2 - xPixel)*g_ParaIni.m_dbXMMPerPixel;
		dbMoveYLength=(g_sys.m_nCCD1PixelY/2 - yPixel)*g_ParaIni.m_dbYMMPerPixel;
	}
	else
	{
		dbMoveXLength=(g_sys.m_nCCD2PixelX/2 - xPixel)*g_ParaIni.m_dbXMMPerPixel2;
		dbMoveYLength=(g_sys.m_nCCD2PixelY/2 - yPixel)*g_ParaIni.m_dbYMMPerPixel2;
	}
	xWorkMM+=dbMoveXLength;
	yWorkMM-=dbMoveYLength;
}

void CCCDCalibrate::MoveToCCDCenter(BOOL bCCD1, double &xWorkPosMM, double &yWorkPosMM)
{
	double dbMoveXLength,dbMoveYLength;
	if (bCCD1)	//粗定位CCD
	{
		dbMoveXLength=(g_sys.m_nCCD1PixelX/2 - m_dbTrigX)*g_ParaIni.m_dbXMMPerPixel;
		dbMoveYLength=(g_sys.m_nCCD1PixelY/2 - m_dbTrigY)*g_ParaIni.m_dbYMMPerPixel;
	}
	else
	{
		dbMoveXLength=(g_sys.m_nCCD2PixelX/2 - m_dbTrigX)*g_ParaIni.m_dbXMMPerPixel2;
		dbMoveYLength=(g_sys.m_nCCD2PixelY/2 - m_dbTrigY)*g_ParaIni.m_dbYMMPerPixel2;
	}
	xWorkPosMM=m_dbCurWorkX+dbMoveXLength;
	yWorkPosMM=m_dbCurWorkY-dbMoveYLength;
}

void CCCDCalibrate::CCD2ToCCD1Center(double &xWorkPosMM, double &yWorkPosMM)
{
	double dbMoveXLength,dbMoveYLength;
	dbMoveXLength=(g_sys.m_nCCD2PixelX/2-m_dbTrigX)*g_ParaIni.m_dbXMMPerPixel;
	dbMoveYLength=-(g_sys.m_nCCD2PixelY/2-m_dbTrigY)*g_ParaIni.m_dbYMMPerPixel;
	xWorkPosMM=m_dbCurWorkX-dbMoveXLength + (g_ParaIni.m_dbMarkToCCD1X - g_ParaIni.m_dbMarkToCCD2X);
	yWorkPosMM=m_dbCurWorkY-dbMoveYLength + (g_ParaIni.m_dbMarkToCCD1Y - g_ParaIni.m_dbMarkToCCD2Y);
}

double CCCDCalibrate::GetCaptPrecision(BOOL bCCD1)
{
	double xPixelerr=fabs(g_sys.m_nCCD2PixelX/2 - m_dbTrigX);
	double yPixelerr=fabs(g_sys.m_nCCD2PixelY/2 - m_dbTrigY);
	if (bCCD1)
	{
		xPixelerr=fabs(g_sys.m_nCCD1PixelX/2 - m_dbTrigX);
		yPixelerr=fabs(g_sys.m_nCCD1PixelY/2 - m_dbTrigY);
	}
	xPixelerr=xPixelerr>yPixelerr?xPixelerr:yPixelerr;
	return xPixelerr;
}

void CCCDCalibrate::DoEvent()
{
	MSG message;
	while( ::PeekMessage(&message,NULL,0,0,PM_REMOVE) )
	{
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
}

BOOL CCCDCalibrate::SaveCCDScreen(LPCTSTR szInfo)
{
	CString strTip;
	DWORD nTotalNum = 0;
	
	m_pCommCCD->ClearBuffer();
	m_pCommCCD->WriteString("BC,CM");
	BYTE *buffer = m_pCommCCD->ReadData(&nTotalNum);
	
	int nBmpSize = 0;
	int nNum = sscanf((char *)buffer, "BC,%d,", &nBmpSize);
	if (nTotalNum<1024*1024 || nBmpSize==0)
	{
		strTip.Format("获取图像失败, RecNum:%d, BmpSize:%d", nTotalNum, nBmpSize);
		WriteRunLog(strTip);
		return FALSE;
	}
	
	CFileStatus status;
	CTimeSpan timeSpan(30,0,0,0);	//默认保留30天的日志
	CString strBmpFile, strCurDir, strDelDir, strTmpDir;
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeDel = timeCur - timeSpan;
	if (timeCur.Format("%Y%m%d").IsEmpty())	//LZQ,20130121
	{
		AfxMessageBox("WriteRunLog: system time error!");
		return FALSE;
	}
	
	strCurDir.Format("D:\\CCD\\%s", timeCur.Format("%Y%m%d"));
	strDelDir.Format("D:\\CCD\\%s", timeDel.Format("%Y%m%d"));
	strBmpFile.Format("%s\\%s_%s.bmp", strCurDir, timeCur.Format("%Y%m%d_%H%M%S"), szInfo);	
	
	strTmpDir = "D:\\CCD";
	if (!CFile::GetStatus(strTmpDir, status))
		::CreateDirectory(strTmpDir, NULL);
	
	if (!CFile::GetStatus(strCurDir, status))
		::CreateDirectory(strCurDir, NULL);	
	
	if (CFile::GetStatus(strDelDir, status))
	{
		CFileFind finder;
		BOOL bWorking = finder.FindFile(strDelDir+"\\*.*");
		while (bWorking)
		{
			bWorking = finder.FindNextFile();		
			::DeleteFile(finder.GetFilePath());
		}
		finder.Close();
		::RemoveDirectory(strDelDir);
	}
	
	CFile fileBmp;
	if (fileBmp.Open(strBmpFile, CFile::modeCreate|CFile::modeReadWrite))
	{
		strTip.Format("保存图像成功: %s", strBmpFile);
		WriteRunLog(strTip);

	}
	else
	{
		strTip.Format("保存图像失败: %s", strBmpFile);
		WriteRunLog(strTip);
		return FALSE;
	}	

	fileBmp.Write(buffer+11, nBmpSize-1);
	fileBmp.Close();
	return TRUE;
}