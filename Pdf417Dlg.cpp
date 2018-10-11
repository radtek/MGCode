// Pdf417Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "Pdf417Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdf417Dlg dialog


CPdf417Dlg::CPdf417Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPdf417Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPdf417Dlg)
	m_nLineNum = 0;
	//}}AFX_DATA_INIT
}


void CPdf417Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPdf417Dlg)
	DDX_Text(pDX, IDC_EDIT_LINENUM, m_nLineNum);
	DDV_MinMaxUInt(pDX, m_nLineNum, 0, 90);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPdf417Dlg, CDialog)
	//{{AFX_MSG_MAP(CPdf417Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPdf417Dlg message handlers

void CPdf417Dlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CPdf417Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( m_nLineNum<3)
		m_nLineNum = 3;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
