// DlgExportPlt.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgExportPlt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExportPlt

IMPLEMENT_DYNAMIC(CDlgExportPlt, CFileDialog)

CDlgExportPlt::CDlgExportPlt(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd,0,0)
		//m_bVistaStyle 在XP下默认为0，Win7下默认为1，当为1时会重载失败
{
		m_bUnExportFill=FALSE;
		m_bScale100=FALSE;
}


BEGIN_MESSAGE_MAP(CDlgExportPlt, CFileDialog)
	//{{AFX_MSG_MAP(CDlgExportPlt)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgExportPlt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDibDlg)
	DDX_Check(pDX, IDC_CHECK_UnExportFill, m_bUnExportFill);
	DDX_Check(pDX,IDC_CHECK_Scale100,m_bScale100);	
	//}}AFX_DATA_MAP
}

int CDlgExportPlt::DoModal()
{
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_EXPORTPLT);
	m_ofn.Flags |= OFN_ENABLETEMPLATE;
	return CFileDialog::DoModal();	
}

BOOL CDlgExportPlt::OnFileNameOK()
{
	UpdateData( TRUE );
	return CFileDialog::OnFileNameOK();
}