// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "HL.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "HLView.h"
#include "DlgWorkbenchMeasure.h"
#include "DlgSetDxfPrecision.h"
#include "MyFont.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMainFrame
HICON g_hIcoList[20];

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_COMMAND_EX(ID_VIEW_CHANGE, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHANGE, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_DRAW, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAW, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_ZOOM, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_PROPERTY, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTY, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_HELPINFO()
	ON_UPDATE_COMMAND_UI(ID_DISPLAY, OnUpdateDisplay)
	ON_UPDATE_COMMAND_UI(ID_HIDE, OnUpdateHide)
	ON_COMMAND(IDM_KEYBIND_SET, OnKeybindSet)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DISPLAY_INFOR,OnDisplayInfor)
	ON_COMMAND(ID_WORKBENCH_MEASURE_TOOL, &CMainFrame::OnWorkbenchMeasureTool)
	ON_COMMAND(ID_SET_DXF_PRECISION, &CMainFrame::OnSetDxfPrecision)
	ON_COMMAND(ID_MENU_UPLISTOBJ, &CMainFrame::OnMenuUplistobj)
	ON_COMMAND(ID_MENU_DOWNLISTOBJ, &CMainFrame::OnMenuDownlistobj)
	ON_UPDATE_COMMAND_UI(ID_MENU_DOWNLISTOBJ, &CMainFrame::OnUpdateMenuDownlistobj)
	ON_UPDATE_COMMAND_UI(ID_MENU_UPLISTOBJ, &CMainFrame::OnUpdateMenuUplistobj)
//	ON_WM_TIMER()
ON_WM_CLOSE()
//ON_WM_ENTERSIZEMOVE()
//ON_WM_SIZE()
//ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_PROCESSPANE,
	ID_INDICATOR_LENGTH,
	ID_INDICATOR_POS,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	m_pView      = NULL;
	m_bActivate  = TRUE;
} 

CMainFrame::~CMainFrame()
{

	g_DAT.BeamOff();
	double bEnd;
	g_pMotion->GetPara("END", "end", bEnd);
	if (bEnd)
	{
		g_pMotion->WaitMoveFinish(eMaskXYZ);
		for (int i = 0; i < 7; i ++)
		{
			g_pMotion->OutBit(i, TRUE);
		}
		g_pMotion->DestoryMotion();
		CWnd *wnd = FindWindow(NULL,"CCDVIEW");
		if (NULL != wnd)
		{
			wnd->SendMessage(WM_CLOSE);
		}
		HANDLE hself = GetCurrentProcess();
		TerminateProcess(hself, 0);
	}
}

