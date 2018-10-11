#if !defined(AFX_DLGCHECKPOWER_H__014B6FC5_4196_4C7F_A50B_97C0FCC60920__INCLUDED_)
#define AFX_DLGCHECKPOWER_H__014B6FC5_4196_4C7F_A50B_97C0FCC60920__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCheckPower.h : header file
//
#include "PowerMeter.h"
#include "LaserController.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCheckPower dialog

class CDlgCheckPower : public CDialog
{
// Construction
public:
	CDlgCheckPower(CWnd* pParent = NULL);   // standard constructor

	CString m_strDocPath;

	double m_dbTestPowerFrequency;
	double m_dbProductPowerMax;
	double m_dbProductPowerMin;

	double m_dbCurPower;
	CPowerMeter *m_pPowerMeter;
	CLaserController *m_pLaserController;

	PARA m_oldMarkPara;

// Dialog Data
	//{{AFX_DATA(CDlgCheckPower)
	enum { IDD = IDD_CHECK_POWER };
	int		m_nThermaTrack;
	int		m_nThermaTrackAdd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCheckPower)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCheckPower)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonStartCheck();
	afx_msg void OnButtonThAdd();
	afx_msg void OnButtonThMinus();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonQuitCheck();
	BOOL m_bTime;


	double m_dbLfPos;
	double m_dbRtPos;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHECKPOWER_H__014B6FC5_4196_4C7F_A50B_97C0FCC60920__INCLUDED_)
