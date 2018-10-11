// DrawGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DrawGridDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawGridDlg dialog


CDrawGridDlg::CDrawGridDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawGridDlg)
	m_nCol = 0;
	m_nRow = 0;
	//}}AFX_DATA_INIT
}


void CDrawGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawGridDlg)
	DDX_Text(pDX, IDC_EDIT_COL, m_nCol);
	DDX_Text(pDX, IDC_EDIT_ROW, m_nRow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawGridDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawGridDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawGridDlg message handlers