int GetDialogFontHeight(int nResID)
{
	HRSRC hResInfo = FindResource(NULL, (LPCSTR)nResID, RT_DIALOG);
	if (hResInfo!=NULL)
	{
		HGLOBAL hResData = LoadResource(NULL, hResInfo);
		if (hResData!=NULL)
		{
			int nIndex = 13;
			short *pData = (short *)LockResource(hResData);
			while(pData[nIndex++]);
			while(pData[nIndex++]);
			while(pData[nIndex++]);

			HDC hDC = GetDC(HWND_DESKTOP);
			int lfHeight = -MulDiv(pData[nIndex], GetDeviceCaps(hDC, LOGPIXELSY),72);
			ReleaseDC(HWND_DESKTOP,hDC);

			return lfHeight;
		}
	}
	return -11;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//EnableLoadDockState(FALSE);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
	CDockingManager::SetDockingMode(DT_SMART);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = FALSE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = FALSE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	CMyFont fontMenu;
	fontMenu.SetHeight(GetDialogFontHeight(IDD_ABOUTBOX));
	m_wndMenuBar.SetMenuFont(&fontMenu.GetLogFont());

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	
	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//显示背景图（公司商标）
	/*if(!m_wndClient.SubclassWindow(m_hWndMDIClient)){
		TRACE("Falled to subclass MDI client window.\n");
		return -1;
	}*/

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT))){
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	int nIndex = 0;
	UINT nID   = 0;
	UINT nStyle= 0;
	int cxWidth= 0;

	nIndex = m_wndStatusBar.CommandToIndex(ID_PROCESSPANE);
	m_wndStatusBar.GetPaneInfo(nIndex, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex, nID, nStyle, 300);
	m_wndStatusBar.EnablePaneProgressBar(nIndex);

	nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_POS);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,160);

	nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_LENGTH);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,140);
	
	//建立常用工具条,设定常用工具栏的放置特性及风格
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), 1010) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//建立缩放工具条,设定缩放工具栏的放置特性及风格
	if (!m_wndZoomBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),1011) ||
		!m_wndZoomBar.LoadToolBar(IDR_ZOOM))
	{
		TRACE0("Failed to create zoombar\n");
		return -1;      // fail to create
	}

	//建立变换工具条,设定变换工具栏的放置特性及风格
	if (!m_wndChangeBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),1012) ||
		!m_wndChangeBar.LoadToolBar(IDR_OBJCHANGE))
	{
		TRACE0("Failed to create changebar\n");
		return -1;      // fail to create
	}

	//建立绘图工具条,设定绘图工具栏的放置特性及风格
	if (!m_wndDrawBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),1013) ||
		!m_wndDrawBar.LoadToolBar(IDR_DRAW))
	{
		TRACE0("Failed to create drawbar\n");
		return -1;      // fail to create
	}
	
	//建立排序工具条
	if (!m_wndSort.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),1014) ||
		!m_wndSort.LoadToolBar(IDR_SORTOBJ))
	{
		TRACE0("Failed to create drawbar\n");
		return -1;      // fail to create
	}
	
	//建立对齐排列工具条
	if (!m_wndArrangeBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),1015) ||
		!m_wndArrangeBar.LoadToolBar(IDR_ARRANGE))
	{
		TRACE0("Failed to create arrangebar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);

	CString str;
	str.LoadString(IDS_TITLE_MAINFRAME);
	m_wndToolBar.SetWindowText(str);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	
	str.LoadString(IDS_TITLE_ZOOM);
	m_wndZoomBar.SetWindowText(str);
	m_wndZoomBar.EnableDocking(CBRS_ALIGN_ANY);

	str.LoadString(IDS_TITLE_CHANGE);
	m_wndChangeBar.SetWindowText(str);
	m_wndChangeBar.EnableDocking(CBRS_ALIGN_ANY);

    str.LoadString(IDS_SORTOBJ);
	m_wndSort.SetWindowText(str);
	m_wndSort.EnableDocking(CBRS_ALIGN_ANY);

	str.LoadString(IDS_TITLE_DRAW);
	m_wndDrawBar.SetWindowText(str);
	m_wndDrawBar.EnableDocking(CBRS_ALIGN_ANY);

	str.LoadString(IDS_TOOLBAR_ALGIN);
	m_wndArrangeBar.SetWindowText(str);
	m_wndArrangeBar.EnableDocking(CBRS_ALIGN_ANY);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	//DockPane(&m_wndToolBar);
	//DockPaneLeftOf(&m_wndChangeBar, &m_wndToolBar);
	//DockPaneLeftOf(&m_wndZoomBar,&m_wndChangeBar);
    //DockPaneLeftOf(&m_wndSort,&m_wndZoomBar);

	DockPane(&m_wndSort);
	DockPaneLeftOf(&m_wndZoomBar,&m_wndSort);
	DockPaneLeftOf(&m_wndChangeBar,&m_wndZoomBar);
	DockPaneLeftOf(&m_wndToolBar, &m_wndChangeBar);

//////////////////////////////////////////////////////////////////////////
	//建立属性面板
	str.LoadString(IDS_OBJECTPROPERTY);
	m_XBarProperty.Create(str,this,CRect(0,0,220,200), FALSE, 1030,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
		WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI);
 	m_XBarProperty.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_XBarProperty,AFX_IDW_DOCKBAR_LEFT);

	RecalcLayout();
	CRect rect;
    m_XBarProperty.GetWindowRect(&rect);
    rect.OffsetRect(0,1);

	//建立打标参数面板
	str.LoadString(IDS_MARKPARA);
	m_XBarMarkPara.Create(str,this, CRect(0,0,220,200), TRUE, 1031,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
		WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI);
	m_XBarMarkPara.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_XBarMarkPara, AFX_IDW_DOCKBAR_LEFT,&rect);
	m_XBarMarkPara.AttachToTabWnd(&m_XBarProperty,DM_SHOW, TRUE);

	//对象列表
	str.LoadString(IDS_OBJLIST);
	m_XBarObjectlist.Create(str,this, CRect(0,0,200,200), TRUE, 1032,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
		WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI);
 	m_XBarObjectlist.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_XBarObjectlist,AFX_IDW_DOCKBAR_RIGHT,&rect);
	m_XBarObjectlist.AttachToTabWnd(&m_XBarProperty, DM_SHOW, TRUE);


	//建立打标面板
	str=("Cutting Control");
	m_XMarkNormalPane.Create(str,this, CRect(0,0,370,200), TRUE, 1033,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
		WS_CLIPCHILDREN | CBRS_RIGHT| CBRS_FLOAT_MULTI);
	m_XMarkNormalPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_XMarkNormalPane,AFX_IDW_DOCKBAR_RIGHT);


	////////////////
	DockPane(&m_wndDrawBar);
	RecalcLayout();
	m_wndDrawBar.GetWindowRect(&rect);
	rect.OffsetRect(0, 1);
	DockPane(&m_wndArrangeBar, AFX_IDW_DOCKBAR_LEFT, rect);

	g_hIcoList[0] = AfxGetApp()->LoadIcon(IDI_MOVE_MARK);
	g_hIcoList[1] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_HIGTH);
	g_hIcoList[2] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_45_ANGLE);
	g_hIcoList[3] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_135_ANGLE);
	g_hIcoList[4] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_WITH);
	g_hIcoList[5] = AfxGetApp()->LoadIcon(IDI_ICON_ROTATE);

	m_XBarProperty.m_PSDlg.ShowWindow(SW_HIDE);
    m_XBarProperty.m_FillEditDlg.ShowWindow(SW_HIDE);
	m_XBarMarkPara.m_PropDlg.ShowWindow(SW_HIDE);
	m_XBarObjectlist.m_ObjectListDlg.ShowWindow(SW_HIDE);
