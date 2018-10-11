#if !defined(AFX_OBJECTLISTDLG_H__615BDF4A_486B_4231_87A0_E4AD8A185093__INCLUDED_)
#define AFX_OBJECTLISTDLG_H__615BDF4A_486B_4231_87A0_E4AD8A185093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectListDlg dialog
#include <map>
#include "Hldoc.h"
#include "VtrlListCtrl.h"


class CObjectListDlg : public CDialog
{
// Construction
public:
	CObjectListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectListDlg)
	enum { IDD = IDD_DIALOG_OBJECT_LIST };
	CVtrlListCtrl	m_DocObjList;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectListDlg)
	afx_msg void OnClickListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL SetHeadings( const CString& strHeadings );
	BOOL InsertColmItem(int nItem, int nSubItem, LPCTSTR lpszText, LPCTSTR lpszNum, int nImage);
	BOOL DelColmItem(int nItem);
	BOOL AddClmItem(CGroupObj *pGroupObj);
	BOOL DelClmItem(CGroupObj *pGroupObj);
	BOOL AddClmItem(CDsfObj *pObj);
	BOOL DelClmItem(CDsfObj *pObj);
	BOOL DelAllClmItem();
	void ObjDel();
	void ObjListLoad();
	void ObjListUnLoad();
	CHLDoc * GetDocument();

	int SetSelByGroupObj(CGroupObj *pGroupObj);
	void SetObjUnSel();
	
public:
	CImageList m_ImageList;
	int        m_nCurItem;

	//CArray<CTree*, CTree*> m_listInfo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAddListObj();
	afx_msg void OnDelListObj();
	afx_msg void OnEditListObj();
	afx_msg void OnUpListObj();
	afx_msg void OnDownListObj();
	afx_msg void OnLvnItemchangedListDocObject(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTLISTDLG_H__615BDF4A_486B_4231_87A0_E4AD8A185093__INCLUDED_)
