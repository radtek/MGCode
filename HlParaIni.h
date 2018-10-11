// HlParaIni.h: interface for the HlParaIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLPARAINI_H__F0C55C9E_2E82_4081_B72B_C4CB3A8371A0__INCLUDED_)
#define AFX_HLPARAINI_H__F0C55C9E_2E82_4081_B72B_C4CB3A8371A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HlIni.h"

#define   IniFile   _T("HLPara.ini")


class CHlParaIni : public CHlIni  
{
public:
	CHlParaIni();
	virtual ~CHlParaIni(){};

	void LoadPara();
	void SavePara();

	//	int m_nRefPointType;  //�ο�������: 0��ʾ��(0,0)��Ϊ���ĵ�, 1��ʾ��(0,0)��Ϊ���Ͻǣ�
	
	//	int m_nCCDNum;
	//	int m_nCCD1PixelX;
	//	int m_nCCD1PixelY;
	//	int m_nCCD2PixelX;
	//	int m_nCCD2PixelY;

	
	//BOX
	int m_nRtcBitmm;
	double m_dbScaleX;
	double m_dbSacleY;
	double m_dbCCWAngle;
	CString m_strCaliFile;


	//Workbench
	int m_nMotionDelayForCCD;
	int m_nMotionDelayForMark;
	double m_dbRefPointX;
	double m_dbRefPointY;
	double m_dbMarkToCCD1X;
	double m_dbMarkToCCD1Y;
	double m_dbMarkToCCD2X;
	double m_dbMarkToCCD2Y;

	double	m_dbWorkbenchFixtureAngle;

	 BOOL m_bUseCutBlow;

//	int m_bEnableAutoChangeFocus;
//	double m_dbZCCDFocus;
//	double m_dbZMarkFocus;

	//Divide
	double m_dbDivideX;
	double m_dbDivideY;
	double m_dbDivideAddX;
	double m_dbDivideAddY;

	//CCD
	double m_dbXMMPerPixel;
	double m_dbYMMPerPixel;
	double m_dbXMMPerPixel2;
	double m_dbYMMPerPixel2;
	double  m_dbCCD1Precision;
	double  m_dbCCD2Precision;

	//PowerMeter
	double m_dbMaxPower;
	double m_dbMinPower;
	double m_dbLastPower;
	double m_dbPowerCheckX;
	double m_dbPowerCheckY;
	double m_dbPowerCheckZ;


	//DXF
	double m_dbDxfPrecision;
	int	m_nDxfUnit;

	//Gerber
	double m_dbGerberOffset;

	//AutoOffset;
	CString m_strAutoOffsetFile;
	double m_dbAutoOffsetLastX;
	double m_dbAutoOffsetLastY;
	double m_dbAutoOffsetDeviationMax;
	double m_dbAutoOffsetDeviationMin;

	//PowerTest
	double m_dbPowerTestTotalTime;
	double m_dbPowerCheckInterval;
	int m_nPowerCheckWait;

	//�Ƿ�ʹ�ÿ��ٶ�λ
	BOOL m_bUseQPosite;
	double m_dbWorkPosX;
	double m_dbWorkPosY;
	int m_nCloseDoorTime;

	//��ȡ������Ȩ�޵�½����
	//nType=1==>���Լ�
	//nType=2==>����Ա��
	CString GetSoftLoginPwd(int nType=2)
	{
		if (nType==1)
			return ReadString2(_T("SOFTWARE"),_T("MIDLOGIN"),_T(""));
		else
			return ReadString2(_T("SOFTWARE"),_T("ADMINLOGIN"),_T(""));
	}
	//���ô�����Ȩ�޵�½����
	//nType=1==>���Լ�
	//nType=2==>����Ա��
	void SetSoftLoginPwd(int nType,CString strPwd)
	{
		if (nType==1)
			WriteString(_T("SOFTWARE"),_T("MIDLOGIN"),strPwd);
		else
			WriteString(_T("SOFTWARE"),_T("ADMINLOGIN"),strPwd);
	}

