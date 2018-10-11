#if !defined(AFX_SETQRCODEBAR_H__E7736681_0E60_11DA_87EF_000FEAD640C3__INCLUDED_)
#define AFX_SETQRCODEBAR_H__E7736681_0E60_11DA_87EF_000FEAD640C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetQRCodebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetQRCodebar dialog

class CSetQRCodebar : public CDialog
{
// Construction
public:
	CSetQRCodebar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetQRCodebar)
	enum { IDD = IDD_SET_QR2D };
	int		m_nErrorValue;
	int		m_nSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetQRCodebar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetQRCodebar)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETQRCODEBAR_H__E7736681_0E60_11DA_87EF_000FEAD640C3__INCLUDED_)
