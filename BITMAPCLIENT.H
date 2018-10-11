#if !defined(AFX_BITMAPCLIENT_H__4A1A7501_029B_11D2_A203_F5208C428340__INCLUDED_)
#define AFX_BITMAPCLIENT_H__4A1A7501_029B_11D2_A203_F5208C428340__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BitmapClient.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapClient window

class CBitmapClient : public CWnd
{
// Construction
public:
	CBitmapClient();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapClient)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitmapClient();

	// Generated message map functions
protected:
	CBitmap m_bmp;
	//{{AFX_MSG(CBitmapClient)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPCLIENT_H__4A1A7501_029B_11D2_A203_F5208C428340__INCLUDED_)
