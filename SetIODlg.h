#if !defined(AFX_SETIODLG_H__435E6B8E_9BCC_49A2_83A3_6142CA43359A__INCLUDED_)
#define AFX_SETIODLG_H__435E6B8E_9BCC_49A2_83A3_6142CA43359A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetIODlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetIODlg dialog

class CSetIODlg : public CDialog
{
// Construction
public:
	CSetIODlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetIODlg)
	enum { IDD = IDD_SET_IO };
	CString	m_strIO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetIODlg)
	afx_msg void OnButtonGet();
	virtual void OnOK();
	afx_msg void OnChangeEditIo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETIODLG_H__435E6B8E_9BCC_49A2_83A3_6142CA43359A__INCLUDED_)
