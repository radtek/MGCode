// Text.cpp: implementation of the CText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "Text.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CText::CText(UINT nTextID)
{
	m_nTextID = nTextID;			//�ı�ID
	m_nMarkTimes = 0;				//��ǰ������
	m_iDataset =2;					//�����ַ�������

	m_bFill=FALSE;					//�Ƿ���λ��
	m_nJumpDis = 1;					//�������
	m_bHex = FALSE;					//�Ƿ���16������ʾ
	m_bRand = FALSE;				//�Ƿ�ȡ���������
	m_nFromFile=0;					//�Ƿ�ȡ�����ļ�
	m_nJumpStart=0;					//���ſ�ʼ��
	m_nJumpCurrent=0;				//���ŵ�ǰֵ
	m_nJumpEnd=999999;				//���Ž�����
	m_nJumpLen=6;					//���ų���
	m_nJumpStep=1;					//����
	//m_nJumpInterval=1;			//���
	m_strPrefix=_T("0");			//��������
	m_saSerial.SetSize(0);

	m_strLeft = "";					//�����������
	m_strRight = "";				//�Ҳ���������
	m_strFromFile = "";				//ȡ�����ļ����ļ���

	m_nText=TEXT_FIX;				   //�ı���ʽ:0�̶��ı�1�����ı�3�����ı�
	m_strFix=_T("ABCDEFG");	           //�ı����ݹ̶������ı�
	m_strDate=_T("%Y/%m/%d,%H:%M:%S"); //���ڸ�ʽ
	m_strBinText = _T("0123456789");

	m_bBarCode = FALSE;
	m_bModel=TRUE;
	m_strModelNo="0";

	m_bUse = TRUE;

	m_pBarCodeBin = NULL;
	m_bBarCode = FALSE;
	m_nBarType		= 0;
	m_strBarString	= "";

	m_bOverMark=FALSE;
}

CText::~CText()
{
	if(m_pBarCodeBin)
		delete []m_pBarCodeBin;
}

//////////////////////////////////////////////////////////////////////////
//CopyToMe �� �� ������
//CopyToMe��ID�����븴�ƣ����������帴���Ұ���ID
//////////////////////////////////////////////////////////////////////////
void CText::operator=(const CText& src)
{
	m_nMarkTimes	= src.m_nMarkTimes;
	m_iDataset		= src.m_iDataset;
	m_nTextID		= src.m_nTextID;
	m_bFill			= src.m_bFill;
	m_nJumpDis		= src.m_nJumpDis;
	m_bHex			= src.m_bHex;
	m_bRand			= src.m_bRand;
	m_nFromFile		= src.m_nFromFile;
	m_nJumpStart	= src.m_nJumpStart;
	m_nJumpEnd		= src.m_nJumpEnd;
	m_nJumpLen		= src.m_nJumpLen;
	m_nJumpStep		= src.m_nJumpStep;
	m_strPrefix		= src.m_strPrefix;
	int nCount = src.m_saSerial.GetSize();
	CString strInfo;

	for(int i=0;i<nCount;i++)
	{
		strInfo = src.m_saSerial.GetAt(i);
		m_saSerial.Add(strInfo);
	}
	m_bModel		=src.m_bModel;
	m_strModelNo	=src.m_strModelNo;

	m_nText			= src.m_nText;
	m_strFix		= src.m_strFix;
	m_strDate		= src.m_strDate;
	m_strFix		= src.m_strFix;
	m_strLeft		= src.m_strLeft;
	m_strRight		= src.m_strRight;
	m_strFromFile	= src.m_strFromFile;
	m_strBinText	= src.m_strBinText;
	m_bBarCode		= src.m_bBarCode;
	m_nBarType		= src.m_nBarType;
	m_strBarString	= src.m_strBarString;
}

