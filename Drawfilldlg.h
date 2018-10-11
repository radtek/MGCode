#if !defined(AFX_DRAWFILLDLG_H__6C15CA01_4D40_11D4_B789_C4B0DE1A197E__INCLUDED_)
#define AFX_DRAWFILLDLG_H__6C15CA01_4D40_11D4_B789_C4B0DE1A197E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawFillDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawFillDlg dialog

class CDrawFillDlg : public CDialog
{
// Construction
public:
	void UpdateDataView();
	CDrawFillDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDrawFillDlg)
	enum { IDD = IDD_DRAW_FILL };
	BOOL	m_bBack;
	BOOL	m_bCross;
	BOOL	m_bEven;
	BOOL	m_bFirstFill;
	double	m_dbAngle1;
	double	m_dbAngle2;
	double	m_dbDistance;
	double	m_dbSpace1;
	double	m_dbSpace2;
	BOOL	m_bPtp;
	double	m_dbPtp;
	double	m_dbCircleLen;
	BOOL	m_bCircle;
	BOOL	m_bBetter;
	double	m_dbHeadKillLen;
	double	m_dbTailKillLen;
	//}}AFX_DATA
	int m_nFill;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawFillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDrawFillDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCross();
	afx_msg void OnCheckCircle();
	afx_msg void OnCheckBack();
	afx_msg void OnCheckBetter();
	afx_msg void OnCheckPtp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DrawFillDlg_H__6C15CA01_4D40_11D4_B789_C4B0DE1A197E__INCLUDED_)
