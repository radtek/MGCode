#if !defined(AFX_MARKPREVIEWDLG_H__B50ACA41_8E53_11D4_A484_00105A615525__INCLUDED_)
#define AFX_MARKPREVIEWDLG_H__B50ACA41_8E53_11D4_A484_00105A615525__INCLUDED_

#include "Chain.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkPreviewDlg.h : header file
//

#include "XZoomWnd.h"

#define WM_PREVIEW_END	WM_USER+50

/////////////////////////////////////////////////////////////////////////////
// CMarkPreviewDlg dialog

class CMarkPreviewDlg : public CDialog
{
// Construction
public:
	CChain *m_pChain;
	CMarkPreviewDlg(CWnd* pParent = NULL);   // standard constructor

	XZoomWnd m_xzoom;
// Dialog Data
	//{{AFX_DATA(CMarkPreviewDlg)
	enum { IDD = IDD_MARK_PREVIEW };
	int		m_nPreviewSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkPreviewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMarkPreviewDlg)
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnPreviewEnd(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	StrokeList m_ViewList;
private:
	BOOL m_bPreview;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKPREVIEWDLG_H__B50ACA41_8E53_11D4_A484_00105A615525__INCLUDED_)
