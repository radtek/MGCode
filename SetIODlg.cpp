//SetIODlg.cpp : implementation file
#include "stdafx.h"
#include "hl.h"
#include "SetIODlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetIODlg dialog


CSetIODlg::CSetIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetIODlg)
	m_strIO = _T("");
	//}}AFX_DATA_INIT
}


void CSetIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetIODlg)
	DDX_Text(pDX, IDC_EDIT_IO, m_strIO);
	DDV_MaxChars(pDX, m_strIO, 5);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetIODlg, CDialog)
	//{{AFX_MSG_MAP(CSetIODlg)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	ON_EN_CHANGE(IDC_EDIT_IO, OnChangeEditIo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetIODlg message handlers

void CSetIODlg::OnButtonGet() 
{
	// TODO: Add your control notification handler code here
	g_DAT.GetInputStr(m_strIO);
	UpdateData(FALSE);
}

void CSetIODlg::OnOK() 
{
	// TODO: Add extra validation here
	if (m_strIO.GetLength()<5){
		AfxMessageBox("IO stauts length is 5 numbems");
		return;
	}
	CDialog::OnOK();
}

void CSetIODlg::OnChangeEditIo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); 
	/////
	char ch;
	int i=0;
	for (i=0; i<m_strIO.GetLength(); i++)
	{
		ch = m_strIO.GetAt(i);
		if (ch=='0' || ch=='1')
			continue;
		else 
			break;
	}
	if ( i<m_strIO.GetLength()){
		AfxMessageBox("must be 0 or 1!");
	}
}
