#include "stdafx.h"
#include "HL.h"
#include "DsfObjectListPane.h"

IMPLEMENT_DYNAMIC(CDsfObjectListPane, CDsfDockablePane)

CDsfObjectListPane::CDsfObjectListPane(void)
{
}


CDsfObjectListPane::~CDsfObjectListPane(void)
{
}

BEGIN_MESSAGE_MAP(CDsfObjectListPane, CDsfDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_ADDLISTOBJ, OnAddListObj)
	ON_COMMAND(ID_MENU_DELLISTOBJ, OnDelListObj)
	ON_COMMAND(ID_MENU_EDITLISTOBJ, OnEditListObj)
	ON_COMMAND(ID_MENU_UPLISTOBJ, OnUpListObj)
	ON_COMMAND(ID_MENU_DOWNLISTOBJ, OnDownListObj)
END_MESSAGE_MAP()



IMPLEMENT_DYNAMIC(CDsfMarkParaPane, CDsfDockablePane)

CDsfMarkParaPane::CDsfMarkParaPane(void)
{
}


CDsfMarkParaPane::~CDsfMarkParaPane(void)
{
}

BEGIN_MESSAGE_MAP(CDsfMarkParaPane, CDsfDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(CDsfObjectPropertyPane, CDsfDockablePane)

CDsfObjectPropertyPane::CDsfObjectPropertyPane(void)
{
}


CDsfObjectPropertyPane::~CDsfObjectPropertyPane(void)
{
}

BEGIN_MESSAGE_MAP(CDsfObjectPropertyPane, CDsfDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int CDsfObjectListPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDsfDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ObjectListDlg.Create(CObjectListDlg::IDD, this);

	BOOL bRtn = m_toolbar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_OBJLIST);
	m_toolbar.LoadToolBar(IDR_OBJLIST, 0, 0, TRUE);
	m_toolbar.SetOwner(this);
	m_toolbar.RemoveButton(0);
	m_toolbar.RemoveButton(0);
	m_toolbar.RemoveButton(0);
	
	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_toolbar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	return 0;
}


void CDsfObjectListPane::OnSize(UINT nType, int cx, int cy)
{
	CDsfDockablePane::OnSize(nType, cx, cy);

	/*if (GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		m_ObjectListDlg.MoveWindow(&rect);
	}*/

	AdjustLayout();
}

void CDsfObjectListPane::AdjustLayout()
{
	if (GetSafeHwnd()==NULL || m_toolbar.GetSafeHwnd()==NULL || m_ObjectListDlg.GetSafeHwnd()==NULL)
		return;

	CRect rectClient;
	GetClientRect(&rectClient);
	int ntlcy = m_toolbar.CalcFixedLayout(FALSE, TRUE).cy;

	m_toolbar.SetWindowPos(NULL, rectClient.left, rectClient.top, 
		rectClient.Width(), ntlcy, SWP_NOACTIVATE | SWP_NOZORDER);

	m_ObjectListDlg.SetWindowPos(NULL, rectClient.left, rectClient.top + ntlcy, 
		rectClient.Width(), rectClient.Height() -(ntlcy), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CDsfObjectListPane::OnAddListObj()
{
	m_ObjectListDlg.OnAddListObj();
}


void CDsfObjectListPane::OnDelListObj()
{
	m_ObjectListDlg.OnDelListObj();
}

void CDsfObjectListPane::OnEditListObj()
{
	m_ObjectListDlg.OnEditListObj();
}

void CDsfObjectListPane::OnUpListObj()
{
	m_ObjectListDlg.OnUpListObj();
}

void CDsfObjectListPane::OnDownListObj()
{
	m_ObjectListDlg.OnDownListObj();
}

int CDsfMarkParaPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDsfDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_PropDlg.Create(CProperDlg::IDD, this);

	return 0;
}


void CDsfMarkParaPane::OnSize(UINT nType, int cx, int cy)
{
	CDsfDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() && m_PropDlg.GetSafeHwnd())
	{
		CRect rectClient,rectPropDlg;
		GetClientRect(&rectClient);

		m_PropDlg.GetWindowRect(rectPropDlg);
		ScreenToClient(rectPropDlg);
		rectPropDlg.bottom = rectClient.bottom;
		m_PropDlg.MoveWindow(rectPropDlg);
	}
}

int CDsfObjectPropertyPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDsfDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_PSDlg.Create(CChangDlg::IDD, this);
	m_PSDlg.CloseDoor();
	m_FillEditDlg.Create(CFillEditDlg::IDD, this);

	return 0;
}


void CDsfObjectPropertyPane::OnSize(UINT nType, int cx, int cy)
{
	CDsfDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() && m_PSDlg.GetSafeHwnd() && m_FillEditDlg.GetSafeHwnd())
	{
		CRect rectClient;
		GetClientRect(&rectClient);

		CRect rectPSDlg;
		m_PSDlg.GetWindowRect(rectPSDlg); 
		ScreenToClient(rectPSDlg);
//		rectPSDlg.bottom = rectClient.bottom;
		m_PSDlg.MoveWindow(rectPSDlg);

		CRect rectFillEditDlg;
		m_FillEditDlg.GetWindowRect(rectFillEditDlg);
		ScreenToClient(rectFillEditDlg);
		rectFillEditDlg.right =  rectPSDlg.left +  rectFillEditDlg.Width();
		rectFillEditDlg.left  = rectPSDlg.left;
		rectFillEditDlg.bottom = rectPSDlg.bottom + rectFillEditDlg.Height()+1;
		rectFillEditDlg.top    = rectPSDlg.bottom+1;
		m_FillEditDlg.MoveWindow(rectFillEditDlg);
		return;

		CRect rect, rect1, rect2;
		GetClientRect(&rect);
		m_PSDlg.GetWindowRect(&rect1);
		rect2 = rect;
		rect2.bottom = rect2.top + rect1.Height();
		m_PSDlg.MoveWindow(&rect2);
		rect1 = rect;
		rect1.top = rect2.bottom;
		if (rect1.top > rect1.bottom)
		{
			rect1.top = rect1.bottom;
		}
		m_FillEditDlg.MoveWindow(&rect1);

		/*
		CRect rectClient;
		GetClientRect(&rectClient);

		CRect rectObjList;
		m_ObjectListDlg.GetWindowRect(&rectObjList);
		ScreenToClient(rectObjList);
		rectObjList.bottom = rectClient.bottom;
		m_ObjectListDlg.MoveWindow(rectObjList);

		CRect rectPSDlg;
		m_PSDlg.GetWindowRect(rectPSDlg);
		ScreenToClient(rectPSDlg);
		rectPSDlg.right = rectObjList.right+rectPSDlg.Width()+1;
		rectPSDlg.left  = rectObjList.right+1;
		m_PSDlg.MoveWindow(rectPSDlg);

		CRect rectFillEditDlg;
		m_FillEditDlg.GetWindowRect(rectFillEditDlg);
		ScreenToClient(rectFillEditDlg);
		rectFillEditDlg.right =  rectPSDlg.left +  rectFillEditDlg.Width();
		rectFillEditDlg.left  = rectPSDlg.left;
		rectFillEditDlg.bottom = rectPSDlg.bottom + rectFillEditDlg.Height()+1;
		rectFillEditDlg.top    = rectPSDlg.bottom+1;
		m_FillEditDlg.MoveWindow(rectFillEditDlg);
		return;

		CRect rect, rect1, rect2;
		GetClientRect(&rect);
		m_PSDlg.GetWindowRect(&rect1);
		rect2 = rect;
		rect2.bottom = rect2.top + rect1.Height();
		m_PSDlg.MoveWindow(&rect2);
		rect1 = rect;
		rect1.top = rect2.bottom;
		if (rect1.top > rect1.bottom)
		{
			rect1.top = rect1.bottom;
		}
		m_FillEditDlg.MoveWindow(&rect1);
		*/
	}
}


IMPLEMENT_DYNAMIC(CDsfMarkNormalPane, CDsfDockablePane)

BEGIN_MESSAGE_MAP(CDsfMarkNormalPane, CDsfDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDsfMarkNormalPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDsfDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_MarkNormalDlg.Create(CDlgMarkNormalPane::IDD, this);


	return 0;
}


void CDsfMarkNormalPane::OnSize(UINT nType, int cx, int cy)
{
	CDsfDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() && m_MarkNormalDlg.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
 		m_MarkNormalDlg.MoveWindow(&rect);
	}
}