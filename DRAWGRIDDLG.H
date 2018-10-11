#if !defined(AFX_DRAWGRIDDLG_H__730317E1_51B5_11D4_A483_00105A615525__INCLUDED_)
#define AFX_DRAWGRIDDLG_H__730317E1_51B5_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawGridDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawGridDlg dialog

class CDrawGridDlg : public CDialog
{
// Construction
public:
	CDrawGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDrawGridDlg)
	enum { IDD = IDD_DRAW_GRID };
	int		m_nCol;
	int		m_nRow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDrawGridDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWGRIDDLG_H__730317E1_51B5_11D4_A483_00105A615525__INCLUDED_)
