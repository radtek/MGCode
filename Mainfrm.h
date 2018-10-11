// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B8BC184B_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_MAINFRM_H__B8BC184B_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bitmapclient.h"
#include "ChangDlg.h"
#include "ProperDlg.h"
#include "MacroDefine.h"
#include "ObjectListDlg.h"
#include "FillEditDlg.h"
#include "KeyBind.h"
#include "DsfDockablePane.h"
#include "DsfObjectListPane.h"
#include "DsfListCtrl.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual CDocument* GetActiveDocument();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateProperty(BOOL bNoDoc=FALSE);
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:

    void RefreshCmdSet();
	void SetProStep(int nStep);

	void SetPos(double dbX,double dbY);
	void SetSize(double dbSizeX,double dbSizeY);
	void SetLengthInfo(double length);
	void SetMousePosInfo(double x, double y);
	void OpenDoor();

	int SetObjecSel(CGroupObj *pGroupObj);
	void SetAllObjecUnSel();

protected:
	CMFCMenuBar m_wndMenuBar;
	CMFCToolBar m_wndToolBar;//视图工具栏
	CMFCToolBar	m_wndChangeBar;
	CMFCToolBar	m_wndDrawBar;//画图工具栏
	CMFCToolBar	m_wndZoomBar;//缩放工具栏
	CMFCToolBar	m_wndArrangeBar;//对齐排列工具条
	CMFCToolBar m_wndSort;
	CMFCStatusBar m_wndStatusBar;

	CKeyBind m_keyBind;
public:
	//CDsfDockablePane m_XDBSpecialty;

	CDsfObjectListPane m_XBarObjectlist;
	CDsfObjectPropertyPane m_XBarProperty;
	CDsfMarkParaPane m_XBarMarkPara;
	CDsfMarkNormalPane m_XMarkNormalPane;

	CBitmapClient m_wndClient;//自定义的一个CWnd派生类，用于显示底面图片
	CView *m_pView;
	BOOL m_bActivate;
	
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateDisplay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHide(CCmdUI* pCmdUI);
	afx_msg void OnKeybindSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDisplayInfor(WPARAM wpara=0,LPARAM lpara=0);
private:
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);
public:
	afx_msg void OnWorkbenchMeasureTool();
	afx_msg void OnSetDxfPrecision();
	afx_msg void OnMenuUplistobj();
	afx_msg void OnMenuDownlistobj();
	afx_msg void OnUpdateMenuDownlistobj(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuUplistobj(CCmdUI *pCmdUI);
//	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B8BC184B_DE24_11D3_845D_444553540001__INCLUDED_)