//	m_XObjectlist.m_toolbar.ShowWindow(SW_HIDE);
	
	CString strCustom;
	strCustom.LoadString(IDS_CUSTOM_PANE);
	EnablePaneMenu(TRUE, ID_MENU_OTHERWINDOW, strCustom, ID_MENU_VIEWPANE);
	
	//AddProgressCtrl();
	ModifyStyle(0, FWS_PREFIXTITLE);	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL bRet = CMDIFrameWndEx::PreCreateWindow(cs);
	//cs.style &= ~WS_MINIMIZEBOX;
	//cs.style &= ~WS_THICKFRAME;
	//cs.style &= ~WS_SIZEBOX;
	//cs.style &= ~SC_MINIMIZE;
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::DockControlBarLeftOf(CToolBar *Bar, CToolBar *LeftOf)
{
    CRect rect;
    DWORD dw;
    UINT n;

    RecalcLayout();
    LeftOf->GetWindowRect(&rect);
    rect.OffsetRect(1,0);
    dw=LeftOf->GetBarStyle();
    n = 0;
    n = (dw & CBRS_ALIGN_TOP)            ? AFX_IDW_DOCKBAR_TOP    :n;
    n = (dw & CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM :n;
    n = (dw & CBRS_ALIGN_LEFT   && n==0) ? AFX_IDW_DOCKBAR_LEFT   :n;
    n = (dw & CBRS_ALIGN_RIGHT  && n==0) ? AFX_IDW_DOCKBAR_RIGHT  :n;

    DockControlBar(Bar,n,&rect);
}


void CMainFrame::RecalcLayout(BOOL bNotify) 
{
 	CMDIFrameWndEx::RecalcLayout(bNotify);
}

void CMainFrame::UpdateProperty(BOOL bNoDoc)
{
//	int nCmdShow = bNoDoc?SW_HIDE:SW_SHOW;
	CHLDoc *pDoc=m_XBarMarkPara.m_PropDlg.GetDocument();
	if(pDoc==NULL)
	{
        m_XBarProperty.m_PSDlg.ShowWindow(SW_HIDE);
        m_XBarMarkPara.m_PropDlg.ShowWindow(SW_HIDE);
		m_XBarProperty.m_FillEditDlg.ShowWindow(SW_HIDE);
		m_XBarObjectlist.m_ObjectListDlg.ShowWindow(SW_HIDE);
//		m_XObjectlist.m_toolbar.ShowWindow(SW_HIDE);
		m_XMarkNormalPane.m_MarkNormalDlg.ShowWindow(SW_HIDE);
	}
	else
	{
		m_XBarProperty.m_PSDlg.ShowWindow(SW_SHOW);
	    m_XBarMarkPara.m_PropDlg.ShowWindow(SW_SHOW);
	    m_XBarProperty.m_FillEditDlg.ShowWindow(SW_SHOW);
		m_XBarObjectlist.m_ObjectListDlg.ShowWindow(SW_SHOW);
//		m_XObjectlist.m_toolbar.ShowWindow(SW_SHOW);
		m_XMarkNormalPane.m_MarkNormalDlg.ShowWindow(SW_SHOW);
//		pDoc->m_chain.DelAllObjInList();
//        pDoc->m_chain.InsertChainObjToList();
	}

//	m_XBarProperty.m_PropDlg.UpdateProperty();
	return;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//消除：打标参数修改后直接按打标工具时，最后修改的参数无效
	CWnd *pWnd=GetFocus();
	if(pWnd)
	{
		HWND hWnd=pWnd->GetSafeHwnd();
		::SendMessage(hWnd,WM_KILLFOCUS,NULL,NULL);
	}

	return CMDIFrameWndEx::OnCommand(wParam, lParam);
}

CDocument* CMainFrame::GetActiveDocument() 
{
	CDocument* pDoc = CMDIFrameWndEx::GetActiveDocument();
	CMDIChildWnd* pChild = MDIGetActive();

	if(pChild)
	{
		UpdateProperty(TRUE);
	}
	else
	{
		UpdateProperty();
	}
	return pDoc;
}

void CMainFrame::OnUpdateDisplay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bShow=m_XBarObjectlist.IsVisible();
	pCmdUI->SetCheck(bShow);

	return;
}

