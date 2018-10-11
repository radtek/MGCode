#if !defined(AFX_DIBDLG_H__AFF1A201_1679_11D5_8738_80394FC10108__INCLUDED_)
#define AFX_DIBDLG_H__AFF1A201_1679_11D5_8738_80394FC10108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DibDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDibDlg dialog
#include "hldoc.h"
class CDibDlg : public CDialog
{
// Construction
public:
	CDibDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDibDlg)
	enum { IDD = IDD_DRAW_DIB };
	CString	m_strBmpName;
	BOOL	m_bOppsitioncol;
	double	m_dbSpaceX;
	double	m_dbSpaceY;
	double	m_dbDotTime;
	BOOL	m_bLoop;
	int		m_nDibType; //=0;灰度，1：二值
	double	m_dbQFre;
	double	m_dbLoopCompate;
	int		m_nAccTime;
	double	m_dbBrightness;
	double	m_dbContrast;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CHLDoc  *m_pdoc;
	int		m_nComSel;
	double	m_dbMarkSpeed; // mm/us用来计算显示Q频
protected:

	// Generated message map functions
	//{{AFX_MSG(CDibDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusSpace();
	afx_msg void OnCheckLoop();
	virtual void OnOK();
	afx_msg void OnKillfocusEditDottime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
public:
	afx_msg void OnBnClickedRadioGray();
public:
	afx_msg void OnBnClickedRadioDiff();
public:
	afx_msg void OnBnClickedButtonGraycurrent();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIBDLG_H__AFF1A201_1679_11D5_8738_80394FC10108__INCLUDED_)
