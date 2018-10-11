#if !defined(AFX_EXTERNDOC_H__F0E882AB_AB81_4308_96DB_CF20848F6CF2__INCLUDED_)
#define AFX_EXTERNDOC_H__F0E882AB_AB81_4308_96DB_CF20848F6CF2__INCLUDED_

#include "Quad.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExternDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExternDoc document

class CExternDoc : public CDocument
{
protected:
	CExternDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CExternDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExternDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExternDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CQuad CalLimit();
	void DeleteAlterList();
	void DeleteList();
	void ApplyExtern(double dbSpace, BOOL bOpt);
	void DrawAlterList(XDC *pDC);
	void DrawList(XDC *pDC);
	CTypedPtrList<CObList, CGroupObj*> m_list;
	CTypedPtrList<CObList, CGroupObj*> m_alterList;
	// Generated message map functions
protected:
	//{{AFX_MSG(CExternDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTERNDOC_H__F0E882AB_AB81_4308_96DB_CF20848F6CF2__INCLUDED_)
