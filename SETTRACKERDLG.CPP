// SetTrackerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetTrackerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTrackerDlg dialog


CSetTrackerDlg::CSetTrackerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTrackerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTrackerDlg)
	m_nHandleSize = 0;
	m_nLine = -1;
	m_nResize = -1;
	//}}AFX_DATA_INIT
}


void CSetTrackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTrackerDlg)
	DDX_Text(pDX, IDC_EDIT_HANDLE_SIZE, m_nHandleSize);
	DDX_Radio(pDX, IDC_RADIO_LINE_DOTTED, m_nLine);
	DDX_Radio(pDX, IDC_RADIO_RESIZE_INSIDE, m_nResize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTrackerDlg, CDialog)
	//{{AFX_MSG_MAP(CSetTrackerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTrackerDlg message handlers

BOOL CSetTrackerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
