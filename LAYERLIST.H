#if !defined(AFX_LAYERLIST_H__7869D467_74F8_11D4_A484_00105A615525__INCLUDED_)
#define AFX_LAYERLIST_H__7869D467_74F8_11D4_A484_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LayerList.h : header file
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CLayerList window

class CLayerList : public CListBox
{
// Construction
public:
	CLayerList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerList)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLayerList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLayerList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERLIST_H__7869D467_74F8_11D4_A484_00105A615525__INCLUDED_)
