#if !defined(AFX_TEXTPAGEROUNDDLG_H__A087055B_EE5F_4578_981D_44AB8155C169__INCLUDED_)
#define AFX_TEXTPAGEROUNDDLG_H__A087055B_EE5F_4578_981D_44AB8155C169__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextPageRoundDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextPageRoundDlg dialog

class CTextPageRoundDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTextPageRoundDlg)

// Construction
public:
	void UpdateDataView();
	CTextPageRoundDlg();
	~CTextPageRoundDlg();

// Dialog Data
	//{{AFX_DATA(CTextPageRoundDlg)
	enum { IDD = IDD_TEXT_PAGE_ROUND };
	BOOL	m_bCCW;
	double	m_dbDirection;
	double	m_dbRadius;
	double	m_dbStart;
	int		m_nAlign;
	int		m_nPath;
	BOOL	m_bInside;
	BOOL	m_bFixSpace;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTextPageRoundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTextPageRoundDlg)
	afx_msg void OnRadioPathLine();
	afx_msg void OnRadioPathRound();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPAGEROUNDDLG_H__A087055B_EE5F_4578_981D_44AB8155C169__INCLUDED_)
