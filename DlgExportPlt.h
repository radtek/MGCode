#if !defined(AFX_DLGEXPORTPLT_H__E88BEBBB_AEB7_4782_8E00_114F11EDCFF7__INCLUDED_)
#define AFX_DLGEXPORTPLT_H__E88BEBBB_AEB7_4782_8E00_114F11EDCFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExportPlt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExportPlt dialog

class CDlgExportPlt : public CFileDialog
{
	DECLARE_DYNAMIC(CDlgExportPlt)

public:
	CDlgExportPlt(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLETEMPLATE,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

	virtual int DoModal();
	BOOL m_bUnExportFill;
	BOOL m_bScale100;
protected:
	//{{AFX_MSG(CDlgExportPlt)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnFileNameOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTPLT_H__E88BEBBB_AEB7_4782_8E00_114F11EDCFF7__INCLUDED_)
