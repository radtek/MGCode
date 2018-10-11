// StyleSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "StyleSaveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleSaveDlg dialog


CStyleSaveDlg::CStyleSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStyleSaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStyleSaveDlg)
	m_strStyle = _T("");
	//}}AFX_DATA_INIT
}


void CStyleSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyleSaveDlg)
	DDX_Text(pDX, IDC_EDIT_STYLE, m_strStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStyleSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CStyleSaveDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleSaveDlg message handlers

void CStyleSaveDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_strStyle.IsEmpty())
	{
		CString strError;
		strError.LoadString(IDS_YH_NAME);
		MessageBox(strError);
		return;
	}

	CDialog::OnOK();
}