void CText::Serialize(FILE* fp,BOOL bFlag)
{
	if(!m_bUse)
		return;

	//����׼�����ı���ʽ��������¼��
	CMySerialize fs(fp);

	if(bFlag)
	{
		fs<<"TXT_B";
		fs<<100;
		fs<<m_bHex;				//�Ƿ���16������ʾ
		fs<<101;
		fs<<m_bRand;			//�Ƿ�ȡ���������
		fs<<102;
		fs<<m_nJumpDis;			//�������
		fs<<103;
		fs<<m_strText;			//�ı�����
		fs<<104;
		fs<<m_nText;			//�ı���ʽ:0�̶��ı�1�����ı�3�����ı�
		fs<<105;
		fs<<m_strDate;			//���ڸ�ʽ
		fs<<106;
		CString strTmp = m_strFix;
		strTmp.Replace("\r\n","\01");
		fs<<strTmp;				//�̶������ı�
		fs<<107;
		fs<<m_nMarkTimes;		//��ǰ������
		fs<<108;
		fs<<"TEXT_FILE_BEGIN";	//ȡ�����ļ�����ʱ�����ļ�����ȡ��
		int n = m_saSerial.GetSize();
		for(int i=0;i<n;i++)
		{
			fs<<m_saSerial.GetAt(i);
		}
		fs<<"TEXT_FILE_END";	
		fs<<109;
		fs<<m_strLeft;
		fs<<110;
		fs<<m_strRight;
		fs<<111;
		fs<<m_strPrefix;
		fs<<112;
		fs<<m_strFromFile;		//ȡ�����ļ����ļ���
		fs<<113;
		fs<<m_nJumpStart;		//���ſ�ʼ��
		fs<<114;
		fs<<m_nJumpEnd;			//���Ž�����
		fs<<115;
		fs<<m_nJumpLen;			//���ų���
		fs<<116;
		fs<<m_nJumpStep;		//����
		fs<<117;
		fs<<m_nFromFile;		//�Ƿ�ȡ�����ļ�
		fs<<118;
		fs<<m_bFill;			//�Ƿ���λ��
		fs<<119;
		fs<<m_nTextID;
		fs<<120;
		fs<<m_iDataset;			//�����ַ�������
		fs<<121;
		fs<<m_strBinText;		//�����ı�����
		fs<<122;
		fs<<m_bBarCode;			//�Ƿ���������Ϣ
		fs<<123;
		fs<<m_nBarType;			//��������
		fs<<124;
		fs<<m_bModel;
		fs<<125;
		fs<<m_strModelNo;
		fs<<"TXT_E";
	}
	else
	{
		CString str;
		while(fs>>str)
		{
			switch(atoi(str)) 
			{
			case 100:
				{
					fs>>m_bHex;				//�Ƿ���16������ʾ
					if(m_bHex)
						m_strBinText = _T("0123456789ABCDEF");
					break;
				}
			case 101:
				fs>>m_bRand;			//�Ƿ�ȡ���������
				break;
			case 102:
				fs>>m_nJumpDis;			//�������
				break;
			case 103:
				fs>>m_strText;			//�ı�����
				break;
			case 104:
				fs>>m_nText;			//�ı���ʽ:0�̶��ı�1�����ı�3�����ı�
				break;
			case 105:
				fs>>m_strDate;			//���ڸ�ʽ
				break;
			case 106:
				{
					CString strTmp;
					fs>>strTmp;			//�̶������ı�
					strTmp.Replace("\01","\r\n");
					m_strFix = strTmp;
					break;
				}
			case 107:
				fs>>m_nMarkTimes;		//��ǰ������
				break;
			case 108:
				{
					CString strTmp;
					BOOL bFlag=FALSE;
					m_saSerial.RemoveAll();

					while(fs>>strTmp)
					{
						if(strTmp == "TEXT_FILE_BEGIN")
						{
							bFlag=TRUE;
							continue;
						}
						if(strTmp == "TEXT_FILE_END")
							break;
						if(bFlag)
						{
							m_saSerial.Add(strTmp);
						}

					}
					break;
				}
			case 109:
				fs>>m_strLeft;
				break;
			case 110:
				fs>>m_strRight;
				break;
			case 111:
				fs>>m_strPrefix;
				break;
			case 112:
				fs>>m_strFromFile;		//ȡ�����ļ����ļ���
				break;
			case 113:
				fs>>m_nJumpStart;		//���ſ�ʼ��
				break;
			case 114:
				fs>>m_nJumpEnd;			//���Ž�����
				break;
			case 115:
				fs>>m_nJumpLen;			//���ų���
				break;
			case 116:
				fs>>m_nJumpStep;		//����
				break;
			case 117:
				fs>>m_nFromFile;		//�Ƿ�ȡ�����ļ�
				break;
			case 118:
				fs>>m_bFill;			//�Ƿ���λ��
				break;
			case 119:
				fs>>m_nTextID;
				break;
			case 120:
				fs>>m_iDataset;			//�����ַ�������
				break;
			case 121:
				fs>>m_strBinText;		//�����ı�����
				break;
			case 122:
				fs>>m_bBarCode;			//�Ƿ���������Ϣ
				break;
			case 123:
				fs>>m_nBarType;
				break;
			case 124:
				fs>>m_bModel;			//ģ���ı�
				break;
			case 125:
				fs>>m_strModelNo;		//ģ���ı�
				break;
			case 0:
				if (str=="TXT_E")
					goto TEXT_SRRI_END;
			default:
				break;
			}
		}
TEXT_SRRI_END:
		;
	}
}

