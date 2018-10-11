#if !defined(AFX_EXTERNVIEWDLG_H__DE5FEB19_6C74_40BB_A0AE_730E7A3F1EB8__INCLUDED_)
#define AFX_EXTERNVIEWDLG_H__DE5FEB19_6C74_40BB_A0AE_730E7A3F1EB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExternViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExternViewDlg dialog
#include "ExternView.h"

class CExternViewDlg : public CDialog
{
// Construction
public:
	CExternViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExternViewDlg)
	enum { IDD = IDD_DIALOG_EXTERNVIEW };
	BOOL	m_bIsKillOldObj;
	double	m_dbLineSpace;
	BOOL	m_bOpt;
	CString	m_strLineSpace;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExternViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetWndOffset(int ox, int oy, UINT nID);
	void SetList(CHLDoc *pHLDoc);
	void CreateView();
	CExternDoc *m_pDoc;
	CExternView *m_pView;
	CFrameWnd *m_pChildFrame;
	CHLDoc *m_pHLDoc;
// Implementation
protected:
	CSize m_oldSize;

	// Generated message map functions
	//{{AFX_MSG(CExternViewDlg)
	afx_msg void OnBtnApply();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnAdjustpage();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTERNVIEWDLG_H__DE5FEB19_6C74_40BB_A0AE_730E7A3F1EB8__INCLUDED_)
