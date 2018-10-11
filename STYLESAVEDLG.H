#if !defined(AFX_STYLESAVEDLG_H__999062E1_D4A3_11D2_A204_90352BB489A8__INCLUDED_)
#define AFX_STYLESAVEDLG_H__999062E1_D4A3_11D2_A204_90352BB489A8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StyleSaveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStyleSaveDlg dialog

class CStyleSaveDlg : public CDialog
{
// Construction
public:
	CStyleSaveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStyleSaveDlg)
	enum { IDD = IDD_STYLE_SAVE };
	CString	m_strStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleSaveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStyleSaveDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLESAVEDLG_H__999062E1_D4A3_11D2_A204_90352BB489A8__INCLUDED_)
