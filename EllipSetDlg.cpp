// EllipSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "EllipSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EllipSetDlg dialog


EllipSetDlg::EllipSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EllipSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EllipSetDlg)
	m_dbRuler = 0.5;
	//}}AFX_DATA_INIT
}


void EllipSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EllipSetDlg)
	DDX_Text(pDX, IDC_RULER, m_dbRuler);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EllipSetDlg, CDialog)
	//{{AFX_MSG_MAP(EllipSetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EllipSetDlg message handlers

void EllipSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_dbRuler <= 0)
	{
		AfxMessageBox("Invalid data.");
		return;
	}

	g_SysCfg.m_dbDrawEllipSet = m_dbRuler;
	g_sys.WriteSysCfg();
	
	CDialog::OnOK();
}
