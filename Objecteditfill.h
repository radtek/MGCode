#if !defined(AFX_OBJECTEDITFILL_H__DD110CE2_D2D7_4E12_BC67_4327AB325E57__INCLUDED_)
#define AFX_OBJECTEDITFILL_H__DD110CE2_D2D7_4E12_BC67_4327AB325E57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Objecteditfill.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjecteditfill dialog

class CObjecteditfill : public CDialog
{
// Construction
public:
	CObjecteditfill(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjecteditfill)
	enum { IDD = IDD_DIALOG_OBJEDITFILL };
	BOOL	m_bBack;
	BOOL	m_bCross;
	BOOL	m_bEven;
	BOOL	m_bFirstFill;
	double	m_dbAngle1;
	double	m_dbAngle2;
	double	m_dbDistance;
	double	m_dbSpace1;
	double	m_dbSpace2;
	BOOL	m_bPtp;
	double	m_dbPtp;
	double	m_dbCircleLen;
	BOOL	m_bCircle;
	BOOL	m_bBetter;
	double	m_dbHeadKillLen;
	double	m_dbTailKillLen;
	//}}AFX_DATA
	int m_nFill;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjecteditfill)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjecteditfill)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCross();
	afx_msg void OnCheckCircle();
	afx_msg void OnCheckBack();
	afx_msg void OnCheckBetter();
	afx_msg void OnCheckPtp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void DisplyFillProp(FILLDATA &fillOld);
	void UpdateUI();
	void UpdateDataView();

	void GetNewFillData(FILLDATA &fillNew);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTEDITFILL_H__DD110CE2_D2D7_4E12_BC67_4327AB325E57__INCLUDED_)
