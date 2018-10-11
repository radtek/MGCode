#if !defined(AFX_DLGGRAYCOMPENSATE_H__F98D23AE_FA7E_4EE3_8FF1_BF2335FCB1EC__INCLUDED_)
#define AFX_DLGGRAYCOMPENSATE_H__F98D23AE_FA7E_4EE3_8FF1_BF2335FCB1EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGrayCompensate.h : header file
//

#define  COLUMN_COUNT 25
/////////////////////////////////////////////////////////////////////////////
// CDlgGrayCompensate dialog

class CDlgGrayCompensate : public CDialog
{
// Construction
public:
	CDlgGrayCompensate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGrayCompensate)
	enum { IDD = IDD_GRAYCOMPENSATE };
	CSliderCtrl	m_SliderCtrl5;
	CSliderCtrl	m_SliderCtrl4;
	CSliderCtrl	m_SliderCtrl3;
	CSliderCtrl	m_SliderCtrl2;
	CSliderCtrl	m_SliderCtrl1;
	CSliderCtrl	m_SliderCtrl0;
	CStatic	m_PicGrayResult;
	CStatic m_PicGraySource;
	UINT	m_nDest0;
	UINT	m_nDest1;
	UINT	m_nDest2;
	UINT	m_nDest3;
	UINT	m_nDest4;
	UINT	m_nDest5;
	UINT	m_nSource0;
	UINT	m_nSource1;
	UINT	m_nSource2;
	UINT	m_nSource3;
	UINT	m_nSource4;
	UINT	m_nSource5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGrayCompensate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateGrayResultRect(CRect rect,UINT nDestStart,UINT nDestEnd,int iCnt);
	void UpdateGraySourceRect(CRect rect,UINT nDestStart,UINT nDestEnd,int iCnt);
	// Generated message map functions
	//{{AFX_MSG(CDlgGrayCompensate)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBtnRestore();
	afx_msg void OnKillfocusEDITDest0();
	afx_msg void OnKillfocusEDITDest1();
	afx_msg void OnKillfocusEDITDest2();
	afx_msg void OnKillfocusEDITDest3();
	afx_msg void OnKillfocusEDITDest4();
	afx_msg void OnKillfocusEDITDest5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAYCOMPENSATE_H__F98D23AE_FA7E_4EE3_8FF1_BF2335FCB1EC__INCLUDED_)