void CMainFrame::OnUpdateHide(CCmdUI* pCmdUI) 
{
	BOOL bShow=m_XBarMarkPara.IsVisible();
	pCmdUI->SetCheck(bShow);
}

void CMainFrame::SetPos(double dbX,double dbY)
{
	m_XBarProperty.m_PSDlg.SetPos(dbX,dbY);
}

void CMainFrame::SetSize(double dbSizeX,double dbSizeY)
{
	m_XBarProperty.m_PSDlg.SetSize(dbSizeX,dbSizeY);
}

void CMainFrame::OpenDoor()
{
	m_XBarProperty.m_PSDlg.OpenDoor();
}

LRESULT CMainFrame::OnDisplayInfor(WPARAM wpara/* =0 */,LPARAM lpara/* =0 */)
{
	//CLayer	m_XBarProperty.m_PropDlg.m_lay.GetLayer()
	return NULL;
}

void CMainFrame::SetMousePosInfo(double x, double y)
{
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_POS);
	CString strTemp1;
	strTemp1.LoadString(ID_INDICATOR_POS);
	CString strTemp2;
	strTemp2.Format("%0.3f, %0.3f", x, y);
	strTemp1 += strTemp2;
	m_wndStatusBar.SetPaneText(index, strTemp1);
}

void CMainFrame::SetLengthInfo(double length)
{
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_LENGTH);
	CString strTemp1;
	strTemp1.LoadString(ID_INDICATOR_LENGTH);
	CString strTemp2;
	strTemp2.Format("%0.3f", length);
	strTemp1 += strTemp2;
	m_wndStatusBar.SetPaneText(index, strTemp1);
}
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) 
	{ 
		if(m_keyBind.m_hAccel != NULL)
		{
			if(::TranslateAccelerator(m_hWnd, m_keyBind.m_hAccel, pMsg) != 0)
			{
				return TRUE;
			}
		}
	}

	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnKeybindSet() 
{
	// TODO: Add your command handler code here
	m_keyBind.DoModal();
	//RefreshCmdSet();
}

