// ArrayposDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ArrayposDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrayposDlg dialog

CArrayposDlg::CArrayposDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArrayposDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrayposDlg)
	m_dbX = 0.0;
	m_dbY = 0.0;
	m_dbR = 0.0;
	//}}AFX_DATA_INIT
}


void CArrayposDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrayposDlg)
	DDX_Text(pDX, IDC_EDIT_X, m_dbX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dbY);
	DDX_Text(pDX, IDC_EDIT_R, m_dbR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrayposDlg, CDialog)
	//{{AFX_MSG_MAP(CArrayposDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrayposDlg message handlers
