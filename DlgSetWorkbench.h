#if !defined(AFX_DLGSETWORKBENCH_H__6197EE6A_ACCE_40CD_B29E_858FE76CBC98__INCLUDED_)
#define AFX_DLGSETWORKBENCH_H__6197EE6A_ACCE_40CD_B29E_858FE76CBC98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetWorkbench.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetWorkbench dialog

class CDlgSetWorkbench : public CDialog
{
// Construction
public:
	CDlgSetWorkbench(CWnd* pParent = NULL);   // standard constructor

	static double m_dbCrossInMarkPosX;
	static double m_dbCrossInMarkPosY;
	double m_dbCrossInCCDPosX;
	double m_dbCrossInCCDPosY;

// Dialog Data
	//{{AFX_DATA(CDlgSetWorkbench)
	enum { IDD = IDD_SET_WORKBENCH };
	double	m_dbMarkToCCD1X;
	double	m_dbMarkToCCD1Y;
	double	m_dbMarkToCCD2X;
	double	m_dbMarkToCCD2Y;
	double	m_dbRefPointX;
	double	m_dbRefPointY;
	double	m_dbLoadPosX;
	double	m_dbLoadPosY;
	double	m_dbUnloadPosX;
	double	m_dbUnloadPosY;
	double	m_dbFastVel;
	double	m_dbSlowVel;
	int		m_nMotionDelayForCCD;
	int		m_nMotionDelayForMark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetWorkbench)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetWorkbench)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButtonCalculateMarktoccd1();
//	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCalculateMarktoccd1();
	double m_dbLeftPtX;
	double m_dbLeftPtY;
	double m_dbRightPtX;
	double m_dbRightPtY;
	double m_dbLoadDis;
	double m_dbUnLoadDis;
	BOOL m_bMove;
	BOOL m_bPosTicps;

	double	m_dbPowerCheckX;
	double	m_dbPowerCheckY;
	double	m_dbPowerCheckZ;
	double	m_dbAutoOffsetDeviationMax;
	double	m_dbAutoOffsetDeviationMin;
	double m_dbWorkbenchFixtureAngle;
	double m_dbCloseDoorTime;
	BOOL m_bDocProGress;
	BOOL m_bUseCutBlow;

	BOOL m_bUseSafeDoor;
	double m_dbWorkPosX;
	double m_dbWorkPosY;
	afx_msg void OnBnClickedButtonGetPosAsWorkPos();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETWORKBENCH_H__6197EE6A_ACCE_40CD_B29E_858FE76CBC98__INCLUDED_)
