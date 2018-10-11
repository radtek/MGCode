#if !defined(AFX_EXTENDOBJDLG_H__AB8E0151_9A77_464A_80F2_EEA3FB1DCB0B__INCLUDED_)
#define AFX_EXTENDOBJDLG_H__AB8E0151_9A77_464A_80F2_EEA3FB1DCB0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtendObjDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtendObjDlg dialog

class CExtendObjDlg : public CDialog
{
// Construction
public:
	CExtendObjDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExtendObjDlg)
	enum { IDD = IDD_DIALOG_EXTENDOBJ };
	double	m_dbLineSpace;
	BOOL	m_bIsKillOldObj;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtendObjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtendObjDlg)
	afx_msg void OnButtonExtendobj();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckIskillOldobj();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTENDOBJDLG_H__AB8E0151_9A77_464A_80F2_EEA3FB1DCB0B__INCLUDED_)
