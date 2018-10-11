// SetQRCodebar.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetQRCodebar.h"
// #include "qrlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetQRCodebar dialog

CSetQRCodebar::CSetQRCodebar(CWnd* pParent /*=NULL*/)
	: CDialog(CSetQRCodebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetQRCodebar)
	m_nErrorValue = 0;
	m_nSize = 0;
	//}}AFX_DATA_INIT
}

void CSetQRCodebar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetQRCodebar)
	DDX_CBIndex(pDX, IDC_COMBO_ERRORLEVEL, m_nErrorValue);
	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_nSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetQRCodebar, CDialog)
	//{{AFX_MSG_MAP(CSetQRCodebar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetQRCodebar message handlers

BOOL CSetQRCodebar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
