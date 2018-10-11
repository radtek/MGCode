#if !defined(AFX_PTPZDLG_H__C36660B6_736F_40C1_A2F2_3CDD334A867F__INCLUDED_)
#define AFX_PTPZDLG_H__C36660B6_736F_40C1_A2F2_3CDD334A867F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTPZDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPTPZDlg dialog

class CPTPZDlg : public CDialog
{
// Construction
public:
	CPTPZDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPTPZDlg)
	enum { IDD = IDD_PTPZDLG };
	double	m_dbPosZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTPZDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPTPZDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTPZDLG_H__C36660B6_736F_40C1_A2F2_3CDD334A867F__INCLUDED_)
