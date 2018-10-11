#if !defined(AFX_SETBARTEXTDLG_H__1953518A_4F14_46F4_9606_17AE34B3FC5C__INCLUDED_)
#define AFX_SETBARTEXTDLG_H__1953518A_4F14_46F4_9606_17AE34B3FC5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetBarTextDlg.h : header file
//

#include "TextPageRoundDlg.h"
#include "TextPageFntDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSetBarTextDlg dialog

class CSetBarTextDlg : public CDialog
{
// Construction
public:
	CSetBarTextDlg(CWnd* pParent = NULL);   // standard constructor

	CTextPageRoundDlg m_PageRound;
	CTextPageFntDlg m_PageFnt;
	CString m_strInfo;
	
// Dialog Data
	//{{AFX_DATA(CSetBarTextDlg)
	enum { IDD = IDD_SET_BARTEXT };
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetBarTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetBarTextDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBARTEXTDLG_H__1953518A_4F14_46F4_9606_17AE34B3FC5C__INCLUDED_)
