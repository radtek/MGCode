#if !defined(AFX_DLGAUTOOFFSET_H__FB6782F5_EA81_49D7_867A_525246594499__INCLUDED_)
#define AFX_DLGAUTOOFFSET_H__FB6782F5_EA81_49D7_867A_525246594499__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoOffset.h : header file
//

#include "HLDoc.h"
#include "FStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoOffset dialog

class CDlgAutoOffset : public CDialog
{
// Construction
public:
	CDlgAutoOffset(CWnd* pParent = NULL);   // standard constructor

	int	   m_nOldMoveModel;
	double m_dbOldHandMoveDis;
	BOOL m_bInit;
	CHLDoc *m_pDoc;

// Dialog Data
	//{{AFX_DATA(CDlgAutoOffset)
	enum { IDD = IDD_AUTO_OFFSET };
	double	m_dbLastX;
	double	m_dbLastY;
	CFStatic	m_left;
	CFStatic	m_right;
	CFStatic	m_top;
	CFStatic	m_bottom;
	double	m_dbHandMoveDis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoOffset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoOffset)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditHandMoveDis();
	afx_msg void OnRadioMoveModel();
	afx_msg void OnRadioMoveModel1();
	afx_msg void OnRadioMoveModel2();
	afx_msg void OnRadioMoveModel3();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedButton1();
	double m_dbLfPos;
	double m_dbRtPos;

	PARA m_oldMarkPara;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOOFFSET_H__FB6782F5_EA81_49D7_867A_525246594499__INCLUDED_)
