#if !defined(AFX_SELECTCOORDINARY_H__2B8E417B_66AA_4A86_8BD6_4EFE3D9575A8__INCLUDED_)
#define AFX_SELECTCOORDINARY_H__2B8E417B_66AA_4A86_8BD6_4EFE3D9575A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectCoordinary.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SelectCoordinary dialog

class SelectCoordinary : public CDialog
{
// Construction
public:
	SelectCoordinary(CWnd* pParent = NULL,double *pdbX=NULL,double *pdbY=NULL,double *pdbSizeX=0,double *pDbSizeY=0,CButton *pButt=0,int *pnIcontype=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SelectCoordinary)
	enum { IDD = IDD_SELECTPOS };
	int		m_nBaseType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SelectCoordinary)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CWnd *m_pView;
	CButton *m_pButton;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SelectCoordinary)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRadioBaseBl();
	afx_msg void OnRadioBaseBm();
	afx_msg void OnRadioBaseBr();
	afx_msg void OnRadioBaseMl();
	afx_msg void OnRadioBaseMm();
	afx_msg void OnRadioBaseMr();
	afx_msg void OnRadioBaseTl();
	afx_msg void OnRadioBaseTm();
	afx_msg void OnRadioBaseTr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void UpdateView();
	void SetSelectIcon();
	double *m_pdbX;
	double *m_pdbY;
	double *m_pSizeX;
	double *m_pSizeY;
	int *m_pIconType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTCOORDINARY_H__2B8E417B_66AA_4A86_8BD6_4EFE3D9575A8__INCLUDED_)
