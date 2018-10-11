// AddUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "AddUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddUserDlg dialog


CAddUserDlg::CAddUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddUserDlg)
	m_strUserName = _T("");
	m_strPassword1 = _T("");
	m_strPassword2 = _T("");
	m_bIsOk = FALSE;
	m_nCount = 0;
	//}}AFX_DATA_INIT
}


void CAddUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddUserDlg)
	DDX_Text(pDX, IDC_EDIT_ADDUSERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD1, m_strPassword1);
	DDX_Text(pDX, IDC_EDIT_PASSWORD2, m_strPassword2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddUserDlg, CDialog)
	//{{AFX_MSG_MAP(CAddUserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddUserDlg message handlers

void CAddUserDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (   (0 == m_strPassword1.CompareNoCase("")) 
		|| (0 == m_strPassword2.CompareNoCase(""))
		)
	{
	}

	if (0 != m_strUserName.CompareNoCase(""))
	{
		AfxMessageBox(IDS_USERNAMECANNTNULL);
		return;
	}

	m_nCount++;
	if (m_nCount > 3)
	{
		m_bIsOk = FALSE; 
	    CDialog::OnOK();
	}
	if (0 != m_strPassword1.CompareNoCase(m_strPassword2))
	{
		AfxMessageBox(IDS_PASSWORDISNOOK);
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

void CAddUserDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bIsOk = FALSE; 
	CDialog::OnCancel();
}