void CText::CopyToMe(CText src)
{
	m_nMarkTimes	= src.m_nMarkTimes;
	m_iDataset		= src.m_iDataset;
//	m_nTextID		= src.m_nTextID;
	m_bFill			= src.m_bFill;
	m_nJumpDis		= src.m_nJumpDis;
	m_bHex			= src.m_bHex;
	m_bRand			= src.m_bRand;
	m_nFromFile		= src.m_nFromFile;
	m_nJumpStart	= src.m_nJumpStart;
	m_nJumpEnd		= src.m_nJumpEnd;
	m_nJumpLen		= src.m_nJumpLen;
	m_nJumpStep		= src.m_nJumpStep;
	m_strPrefix		= src.m_strPrefix;
	
	m_saSerial.RemoveAll();
	int nCount = src.m_saSerial.GetSize();
	CString strInfo;
	
	for(int i=0;i<nCount;i++)
	{
		strInfo = src.m_saSerial.GetAt(i);
		m_saSerial.Add(strInfo);
	}
	m_bModel		= src.m_bModel;
	m_strModelNo	= src.m_strModelNo;
	m_nText			= src.m_nText;
	m_strFix		= src.m_strFix;
	m_strDate		= src.m_strDate;
	m_strFix		= src.m_strFix;
	m_strLeft		= src.m_strLeft;
	m_strRight		= src.m_strRight;
	m_strFromFile	= src.m_strFromFile;
	m_strBinText	= src.m_strBinText;
	m_bBarCode		= src.m_bBarCode;
	m_nBarType		= src.m_nBarType;
	m_strBarString	= src.m_strBarString;

	m_bOverMark	=src.m_bOverMark;
}

void CText::GenString(DWORD nStyle)
{
	switch(m_nText)
	{
	case TEXT_FIX:
		m_strText=m_strFix;
		break;
	case TEXT_SERIAL:
		{
			int cur=0;
			if(m_nJumpDis<=0)
				m_nJumpDis = 1;
			
			cur =(int)(m_nJumpStart+m_nJumpStep*(int)(m_nMarkTimes/m_nJumpDis));
			if(cur>=m_nJumpEnd)
			{				
				if(m_nFromFile){
					m_strText=m_saSerial[m_nJumpEnd-1];
				}
				else{
					m_strText = FormatString(m_nJumpEnd);
					if(m_bFill){	
						int n;
						int sign,dec;
						CString s = _fcvt(m_nJumpEnd,0,&dec,&sign);
						n=m_nJumpLen-m_strText.GetLength();
						for(int j=0;j<n;j++)
							m_strText=m_strPrefix+m_strText;
					}
					m_strText=m_strLeft+m_strText+m_strRight;
					//end add
				}
				//add ��ʾ��Ϣ
				// Give instructions in the statusbar
				m_bOverMark=TRUE;
				
			}
			else{
				m_bOverMark=FALSE;
				if(m_nFromFile){
					//ȡ���ļ�
					m_strText=m_saSerial[cur];
				}
				else{
					//�Զ�������		

					m_strText = FormatString(cur);
					if(m_bFill){
						int n;
						int sign,dec;
						CString s = _fcvt(m_nJumpEnd,0,&dec,&sign);
						n=m_nJumpLen-m_strText.GetLength();
						for(int j=0;j<n;j++)
							m_strText=m_strPrefix+m_strText;
					}
					m_strText=m_strLeft+m_strText+m_strRight;
				}
			}
			break;
		}
	case TEXT_DATE:
		CTime t;
		t=CTime::GetCurrentTime();
		m_strText=t.Format(m_strDate);
		break;
	}

	if(m_bBarCode)
		GenBarString(nStyle);
}

