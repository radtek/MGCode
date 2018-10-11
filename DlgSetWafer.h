#if !defined(AFX_DLGSETWAFER_H__7CCEE525_3F86_43DB_9889_865715D1486A__INCLUDED_)
#define AFX_DLGSETWAFER_H__7CCEE525_3F86_43DB_9889_865715D1486A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetWafer.h : header file
//
#include "HLDoc.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetWafer dialog

class CDlgSetWafer : public CDialog
{
// Construction
public:
	CDlgSetWafer(CWnd* pParent = NULL);   // standard constructor

	CHLDoc *m_pDoc;
	void RefreshFocusID();

// Dialog Data
	//{{AFX_DATA(CDlgSetWafer)
	enum { IDD = IDD_SET_WAFER };
	int		m_nLight1;
	int		m_nLight2;
	int		m_nLight3;
	int		m_nLight4;
	double	m_dbDisMaxErr;
	double	m_dbAngleMaxErr;
	double	m_dbHandOffsetX;
	double	m_dbHandOffsetY;
	double	m_dbHandOffsetCCWAngle;
	double	m_dbXAutoScaleAdd;
	double	m_dbYAutoScaleAdd;
	BOOL	m_bEnableXFixSacle;
	BOOL	m_bEnableYFixScale;
	double	m_dbFixXScale;
	double	m_dbFixYScale;
	double	m_dbPoint0X;
	double	m_dbPoint0Y;
	double	m_dbPoint1X;
	double	m_dbPoint1Y;
	double	m_dbPoint2X;
	double	m_dbPoint2Y;
	double	m_dbPoint3X;
	double	m_dbPoint3Y;
	int		m_nLocateModel;
	BOOL	m_bEnableMarkFocus;
	double	m_dbZCCDFocus;
	double	m_dbZMarkFocus;
	CString	m_strProgID1;
	CString	m_strProgNewID2;
	CString	m_strProgNewID3;
	CString	m_strProgNewID4;


	int		m_nGasModel;
	int		m_nDivideType;
	double	m_dbDivideX;
	double	m_dbDivideY;
	double	m_dbDivideAddX;
	double	m_dbDivideAddY;
	double  m_dbDivideOffsetX;
	double  m_dbDivideOffsetY;

	double	m_dbTestPowerFrequency;
	double	m_dbProductPowerMax;
	double	m_dbProductPowerMin;

	int m_dMarkLay;
	double m_dDownHigh;
	CListCtrl m_listCtrlFocusType;
	CString m_strFocus;
	CString m_strMoveDis;
	CString m_strMoveNum;



	BOOL m_bScan;
	double m_dbScanZpos;
	double m_dbScanPtX;
	double m_dbScanPtY;
	int m_ScanPriNum;

	BOOL m_bQPosite;


	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetWafer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgSetWafer)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckEnableMarkingFocus();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnNMClickListFocusType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSetlight();
//	afx_msg void OnBnClickedRadioDivideByStroke();
//	afx_msg void OnBnClickedRadioDivideByObject();
//	afx_msg void OnBnClickedRadioDivideByOne();
	double m_dbScaleLimit;
	CStatic m_picMark1;
	CStatic m_picMark2;
	CStatic m_picMark3;
	CStatic m_picMark4;	
	afx_msg void OnCbnSelchangeComboLocateModel();
	CStatic m_PicMarkBorder;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETWAFER_H__7CCEE525_3F86_43DB_9889_865715D1486A__INCLUDED_)
