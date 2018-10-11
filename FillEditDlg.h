#if !defined(AFX_FILLEDITDLG_H__2BF5CC1D_6101_46E9_B20A_B6E8E6793A99__INCLUDED_)
#define AFX_FILLEDITDLG_H__2BF5CC1D_6101_46E9_B20A_B6E8E6793A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FillEditDlg.h : header file
//
#include "Objecteditfill.h"
#include "FillObjmMarkSetDlg.h"
#include "TextEditDlg.h"
#include "BarEditDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CFillEditDlg dialog

class CFillEditDlg : public CDialog
{
// Construction
public:
	CFillEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFillEditDlg)
	enum { IDD = IDD_DIALOG_EDITFILL };
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFillEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFillEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApplyfill();
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFastmark();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetTable(int id);

	void ShowSetTable(BOOL bShow);
	void CheckFastmark();

	void ShowTextItem(CNode *pNode, FILLDATA &fill, BOOL bShow = FALSE);
	void ShowBarItem(CNode *pNode,  FILLDATA &fill, BOOL bShow = FALSE);

public:
	CObjecteditfill     m_ObjFillEdit;
	CFillObjmMarkSetDlg m_FillObjMarkSet;
	CTextEditDlg        m_TextEditDlg;
	CBarEditDlg         m_BarEditDlg;

	int m_nTabIndex;

#ifdef _DOUBLE_HEAD_MARK
	CDbSpecialtyDlg m_DBSpecialtyDlg;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLEDITDLG_H__2BF5CC1D_6101_46E9_B20A_B6E8E6793A99__INCLUDED_)
