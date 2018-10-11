#if !defined(AFX_ARRAYDLG_H__BA909241_DC9D_11D4_A485_00105A615525__INCLUDED_)
#define AFX_ARRAYDLG_H__BA909241_DC9D_11D4_A485_00105A615525__INCLUDED_

#include "Dot.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArrayDlg.h : header file
//
#include "XZoomWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CArrayDlg dialog

class CArrayDlg : public CDialog
{
// Construction
public:
	CDot m_dotBase;
	void Save(CString file);
	void Load(CString file);
	CArray<OBJPOS, OBJPOS> m_dtArray;
	void GetArrayDot();
	void UpdateView();
	CArrayDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CArrayDlg)
	enum { IDD = IDD_ARRAY };
	CStatic	m_objView;
	CListCtrl	m_list;
	int		m_nArray;
	int     m_nRedMark;
	int		m_nColumn;
	double	m_dbColumnDistance;
	int		m_nRow;
	double	m_dbRowDistance;
	BOOL	m_bAuto;
	int		m_nRowFirst;
	int		m_nRoundType;
	double	m_dbRotate;
	double	m_dbLX;
	double	m_dbLY;
	double	m_dbRX;
	double	m_dbRY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArrayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArrayDlg)
	afx_msg void OnRadioCustom();
	afx_msg void OnRadioStandard();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonRefurbish();
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonRotate();
	afx_msg void OnButtonImporttemplate();
	afx_msg void OnButtonExporttemplate();
	afx_msg void OnRadioTemplate();
	afx_msg void OnButtonOffset();
	afx_msg void OnButtonMark();
	afx_msg void OnRadioRed();
	afx_msg void OnRadioLaser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL MulObjSel(CPoint &point, CRect &rect, CWnd *pWnd);
	void LastBrush(CWnd *pWnd, CPoint &ptf, CPoint &ptl);
	void DrawRect(CWnd *pWnd,CPoint &ptf,CPoint &ptl,CPoint &point);

	void DrawSelTraker(XDC *pDC, CQuad &Quad,CWnd *pWnd, CArray<SELQUAD, SELQUAD> &QuadList);
	void DrawSelObjTracker(XDC *pDC, CQuad &Quad,BOOL bFillQuad,CWnd *pWnd);
	void DrawTracker(XDC *pDC,CQuad &quad,CWnd *pWnd);
	void UpdateRectCtrl();

	void LoadTemlate(CString &fileName, CArray<OBJPOS, OBJPOS> &dtArray);
	void SaveTemlate(CString &fileName, CArray<OBJPOS, OBJPOS> &dtArray);

	void DispSelObjProp(int nNum);
	void SetRedMarkQuadlist(CArray<SELQUAD, SELQUAD> &QuadList);
	
	CHLDoc * GetDocument();

public:
	CChain  *m_pChain;
	XZoomWnd m_xzoom;

	CPoint   m_ptSPrev;
	CPoint   m_ptSNow;

	CQuad  m_quad[8];
	CQuad  m_quadSave;
	double m_HITBORDER;

	CArray<SELQUAD, SELQUAD> m_QuadList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARRAYDLG_H__BA909241_DC9D_11D4_A485_00105A615525__INCLUDED_)
