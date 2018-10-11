// PtpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "PtpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPtpDlg dialog


CPtpDlg::CPtpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPtpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPtpDlg)
	m_dbX = 0.0;
	m_dbY = 0.0;
	//}}AFX_DATA_INIT
}


void CPtpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPtpDlg)
	DDX_Text(pDX, IDC_EDITX, m_dbX);
	DDX_Text(pDX, IDC_EDITY, m_dbY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPtpDlg, CDialog)
	//{{AFX_MSG_MAP(CPtpDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPtpDlg message handlers
