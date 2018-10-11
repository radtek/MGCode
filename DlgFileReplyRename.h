#if !defined(AFX_DLGFILEREPLYRENAME_H__047F3188_5C84_4ECC_9C74_7C69B358A442__INCLUDED_)
#define AFX_DLGFILEREPLYRENAME_H__047F3188_5C84_4ECC_9C74_7C69B358A442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileReplyRename.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFileReplyRename dialog

class CDlgFileReplyRename : public CDialog
{
// Construction
public:
	CDlgFileReplyRename(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFileReplyRename)
	enum { IDD = IDD_FILEREPLYRENAME };
	CString	m_strPromptMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFileReplyRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFileReplyRename)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILEREPLYRENAME_H__047F3188_5C84_4ECC_9C74_7C69B358A442__INCLUDED_)
