#if !defined(AFX_SETTRACKERDLG_H__186658A4_A0BD_11D3_A07E_00105A615525__INCLUDED_)
#define AFX_SETTRACKERDLG_H__186658A4_A0BD_11D3_A07E_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTrackerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTrackerDlg dialog

class CSetTrackerDlg : public CDialog
{
// Construction
public:
	CSetTrackerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTrackerDlg)
	enum { IDD = IDD_SET_TRACKER };
	int		m_nHandleSize;
	int		m_nLine;
	int		m_nResize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTrackerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTrackerDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTRACKERDLG_H__186658A4_A0BD_11D3_A07E_00105A615525__INCLUDED_)
