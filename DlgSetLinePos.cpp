// DlgSetLinePos.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgSetLinePos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetLinePos dialog


CDlgSetLinePos::CDlgSetLinePos(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetLinePos::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetLinePos)
	m_dbLinePos = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgSetLinePos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetLinePos)
	DDX_Text(pDX, IDC_EDIT_LINE_POS, m_dbLinePos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetLinePos, CDialog)
	//{{AFX_MSG_MAP(CDlgSetLinePos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetLinePos message handlers

void CDlgSetLinePos::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
