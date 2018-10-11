// DataMatrixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DataMatrixDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMatrixDlg dialog


CDataMatrixDlg::CDataMatrixDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataMatrixDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataMatrixDlg)
	m_nRadio = 0;
	m_strEcc = _T("ECC200");
	m_nHPoint = 0;
	m_nWPoint = 0;
	m_bAutoSet = FALSE;
	//}}AFX_DATA_INIT
	m_nErrLev=5;
}


void CDataMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMatrixDlg)
	DDX_Control(pDX, IDC_COMBO1, m_conComb);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	DDX_CBString(pDX, IDC_COMBO1, m_strEcc);
	DDX_Text(pDX, IDC_EDIT_H_POINTNUM, m_nHPoint);
	DDX_Text(pDX, IDC_EDIT_W_POINTNUM, m_nWPoint);
	DDX_Check(pDX, IDC_CHECK1, m_bAutoSet);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataMatrixDlg, CDialog)
	//{{AFX_MSG_MAP(CDataMatrixDlg)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_EN_KILLFOCUS(IDC_EDIT_W_POINTNUM, OnKillfocusEditWPointnum)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMatrixDlg message handlers


void CDataMatrixDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_strEcc == "ECC200")
		m_nErrLev = 5;
	if(m_strEcc == "ECC140")
		m_nErrLev = 4;
	if(m_strEcc == "ECC100")
		m_nErrLev = 3;
	if(m_strEcc == "ECC080")
		m_nErrLev = 2;
	if(m_strEcc == "ECC050")
		m_nErrLev = 1;
	if(m_strEcc == "ECC000")
		m_nErrLev = 0;
	CDialog::OnOK();
}

void CDataMatrixDlg::OnCloseupCombo1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i = m_conComb.GetCurSel();
	m_conComb.GetLBText(i,m_strEcc);
	m_conComb.SetCurSel(i);
	if(m_strEcc != "ECC200")
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
		
}

BOOL CDataMatrixDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	switch(m_nErrLev)
	{
	case 5:
		m_conComb.SetCurSel(0);
		break;
	case 4:
		m_conComb.SetCurSel(1);
		break;
	case 3:
		m_conComb.SetCurSel(2);
		break;
	case 2:
		m_conComb.SetCurSel(3);
		break;
	case 1:
		m_conComb.SetCurSel(4);
		break;
	case 0:
		m_conComb.SetCurSel(5);
		break;
	default:
		m_conComb.SetCurSel(0);
		
	}

	if(m_strEcc != "ECC200")
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);

	UpdateDialogView() 	;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataMatrixDlg::UpdateDialogView() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_W_POINTNUM)->EnableWindow(m_bAutoSet);
	GetDlgItem(IDC_EDIT_H_POINTNUM)->EnableWindow(m_bAutoSet);
	if(m_nRadio ==0)
		GetDlgItem(IDC_EDIT_H_POINTNUM)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CDataMatrixDlg::OnCheck1() 
{
	UpdateDialogView();
}

void CDataMatrixDlg::OnKillfocusEditWPointnum() 
{
	UpdateDialogView();
}

void CDataMatrixDlg::OnRadio1() 
{
	UpdateDialogView();
}
void CDataMatrixDlg::OnRadio2() 
{
	UpdateDialogView();
}
