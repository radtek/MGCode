#pragma once
#include "afxdockablepane.h"
#include "DsfDockablePane.h"
#include "ObjectListDlg.h"
#include "ProperDlg.h"
#include "ChangDlg.h"
#include "FillEditDlg.h"
#include "DlgMarkNormalPane.h"

class CDsfObjectListPane : public CDsfDockablePane
{
	DECLARE_DYNAMIC(CDsfObjectListPane)
public:
	CMFCToolBar m_toolbar;
	CObjectListDlg m_ObjectListDlg;
	CDsfObjectListPane(void);
	virtual ~CDsfObjectListPane(void);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AdjustLayout();
	afx_msg void OnAddListObj();
	afx_msg void OnDelListObj();
	afx_msg void OnEditListObj();
	afx_msg void OnUpListObj();
	afx_msg void OnDownListObj();

};

class CDsfMarkParaPane : public CDsfDockablePane
{
	DECLARE_DYNAMIC(CDsfMarkParaPane)
public:
	CProperDlg m_PropDlg;
	CDsfMarkParaPane(void);
	virtual ~CDsfMarkParaPane(void);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

class CDsfObjectPropertyPane : public CDsfDockablePane
{
	DECLARE_DYNAMIC(CDsfObjectPropertyPane)
public:
	CChangDlg m_PSDlg;
	CFillEditDlg m_FillEditDlg;
	CDsfObjectPropertyPane(void);
	virtual ~CDsfObjectPropertyPane(void);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

class CDsfMarkNormalPane : public CDsfDockablePane
{
	DECLARE_DYNAMIC(CDsfMarkNormalPane)
public:
	CDlgMarkNormalPane m_MarkNormalDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};