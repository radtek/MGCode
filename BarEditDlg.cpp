// BarEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "BarEditDlg.h"

#include "DataMatrixDlg.h"
#include "SetQRCodebar.h"
#include "Pdf417Dlg.h"
#include "TextSerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarEditDlg dialog


CBarEditDlg::CBarEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBarEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarEditDlg)
	m_nText       = -1;
	m_bIsShow     = FALSE;
	m_pCurSelText = NULL;
	m_pNodeBar    = NULL;
	m_strDate     = _T("");
	m_strFix      = _T("");
	m_nBarType    = -1;
	//}}AFX_DATA_INIT
}


void CBarEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarEditDlg)
	DDX_Control(pDX, IDC_COMBO_BARCODETYPE, m_ctrlType);
	DDX_Radio(pDX, IDC_RADIO_FIX, m_nText);
	DDX_Text(pDX, IDC_DATE_EDIT, m_strDate);
	DDX_Text(pDX, IDC_FIX_EDIT, m_strFix);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBarEditDlg, CDialog)
	//{{AFX_MSG_MAP(CBarEditDlg)
	ON_BN_CLICKED(IDC_BUT_BARCODESET, OnButBarcodeset)
	ON_CBN_SELCHANGE(IDC_COMBO_BARCODETYPE, OnEditchangeComboBarcodetype)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL, OnButtonSerial)
	ON_BN_CLICKED(IDC_RADIO_FIX, OnRadioFix)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, OnRadioSerial)
	ON_BN_CLICKED(IDC_RADIO_TIME, OnRadioTime)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, OnButtonAdvance)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarEditDlg message handlers

