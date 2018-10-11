#if !defined(AFX_PTPDLG_H__66FEDC05_B11F_11D7_8454_00055DAA27A9__INCLUDED_)
#define AFX_PTPDLG_H__66FEDC05_B11F_11D7_8454_00055DAA27A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PtpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPtpDlg dialog

class CPtpDlg : public CDialog
{
// Construction
public:
	CPtpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPtpDlg)
	enum { IDD = IDD_PTPDLG };
	double	m_dbX;
	double	m_dbY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPtpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPtpDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTPDLG_H__66FEDC05_B11F_11D7_8454_00055DAA27A9__INCLUDED_)
