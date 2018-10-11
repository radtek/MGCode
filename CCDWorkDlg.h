#if !defined(AFX_CCDWORKDLG_H__83E8D50E_08E9_4DE3_8159_159D497D3687__INCLUDED_)
#define AFX_CCDWORKDLG_H__83E8D50E_08E9_4DE3_8159_159D497D3687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCDWorkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCCDWorkDlg dialog

class CCCDWorkDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CCCDWorkDlg)
// Construction
public:
	double m_dbCCDtoWorkx1;
	double m_dbCCDtoWorky1;
	double m_dbCCDtoWorkx2;
	double m_dbCCDtoWorky2;

	void SaveData();
	void LoadData();
	CCCDWorkDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCCDWorkDlg)
	enum { IDD = IDD_CCD_WORK };
	double	m_dbPixelX1;
	double	m_dbPixelX2;
	double	m_dbPixelY1;
	double	m_dbPixelY2;
	double	m_dbWorkX1;
	double	m_dbWorkX2;
	double	m_dbWorkY1;
	double	m_dbWorkY2;
	int		m_nProgID1;
	int		m_nProgID2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCDWorkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCCDWorkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCcdlocate1();
	afx_msg void OnButtonCcdlocate2();
	afx_msg void OnButtonSavePara();
	afx_msg void OnButtonMoveToWork1();
	afx_msg void OnButtonMoveToWork2();
	afx_msg void OnButtonCcdCenter1();
	afx_msg void OnButtonCcdCenter2();
	afx_msg void OnButtonCcd1ToCcd2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	double m_dbRotateAngle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCDWORKDLG_H__83E8D50E_08E9_4DE3_8159_159D497D3687__INCLUDED_)
