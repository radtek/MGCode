// DlgImport.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgImport.h"
#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImport

IMPLEMENT_DYNAMIC(CDlgImport, CFileDialog)

CDlgImport::CDlgImport(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, 0, FALSE)
{
			m_bCenter=TRUE;
			m_bPltBetter = FALSE;
			m_bGerberEnable = FALSE;
}


BEGIN_MESSAGE_MAP(CDlgImport, CFileDialog)
	//{{AFX_MSG_MAP(CDlgImport)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDibDlg)
	DDX_Check(pDX, IDC_CHECK_Center, m_bCenter);
    DDX_Check(pDX, IDC_CHECK_PLT_BETTER, m_bPltBetter);
	//}}AFX_DATA_MAP
}

int CDlgImport::DoModal()
{
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_IMPORT);
	m_ofn.Flags |= OFN_ENABLETEMPLATE;
	return CFileDialog::DoModal();	
}

BOOL CDlgImport::OnFileNameOK()
{
	UpdateData( TRUE );
	return CFileDialog::OnFileNameOK();
}

void CDlgImport::OnTypeChange()
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd *hwndParent = (CWnd*)GetParent();
	CWnd *wndOK = hwndParent->GetDlgItem(IDOK);
	if (wndOK)
	{
		wndOK->EnableWindow(TRUE);

		CString strText;
		hwndParent->GetDlgItemText(cmb1, strText);
		if (strText.Find("Gerber")>=0)
		{
			wndOK->EnableWindow(m_bGerberEnable);
		}
	}
	
	CFileDialog::OnTypeChange();
}

