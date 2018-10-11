#if !defined(AFX_BAREDITDLG_H__B72024C3_6195_451A_BB82_5A8372D00770__INCLUDED_)
#define AFX_BAREDITDLG_H__B72024C3_6195_451A_BB82_5A8372D00770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarEditDlg.h : header file
//

#include "NodeCodebar.h"

/////////////////////////////////////////////////////////////////////////////
// CBarEditDlg dialog

class CBarEditDlg : public CDialog
{
// Construction
public:
	CBarEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarEditDlg)
	enum { IDD = IDD_DIALOG_BAREDIT };
	CComboBox	m_ctrlType;
	int		m_nText;
	CString	m_strDate;
	CString	m_strFix;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButBarcodeset();
	afx_msg void OnEditchangeComboBarcodetype();
	afx_msg void OnButtonSerial();
	afx_msg void OnRadioFix();
	afx_msg void OnRadioSerial();
	afx_msg void OnRadioTime();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnButtonAdvance();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FlashBarDefaultProperty();
	CHLDoc *GetDocument();

public:
	BOOL CreateCodeBarStatus(long lType);
	BOOL IsCreateDlg();
	void SetShow(BOOL bIsShow);
	CString GetSerialString();
	void UpdateDataView();
	void SetTextInfo(CNode *pNode, FILLDATA &fill);

	void Apply();

public:
	BOOL    m_bIsShow;
	UINT    m_nBarType;
	int     m_nOutline;		//PDF417条码行数
	int     m_nErrLev;		//纠错级别，用于QR,DATAMATRIX
	int		m_nHPoint;		//垂直方向点个数,用于QR,DATAMATRIX
	int		m_nWPoint;		//水平方向点个数,用于DATAMATRIX
	BOOL	m_bAutoSet;		//设置条码长宽属性;//true 自动设置;FALSE 用户设置
	DWORD   m_nSingleLineMarkCount;
	UINT    m_nNarrowLineNum;
	CText*  m_pCurSelText;
	CNodeCodebar *m_pNodeBar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BAREDITDLG_H__B72024C3_6195_451A_BB82_5A8372D00770__INCLUDED_)
