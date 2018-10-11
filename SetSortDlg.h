#if !defined(AFX_SETSORTDLG_H__F1EB6714_2A56_47A0_88A0_744779FC35F7__INCLUDED_)
#define AFX_SETSORTDLG_H__F1EB6714_2A56_47A0_88A0_744779FC35F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSortDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetSortDlg dialog

class CSetSortDlg : public CDialog
{
// Construction
public:
	CSetSortDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetSortDlg)
	enum { IDD = IDD_DIALOG_SETSORT };
	int		m_nNewNum;
	int		m_nChangeType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSortDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetSortDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangPos();
    afx_msg void OnExchangPos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSORTDLG_H__F1EB6714_2A56_47A0_88A0_744779FC35F7__INCLUDED_)
