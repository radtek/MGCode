#if !defined(AFX_CCDADJUSTDLG_H__D526450C_6E53_435F_B017_1C9D541B1FF1__INCLUDED_)
#define AFX_CCDADJUSTDLG_H__D526450C_6E53_435F_B017_1C9D541B1FF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCDAdjustDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CCCDAdjustDlg dialog

class CCCDAdjustDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CCCDAdjustDlg)
// Construction
public:
	CCCDAdjustDlg();   // standard constructor

	void CountRatio(double &dbXRatio, double &dbYRatio, double &dbXAngle, double dbMove=0.5);
	void SaveData();
	void LoadData();

// Dialog Data
	//{{AFX_DATA(CCCDAdjustDlg)
	enum { IDD = IDD_CCD_ADJUST };
	double	m_dbXMMPerPixel;
	double	m_dbYMMPerPixel;
	double	m_dbAngleCCDAndWorktable;
	double	m_dbXMMPerPixel2;
	double	m_dbYMMPerPixel2;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCDAdjustDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:	

	// Generated message map functions
	//{{AFX_MSG(CCCDAdjustDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCountRatio();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonCountRatio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCDADJUSTDLG_H__D526450C_6E53_435F_B017_1C9D541B1FF1__INCLUDED_)
