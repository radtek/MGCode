// EditUserNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "EditUserNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditUserNameDlg dialog


CEditUserNameDlg::CEditUserNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditUserNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditUserNameDlg)
	m_strNewUserName = _T("");
	m_strOldUserName = _T("");
	m_bIsOk  = FALSE;
	m_nCount = 0;
	//}}AFX_DATA_INIT
}


void CEditUserNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditUserNameDlg)
	DDX_Text(pDX, IDC_EDIT_NEWUSERNAME, m_strNewUserName);
	DDX_Text(pDX, IDC_EDIT_OLDUSERNAME, m_strOldUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditUserNameDlg, CDialog)
	//{{AFX_MSG_MAP(CEditUserNameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditUserNameDlg message handlers

void CEditUserNameDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_nCount++;
	if (m_nCount >= 3)
	{
		CDialog::OnOK();
		return;
	}

	UpdateData(TRUE);

	if (0 == m_strNewUserName.CompareNoCase(""))
	{
		AfxMessageBox(IDS_USERNAMECANNTNULL);
		return;
	}
	else
	{
		m_bIsOk = TRUE;
		CDialog::OnOK();
		return;
	}
	
	return;
}

void CEditUserNameDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bIsOk = FALSE;
	CDialog::OnCancel();
}

BOOL CEditUserNameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
