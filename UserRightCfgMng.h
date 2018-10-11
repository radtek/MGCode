#if !defined(AFX_USERRIGHTCFGMNG_H__B2F94C77_A546_4A92_8D94_2D146C3556D4__INCLUDED_)
#define AFX_USERRIGHTCFGMNG_H__B2F94C77_A546_4A92_8D94_2D146C3556D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserRightCfgMng.h : header file
//

#include "DsfListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CUserRightCfgMng dialog

class CUserRightCfgMng : public CDialog
{
// Construction
public:
	CUserRightCfgMng(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserRightCfgMng)
	enum { IDD = IDD_DIALOG_USERRIGHTCFGMNG };
	CDsfListCtrl	m_UserNameList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserRightCfgMng)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserRightCfgMng)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListUsernamelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListUsernamelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddUser();
	afx_msg void OnDelUser();
    afx_msg void OnEidtUserName();
    afx_msg void OnEidtUserPassword();
	afx_msg void OnCheckMarknormal();
	afx_msg void OnCheckMarkmuldoc();
	afx_msg void OnCheckCreateobj();
	afx_msg void OnCheckObjedit();
	afx_msg void OnCheckSystemset();
	afx_msg void OnCheckCalibset();
	afx_msg void OnCheckFilesave();
	afx_msg void OnCheckOpenfile();
	afx_msg void OnCheckLaymarkpara();
    afx_msg void OnCheckADVpara();
	afx_msg void OnCheckMng();
	afx_msg void OnCheckSetDefaultuser();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem, int &nSubItem);

	void SetRightItemStatus(USERINFO &UserInfo);
	void GetRightItemStatus(USERINFO &UserInfo);

	void OnSaveRightChange();
	void OnSetDefautUser(BOOL bSel);

	void FlashRightInfo();
	
public:
	int m_nItem;
    int m_nSubItem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERRIGHTCFGMNG_H__B2F94C77_A546_4A92_8D94_2D146C3556D4__INCLUDED_)
