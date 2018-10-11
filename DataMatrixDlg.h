#if !defined(AFX_DATAMATRIXDLG_H__1C6769E1_6F5D_11D7_8C60_000AE64F09B1__INCLUDED_)
#define AFX_DATAMATRIXDLG_H__1C6769E1_6F5D_11D7_8C60_000AE64F09B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMatrixDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMatrixDlg dialog

class CDataMatrixDlg : public CDialog
{
// Construction
public:
	CDataMatrixDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataMatrixDlg)
	enum { IDD = IDD_SET_DATAMATRIX2D };
	CComboBox	m_conComb;
	int		m_nRadio;
	CString	m_strEcc;
	int		m_nHPoint;
	int		m_nWPoint;
	BOOL	m_bAutoSet;
	//}}AFX_DATA
	int m_nErrLev;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMatrixDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMatrixDlg)
	virtual void OnOK();
	afx_msg void OnCloseupCombo1();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	afx_msg void OnKillfocusEditWPointnum();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void CDataMatrixDlg::UpdateDialogView() ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMATRIXDLG_H__1C6769E1_6F5D_11D7_8C60_000AE64F09B1__INCLUDED_)
