#if !defined(AFX_FASTMARKSTOPDLG_H__3F6D436C_DBC3_434D_A148_2AF81E0C89FB__INCLUDED_)
#define AFX_FASTMARKSTOPDLG_H__3F6D436C_DBC3_434D_A148_2AF81E0C89FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FastMarkStopDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFastMarkStopDlg dialog

class CFastMarkStopDlg : public CDialog
{
// Construction
public:
	CFastMarkStopDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFastMarkStopDlg)
	enum { IDD = IDD_DIALOG_FASTMARKSTOP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastMarkStopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFastMarkStopDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnFastMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	void OnFastmark();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTMARKSTOPDLG_H__3F6D436C_DBC3_434D_A148_2AF81E0C89FB__INCLUDED_)
