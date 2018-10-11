#if !defined(AFX_BARCODEDLG_H__5FA7ECE9_D0FD_4BA8_A917_D522C621FB7D__INCLUDED_)
#define AFX_BARCODEDLG_H__5FA7ECE9_D0FD_4BA8_A917_D522C621FB7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarCodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBarCodeDlg dialog
#include "TextPageTextDlg.h"
#include "BarPage.h"

class CBarCodeDlg : public CDialog
{
// Construction
public:
	CBarCodeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarCodeDlg)
	enum { IDD = IDD_BARCODE_DLG };
	CTabCtrl	m_tab;
	CComboBox	m_ctrlType;
	//}}AFX_DATA
	CTextPageTextDlg m_PageText;
	CBarPage m_BarPage;
	UINT m_nBarType;
	int     m_nOutline;		//PDF417条码行数
	int     m_nErrLev;		//纠错级别，用于QR,DATAMATRIX
	int		m_nHPoint;		//垂直方向点个数,用于QR,DATAMATRIX
	int		m_nWPoint;		//水平方向点个数,用于DATAMATRIX
	BOOL	m_bAutoSet;		//设置条码长宽属性;//true 自动设置;FALSE 用户设置
	
	DWORD   m_nSingleLineMarkCount;
	UINT    m_nNarrowLineNum;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTabText(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangeComboBarcodetype();
	afx_msg void OnButBarcodeset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void FlashBarDefaultProperty();

public:
	BOOL CreateCodeBarStatus(long lType);

public:
	CText* m_pText;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODEDLG_H__5FA7ECE9_D0FD_4BA8_A917_D522C621FB7D__INCLUDED_)
