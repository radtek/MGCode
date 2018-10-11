#if !defined(AFX_DLGCALISET_H__78F5440D_F671_49CB_943C_013F53458291__INCLUDED_)
#define AFX_DLGCALISET_H__78F5440D_F671_49CB_943C_013F53458291__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSet dialog

class CDlgCaliSet : public CDialog
{
// Construction
public:
	CDlgCaliSet(CWnd* pParent = NULL);   // standard constructor

	CString m_strIniPath;
// Dialog Data
	//{{AFX_DATA(CDlgCaliSet)
	enum { IDD = IDD_CALI_SET };
	CString	m_strPath;
	double	m_dbXScale;
	double	m_dbYScale;
	int		m_nBitMM;
	double	m_dbCCWAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliSet)
	afx_msg void OnButtonSelect();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nLayDelay;
	BOOL m_bEnableAutoCal;
//	afx_msg void OnBnClickedCheckAutoCal();
	afx_msg void OnBnClickedBtnResetauto();
	afx_msg void OnBnClickedCheckAutoCal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALISET_H__78F5440D_F671_49CB_943C_013F53458291__INCLUDED_)
