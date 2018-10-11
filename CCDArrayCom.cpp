// CCDArrayCom.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "CCDArrayCom.h"
#include "CCDCalibrate.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCDArrayCom dialog

IMPLEMENT_DYNCREATE(CCCDArrayCom, CPropertyPage)
CCCDArrayCom::CCCDArrayCom() : CPropertyPage(CCCDArrayCom::IDD)
{
	//{{AFX_DATA_INIT(CCCDArrayCom)
	m_dbCCD1Precision = 0.0;
	m_dbCCD2Precision = 0.0;
	//}}AFX_DATA_INIT
	m_bStop=TRUE;
}


void CCCDArrayCom::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCCDArrayCom)
	DDX_Text(pDX, IDC_EDIT_CCD1_PRECISION, m_dbCCD1Precision);
	DDX_Text(pDX, IDC_EDIT_CCD2_PRECISION, m_dbCCD2Precision);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCCDArrayCom, CDialog)
	//{{AFX_MSG_MAP(CCCDArrayCom)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCDArrayCom message handlers

void CCCDArrayCom::LoadData()
{
	m_dbCCD1Precision = g_ParaIni.ReadDouble("CCDPARA","CCD1Precision",1.0);
	m_dbCCD2Precision = g_ParaIni.ReadDouble("CCDPARA","CCD2Precision",6.0);
}

void CCCDArrayCom::SaveData()
{ 
	g_ParaIni.m_dbCCD1Precision = m_dbCCD1Precision;
	g_ParaIni.m_dbCCD2Precision = m_dbCCD2Precision;
	g_ParaIni.SavePara();
}

void CCCDArrayCom::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); 
	SaveData();
}

BOOL CCCDArrayCom::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strCCD;
	strCCD.Format("CCD1: %d X %d", g_sys.m_nCCD1PixelX, g_sys.m_nCCD1PixelY);
	SetDlgItemText(IDC_STATIC_CCD1, strCCD);

	strCCD.Format("CCD2: %d X %d", g_sys.m_nCCD2PixelX, g_sys.m_nCCD2PixelY);
	SetDlgItemText(IDC_STATIC_CCD2, strCCD);

	LoadData();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

