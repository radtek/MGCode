#if !defined(AFX_DRAWPOLYGONDLG_H__F4FC5221_58C7_11D4_A483_00105A615525__INCLUDED_)
#define AFX_DRAWPOLYGONDLG_H__F4FC5221_58C7_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawPolygonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawPolygonDlg dialog

class CDrawPolygonDlg : public CDialog
{
// Construction
public:
	int m_nCross;
	int m_nStar;

	CDrawPolygonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDrawPolygonDlg)
	enum { IDD = IDD_DRAW_POLYGON };
	CSliderCtrl	m_sliderOption;
	CSpinButtonCtrl	m_spinNum;
	int		m_nNum;
	int		m_nPolygon;
	int		m_nOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawPolygonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateView();
	BOOL m_bFlag;

	// Generated message map functions
	//{{AFX_MSG(CDrawPolygonDlg)
	afx_msg void OnRadioPolygonCross();
	afx_msg void OnRadioPolygonLine();
	afx_msg void OnRadioPolygonStar();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEditOption();
	afx_msg void OnChangeEditNum();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWPOLYGONDLG_H__F4FC5221_58C7_11D4_A483_00105A615525__INCLUDED_)