BOOL CBarEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strName("");
	int nCount = g_BarCodeMgr.m_BarNameArray.GetSize();
	for(int i=0;i<nCount;i++)
	{
		strName = g_BarCodeMgr.m_BarNameArray.GetAt(i);
		m_ctrlType.AddString(strName);
	}
	m_ctrlType.SetCurSel(m_nBarType);
	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow(m_nBarType>=PDF417);

	UpdateDataView();

	m_pCurSelText = new CText(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBarEditDlg::OnButBarcodeset() 
{
	UpdateData();

	m_nBarType = m_ctrlType.GetCurSel();
	switch(m_nBarType)
	{
	case PDF417:
		{
			CPdf417Dlg dlg;
			dlg.m_nLineNum = m_nOutline;
			if(dlg.DoModal()==IDOK)
			{
				m_nOutline = dlg.m_nLineNum;
			}
			break;
		}
	case DATAMATRIX:
		{
			CDataMatrixDlg dlg;
			dlg.m_nHPoint = m_nHPoint;
			dlg.m_nWPoint = m_nWPoint;
			dlg.m_bAutoSet = m_bAutoSet;
			dlg.m_nErrLev = m_nErrLev;
			dlg.m_nRadio = m_nOutline;
			
			if(dlg.DoModal()==IDOK)
			{
				m_nHPoint	 = dlg.m_nHPoint;
				m_nWPoint	 = dlg.m_nWPoint;
				m_bAutoSet	 = dlg.m_bAutoSet;
				m_nErrLev	 = dlg.m_nErrLev;
				m_nOutline	 = dlg.m_nRadio;
			}
			break;
		}
	case QR:
		{
			CSetQRCodebar dlg;
			dlg.m_nErrorValue=m_nErrLev-1;
			dlg.m_nSize=m_nHPoint-1;//QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号-1
			if(dlg.DoModal()==IDOK)
			{
				m_nErrLev=dlg.m_nErrorValue+1;//QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号+1
				m_nHPoint=dlg.m_nSize+1;
			}
			break;
		}
	}

	return;
	
}


void CBarEditDlg::FlashBarDefaultProperty()
{
	switch(m_nBarType) {
	case PDF417:
		m_nOutline = 12;
		break;
	case DATAMATRIX:
		m_nHPoint	 = 18;
		m_nWPoint	 = 18;
		m_bAutoSet	 = FALSE;
		m_nErrLev	 = 5;
		m_nOutline	 = 0;
		break;
	case QR:
		m_nErrLev = 2;
		m_nHPoint = 1;
		break;
	}
}


BOOL CBarEditDlg::CreateCodeBarStatus(long lType)
{
	if (NULL == m_pCurSelText) 
	{
		return FALSE;
	}
	
//	m_PageText.UpdateData(TRUE);
	BYTE *pByte = new BYTE[256*256];
	if (NULL == pByte)
	{
		return FALSE;
	}
/*
	m_pCurSelText->m_bBarCode   = TRUE;
	m_pCurSelText->m_nText	  = m_PageText.m_nText;
	m_pCurSelText->m_strFix	  = m_PageText.m_strFix;
	m_pCurSelText->m_strDate	  = m_PageText.m_strDate;
	m_pCurSelText->m_nJumpStart = m_PageText.m_nJumpStart;
	m_pCurSelText->m_nJumpEnd	  = m_PageText.m_nJumpEnd;
	m_pCurSelText->m_strLeft	  = m_PageText.m_strLeft;
	m_pCurSelText->m_strRight	  = m_PageText.m_strRight;
	m_pCurSelText->m_nJumpLen	  = m_PageText.m_nJumpLen;
	m_pCurSelText->m_strPrefix  = m_PageText.m_strPrefix;
	m_pCurSelText->m_nJumpStep  = m_PageText.m_nJumpStep;
	m_pCurSelText->m_bFill	  = m_PageText.m_bFill;
	m_pCurSelText->m_nFromFile  = m_PageText.m_nFromFile;
	m_pCurSelText->m_strFromFile= m_PageText.m_strFromFile;
	m_pCurSelText->m_strBinText = m_PageText.m_strBinText;
*/

	
	m_pCurSelText->m_bBarCode = TRUE;
	m_pCurSelText->m_nBarType = lType;		
	m_pCurSelText->FlashNextText(0x00);	//得到字串数据	

	BOOL bRtn = TRUE;
	switch(lType) 
	{
	case EAN8:
		{
			//数字，位数只能为7位＋1个校验位
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();

			int nLen = strText.GetLength();
			if(nLen>7)
			{
				strText.SetAt(7,TCHAR(0X00));
			}
			else
			{
				for(int i=0;i<7-nLen;i++)
					strText.Insert(0,'0');
			}

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Ean8;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case EAN13:
		{
			//数字，位数只能为12位＋1个校验位
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();

			int nLen = strText.GetLength();
			if(nLen>7)
			{
				strText.SetAt(12,TCHAR(0X00));
			}
			else
			{
				for(int i=0;i<12-nLen;i++)
					strText.Insert(0,'0');
			}

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Ean13;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case EAN128:
		{
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Ean128;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case CODE128A:
		{
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();
			strText.Insert(0,TCHAR(0xF1));
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case CODE128B:
		{
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();
			strText.Insert(0,TCHAR(0xF2));
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case CODE128C:
		{
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();
			strText.Insert(0,TCHAR(0xF3));
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case CODE128AUTO:
		{
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code128Auto;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case CODE39:
		{
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();
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
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case INT25:
		{
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();
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
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}

			break;
		}
	case EXT39:
		{
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();
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
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case UPCA:
		{
			//数字，位数只能为11位＋1个校验位
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();

			int nLen = strText.GetLength();
			if(nLen > 7)
			{
				strText.SetAt(11,TCHAR(0X00));
			}
			else
			{
				for(int i=0;i<11-nLen;i++)
					strText.Insert(0,'0');
			}

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_UpcA;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case UPCE:
		{
			//数字，位数只能为6位＋2个校验位
			BYTE szOutString[256];
			CString strText = m_pCurSelText->GetBarString();

			int nLen = strText.GetLength();
			if(nLen>7)
			{
				strText.SetAt(6,TCHAR(0X00));
			}
			else
			{
				for(int i=0;i<6-nLen;i++)
					strText.Insert(0,'0');
			}

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_UpcE;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case PDF417:
		{
			CString strText = m_pCurSelText->GetBarString();
			IMATRIXBAR iMatrix;
			iMatrix._type = 3;
 			iMatrix._u_value._pdf417._bits._bConver=0;
			iMatrix._u_value._pdf417._bits._countsConver=0;
			iMatrix._u_value._pdf417._bits._eccLevel=1;
			iMatrix._u_value._pdf417._bits._nCols = 0;		
			iMatrix._u_value._pdf417._bits._nRows = m_nOutline;		//
			iMatrix._u_value._pdf417._xScale = 1;
			iMatrix._u_value._pdf417._yScale = 1;
			iMatrix._u_value._pdf417._sizeSrcBytes = strText.GetLength();
			iMatrix._u_value._pdf417._lpszSrc = (BYTE*)((LPCTSTR)strText);
			iMatrix._u_value._pdf417._lpszRetBuffer = pByte;
			iMatrix._u_value._pdf417._sizeRetBuffer = 256*256;

			int nRtn = g_BarCodeMgr.m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode2.GetErrInfo(nRtn,g_LangID));
			}

			break;
		}
	case DATAMATRIX:
		{
			CString strText = m_pCurSelText->GetBarString();
			IMATRIXBAR iMatrix;
			iMatrix._type = 2;
			iMatrix._u_value._dataMatrix._bits._bConver =0;
			iMatrix._u_value._dataMatrix._bits._countsConver = 0;
			iMatrix._u_value._dataMatrix._bits._bUserDefined=m_bAutoSet;	//
			iMatrix._u_value._dataMatrix._bits._eccType =m_nErrLev;			//
			iMatrix._u_value._dataMatrix._bits._outStyle=m_nOutline;		//
			iMatrix._u_value._dataMatrix._heightNums = m_nHPoint;			//
			iMatrix._u_value._dataMatrix._widthNums = m_nWPoint;			//
			iMatrix._u_value._dataMatrix._lpszSrc = (BYTE*)((LPCTSTR)strText);
			iMatrix._u_value._dataMatrix._sizeSrcBytes = strText.GetLength();
			iMatrix._u_value._dataMatrix._lpszRetBuffer = pByte;
			iMatrix._u_value._dataMatrix._sizeRetBuffer = 256*256;
			
			int nRtn = g_BarCodeMgr.m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode2.GetErrInfo(nRtn,g_LangID));
			}
			break;
		}
	case QR:
		{
			CString strText = m_pCurSelText->GetBarString();
			IMATRIXBAR iMatrix;
			iMatrix._type = 1;
			iMatrix._u_value._qr._bits._bConver=0;
			iMatrix._u_value._qr._bits._countsConver =0;
			iMatrix._u_value._qr._bits._levelType=m_nErrLev;	//
			iMatrix._u_value._qr._bits._versionType=m_nHPoint;	//
			iMatrix._u_value._qr._lpszRetBuffer = pByte;
			iMatrix._u_value._qr._lpszSrc = (BYTE*)((LPCTSTR)strText);
			iMatrix._u_value._qr._sizeRetBuffer = 256*256;
			iMatrix._u_value._qr._sizeSrcBytes = strText.GetLength();

			int nRtn = g_BarCodeMgr.m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode2.GetErrInfo(nRtn,g_LangID));
			}

			break;
		}
	}
	
	//////////////////////////


	if (pByte)
	{
		delete[] pByte;
		pByte = NULL;
	}

	return bRtn;
}

void CBarEditDlg::OnEditchangeComboBarcodetype() 
{
	UpdateData();
	int nCurSel = m_ctrlType.GetCurSel();
	m_nBarType  = nCurSel;
	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow(nCurSel>=PDF417);
	FlashBarDefaultProperty();
	
	return;
}

BOOL CBarEditDlg::IsCreateDlg() 
{
	return (NULL != m_pCurSelText);
}

void CBarEditDlg::SetShow(BOOL bIsShow) 
{
	m_bIsShow = bIsShow;
}

CString CBarEditDlg::GetSerialString()
{
	if (NULL == m_pCurSelText)
	{
		return "";
	}
	
	CString str="";
	int i = m_pCurSelText->m_nMarkTimes;
	if(m_nText==TEXT_SERIAL)
	{
		if(m_pCurSelText->m_nFromFile)
		{
			//取于文件
			str=m_pCurSelText->m_saSerial.GetAt(i);
		}
		else
		{
			//自定义跳号
			int nItems=(m_pCurSelText->m_nJumpEnd - m_pCurSelText->m_nJumpStart)/m_pCurSelText->m_nJumpStep+1;
			if(i<nItems)
			{
				int cur=m_pCurSelText->m_nJumpStart+m_pCurSelText->m_nJumpStep*i;
				str.Format("%d",cur);
				
				if(m_pCurSelText->m_bFill)
				{	
					int n;
					
					n=m_pCurSelText->m_nJumpLen-str.GetLength();
					for(int j=0;j<n;j++)
						str=m_pCurSelText->m_strPrefix+str;
				}
				
				str=m_pCurSelText->m_strLeft+str+m_pCurSelText->m_strRight;
			}
		}
	}
	return str;
}


void CBarEditDlg::OnButtonSerial() 
{
	CTextSerialDlg dlg;
	dlg.m_nJumpStart	= m_pCurSelText->m_nJumpStart;
	dlg.m_nJumpEnd  	= m_pCurSelText->m_nJumpEnd  ;
	dlg.m_strLeft	    = m_pCurSelText->m_strLeft	  ;
	dlg.m_strRight	    = m_pCurSelText->m_strRight  ;
	dlg.m_nJumpLen	    = m_pCurSelText->m_nJumpLen  ;
	dlg.m_strPrefix		= m_pCurSelText->m_strPrefix ;
	dlg.m_nJumpStep	    = m_pCurSelText->m_nJumpStep ;
	dlg.m_nJumpInterval = m_pCurSelText->m_nJumpDis  ;
	dlg.m_bFill		    = m_pCurSelText->m_bFill     ;
	dlg.m_nFromFile	    = m_pCurSelText->m_nFromFile ;
	dlg.m_strFromFile   = m_pCurSelText->m_strFromFile;
	dlg.m_pSerial	    = &(m_pCurSelText->m_saSerial);
	dlg.m_pCur		    = &(m_pCurSelText->m_nMarkTimes);
	dlg.m_strBinText	= m_pCurSelText->m_strBinText;
	
	if(dlg.DoModal()==IDOK)
	{
		m_pCurSelText->m_nJumpStart	= dlg.m_nJumpStart;
		m_pCurSelText->m_nJumpEnd		= dlg.m_nJumpEnd;
		m_pCurSelText->m_strLeft		= dlg.m_strLeft;
		m_pCurSelText->m_strRight		= dlg.m_strRight;
		m_pCurSelText->m_strPrefix		= dlg.m_strPrefix;
		m_pCurSelText->m_nJumpLen		= dlg.m_nJumpLen;
		m_pCurSelText->m_nJumpStep		= dlg.m_nJumpStep;
		m_pCurSelText->m_bFill			= dlg.m_bFill;
		m_pCurSelText->m_nFromFile		= dlg.m_nFromFile;
		m_pCurSelText->m_strFromFile	= dlg.m_strFromFile;
		m_pCurSelText->m_strBinText	= dlg.m_strBinText;
		m_pCurSelText->m_nJumpDis      = dlg.m_nJumpInterval;
		m_pCurSelText->m_nMarkTimes    = *(dlg.m_pCur);
		
		CString strSerial;
		strSerial = GetSerialString();
		SetDlgItemText(IDC_SERIAL_EDIT, strSerial);
	}

	return;
}

//用于配合RADIO刷新界面的
void CBarEditDlg::UpdateDataView()
{
	BOOL bFlag;
	switch(m_nText) {
	case 0:
		//固定文本
		bFlag = (m_nText == 0);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(!bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(!bFlag);
		break;
	case 1:
		//跳号部分
		bFlag = (m_nText == 1);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(!bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(!bFlag);
		break;
	case 2:
		//时间日期
		bFlag = (m_nText == 2);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(!bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(!bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(bFlag);
		break;
	}
}

void CBarEditDlg::OnRadioFix() 
{
	SetDlgItemText(IDC_TIME_INFO,"");
	KillTimer(1);
	UpdateData();
	UpdateDataView();
}

void CBarEditDlg::OnRadioSerial() 
{
	SetDlgItemText(IDC_TIME_INFO,"");
	KillTimer(1);
	UpdateData();
	UpdateDataView();
}

void CBarEditDlg::OnRadioTime() 
{
	UpdateData();
	SetTimer(1,1000,NULL);
	UpdateDataView();
}


void CBarEditDlg::OnTimer(UINT nIDEvent) 
{
	if (NULL == m_pCurSelText) 
	{
		return;
	}

	CTime t;
	t=CTime::GetCurrentTime();
	CString strText=t.Format(m_pCurSelText->m_strDate);
	SetDlgItemText(IDC_TIME_INFO,strText);

	return;
}

void CBarEditDlg::SetTextInfo(CNode *pNode, FILLDATA &fill) 
{
	if (!m_bIsShow)
	{
		return;
	}

	if (NULL == pNode)
	{
		return;
	}

	if (NODE_CODEBAR != pNode->GetType())
	{
		return;
	}

	m_pNodeBar = (CNodeCodebar*)pNode;

	m_strFix = "";
    KillTimer(1);
	if (NULL == m_pNodeBar)
	{
		UpdateData(FALSE);
		return;
	}

	CText* pText = g_MgrText->GetAt(m_pNodeBar->m_nTextID);
	if (NULL == pText)
	{
		return;
	}
	*m_pCurSelText = *pText;

	m_nText	  = pText->m_nText;
	m_strFix  = pText->m_strFix;
	m_strDate = pText->m_strDate;
	if (2 == m_nText)
	{
		SetTimer(1,1000,NULL);
		UpdateDataView();
	}

	m_nHPoint  = m_pNodeBar->m_nHPoint;
	m_nWPoint  = m_pNodeBar->m_nWPoint;
	m_bAutoSet = m_pNodeBar->m_bAutoSet;
	m_nErrLev  = m_pNodeBar->m_nErrLev;
	m_nOutline = m_pNodeBar->m_nOutline;

	m_nBarType = m_pNodeBar->m_BarType;
	m_ctrlType.SetCurSel(m_nBarType);

	UpdateData(FALSE);

	UpdateDataView();

	return;
}

void CBarEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_pCurSelText)
	{
		delete m_pCurSelText;
		m_pCurSelText = NULL;
	}

	return;
}

CHLDoc *CBarEditDlg::GetDocument() 
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return NULL;
	}

	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{	
		return NULL;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return NULL;
	}

	return pDoc;
}

void CBarEditDlg::OnButtonAdvance() 
{
	if (NULL == m_pNodeBar)
	{
		return;
	}

	CHLDoc*pDoc = GetDocument();
	if (NULL == pDoc) 
	{
		return;
	}

	pDoc->EditBarNode(m_pNodeBar);

	return;
}

void CBarEditDlg::Apply()
{
	if (NULL == m_pNodeBar)
	{
		return;
	}

	CHLDoc*pDoc = GetDocument();
	if (NULL == pDoc) 
	{
		return;
	}

	UpdateData(TRUE);

	CNodeCodebar* pBarNode = (CNodeCodebar*)(pDoc->IsHaveSelBarNode());
	if (NULL == pBarNode)
	{
		return;
	}

	if (/*(pBarNode->m_nTextID != m_pNodeBar->m_nTextID) ||*/
		(pBarNode != m_pNodeBar))
	{
		return;
	}

	CText* pText      = g_MgrText->GetAt(m_pNodeBar->m_nTextID);
	if (NULL == pText)
	{
		return;
	}
	
	*pText            = *m_pCurSelText;
	int nOldCode = pText->m_nText;
	pText->m_nText	  = m_nText;
	pText->m_strFix	  = m_strFix;
	pText->m_strDate  = m_strDate;
	pText->m_bBarCode = TRUE;

	int nOldBarType = m_pNodeBar->m_BarType;
	m_nBarType = m_ctrlType.GetCurSel();
	m_pNodeBar->m_BarType = (BARTYPE)m_nBarType;
	pText->m_nBarType     = m_nBarType;

	m_pNodeBar->m_nHPoint	 = m_nHPoint;
	m_pNodeBar->m_nWPoint	 = m_nWPoint;
	m_pNodeBar->m_bAutoSet	 = m_bAutoSet;
	m_pNodeBar->m_nErrLev	 = m_nErrLev;
	m_pNodeBar->m_nOutline	 = m_nOutline;

	m_pNodeBar->m_nErrLev=m_nErrLev;//QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号+1
	m_pNodeBar->m_nHPoint=m_nHPoint;
	
	// 得到字串数据
	pText->FlashNextText(0x00);
	BOOL bRet = m_pNodeBar->GenBarCode(0x00);
	if (!bRet)
	{
		pText->m_nBarType     = nOldBarType;
		m_pNodeBar->m_BarType = (BARTYPE)nOldBarType;
		pText->m_nText = nOldCode;
		m_nText        = nOldCode;
	}

	KillTimer(1);
	if (2 == m_nText)
	{
		SetTimer(1,1000,NULL);
		UpdateDataView();
	}

	m_pNodeBar->SetSelect(TRUE);
	m_pNodeBar->CalLimit();

	pDoc->UpdateSelBar();

	return;
}