CString CText::GetString(DWORD nStyle)
{
	//�õ�����У������
	m_bUse = TRUE;
	return m_strText;
}

CString CText::GetInfo()
{
	FlashNextText(0x00);
	return GetString(0x00);
}

CString CText::FormatString(int nVal)
{
	//�����������
	int nCount = m_strBinText.GetLength();
	char szInfo[256];
	memset(szInfo,0,256);
	
	int nTmp;
	UINT nPow;
	int nPos=254;
	int i=1;
	nPow = (UINT)(pow(nCount*1.0,1.0)+0.5);
	//1����ʱҪ���⴦��
	if (nCount==1&&nVal!=0)
	{
		for (int j=0;j<nVal;j++)
		{
			szInfo[nPos] = m_strBinText.GetAt(0);
			if (nPos==0){
				break;
			}
			nPos--;
		}
	}
	else
	{
		while(1)
		{
			nTmp = nVal%nPow;
			szInfo[nPos] = m_strBinText.GetAt(nTmp);
			i++;
			if (nPos==0){
				break;
			}
			nPos--;
			if((UINT)nVal<nPow)
				break;
			nVal = nVal/nPow;
		}
	}	
	CString strRtn;
	strRtn.Format("%s",szInfo+nPos+1);	
	return strRtn;
}

//����������򷵻�TRUE�����򷵻�FALSE
//0x01 ������һ���ַ�
//0x03 ȷ������ʱ�Ƿ��õ��˲���
//0x00 ��������һ���ַ���ֻ��������ͼ��ĵ�һ�γ�ʼ������
//0x04 ˢ�½����á�ʵ�������ѱ��
BOOL CText::FlashNextText(DWORD nStyle)
{
	if(0x01 == nStyle)
		m_nMarkTimes++;

	GenString(nStyle);

	if(m_bModel&&m_bChangeFlag)
	{
		return TRUE;
	}
	
	if((m_nText == TEXT_FIX)&&(nStyle == 0x00))
		return FALSE;

	return TRUE;
}

BOOL CText::GenBarString(UINT nStyle)
{
	CString strText;
	BOOL bRtn = TRUE;
	switch(m_nBarType) {
	case EAN8:
		{
			//���֣�λ��ֻ��Ϊ7λ��1��У��λ
			CaseNum(m_strText);
			strText = m_strText;

			int nLen = strText.GetLength();
			if(nLen>7){
				strText.SetAt(7,TCHAR(0X00));
			}
			else{
				for(int i=0;i<7-nLen;i++)
					strText.Insert(0,'0');
			}
			m_strText = strText;
			break;
		}
	case EAN13:
		{
			//���֣�λ��ֻ��Ϊ12λ��1��У��λ
			CaseNum(m_strText);
			strText = m_strText;

			int nLen = strText.GetLength();
			if(nLen>7){
				strText.SetAt(12,TCHAR(0X00));
			}
			else{
				for(int i=0;i<12-nLen;i++)
					strText.Insert(0,'0');
			}
			m_strText = strText;
			break;
		}
	case EAN128:
	case CODE128A:
	case CODE128B:
	case CODE128C:
	case CODE128AUTO:
	case INT25:
		strText = m_strText;
		break;
	case CODE39:
	case EXT39:
		{
			strText = m_strText;
			m_strText = "*"+m_strText+"*";
			break;
		}
	case UPCA:
		{
			//���֣�λ��ֻ��Ϊ11λ��1��У��λ
			CaseNum(m_strText);
			strText = m_strText;

			int nLen = strText.GetLength();
			if(nLen>7){
				strText.SetAt(11,TCHAR(0X00));
			}
			else{
				for(int i=0;i<11-nLen;i++)
					strText.Insert(0,'0');
			}
			m_strText = strText;
			
			break;
		}
	case UPCE:
		{
			//���֣�λ��ֻ��Ϊ6λ��2��У��λ
			CaseNum(m_strText);
			strText = m_strText;
			
			int nLen = strText.GetLength();
			if(nLen>7){
				strText.SetAt(6,TCHAR(0X00));
			}
			else{
				for(int i=0;i<6-nLen;i++)
					strText.Insert(0,'0');
			}
			m_strText = strText;
			break;
		}
	case PDF417:
	case DATAMATRIX:
	case QR:
		strText = m_strText;
		break;
	}

	m_strBarString.Empty();

	m_strBarString = strText;
	GenBarCode();

	return bRtn;
}

