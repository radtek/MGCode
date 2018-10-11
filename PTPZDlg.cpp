// PTPZDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "PTPZDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTPZDlg dialog


CPTPZDlg::CPTPZDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPTPZDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPTPZDlg)
	m_dbPosZ = 0.0;
	//}}AFX_DATA_INIT
}


void CPTPZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTPZDlg)
	DDX_Text(pDX, IDC_EDITZ, m_dbPosZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTPZDlg, CDialog)
	//{{AFX_MSG_MAP(CPTPZDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTPZDlg message handlers
