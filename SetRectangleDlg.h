#if !defined(AFX_SETRECTANGLEDLG_H__0DDEB245_3FB1_11D9_87F8_00E04C779BB9__INCLUDED_)
#define AFX_SETRECTANGLEDLG_H__0DDEB245_3FB1_11D9_87F8_00E04C779BB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRectangleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRectangleDlg dialog

class CSetRectangleDlg : public CDialog
{
// Construction
public:
	CSetRectangleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetRectangleDlg)
	enum { IDD = IDD_SETRECTANGLE };
	double	m_dbValue;
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRectangleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRectangleDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETRECTANGLEDLG_H__0DDEB245_3FB1_11D9_87F8_00E04C779BB9__INCLUDED_)
