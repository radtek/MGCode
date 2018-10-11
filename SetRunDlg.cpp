// SetRunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetRunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRunDlg dialog


CSetRunDlg::CSetRunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRunDlg)
	m_nDelay = 0;
	m_nMove = 0;
	m_nDirection = -1;
	//}}AFX_DATA_INIT
}


void CSetRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRunDlg)
	DDX_Text(pDX, IDC_EDIT_DELAY, m_nDelay);
	DDX_Text(pDX, IDC_EDIT_MOVE, m_nMove);
	DDX_Radio(pDX, IDC_RADIO_DIRECTION, m_nDirection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRunDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRunDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRunDlg message handlers
