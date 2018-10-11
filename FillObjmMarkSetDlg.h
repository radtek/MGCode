#if !defined(AFX_FILLOBJMMARKSETDLG_H__52ECCAF2_B69B_4F03_8CB9_9409B3F53D59__INCLUDED_)
#define AFX_FILLOBJMMARKSETDLG_H__52ECCAF2_B69B_4F03_8CB9_9409B3F53D59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FillObjmMarkSetDlg.h : header file
//
#include "ColorPickerCB.h"
#include "DsfListCtrl.h"
#include "HLView.h"

/////////////////////////////////////////////////////////////////////////////
// CFillObjmMarkSetDlg dialog

class CFillObjmMarkSetDlg : public CDialog
{
// Construction
public:
	CFillObjmMarkSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFillObjmMarkSetDlg)
	enum { IDD = IDD_DIALOG_FILLOBJMARKSET };
	CDsfListCtrl	m_MarkInfoList;
	CComboBox	m_combLayerCnt;
    CComboBox	m_spParaList;
	CColorPickerCB	m_MarkParaList;
	BOOL	m_IsFirstMarkFill;
	BOOL	m_IsMarkFill;
	BOOL	m_IsMarkFram;
	int		m_nMarkCnt;
	BOOL	m_bIsResMark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFillObjmMarkSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage (MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFillObjmMarkSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMarklayercount();
	afx_msg void OnCheckIsmarkfram();
	afx_msg void OnCheckIsmarkfill();
	afx_msg void OnCheckIsfirstmarkfill();
	afx_msg void OnChangeEditMarkcount();
	afx_msg void OnSelchangeComboMarkpara();
	afx_msg void OnClickListMarkinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckSelmark();
	afx_msg LRESULT OnSelMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCheckUseed();
	afx_msg void OnSelchangeComboParaList();
	afx_msg void OnButtonExport();
	afx_msg void OnBtnDelete();
	afx_msg void OnCheckIsresmark();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL SetLayerCount(CLay &lay);
	void GetFillMarkSet(OBJMARKSET &ObjMarkSet);
	void DisplayMarkSet(OBJMARKSET &ObjMarkSet);

	BOOL InitMarkInfoList();
	BOOL SetMarkObjInfo(OBJMARKSET &ObjMarkSet);
	BOOL SetSubInfoToList(OBJMARKSET& ObjMarkSet, int nNum, BOOL bAddFlag);
	CHLDoc *GetDocument();

	void SetSelObjColor(OBJMARKSET &ObjMarkSet, int nItem);
	BOOL HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem, int &nSubItem);

	void UsedMarkSet(BOOL bUsed);

	void InitParaList();

	void Apply();

public:
	OBJMARKSET m_ObjMarkSet;
	int        m_nCurrSelItem;
	afx_msg void OnLvnItemchangedListMarkinfo(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLOBJMMARKSETDLG_H__52ECCAF2_B69B_4F03_8CB9_9409B3F53D59__INCLUDED_)
