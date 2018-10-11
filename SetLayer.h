#if !defined(AFX_SETLAYER_H__BD215B81_A999_11D8_ABC5_000AE64C79F3__INCLUDED_)
#define AFX_SETLAYER_H__BD215B81_A999_11D8_ABC5_000AE64C79F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetLayer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetLayer dialog

class CSetLayer : public CDialog
{
// Construction
public:
	CSetLayer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetLayer)
	enum { IDD = IDD_SET_LAY };
	int		m_nLayer;
	int	m_nFillLayer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetLayer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetLayer)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETLAYER_H__BD215B81_A999_11D8_ABC5_000AE64C79F3__INCLUDED_)