void CMainFrame::SetProStep(int nStep)
{
	int nIndex = m_wndStatusBar.CommandToIndex(ID_PROCESSPANE);
	if (nStep > 100)
	{
		nStep = 100;
	}
	m_wndStatusBar.SetPaneProgress(nIndex, nStep);
	return;
}

void CMainFrame::RefreshCmdSet()
{/*
	CString strFilePath;
	char strTemp[1024];
	GetModuleFileName(NULL, strTemp, 1024);
	strFilePath = strTemp;
	int index = strFilePath.ReverseFind('\\');
	strFilePath = strFilePath.Left(index+1);

	try
	{
		UINT nId,nStyle;
		int iImage;
		int i=0;
		CStdioFile file(strFilePath + "cmd.set", CFile::modeCreate|CFile::modeWrite);
		int count = m_wndDrawBar.GetCount();
		for (i=0; i<count; i++)
		{
			m_wndDrawBar.GetButtonInfo(i, nId, nStyle, iImage);
			if (nStyle != TBBS_SEPARATOR && nId != 0)
			{
				CString strDesc;
				strDesc.LoadString(nId);
				int index = strDesc.Find('\n');
				strDesc = strDesc.Left(index);
				CString strLine;
				strLine.Format("%d,绘图,%s\r\n", nId, strDesc);
				file.WriteString(strLine);
			}
		}

		count = m_wndChangeBar.GetCount();
		for (i=0; i<count; i++)
		{
			m_wndChangeBar.GetButtonInfo(i, nId, nStyle, iImage);
			if (nStyle != TBBS_SEPARATOR && nId != 0)
			{
				CString strDesc;
				strDesc.LoadString(nId);
				int index = strDesc.Find('\n');
				strDesc = strDesc.Left(index);
				CString strLine;
				strLine.Format("%d,变换对象,%s\r\n", nId, strDesc);
				file.WriteString(strLine);
			}
		}

		count = m_wndZoomBar.GetCount();
		for (i=0; i<count; i++)
		{
			m_wndZoomBar.GetButtonInfo(i, nId, nStyle, iImage);
			if (nStyle != TBBS_SEPARATOR && nId != 0)
			{
				CString strDesc;
				strDesc.LoadString(nId);
				int index = strDesc.Find('\n');
				strDesc = strDesc.Left(index);
				CString strLine;
				strLine.Format("%d,缩放排序,%s\r\n", nId, strDesc);
				file.WriteString(strLine);
			}
		}

		count = m_wndArrangeBar.GetCount();
		for (i=0; i<count; i++)
		{
			m_wndArrangeBar.GetButtonInfo(i, nId, nStyle, iImage);
			if (nStyle != TBBS_SEPARATOR && nId != 0)
			{
				CString strDesc;
				strDesc.LoadString(nId);
				int index = strDesc.Find('\n');
				strDesc = strDesc.Left(index);
				CString strLine;
				strLine.Format("%d,排列对齐,%s\r\n", nId, strDesc);
				file.WriteString(strLine);
			}
		}

		file.Flush();
		file.Close();
		AfxMessageBox(IDS_CREATSUCCSED);
	}
	catch (CMemoryException* e){e;}
	catch (CFileException* e){e;}
	catch (CException* e){e;}
	*/
}


