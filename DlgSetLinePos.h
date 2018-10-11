#if !defined(AFX_DLGSETLINEPOS_H__D804E3C1_CD45_4CBD_8259_96DF3338BE5E__INCLUDED_)
#define AFX_DLGSETLINEPOS_H__D804E3C1_CD45_4CBD_8259_96DF3338BE5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetLinePos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetLinePos dialog

class CDlgSetLinePos : public CDialog
{
// Construction
public:
	CDlgSetLinePos(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetLinePos)
	enum { IDD = IDD_DIALOG_SETLINEPOS };
	double	m_dbLinePos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetLinePos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetLinePos)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETLINEPOS_H__D804E3C1_CD45_4CBD_8259_96DF3338BE5E__INCLUDED_)
