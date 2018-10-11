#if !defined(AFX_EDITUSERRIGHTPASSWORD_H__E172BFA2_6110_437F_8942_39CDAD47EEE1__INCLUDED_)
#define AFX_EDITUSERRIGHTPASSWORD_H__E172BFA2_6110_437F_8942_39CDAD47EEE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditUserRightPassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditUserRightPassword dialog

class CEditUserRightPassword : public CDialog
{
// Construction
public:
	CEditUserRightPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditUserRightPassword)
	enum { IDD = IDD_DIALOG_EDITUSERRIGHTPASSWORD };
	CString	m_strOldPassword;
	CString	m_strNewPassword1;
	CString	m_strNewPassword2;
	CString	m_strUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditUserRightPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditUserRightPassword)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int  m_nCount;
	BOOL m_bIsOk;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITUSERRIGHTPASSWORD_H__E172BFA2_6110_437F_8942_39CDAD47EEE1__INCLUDED_)
