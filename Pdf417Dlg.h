#if !defined(AFX_PDF417DLG_H__1C6769E2_6F5D_11D7_8C60_000AE64F09B1__INCLUDED_)
#define AFX_PDF417DLG_H__1C6769E2_6F5D_11D7_8C60_000AE64F09B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pdf417Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPdf417Dlg dialog

class CPdf417Dlg : public CDialog
{
// Construction
public:
	CPdf417Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPdf417Dlg)
	enum { IDD = IDD_SET_PDF4172D };
	UINT	m_nLineNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdf417Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPdf417Dlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDF417DLG_H__1C6769E2_6F5D_11D7_8C60_000AE64F09B1__INCLUDED_)
