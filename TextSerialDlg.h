#if !defined(AFX_TEXTSERIALDLG_H__D6B8B2C6_AAE4_48C1_AE4D_01ABE562B082__INCLUDED_)
#define AFX_TEXTSERIALDLG_H__D6B8B2C6_AAE4_48C1_AE4D_01ABE562B082__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextSerialDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextSerialDlg dialog

class CTextSerialDlg : public CDialog
{
// Construction
public:
	void SetSerial();
	void GetSerial(LPCTSTR lpszString);
	CStringArray * m_pSerial;
	int *m_pCur;
	
	CString m_strFromFile;
	void UpdateView();
	CTextSerialDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextSerialDlg)
	enum { IDD = IDD_TEXT_SERIAL };
	CComboBox	m_comboSerial;
	CEdit	m_editPrefix;
	CString	m_strPrefix;
	CString	m_strLeft;
	CString	m_strRight;
	UINT	m_nJumpLen;
	BOOL	m_bFill;
	int		m_nJumpStart;
	int		m_nJumpCurrent; //add by henry;
	int		m_nJumpEnd;
	int		m_nFromFile;
	int		m_nJumpStep;
	int		m_nJumpInterval; //add by henry;
	CString	m_strBinText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextSerialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int GetLength(int value);
	
	// Generated message map functions
	//{{AFX_MSG(CTextSerialDlg)
	afx_msg void OnRadioCustom();
	afx_msg void OnRadioFrom();
	afx_msg void OnCheckFill();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditLen();
	afx_msg void OnKillfocusEditStart();
	afx_msg void OnKillfocusEditEnd();
	afx_msg void OnButtonSet();
	afx_msg void OnSelchangeComboSerial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// 允许设定mark次数
	BOOL m_bChageMarkCnt;
public:
	afx_msg void OnBnClickedCheck1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTSERIALDLG_H__D6B8B2C6_AAE4_48C1_AE4D_01ABE562B082__INCLUDED_)
