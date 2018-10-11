#if !defined(AFX_DLGHELPPATH_H__D315ED7E_9542_4D70_B8E6_C767D1964024__INCLUDED_)
#define AFX_DLGHELPPATH_H__D315ED7E_9542_4D70_B8E6_C767D1964024__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHelpPath.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHelpPath dialog

class CDlgHelpPath : public CDialog
{
// Construction
public:
	CDlgHelpPath(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHelpPath)
	enum { IDD = IDD_HELPPATH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHelpPath)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHelpPath)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHELPPATH_H__D315ED7E_9542_4D70_B8E6_C767D1964024__INCLUDED_)