int CMainFrame::SetObjecSel(CGroupObj *pGroupObj)
{
	if ( NULL == pGroupObj )
		return -1;
	
	return m_XBarObjectlist.m_ObjectListDlg.SetSelByGroupObj(pGroupObj);
}

void CMainFrame::SetAllObjecUnSel()
{
	m_XBarObjectlist.m_ObjectListDlg.SetObjUnSel();
	m_XBarObjectlist.m_ObjectListDlg.m_nCurItem=-1;
	return;
}

void CMainFrame::OnWorkbenchMeasureTool()
{
	// TODO: 在此添加命令处理程序代码
	static CDlgWorkbenchMeasure *pMeasureDlg=NULL;
	if(pMeasureDlg)
	{
		pMeasureDlg->ShowWindow(SW_SHOW);
	}
	else
	{	
		pMeasureDlg=new CDlgWorkbenchMeasure();
		pMeasureDlg->Create(CDlgWorkbenchMeasure::IDD);
		pMeasureDlg->CenterWindow();
		pMeasureDlg->ShowWindow(SW_SHOW);
	}
}


void CMainFrame::OnSetDxfPrecision()
{
	// TODO: 在此添加命令处理程序代码
	CDlgSetDxfPrecision dlg;
	dlg.m_dbDxfPrecision = g_ParaIni.ReadDouble("DXF", "Precison", 0.2);
	dlg.m_dbGerberOffset = g_ParaIni.ReadDouble("Gerber", "Offset", 0.2);
	if (dlg.DoModal()==IDOK)
	{
		g_ParaIni.WriteDouble("DXF", "Precison", dlg.m_dbDxfPrecision);;
		g_ParaIni.WriteDouble("Gerber", "Offset", dlg.m_dbGerberOffset);
	}
}


void CMainFrame::OnMenuUplistobj()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnMenuDownlistobj()
{
	// TODO: 在此添加命令处理程序代码
}

void CMainFrame::OnUpdateMenuDownlistobj(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BOOL bOn = FALSE;

	CHLDoc *pDoc=NULL;
	CMDIChildWnd *pMDIChildWnd = dynamic_cast<CMDIChildWnd *>(GetActiveFrame());
	if (pMDIChildWnd) pDoc =  dynamic_cast<CHLDoc *>(pMDIChildWnd->GetActiveDocument());
	if (pDoc) bOn = (pDoc->m_chain.m_list.GetCount()>1);

	pCmdUI->Enable(bOn);	
}


void CMainFrame::OnUpdateMenuUplistobj(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BOOL bOn = FALSE;

	CHLDoc *pDoc=NULL;
	CMDIChildWnd *pMDIChildWnd = dynamic_cast<CMDIChildWnd *>(GetActiveFrame());
	if (pMDIChildWnd) pDoc =  dynamic_cast<CHLDoc *>(pMDIChildWnd->GetActiveDocument());
	if (pDoc) bOn = (pDoc->m_chain.m_list.GetCount()>1);
	
	pCmdUI->Enable(bOn);
}


//void CMainFrame::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CMDIFrameWndEx::OnTimer(nIDEvent);
//}



void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (g_pDlgMarkNormalPane != NULL )
	{
		if (g_pDlgMarkNormalPane->m_bMarking)
		{
			return ;
		}
	}
	CMDIFrameWndEx::OnClose();
}



LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	//if (message == WM_NCLBUTTONDBLCLK)
	//{
	//	if (g_pDlgMarkNormalPane != NULL )
	//	{
	//		if (g_pDlgMarkNormalPane->m_bMarking)
	//		{
	//			return FALSE;
	//		}
	//	}
	//}
	return CMDIFrameWndEx::DefWindowProc(message, wParam, lParam);
}
