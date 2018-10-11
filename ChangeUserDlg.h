#if !defined(AFX_CHANGEUSERDLG_H__7711588A_A9EA_4265_9494_974A93CC95E0__INCLUDED_)
#define AFX_CHANGEUSERDLG_H__7711588A_A9EA_4265_9494_974A93CC95E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeUserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangeUserDlg dialog

class CChangeUserDlg : public CDialog
{
// Construction
public:
	CChangeUserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeUserDlg)
	enum { IDD = IDD_DIALOG_CHANGUSER };
	CString	m_strUserName;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangeUserDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEUSERDLG_H__7711588A_A9EA_4265_9494_974A93CC95E0__INCLUDED_)
