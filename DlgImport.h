#if !defined(AFX_DLGIMPORT_H__0011AD2A_DC9F_43D0_8224_3E76F9EA5FC7__INCLUDED_)
#define AFX_DLGIMPORT_H__0011AD2A_DC9F_43D0_8224_3E76F9EA5FC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgImport dialog

class CDlgImport : public CFileDialog
{
	DECLARE_DYNAMIC(CDlgImport)

public:
	CDlgImport(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT| OFN_ENABLETEMPLATE,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

		virtual int DoModal();
		BOOL m_bCenter;
        BOOL m_bPltBetter;
		BOOL m_bGerberEnable;
protected:
	//{{AFX_MSG(CDlgImport)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnFileNameOK();
	virtual void OnTypeChange();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPORT_H__0011AD2A_DC9F_43D0_8224_3E76F9EA5FC7__INCLUDED_)
