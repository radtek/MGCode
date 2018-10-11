#if !defined(AFX_TEXTBARCODEPAGEDLG_H__5FA76AD1_FEC9_4B06_875F_09ADABB14054__INCLUDED_)
#define AFX_TEXTBARCODEPAGEDLG_H__5FA76AD1_FEC9_4B06_875F_09ADABB14054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextBarCodePageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextBarCodePageDlg dialog

class CTextBarCodePageDlg : public CDialog
{
// Construction
public:
	CTextBarCodePageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextBarCodePageDlg)
	enum { IDD = IDD_TEXT_BARCODEPAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextBarCodePageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextBarCodePageDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTBARCODEPAGEDLG_H__5FA76AD1_FEC9_4B06_875F_09ADABB14054__INCLUDED_)
