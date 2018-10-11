// BarCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "BarCodeDlg.h"

#include "DataMatrixDlg.h"
#include "SetQRCodebar.h"
#include "Pdf417Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarCodeDlg dialog


CBarCodeDlg::CBarCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBarCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarCodeDlg)
	m_pText = NULL;
	//}}AFX_DATA_INIT
}


void CBarCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarCodeDlg)
	DDX_Control(pDX, IDC_TAB_TEXT, m_tab);
	DDX_Control(pDX, IDC_COMBO_BARCODETYPE, m_ctrlType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBarCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CBarCodeDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TEXT, OnSelchangeTabText)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_TEXT, OnSelchangingTabText)
	ON_CBN_SELCHANGE(IDC_COMBO_BARCODETYPE, OnSelchangeComboBarcodetype)
	ON_BN_CLICKED(IDC_BUT_BARCODESET, OnButBarcodeset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarCodeDlg message handlers

BOOL CBarCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strName;
	int nCount = g_BarCodeMgr.m_BarNameArray.GetSize();
	for(int i=0;i<nCount;i++)
	{
		strName = g_BarCodeMgr.m_BarNameArray.GetAt(i);
		m_ctrlType.AddString(strName);
	}
	m_ctrlType.SetCurSel(m_nBarType);
	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow(m_nBarType>=PDF417);
	
	CString str;
	//生成标签
	TC_ITEM TabItem;
	TabItem.mask=TCIF_PARAM|TCIF_TEXT;
	TabItem.pszText=new TCHAR[255];
	
	TabItem.lParam=(LPARAM)&m_PageText;
	VERIFY(m_PageText.Create(CTextPageTextDlg::IDD,&m_tab));
	str.LoadString(IDS_TEXTTITLE);
	_tcscpy_s(TabItem.pszText,255,str);
	m_tab.InsertItem(0,&TabItem);
	m_PageText.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageText.ShowWindow(SW_SHOW);
	m_PageText.EnableWindow(TRUE);

	TabItem.lParam=(LPARAM)&m_BarPage;
	VERIFY(m_BarPage.Create(CBarPage::IDD,&m_tab));
	str.LoadString(IDS_BARTITLE);
	_tcscpy_s(TabItem.pszText,255,str);
	m_tab.InsertItem(1,&TabItem);
	m_BarPage.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BarPage.ShowWindow(SW_HIDE);
	m_BarPage.EnableWindow(FALSE);
	m_BarPage.UpdateData(FALSE);
	
	delete TabItem.pszText;
	
	return TRUE;
}

void CBarCodeDlg::OnSelchangeTabText(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab=m_tab.GetCurSel();
	TC_ITEM tci;
	
	tci.mask=TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_SHOW);
	pWnd->EnableWindow(TRUE);

	*pResult = 0;
}

void CBarCodeDlg::OnSelchangingTabText(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab=m_tab.GetCurSel();
	TC_ITEM tci;
	
	tci.mask=TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_HIDE);
	pWnd->EnableWindow(FALSE);

	*pResult = 0;
}

void CBarCodeDlg::OnOK() 
{
	m_nBarType = m_ctrlType.GetCurSel();
	m_PageText.UpdateData();
	m_BarPage.UpdateData();

	if (m_BarPage.m_nSingleLineMarkCount < 1)
	{
		m_BarPage.m_nSingleLineMarkCount = 1;
	}
    if (m_BarPage.m_nNarrowLineNum < 2)
	{
		m_BarPage.m_nNarrowLineNum = 2;
	}

	BOOL bCreate = CreateCodeBarStatus(m_nBarType);
	if (bCreate)
	{
		CDialog::OnOK();
	}
	
	return;
}

void CBarCodeDlg::OnSelchangeComboBarcodetype() 
{
	UpdateData();
	int nCurSel = m_ctrlType.GetCurSel();
	m_nBarType=nCurSel;
	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow(nCurSel>=PDF417);
	FlashBarDefaultProperty();
}

void CBarCodeDlg::OnButBarcodeset() 
{
	UpdateData();

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
}

void CBarCodeDlg::FlashBarDefaultProperty()
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


BOOL CBarCodeDlg::CreateCodeBarStatus(long lType)
{
	if (NULL == m_pText) 
	{
		return FALSE;
	}
	
	m_PageText.UpdateData(TRUE);
	BYTE *pByte = new BYTE[256*256];
	if (NULL == pByte)
	{
		return FALSE;
	}

	m_pText->m_bBarCode = TRUE;
	m_pText->m_nText			= m_PageText.m_nText;
	m_pText->m_strFix			= m_PageText.m_strFix;
	m_pText->m_strDate		= m_PageText.m_strDate;
	m_pText->m_nJumpStart		= m_PageText.m_nJumpStart;
	m_pText->m_nJumpEnd		= m_PageText.m_nJumpEnd;
	m_pText->m_strLeft		= m_PageText.m_strLeft;
	m_pText->m_strRight		= m_PageText.m_strRight;
	m_pText->m_nJumpLen		= m_PageText.m_nJumpLen;
	m_pText->m_strPrefix		= m_PageText.m_strPrefix;
	m_pText->m_nJumpStep		= m_PageText.m_nJumpStep;
	m_pText->m_bFill			= m_PageText.m_bFill;
	m_pText->m_nFromFile		= m_PageText.m_nFromFile;
	m_pText->m_strFromFile	= m_PageText.m_strFromFile;
	m_pText->m_strBinText		= m_PageText.m_strBinText;
	
	m_pText->m_bBarCode = TRUE;
	m_pText->m_nBarType = lType;		
	m_pText->FlashNextText(0x00);	//得到字串数据	

	BOOL bRtn = TRUE;
	switch(lType) 
	{
	case EAN8:
		{
			//数字，位数只能为7位＋1个校验位
			BYTE szOutString[256];
			CString strText = m_pText->GetBarString();

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
			CString strText = m_pText->GetBarString();

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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();

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
			CString strText = m_pText->GetBarString();

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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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
			CString strText = m_pText->GetBarString();
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