	//�ܼ򵥵ļ����㷨��������ܣ����������
	CString EncryptPwd(CString strPwd)
	{
		CString strDest,strMid;
		strDest.Empty();
		char ch,chP;
		TCHAR chKey[]=_T("Hi,howareyou!");

		unsigned int nLen = strPwd.GetLength();
		for (unsigned int i=0; i<nLen; i++)
		{
			if (i<::_tcslen(chKey))///_tcslen
			{
				chP=strPwd.GetAt(i);
				if (chP > chKey[i])
					ch=chP-chKey[i];
				else
					ch=chKey[i]-chP;
				if (ch<=0)   ch^=ch;
				if (ch>=255) ch^=chKey[i];
			}
			else 
			{
				unsigned int nPos = i-::_tcslen(chKey)+1;
				while(nPos>=::_tcslen(chKey))
					nPos=nPos-::_tcslen(chKey)+3;

				chP = strPwd.GetAt(i);
				if (chP>chKey[nPos])
					ch=chP-chKey[nPos];
				else
					ch=chKey[nPos]-chP;
				if (ch<=0)   ch ^=ch;
				if (ch>=255) ch ^=chKey[nPos];
			}
			strMid.Format(_T("%d"),ch);
			strDest+=strMid;
		}
		return strDest;
	}
	////////////////////////////////////
	///��ȡԴ�ļ�·��
	CString GetSourceFileFolder()
	{
		return ReadString2(_T("FILE"),_T("SOURCEFOLDER"),_T("D:\\Test"));
	}
	void SetSourceFileFolder(CString strPath)
	{
		WriteString(_T("FILE"),_T("SOURCEFOLDER"),strPath);
	}
	////////////////////////////////////
	///��ȡĿ���ļ�·��
	CString GetDestFileFolder()
	{
		return ReadString2(_T("FILE"),_T("DESTFOLDER"),_T("D:\\"));
	}
	void SetDestFileFolder(CString strPath)
	{
		WriteString(_T("FILE"),_T("DESTFOLDER"),strPath);
	}
	//////////////////////////////////
	/// ��ȡ���ݿ��½�û���
	CString GetDbLoginUser()
	{
		return ReadString2(_T("DATABASE"),_T("USER"),_T(""));
	}
	void SetDbLoginUser(CString strUser){
		WriteString(_T("DATABASE"),_T("USER"),strUser);
	}
	////////////////////////////////////
	/// ��ȡ���ݿ��½����
	CString GetDbLoginPwd()
	{
		return ReadString2(_T("DATABASE"),_T("PWD"),_T(""));
	}
	void SetDbLoginPwd(CString strPwd)
	{
		WriteString(_T("DATABASE"),_T("PWD"),strPwd);
	}
	//////////////////////////////////
	/// ��ȡ���ݿ�����
	CString GetDbDatabase()
	{
		return ReadString2(_T("DATABASE"),_T("DATABASE"),_T(""));
	}
	void SetDbDatabase(CString strName)
	{
		WriteString(_T("DATABASE"),_T("DATABASE"),strName);
	}
	/////////////////////////////////
	/// ��ȡ���ݿ����������
	CString GetDbServer()
	{
		return ReadString2(_T("DATABASE"),_T("SERVER"),_T("127.0.0.1"));
	}
	void SetDbServer(CString strName)
	{
		WriteString(_T("DATABASE"),_T("SERVER"),strName);
	}
	//////////////////////////
	int GetDbDataType()
	{
		return ReadInt(_T("DATABASE"),_T("DATATYPE"),0);
	}
	void SetDbDataType(int nType)
	{
		WriteInt(_T("DATABASE"),_T("DATATYPE"),nType);
	}
	//////////////////////////
	int GetComNo()
	{
		return ReadInt(_T("COMPARA"),_T("COMNO"),1);
	}
	void SetComNo(int nNo)
	{
		WriteInt(_T("COMPARA"),_T("COMNO"),nNo);
	}
	//////////////////////////
	int GetComBaud(){
		return ReadInt(_T("COMPARA"),_T("BAUDRATE"),9600);
	}
	void SetComBaud(int nBaud)
	{
		WriteInt(_T("COMPARA"),_T("BAUDRATE"),nBaud);
	}
	//////////////////////////
	int GetComDataBit()
	{
		return ReadInt(_T("COMPARA"),_T("DATABIT"),8);
	}
	void SetComDataBit(int nBit)
	{
		WriteInt(_T("COMPARA"),_T("DATABIT"),nBit);
	}
	//////////////////////////
	int GetComParity()
	{
		return ReadInt(_T("COMPARA"),_T("PARITY"),0);
	}
	void SetComParity(int nParity)
	{
		WriteInt(_T("COMPARA"),_T("PARITY"),nParity);
	}
	//////////////////////////
	int GetComStopBit()
	{
		return ReadInt(_T("COMPARA"),_T("STOPBIT"),1);
	}
	void SetComStopBit(int nStopBit)
	{
		WriteInt(_T("COMPARA"),_T("STOPBIT"),nStopBit);
	}
	///////////////////////////////////////
	int GetBarComNo()
	{
		return ReadInt(_T("COMPARA"),_T("BARCOMNO"),1);
	}
	void SetBarComNo(int nNo)
	{
		WriteInt(_T("COMPARA"),_T("BARCOMNO"),nNo);
	}
	//////////////////////////
	int GetBarComBaud(){
		return ReadInt(_T("COMPARA"),_T("BARBAUDRATE"),9600);
	}
	void SetBarComBaud(int nBaud)
	{
		WriteInt(_T("COMPARA"),_T("BARBAUDRATE"),nBaud);
	}
	//////////////////////////
	int GetBarComDataBit()
	{
		return ReadInt(_T("COMPARA"),_T("BARDATABIT"),8);
	}
	void SetBarComDataBit(int nBit)
	{
		WriteInt(_T("COMPARA"),_T("BARDATABIT"),nBit);
	}
	//////////////////////////
	int GetBarComParity()
	{
		return ReadInt(_T("COMPARA"),_T("BARPARITY"),0);
	}
	void SetBarComParity(int nParity)
	{
		WriteInt(_T("COMPARA"),_T("BARPARITY"),nParity);
	}
	//////////////////////////
	int GetBarComStopBit()
	{
		return ReadInt(_T("COMPARA"),_T("BARSTOPBIT"),1);
	}
	void SetBarComStopBit(int nStopBit)
	{
		WriteInt(_T("COMPARA"),_T("BARSTOPBIT"),nStopBit);
	}
	///////////////////////////////////////
	//3Mֽ�е����
	double GetPaperDist()
	{
		return ReadDouble(_T("PAPER"),_T("DISTANCE"),70);
	}
	void SetPaperDist(double dbDist)
	{
		WriteDouble(_T("PAPER"),_T("DISTANCE"),dbDist);
	}
	////////////////////////////////////////
	//3Mֽ�и���
	double GetPaperDivDist()
	{
		return ReadDouble(_T("PAPER"),_T("DIVDISTANCE"),2);
	}
	void SetPaperDivDist(double dbDist)
	{
		WriteDouble(_T("PAPER"),_T("DIVDISTANCE"),dbDist);
	}
	///////////////////////////////////////
	//3Mֽ��ֽ�ٶ�
	int GetPaperMoveSpeed()
	{
		return ReadInt(_T("PAPER"),_T("SPEED"),100);
	}
	void SetPaperMoveSpeed(int nSpeed)
	{
		WriteInt(_T("PAPER"),_T("SPEED"),nSpeed);
	}
};

extern CHlParaIni g_ParaIni;

#endif // !defined(AFX_HLPARAINI_H__F0C55C9E_2E82_4081_B72B_C4CB3A8371A0__INCLUDED_)
