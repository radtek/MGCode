#if !defined(AFX_MARKDATASTATDLG_H__93C6E063_5349_11D5_97CE_00E04C3AA16B__INCLUDED_)
#define AFX_MARKDATASTATDLG_H__93C6E063_5349_11D5_97CE_00E04C3AA16B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkdataStatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarkdataStatDlg dialog

class CMarkdataStatDlg : public CDialog
{
// Construction
public:
	void UpdateView();
	CMarkdataStatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMarkdataStatDlg)
	enum { IDD = IDD_MARKDATA_STAT };
	CString	m_strDate;
	CString	m_strMarknum;
	CString	m_strMarktime;
	//}}AFX_DATA

public:
	char m_chPassWord[8];
	int m_nWordLength;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkdataStatDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMarkdataStatDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKDATASTATDLG_H__93C6E063_5349_11D5_97CE_00E04C3AA16B__INCLUDED_)
