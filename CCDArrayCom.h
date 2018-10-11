#if !defined(AFX_CCDARRAYCOM_H__FE41309D_BEA8_4151_B1D9_F8641D1AC449__INCLUDED_)
#define AFX_CCDARRAYCOM_H__FE41309D_BEA8_4151_B1D9_F8641D1AC449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCDArrayCom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCCDArrayCom dialog

class CCCDArrayCom : public CPropertyPage
{
	DECLARE_DYNCREATE(CCCDArrayCom)
// Construction
public:
	void SaveData();
	void LoadData();
	CCCDArrayCom();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCCDArrayCom)
	enum { IDD = IDD_CCD_ARRAY_COM };
	double	m_dbCCD1Precision;
	double	m_dbCCD2Precision;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCDArrayCom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCCDArrayCom)
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bStop;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCDARRAYCOM_H__FE41309D_BEA8_4151_B1D9_F8641D1AC449__INCLUDED_)
