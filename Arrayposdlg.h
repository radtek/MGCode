#if !defined(AFX_ARRAYPOSDLG_H__BA909242_DC9D_11D4_A485_00105A615525__INCLUDED_)
#define AFX_ARRAYPOSDLG_H__BA909242_DC9D_11D4_A485_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArrayposDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArrayposDlg dialog

class CArrayposDlg : public CDialog
{
// Construction
public:
	CArrayposDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CArrayposDlg)
	enum { IDD = IDD_ARRAYPOS };
	double	m_dbX;
	double	m_dbY;
	double	m_dbR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArrayposDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArrayposDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARRAYPOSDLG_H__BA909242_DC9D_11D4_A485_00105A615525__INCLUDED_)
