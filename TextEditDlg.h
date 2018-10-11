#if !defined(AFX_TEXTEDITDLG_H__9DAFB2EC_C74C_4F0B_8D18_97F732FFBF66__INCLUDED_)
#define AFX_TEXTEDITDLG_H__9DAFB2EC_C74C_4F0B_8D18_97F732FFBF66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextEditDlg.h : header file
//

#include "NodeText.h"

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg dialog

class CTextEditDlg : public CDialog
{
// Construction
public:
	CTextEditDlg(CWnd* pParent = NULL);   // standard constructor
	void UpdateDataView();

	CString GetSerialString();
//	CStringArray * m_pSerial;
//	int *m_pCur;

// Dialog Data
	//{{AFX_DATA(CTextEditDlg)
	enum { IDD = IDD_TEXTEDIT_DLG };
	CListBox	m_ListData;
	CString	m_strDate;
	CString	m_strFix;
	int		m_nText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextEditDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSerial();
	afx_msg void OnRadioFix();
	afx_msg void OnRadioSerial();
	afx_msg void OnRadioTime();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeDateEdit();
	afx_msg void OnButtonTextadd();
	afx_msg void OnButtonTextdel();
	afx_msg void OnButtonTextedit();
	afx_msg void OnSelchangeListEditdata();
	afx_msg void OnDestroy();
	afx_msg void OnCancelMode();
	afx_msg void OnButtonAdvance();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetTextInfo(CNode *pNode, FILLDATA &fill);
	BOOL FindTextInfo(int nIndex, TEXTINFO &TextInfo);
	void ListFlash();
	CHLDoc *GetDocument();

	void OnNewDocment();

	void SetShow(BOOL bIsShow);

	BOOL IsCreateDlg();

	void Apply();

public:

public:
	CNodeText *m_pNodeText;
	FILLDATA   m_fill;

	CText   *m_pCurSelText;

	CArray<TEXTINFO, TEXTINFO> m_TextList;

	BOOL m_bIsShow;

	afx_msg void OnBnClickedButton1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTEDITDLG_H__9DAFB2EC_C74C_4F0B_8D18_97F732FFBF66__INCLUDED_)
