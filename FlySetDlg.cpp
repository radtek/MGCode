// FlySetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "FlySetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlySetDlg dialog


CFlySetDlg::CFlySetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlySetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlySetDlg)
	m_dbXSpeed = 0.0;
	m_dbYSpeed = 0.0;
	//}}AFX_DATA_INIT
}


void CFlySetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlySetDlg)
	DDX_Text(pDX, IDC_EDIT_SPEEDX, m_dbXSpeed);
	DDX_Text(pDX, IDC_EDIT_SPEEDY, m_dbYSpeed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlySetDlg, CDialog)
	//{{AFX_MSG_MAP(CFlySetDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlySetDlg message handlers
