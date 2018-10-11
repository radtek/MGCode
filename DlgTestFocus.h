#if !defined(AFX_DLGTESTFOCUS_H__26780C50_BBE5_48C3_9407_F54362F9E1F8__INCLUDED_)
#define AFX_DLGTESTFOCUS_H__26780C50_BBE5_48C3_9407_F54362F9E1F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTestFocus.h : header file
//
#include "HLDoc.h"
#include "afxwin.h"
#include "FStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTestFocus dialog

class CDlgTestFocus : public CDialog
{
// Construction
public:
	CDlgTestFocus(CWnd* pParent = NULL);   // standard constructor

	CHLDoc *m_pDoc;
// Dialog Data
	//{{AFX_DATA(CDlgTestFocus)
	enum { IDD = IDD_TEST_FOCUS };
	double	m_dbZStart;
	double	m_dbZStep;
	double	m_dbZStepNum;
	double	m_dbXDis;
	double	m_dbYDis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTestFocus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTestFocus)
	afx_msg void OnButtonMark();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nLay;
	afx_msg void OnBnClickedBtnTest();
	CFStatic m_up;
	CFStatic m_Down;
	double m_dbHandMoveDis;
	int	   m_nOldMoveModel;
	afx_msg void OnBnClickedRadioMoveModel();
	afx_msg void OnBnClickedRadioMoveModel1();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL m_bTime;

	PARA m_oldMarkPara;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTESTFOCUS_H__26780C50_BBE5_48C3_9407_F54362F9E1F8__INCLUDED_)
