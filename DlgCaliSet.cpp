// DlgCaliSet.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgCaliSet.h"
#include "DlgMarkNormalPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSet dialog


CDlgCaliSet::CDlgCaliSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCaliSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCaliSet)
	m_strPath = _T("");
	m_dbXScale = 0.0;
	m_dbYScale = 0.0;
	m_nBitMM = 0;
	m_dbCCWAngle = 0.0;
	m_nLayDelay = 0;
	m_bEnableAutoCal = TRUE;
	//}}AFX_DATA_INIT
}


void CDlgCaliSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCaliSet)
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_BOX_X_SCALE, m_dbXScale);
	DDX_Text(pDX, IDC_EDIT_BOX_Y_SCALE, m_dbYScale);
	DDX_Text(pDX, IDC_EDIT_BitMM, m_nBitMM);
	DDX_Text(pDX, IDC_EDIT_CCW_ANGLE, m_dbCCWAngle);
	DDX_Text(pDX, IDC_EDIT_LAY_DELAY, m_nLayDelay);
	DDX_Check(pDX, IDC_CHECK_AUTO_CAL, m_bEnableAutoCal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCaliSet, CDialog)
	//{{AFX_MSG_MAP(CDlgCaliSet)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDC_CHECK_AUTO_CAL, &CDlgCaliSet::OnBnClickedCheckAutoCal)
ON_BN_CLICKED(IDC_BTN_RESETAUTO, &CDlgCaliSet::OnBnClickedBtnResetauto)
ON_BN_CLICKED(IDC_CHECK_AUTO_CAL, &CDlgCaliSet::OnBnClickedCheckAutoCal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSet message handlers

BOOL CDlgCaliSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetModuleFileName(NULL, m_strIniPath.GetBuffer(MAX_PATH), MAX_PATH);
	m_strIniPath.ReleaseBuffer();
	m_strIniPath = m_strIniPath.Left(m_strIniPath.ReverseFind('\\')+1) + g_sys.m_strCardName + "\\CardPara.ini";

	CFileStatus status;
	if (!CFile::GetStatus(m_strIniPath, status))
	{
		CString strTip;
		strTip.Format("找不到文件!\n%s", m_strIniPath);
		AfxMessageBox(strTip);
		CDialog::OnCancel();
		return TRUE;
	}

	char szBuffer[256];
	memset(szBuffer, 0, sizeof(szBuffer));
	GetPrivateProfileStringA("BOX", "BMM", "1000", szBuffer, sizeof(szBuffer), m_strIniPath);
	m_nBitMM = atoi(szBuffer);

	memset(szBuffer, 0, sizeof(szBuffer));
	GetPrivateProfileStringA("BOX", "CORFILE", "790", szBuffer, sizeof(szBuffer), m_strIniPath);
	m_strPath = szBuffer;

	memset(szBuffer, 0, sizeof(szBuffer));
	GetPrivateProfileStringA("BOX", "CCW", "0", szBuffer, sizeof(szBuffer), m_strIniPath);
	m_dbCCWAngle = atof(szBuffer);

	memset(szBuffer, 0, sizeof(szBuffer));
	GetPrivateProfileStringA("BOX", "XScale", "1", szBuffer, sizeof(szBuffer), m_strIniPath);
	m_dbXScale = atof(szBuffer);

	memset(szBuffer, 0, sizeof(szBuffer));
	GetPrivateProfileStringA("BOX", "YScale", "1", szBuffer, sizeof(szBuffer), m_strIniPath);
	m_dbYScale = atof(szBuffer);

	memset(szBuffer, 0, sizeof(szBuffer));
	GetPrivateProfileStringA("CONTROL", "EnableAutoCal", "1", szBuffer, sizeof(szBuffer), m_strIniPath);
	m_bEnableAutoCal = atoi(szBuffer);

	m_nLayDelay = g_sys.m_nLaserLayDelay;

	UpdateData(FALSE);

	GetDlgItem(IDC_BTN_RESETAUTO)->EnableWindow(m_bEnableAutoCal);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgCaliSet::OnButtonSelect() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData())
		return;

	CString strFilter;
	if (g_sys.m_strCardName=="CARD_RTC4")
		strFilter = "ctb File(*.ctb)|*.ctb|All Files(*.*)|*.*||";
	else
		strFilter = "ctb File(*.ct5)|*.ct5|All Files(*.*)|*.*||";

	CFileDialog dlg(TRUE,NULL,m_strPath,OFN_HIDEREADONLY,strFilter);
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_PATH, dlg.GetPathName());
	}

	CString str;
	str.Format(_T("test%d"),CommDlgExtendedError());
	OutputDebugString(str);
}



void CDlgCaliSet::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData())
		return;

	char szBuffer[256];
	sprintf(szBuffer, "%d", m_nBitMM);
	WritePrivateProfileStringA("BOX", "BMM", szBuffer, m_strIniPath);

	WritePrivateProfileStringA("BOX", "CORFILE", m_strPath, m_strIniPath);

	sprintf(szBuffer, "%f", m_dbCCWAngle);
	WritePrivateProfileStringA("BOX", "CCW", szBuffer,  m_strIniPath);

	sprintf(szBuffer, "%f", m_dbXScale);
	WritePrivateProfileStringA("BOX", "XScale", szBuffer,m_strIniPath);

	sprintf(szBuffer, "%f", m_dbYScale);
	WritePrivateProfileStringA("BOX", "YScale", szBuffer, m_strIniPath);
	
	sprintf(szBuffer, "%d", m_bEnableAutoCal);
	WritePrivateProfileStringA("CONTROL", "EnableAutoCal", szBuffer, m_strIniPath);

	g_DAT.InitCard();


	if (g_sys.m_nLaserCurrentKeep)//维持电流
	{
		g_pDlgMarkNormalPane->SetCurrent(g_sys.m_nLaserCurrentKeep);
	}

	CDialog::OnOK();
}

void CDlgCaliSet::OnBnClickedBtnResetauto()
{
	// TODO: 在此添加控件通知处理程序代码
	g_DAT.SetAutoCalRefPos();
}


void CDlgCaliSet::OnBnClickedCheckAutoCal()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_RESETAUTO)->EnableWindow(m_bEnableAutoCal);
}
