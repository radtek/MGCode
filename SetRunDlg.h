#if !defined(AFX_SETRUNDLG_H__05E78BCE_19EA_11D5_97CC_00E04C3AA16B__INCLUDED_)
#define AFX_SETRUNDLG_H__05E78BCE_19EA_11D5_97CC_00E04C3AA16B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRunDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRunDlg dialog

class CSetRunDlg : public CDialog
{
// Construction
public:
	CSetRunDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetRunDlg)
	enum { IDD = IDD_SET_RUN };
	int		m_nDelay;
	long	m_nMove;
	int		m_nDirection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRunDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRunDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETRUNDLG_H__05E78BCE_19EA_11D5_97CC_00E04C3AA16B__INCLUDED_)
