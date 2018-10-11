// ChangeUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ChangeUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeUserDlg dialog


CChangeUserDlg::CChangeUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeUserDlg)
	m_strUserName = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


void CChangeUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeUserDlg)
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeUserDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeUserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeUserDlg message handlers

void CChangeUserDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	BOOL bRet = g_sys.IsAvailUser(m_strUserName, m_strPassword);
	if (!bRet)
	{
		AfxMessageBox(IDS_USERORPASSWORDNOOK);
	}
	else
	{
		CDialog::OnOK();
	}

	return;
}
