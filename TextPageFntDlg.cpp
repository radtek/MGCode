// TextPageFntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextPageFntDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextPageFntDlg property page

IMPLEMENT_DYNCREATE(CTextPageFntDlg, CPropertyPage)

CTextPageFntDlg::CTextPageFntDlg() : CPropertyPage(CTextPageFntDlg::IDD)
{
	//{{AFX_DATA_INIT(CTextPageFntDlg)
	m_nFont = 1;
	m_dbCharSpace = 0.0;
	m_dbWidthFactor = 0.0;
	m_dbHeight = 0.0;
	m_dbLineSpace = 0.0;
	m_strFontCHS = _T("");
	m_strFontEN = _T("");
	m_strFontNUM = _T("");
	m_strFormat = _T("");
	m_bCustomFormat = FALSE;
	m_strPrecisK = _T("100");
	//}}AFX_DATA_INIT
}

CTextPageFntDlg::~CTextPageFntDlg()
{
}

void CTextPageFntDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextPageFntDlg)
	DDX_Radio(pDX, IDC_RADIO_FONT_TTF, m_nFont);
	DDX_Control(pDX, IDC_COMBO_NUM, m_comboNUM);
	DDX_Control(pDX, IDC_COMBO_EN, m_comboEN);
	DDX_Control(pDX, IDC_COMBO_CHS, m_comboCHS);
	DDX_Text(pDX, IDC_EDIT_CHARSPACE, m_dbCharSpace);
	DDX_Text(pDX, IDC_EDIT_WIDTHFACTOR, m_dbWidthFactor);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_dbHeight);
	DDX_Text(pDX, IDC_EDIT_LINESPACE, m_dbLineSpace);
	DDX_CBString(pDX, IDC_COMBO_CHS, m_strFontCHS);
	DDX_CBString(pDX, IDC_COMBO_EN, m_strFontEN);
	DDX_CBString(pDX, IDC_COMBO_NUM, m_strFontNUM);
	DDX_Text(pDX, IDC_EDIT_FORMAT, m_strFormat);
	DDX_Check(pDX, IDC_CHECK_CUSTOM, m_bCustomFormat);
	DDX_CBString(pDX, IDC_COMBO_PRECIS_K, m_strPrecisK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextPageFntDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CTextPageFntDlg)
	ON_BN_CLICKED(IDC_RADIO_FONT_SHX, OnRadioFontShx)
	ON_BN_CLICKED(IDC_RADIO_FONT_TTF, OnRadioFontTtf)
	ON_BN_CLICKED(IDC_BUTTON_TTF, OnButtonTtf)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM, OnCheckCustom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextPageFntDlg message handlers

void CTextPageFntDlg::UpdateDataView()
{
	BOOL bFlag;
	switch(m_nFont) 
	{
	case 0:
		//TTF字体
		bFlag = (m_nFont == 0);
		GetDlgItem(IDC_BUTTON_TTF)->EnableWindow(bFlag);
		GetDlgItem(IDC_COMBO_CHS)->EnableWindow(!bFlag);
		GetDlgItem(IDC_COMBO_EN )->EnableWindow(!bFlag);
		GetDlgItem(IDC_COMBO_NUM)->EnableWindow(!bFlag);
		break;
	case 1:
		//SHX字体
		bFlag = (m_nFont == 1);
		GetDlgItem(IDC_BUTTON_TTF)->EnableWindow(!bFlag);
		GetDlgItem(IDC_COMBO_CHS)->EnableWindow(bFlag);
		GetDlgItem(IDC_COMBO_EN )->EnableWindow(bFlag);
		GetDlgItem(IDC_COMBO_NUM)->EnableWindow(bFlag);
		break;
	}

	GetDlgItem(IDC_EDIT_FORMAT)->EnableWindow(m_bCustomFormat);

	return;
}

void CTextPageFntDlg::OnRadioFontShx() 
{
	UpdateData();
	UpdateDataView();
}

void CTextPageFntDlg::OnRadioFontTtf() 
{
	UpdateData();
	UpdateDataView();
}

BOOL CTextPageFntDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//初始化字体信息
	CString path;
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	path=pApp->GetInstallPath();
	
	CString searchFile;
	CString font;
	CFileFind finder;
    BOOL bWorking;
	
	searchFile=path+"\\pfonts\\*.shx";
	bWorking=finder.FindFile(searchFile);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		font=finder.GetFileName();
		font=font.Left(font.GetLength()-4);
		m_comboCHS.AddString(font);
	}
	m_comboCHS.SelectString(-1,m_strFontCHS);
	
	searchFile=path+"\\efonts\\*.shx";
	bWorking=finder.FindFile(searchFile);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		font=finder.GetFileName();
		font=font.Left(font.GetLength()-4);
		m_comboEN.AddString(font);
	}
	m_comboEN.SelectString(-1,m_strFontEN);	
	
	searchFile=path+"\\efonts\\*.shx";
	bWorking=finder.FindFile(searchFile);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		font=finder.GetFileName();
		font=font.Left(font.GetLength()-4);
		m_comboNUM.AddString(font);
	}
	m_comboNUM.SelectString(-1,m_strFontNUM);	
	
	UpdateDataView();
	GetDlgItem(IDC_EDIT_FORMAT)->EnableWindow(m_bCustomFormat);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextPageFntDlg::OnButtonTtf() 
{
	CFontDialog dlg(&m_lf);
	dlg.m_cf.Flags&=~CF_EFFECTS;
	dlg.m_cf.Flags|=CF_TTONLY;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetCurrentFont(&m_lf);
	}

	return;
}

void CTextPageFntDlg::OnCheckCustom() 
{
	UpdateData();
	GetDlgItem(IDC_EDIT_FORMAT)->EnableWindow(m_bCustomFormat);
}
