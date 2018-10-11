#if !defined(AFX_MOVEACCUDLG_H__D567EF06_A139_4567_A821_4C48D4A3C6EA__INCLUDED_)
#define AFX_MOVEACCUDLG_H__D567EF06_A139_4567_A821_4C48D4A3C6EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MOVEACCUDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MOVEACCUDLG dialog

class MOVEACCUDLG : public CDialog
{
// Construction
public:
	MOVEACCUDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MOVEACCUDLG)
	enum { IDD = IDD_MOVE_DLG };
	double	m_dbMoveX;
	double	m_dbMoveY;
	double	m_dbMoveStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MOVEACCUDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MOVEACCUDLG)
	virtual void OnOK();
	afx_msg void OnButtonMoveUp();
	afx_msg void OnButtonMoveDwon();
	afx_msg void OnButtonMoveLeft();
	afx_msg void OnButtonMoveRigth();
	afx_msg void OnButtonSelMark();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CHLDoc * GetDocument();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVEACCUDLG_H__D567EF06_A139_4567_A821_4C48D4A3C6EA__INCLUDED_)
