// SetSortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetSortDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetSortDlg dialog


CSetSortDlg::CSetSortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSortDlg)
	m_nNewNum = 0;
	m_nChangeType = 0;
	//}}AFX_DATA_INIT
}


void CSetSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSortDlg)
	DDX_Text(pDX, IDC_EDIT_NEWSORT,    m_nNewNum);
    DDX_Radio(pDX, IDC_RADIO_CHANG_POS, m_nChangeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSortDlg, CDialog)
	//{{AFX_MSG_MAP(CSetSortDlg)
	ON_BN_CLICKED(IDC_RADIO_CHANG_POS, OnChangPos)
    ON_BN_CLICKED(IDC_RADIO_EXCHANG_POS, OnExchangPos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSortDlg message handlers

void CSetSortDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	CDialog::OnOK();
}

BOOL CSetSortDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetSortDlg::OnChangPos() 
{
	UpdateData(TRUE);
	CString str("");
	str.LoadString(IDS_INPUTNEWNUM);
	GetDlgItem(IDC_STATIC_ORD_MSG)->SetWindowText(str);
}

void CSetSortDlg::OnExchangPos() 
{
	UpdateData(TRUE);
	CString str("");
	str.LoadString(IDS_INPUTOTHERNUM);
	GetDlgItem(IDC_STATIC_ORD_MSG)->SetWindowText(str);
}