BOOL CText::GenBarCode()
{
	BYTE pByte[256*256];
	BOOL bRtn = TRUE;
	switch(m_nBarType) {
	case EAN8:
		{
			//���֣�λ��ֻ��Ϊ7λ��1��У��λ
			BYTE szOutString[256];
			CString strText = m_strBarString;

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Ean8;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			else{
				m_strText.Format("%s",szOutString);
			}
			break;
		}
	case EAN13:
		{
			//���֣�λ��ֻ��Ϊ12λ��1��У��λ
			BYTE szOutString[256];
			CString strText = m_strBarString;
			
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Ean13;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			else{
				m_strText.Format("%s",szOutString);
			}
			break;
		}
	case CODE39:
		{
			BYTE szOutString[256];
			CString strText = m_strBarString;
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code39;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			iCodeBar1._s_value._uExtraInput._dw39 = 0;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			else{
				m_strText.Format("%s",szOutString);
			}
			break;
		}
	case INT25:
		{
			BYTE szOutString[256];
			CString strText = m_strBarString;
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Inter25;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			iCodeBar1._s_value._uExtraInput._dw25 = 0;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			else{
				m_strText.Format("%s",szOutString);
			}
			break;
		}
	case EXT39:
		{
			BYTE szOutString[256];
			CString strText = m_strBarString;
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code39Ex;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			iCodeBar1._s_value._uExtraInput._dw39 = 0;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			else{
				m_strText.Format("%s",szOutString);
			}
			break;
		}
	case UPCA:
		{
			//���֣�λ��ֻ��Ϊ11λ��1��У��λ
			BYTE szOutString[256];
			CString strText = m_strBarString;
			
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_UpcA;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			else{
				m_strText.Format("%s",szOutString);
			}
			break;
		}
	case UPCE:
		{
			//���֣�λ��ֻ��Ϊ6λ��2��У��λ
			BYTE szOutString[256];
			CString strText = m_strBarString;
			
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_UpcE;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			else{
				m_strText.Format("%s",szOutString);
			}
			break;
		}
	case CODE128A:
	case CODE128B:
	case CODE128C:
	case CODE128AUTO:
	case EAN128:
	case PDF417:
	case DATAMATRIX:
	case QR:
		{
			m_strText = m_strBarString;
			break;
		}
	}

	return bRtn;
}

//�����еĳ������������Ϣȥ��
void CText::CaseNum(CString &str)
{
	CString tmp;
	int nCount = str.GetLength();
	tmp = str;
	str.Empty();
	TCHAR ch;

	for(int i=0;i<nCount;i++)
	{
		ch = tmp.GetAt(i);
		if((ch>0X29)&&(ch<0x40)){
			str+=ch;
		}
	}
}

CString CText::GetBarString()
{
	//�õ�����У������
	m_bUse = TRUE;
	return m_strBarString;
}

BOOL CText::ChangeText(CString strTxt,CString ModelNo)
{
	if ( m_bModel )
	{
		if(ModelNo==m_strModelNo)
		{
			m_strFix=strTxt;
			m_strText=strTxt;
			m_bChangeFlag=TRUE;
			return TRUE;
		}
	}
	return FALSE;
}