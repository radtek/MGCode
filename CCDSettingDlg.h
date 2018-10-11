#if !defined(AFX_CCDSETTINGDLG_H__F5ADBF9D_851E_4597_A399_8A10EC72715F__INCLUDED_)
#define AFX_CCDSETTINGDLG_H__F5ADBF9D_851E_4597_A399_8A10EC72715F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCDSettingDlg.h : header file
//
#include "CCDArrayCom.h"
#include "CCDAdjustDlg.h"
#include "CCDWorkDlg.h"



/////////////////////////////////////////////////////////////////////////////
// CCCDSettingDlg dialog

class CCCDSettingDlg : public CDialog
{
// Construction
public:	
	void Save(CString file);
	void Load(CString file);
	void SetTable(int nItem);
	CCCDSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCCDSettingDlg)
	enum { IDD = IDD_CCD_DLG };
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCDSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCCDSettingDlg)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	CCCDArrayCom m_page1;
	CCCDAdjustDlg m_page2;
	CCCDWorkDlg m_page3;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCDSETTINGDLG_H__F5ADBF9D_851E_4597_A399_8A10EC72715F__INCLUDED_)
