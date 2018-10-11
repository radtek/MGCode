#if !defined(AFX_DLGFPK_H__65D6EF8F_B1E6_4457_B9F2_27C13D59AB4D__INCLUDED_)
#define AFX_DLGFPK_H__65D6EF8F_B1E6_4457_B9F2_27C13D59AB4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFPK.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFPK dialog
#include "LayParam/fpkframe.h"
#include "Propertylist.h"

class CDlgFPK : public CDialog
{
// Construction
public:
	void SetPropertyItem(CPropertyItem* pItem, int lbSel, int nFpkType);
	void InitData(CPropertyItem* pItem);
	CFPKDoc* GetDocument();
	void CreateFrame();
	void GetCurPara(CLayer &ly, int nQSwitchdelay);
	CDlgFPK(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL m_bShowIsLine;			//是否显示线性

// Dialog Data
	//{{AFX_DATA(CDlgFPK)
	enum { IDD = IDD_DIALOG_FPK };
	BOOL	m_bApplyAll;
	UINT	m_nNum;
	double	m_dbFpkStartPrec;
	double	m_dbFpkEndPrec;
	int		m_nFpkTime;
	BOOL	m_bIsLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFPK)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFPKFrame *m_pFPKFrame;
	CPropertyItem* m_pItem;
	int m_lbSel;
    int m_nFpkType;

	// Generated message map functions
	//{{AFX_MSG(CDlgFPK)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnModify();
	virtual void OnOK();
	afx_msg void OnBtnMark();
	afx_msg void OnChangeEditNum();
	afx_msg void OnDestroy();
	afx_msg void OnCheckIsline();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void OnSelectMod(int nMod);

	void DispData(PARA &para);

	void OnApply(BOOL bApplyAll, int nQSwitchdelay, double dbFreq, double dbQRelease, double dbCurret, double dbSpeed);
private:
	void UpdateView();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFPK_H__65D6EF8F_B1E6_4457_B9F2_27C13D59AB4D__INCLUDED_)
