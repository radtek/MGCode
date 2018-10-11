// HLView.cpp : implementation of the CHLView class
//

#include "stdafx.h"
#include "HL.h"
#include "MainFrm.h"
#include "EllipSetDlg.h"
#include "HLDoc.h"
#include "HLView.h"
#include "NodePlt.h"
#include "NodeLine.h"
#include "NodeRect.h"
#include "NodeEllipse.h"
#include "NodeGrid.h"
#include "NodePolygon.h"
#include "NodeText.h"
#include "NodeDib.h"
#include "DrawPolygonDlg.h"
#include "NodeCodebar.h"
#include "DrawGridDlg.h"
#include "SetRectangleDlg.h"
#include "TextBaseDlg.h"
#include "DlgImport.h"
#include "DlgHelpPath.h"
#include "MOVEACCUDLG.h"
#include "CmdMAlign.h"

#include "KBChar.h"
#include "UserRightCfgMng.h"
#include "ChangeUserDlg.h"

#include "Nodedxf.h"
#include "NodeNC.h"

#include "DivideLayer.h"

#include "VectorMgr.h"
#include "NodeDot.h"
#include "DlgImportAutoCadFile.h"
#include "NodeGerber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString g_OpenFileName;
BOOL g_bSure=FALSE;

void GenImportFullName(CString& file)
{
	if (-1 != file.Find(':'))
	{
		return;
	}

	CString strHLFileName;
	CMDIFrameWnd *pMainWnd=(CMDIFrameWnd*)AfxGetMainWnd();
	BOOL bFindDocument=FALSE;
	
	if (pMainWnd)
	{
		CMDIChildWnd *pChildWnd=pMainWnd->MDIGetActive();
		if (pChildWnd)
		{
			CHLDoc* pDoc=(CHLDoc*) pChildWnd->GetActiveDocument();
			if (pDoc)
			{
				strHLFileName=pDoc->GetPathName( );
				bFindDocument=TRUE;
			}
		}
	} 

	if (!bFindDocument)
	{
		strHLFileName=g_OpenFileName;
	}

	CString strHLFilePath;
	CString strHLImportFilePath;	
	strHLFilePath=strHLFileName.Left(strHLFileName.ReverseFind('\\'));
	strHLImportFilePath=strHLFilePath+strHLFileName.Mid(strHLFileName.ReverseFind('\\'),strHLFileName.ReverseFind('.')-strHLFileName.ReverseFind('\\'));
	file=strHLImportFilePath+"\\"+file;
	
	return;
}

CView *CHLView::m_pActiveView=NULL;
/////////////////////////////////////////////////////////////////////////////
// CHLView
IMPLEMENT_DYNCREATE(CHLView, CZoomView)

BEGIN_MESSAGE_MAP(CHLView, CZoomView)
	//{{AFX_MSG_MAP(CHLView)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS,OnUpdatePos)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_PAGE, OnZoomPage)
	ON_COMMAND(ID_DRAW_CLICK, OnDrawClick)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CLICK, OnUpdateDrawClick)
	ON_COMMAND(ID_DRAW_HAND, OnDrawHand)
	ON_UPDATE_COMMAND_UI(ID_DRAW_HAND, OnUpdateDrawHand)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAW_RECTANGLE, OnDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, OnUpdateDrawRectangle)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_ZOOM_SELECT, OnZoomSelect)
	ON_COMMAND(ID_ZOOM_ALL, OnZoomAll)
	ON_COMMAND(ID_DRAW_TEXT, OnDrawText)
	ON_UPDATE_COMMAND_UI(ID_DRAW_TEXT, OnUpdateDrawText)
	ON_COMMAND(ID_DRAW_ELLIPSE, OnDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, OnUpdateDrawEllipse)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_LOAD_OBJLIST, OnLoadObjList)
    ON_COMMAND(ID_LOADOBJLIST, OnLoadObjList)
	ON_COMMAND(ID_DRAW_GRID, OnDrawGrid)
	ON_UPDATE_COMMAND_UI(ID_DRAW_GRID, OnUpdateDrawGrid)
	ON_COMMAND(ID_DRAW_POLYGON, OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, OnUpdateDrawPolygon)
	ON_COMMAND(ID_DRAW_CODEBAR, OnDrawCodebar)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CODEBAR, OnUpdateDrawCodebar)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDM_HELPPATH, OnHelppath)
	ON_COMMAND(ID_ZOOMOFF, OnZoomoff)
	ON_COMMAND(ID_ZOOMOPER, OnZoomoper)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOFF, OnUpdateZoomoff)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOPER, OnUpdateZoomoper)
	ON_COMMAND(ID_SORTOBJ, OnSortObj)
	ON_UPDATE_COMMAND_UI(ID_SORTOBJ, OnUpdateSortObj)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_DRAW_DRAG, OnDrawDrag)
	ON_UPDATE_COMMAND_UI(ID_DRAW_DRAG, OnUpdateDrawDrag)
	ON_COMMAND(IDD_MOVEACCU, OnMoveaccu)
	ON_COMMAND(IDD_CONVERTDSF, OnConvert)
	ON_COMMAND(ID_DELDOT, OnDeldot)
	ON_COMMAND(ID_JOINDOT, OnJoindot)
	ON_COMMAND(ID_MOVEACCU, OnMoveaccu)
	ON_COMMAND(ID_SPLITDOT, OnSplitdot)
    ON_COMMAND(ID_SEL_ALLSTROKE, OnSelAllStroke)
	ON_COMMAND(ID_SET_CURPOINT_BEGIN, OnSetCurDotToStart)
	ON_COMMAND(ID_FILTER_POINT, OnFilterPoint)
	ON_COMMAND(ID_REVERRIS_STROKE, OnReverseStroke)
	ON_COMMAND(ID_FILTER_CURDOC_POINT, OnFilterCurDocPoint)
    ON_COMMAND(ID_KILL_ANGLE, OnKillAngle)
	ON_COMMAND(ID_AUTOSORTOBJ, OnAutoSortObj)
	ON_UPDATE_COMMAND_UI(ID_AUTOSORTOBJ, OnUpdateAutoSortObj)
    ON_COMMAND(ID_BUTTON_GO_SORT, OnGSortObj)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_GO_SORT, OnUpdateGSortObj)
	ON_COMMAND(ID_SELSORT, OnSelSortObj)
	ON_UPDATE_COMMAND_UI(ID_SELSORT, OnUpdateSelSortObj)
	ON_COMMAND(ID_ALIGN_BOTTOM, OnAlignBottom)
	ON_COMMAND(ID_ALIGN_LEFT, OnAlignLeft)
	ON_COMMAND(ID_ALIGN_RIGHT, OnAlignRight)
	ON_COMMAND(ID_ALIGN_TOP, OnAlignTop)
	ON_COMMAND(ID_DISTANCE_X, OnDistanceX)
	ON_COMMAND(ID_DISTANCE_Y, OnDistanceY)
	ON_COMMAND(ID_EQUAL_HEIGHT, OnEqualHeight)
	ON_COMMAND(ID_EQUAL_WIDTH, OnEqualWidth)
	ON_COMMAND(ID_EQUAL_WH, OnEqualWh)
	ON_COMMAND(IDM_RIGHTMNG, OnRightMng)
    ON_COMMAND(IDM_CHANGEUSER, OnChangeUser)
	ON_COMMAND(ID_DRAW_DOT, OnDrawDot)
	ON_UPDATE_COMMAND_UI(ID_DRAW_DOT, OnUpdateDrawDot)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CZoomView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CZoomView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CZoomView::OnFilePrintPreview)
//	ON_WM_ACTIVATE()
//ON_WM_CLOSE()
//ON_WM_DESTROY()
ON_WM_CREATE()
ON_WM_TIMER()
ON_WM_ERASEBKGND()
ON_COMMAND(ID_JOINTSTROKE_CURDOC, &CHLView::OnJointstrokeCurdoc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLView construction/destruction

CHLView::CHLView()
{
	m_hCursorClick     =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CLICK));
	m_hCursorShape     =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_SHAPE));
	m_hCursorCross     =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CROSS));
	m_hCursorCrossRot  =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CROSSIN));
	m_hCursorRotateDrag=::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MOUSEROTATE));
	m_hCursorTBDrag    =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_TB_DRAG));
	m_hCursorLRDrag    =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_LR_DRAG));
	m_hDrag=::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_OBJMOVE));
	m_hCursorDoxMove   =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MOVE_BOX));
    m_hCursorZoomIn    =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_ZOOMIN));
    m_hCursorMLineV    =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MLINE_V));
	m_hCursorMLineH    =::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MLINE_H));

	m_hCursorDot		=::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_DOT));

	m_hCursorCur=m_hCursorClick;
	m_nDrawTool=ID_DRAW_CLICK;
	m_bRotate = FALSE;
	m_bDrag=FALSE;
	m_bLBDown=FALSE;
	m_nResult=9;
	///////////////////////////////////////
	m_LTBitmap.LoadBitmap(IDB_MOUSEROTATE_LT);
	m_LRBitmap.LoadBitmap(IDB_MOUSEROTATE_TOP);
	m_RTBitmap.LoadBitmap(IDB_MOUSEROTATE_RT);
	m_TBBitmap.LoadBitmap(IDB_MOUSEROTATE_LEFT);
	m_RBBitmap.LoadBitmap(IDB_MOUSEROTATE_RB);
	m_LBBitmap.LoadBitmap(IDB_MOUSEROTATE_LB);
	///////////////////////////////////////////////

	m_strAutoSortInfo = "";

	m_bActive = FALSE;

	InitIcon();
	CFrameWnd *pFrame=(CFrameWnd*)AfxGetMainWnd();
	CMainFrame *pMFrame=(CMainFrame*)pFrame;
	pMFrame->OpenDoor();

}

CHLView::~CHLView()
{	
	//删除光标
	if (m_hCursorClick) ::DestroyCursor(m_hCursorClick);
	if (m_hCursorShape) ::DestroyCursor(m_hCursorShape);
	if (m_hCursorCross) ::DestroyCursor(m_hCursorCross);
	if (m_hCursorCrossRot) ::DestroyCursor(m_hCursorCrossRot);
    if (m_hCursorDoxMove)  ::DestroyCursor(m_hCursorDoxMove);
    if (m_hCursorZoomIn)   ::DestroyCursor(m_hCursorZoomIn);
    if (m_hCursorMLineH)   ::DestroyCursor(m_hCursorMLineH);
    if (m_hCursorMLineV)   ::DestroyCursor(m_hCursorMLineV);
	if (m_hCursorDot   )		::DestroyCursor(m_hCursorDot);    
	m_track.ClearAll();
}

BOOL CHLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CZoomView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHLView drawing

void CHLView::OnDraw(CDC* pDC)
{
	CHLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	/*********************************************************************/
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());
	CBitmap *pOldBmp = (CBitmap*)memDC.SelectObject(&bm);
	memDC.FillSolidRect(rcClient,RGB(255,255,255));
	/*********************************************************************/

	if(g_nLeftDay < 5)
	{
		CRect rect;
		GetClientRect(&rect);
		CString strFormat;
		strFormat.LoadString(IDS_DOG_DAYLEFT);
		CString strTemp;
		strTemp.Format((const char*)strFormat, g_nLeftDay);
		CFont fontSort;
		fontSort.CreateFont(50, 0,300,0, FW_HEAVY, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS, "Arial");
		CFont* pOldFont = pDC->SelectObject(&fontSort);
		COLORREF oldClr = pDC->SetTextColor(RGB(220, 220, 220));
		int cx = rect.left + rect.Width()/2;
		int cy = rect.top + rect.Height()/2;
		memDC.SetTextAlign(TA_CENTER|TA_BASELINE);
		memDC.TextOut(cx,cy,strTemp);
		memDC.SetTextColor(oldClr);
		memDC.SelectObject(pOldFont);
	}

	DrawMeasureLine(&memDC);

	// TODO: add draw code for native data here
	pDoc->m_pTracker = &m_track;
	XDC *pXDC=PrepareDC(&memDC);

	double dbCenterX = 0;
	double dbCenterY = 0;
	pXDC->PhysicToLogic(dbCenterX, dbCenterY);

	pDoc->Draw(pXDC);

//	DrawBox(pXDC);
	pDoc->m_DivideLayer.Draw(pXDC, (pDoc->m_chain.m_bSelMark && pDoc->m_chain.m_nSelectMarkType==1));
// 	m_pDC = pDC;
	
	if(GetZoomMode()==MODE_ZOOMOPER)
	{
//		m_nDrawTool=ID_DRAW_DRAG;
	}

	if(ID_SORT_OBJECT_LIST == m_nDrawTool)
	{
		DrawMark(pDC);
	}
	
	if(ID_ASORT_OBJECT_LIST == m_nDrawTool)
	{
		AutoSortDraw(pDC);
	}


	CDot dotCCD;
	dotCCD.x = g_ParaIni.m_dbRefPointX - g_pMotion->GetPos(eMaskX);
	dotCCD.y = g_ParaIni.m_dbRefPointY - g_pMotion->GetPos(eMaskY);
	pXDC->PhysicToLogic(dotCCD.x, dotCCD.y);

	CPoint ptDraw;
	ptDraw.x = static_cast<LONG>(dotCCD.x)-16;
	ptDraw.y = static_cast<LONG>(dotCCD.y)-11;

	HICON hIcon1 = AfxGetApp()->LoadIconA(IDI_CCD_MIN);
	pXDC->m_pDC->DrawIcon(ptDraw, hIcon1);
	::DestroyIcon(hIcon1);	

	//
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBmp);


	bm.DeleteObject();
	memDC.DeleteDC();

	
	if(m_nDrawTool!=ID_DRAW_DRAG)
	{
		pDoc->m_chain.RefreshSelectQuadList(pDoc->m_QuadList);
		DrawTracker(pXDC);
	}
	else
	{
		DrawRect();
	}


	return;
}

void CHLView::OnInitialUpdate()
{
	CZoomView::OnInitialUpdate();
	OnZoomPage();

	//设置UNDO
	GetDocument()->GetCommandProcessor().SetFrame((CMainFrame *)AfxGetMainWnd());
    PostMessage(WM_COMMAND, ID_ZOOM_PAGE & 0x0FFFF, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CHLView printing

BOOL CHLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->m_bPreview = TRUE;
	return DoPreparePrinting(pInfo);
}

void CHLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CHLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CHLView diagnostics

#ifdef _DEBUG
void CHLView::AssertValid() const
{
	CZoomView::AssertValid();
}

void CHLView::Dump(CDumpContext& dc) const
{
	CZoomView::Dump(dc);
}

CHLDoc* CHLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHLDoc)));
	return (CHLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHLView message handlers

void CHLView::OnZoomIn() 
{
	// Toggle zoomin mode
	CZoomView::ZoomIn();
}

void CHLView::OnZoomOut() 
{
	// Toggle zoomout mode
	CZoomView::ZoomOut();
}

void CHLView::OnZoomPage() 
{
	//取BOX大小
	double x,y;
	g_DAT.GetMarkRange(x,y);
	double width =x;
	double height=y;

	CQuad quad;
	quad  = CQuad(CDot(0,0),CDot(width,height));
	width = quad.Width()*1.2;
	height= quad.Height()*1.2;
	DoZoomIn(width,height);

	return;
}

void CHLView::NotifyZoom(void)
{
	CHLDoc *pDoc=GetDocument();
	m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());//先不动
}

void CHLView::DrawBox(XDC *pDC)
{
	//取BOX大小
	double x,y;
	g_DAT.GetMarkRange(x,y);

	HPEN  hPenOld,hPen1,hPen2;
	HBRUSH hBrshOld,hBrsh2;
	hPen1 = CreatePen(PS_SOLID,0,BLACK);
	hPen2 = CreatePen(PS_DOT,1,RED);
	hBrsh2= (HBRUSH)GetStockObject(GRAY_BRUSH);

	//画阴影
	hPenOld=(HPEN)pDC->SelectObject(hPen2);
	hBrshOld=(HBRUSH)pDC->SelectObject(hBrsh2);
	
	//画方框
	pDC->SelectObject(hPen1);
	pDC->MoveTo(-x/2,y/2);
	pDC->LineTo(x/2,y/2);
	pDC->LineTo(x/2,-y/2);
	pDC->LineTo(-x/2,-y/2);
	pDC->LineTo(-x/2,y/2);

	//画标准线
	g_bSure=TRUE;
	pDC->SelectObject(hPen2);
	pDC->MoveTo(-2000000,0);
	pDC->LineTo(2000000,0);
	pDC->MoveTo(0,-2000000);
	pDC->LineTo(0,2000000);

	//恢复GDI对象
	pDC->SelectObject(hPenOld);
	pDC->SelectObject(hBrshOld);

	//删除自定义GDI对象
	DeleteObject(hPen1);
	DeleteObject(hPen2);
	DeleteObject(hBrsh2);

	return;
}

void CHLView::OnDrawDrag() 
{
	m_nDrawTool = ID_DRAW_DRAG;
	m_hCursorCur= m_hCursorClick;

	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	BOOL bRet = pDoc->IsHaveTextOrCodeBar();
	if (bRet) 
	{
		CString strMsg("");
		strMsg.LoadString(IDS_OBJHAVETXTORCODEBAR_COVERTTOLINE);
		if (IDOK == AfxMessageBox(strMsg, MB_OKCANCEL))
		{
			pDoc->Changeconvert(FALSE);
			Invalidate();
		}
		else
		{
			return;
		}
	}
	
	if(m_track.HaveObject())
	{
		Invalidate();
	}

	return;
}

void CHLView::OnUpdateDrawDrag(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((m_nDrawTool==ID_DRAW_DRAG));
	return;
}

void CHLView::OnDrawClick() 
{
	m_nDrawTool =ID_DRAW_CLICK;
	m_hCursorCur=m_hCursorClick;
	SetCursor(m_hCursorCur);
	m_track.ClearDot();
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	Invalidate();
	if(m_bRotate)
	{
		m_bRotate=FALSE;
		Invalidate();
		CHLDoc *pDoc=GetDocument();
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	} 

	return;
}

void CHLView::OnUpdateDrawClick(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_nDrawTool==ID_DRAW_CLICK));
}

void CHLView::OnDrawHand() 
{
	if(GetZoomMode() != MODE_ZOOMOFF)
	{
		SetZoomMode(MODE_ZOOMOFF);
	}

	m_nDrawTool =ID_DRAW_HAND;
	m_hCursorCur=m_hCursorCross;
	
	if(m_bRotate)
	{
		m_bRotate = FALSE;
		Invalidate();
		CHLDoc *pDoc = GetDocument();
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	} 

	return;
}

void CHLView::OnUpdateDrawHand(CCmdUI* pCmdUI) 
{
	if(m_nDrawTool==ID_DRAW_HAND)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
	
	return;
}

BOOL CHLView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(GetZoomMode()!=MODE_ZOOMOFF || message==WM_LBUTTONDOWN)
	return CZoomView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}

void CHLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CZoomView::OnLButtonDown(nFlags,point);

	CHLDoc *pDoc     = GetDocument();
	pDoc->m_pTracker = &m_track;
	m_track.m_ptMeasureLine = point;
	CLayer ly        = pDoc->GetCurLayer();
	CProperty pty    = ly.GetProperty();

	if (m_bCanMoveMeasureLine)
	{
		m_bCanMoveMeasureLine = FALSE;
		return;
	}
	if (point.x==0 && point.y==0)
	{
		return;
	}

	CQuad quad;

	//LZQ
	if (pDoc->m_chain.m_bSelMark && pDoc->m_chain.m_nSelectMarkType==1)
	{
		if (m_nDrawTool!=ID_DRAW_CLICK)
		{
			m_nDrawTool =ID_DRAW_CLICK;
			m_hCursorCur=m_hCursorClick;
			SetCursor(m_hCursorCur);
		}

		m_track.Track(point,this);
		quad=(CQuad)m_track.m_rectOut;
		LogicToPhysic(quad);
		quad.Normal();

		SHORT state = HIBYTE(GetKeyState(VK_SHIFT));
		if (state==0)
		{
			pDoc->m_DivideLayer.SelectCell(quad, FALSE);
		}	
		pDoc->m_DivideLayer.SelectCell(quad);

		Invalidate();
		return;
	}
	

	SetCursor(m_hCursorCur);

	switch(m_nDrawTool)
	{
	case ID_DRAW_CLICK:
		{
			if(pDoc->HaveSelectObject())
			{
				quad = pDoc->GetLimit();
				CDot dot;
				double dbRatioX(0),dbRatioY(0),dbMoveX(0),dbMoveY(0);
				quad = pDoc->GetLimit();
				m_nResult = m_track.GetResult(point,quad,this);
				if (2 == m_nResult) 
				{
//					TrackRotate(point,TRUE/*bShift*/,0);
//					Invalidate();
//					break;
				} 

				CDot dt(point);
				LogicToPhysic(dt.x, dt.y);
				m_track.GetDragPara(m_nResult,quad,dot,dbRatioX,dbRatioY,dbMoveX,dbMoveY,this);
				if(m_nResult>=0 && m_nResult<=8)
				{
					pDoc->Drag(dot,dbRatioX,dbRatioY,dbMoveX,dbMoveY);//Drag操作
//					Invalidate();
			        break;
				}
			}

			if(!HitOtherObject(point))
			{
				m_track.Track(point,this);
				quad=(CQuad)m_track.m_rectOut;
				LogicToPhysic(quad);
				quad.Normal();
				pDoc->MultiSelect(quad);// 多选操作
			}

			pDoc->SetObjSelInQuadList();

//			Invalidate();
			break;
		}
	/////////////////////////////////////////////////////////////////////////////////////
	case ID_DRAW_DRAG:
		{
			if(!m_track.HaveObject())
			{
				OnDrawClick();
			}

			if(!m_track.ListEmpty())
			{
				//是否存在选中点，如果有选中点进入
				if(m_track.HitSelectDot(point,this))
				{
					//对选中点进行编辑
					pDoc->DotEdit(&m_track,point,this);
				}
				else 
				{
					//当前末选中，但MOUSE已选中其它点时进入
					if (m_track.HitOtherDot(point,this))
					{
						m_track.ClearAndStore(point,this);
						pDoc->DotEdit(&m_track,point,this);
					}
					else 
					{
						m_track.Track(point,this);
						quad=(CQuad)m_track.m_rectOut;
						LogicToPhysic(quad);
						quad.Normal();
						if(m_track.HasDot(quad))
						{
							m_track.ClearAndStore(quad,this);
						}
						else 
						{
							m_track.DelRect(this);
							m_track.InverDrawDot(this);
							m_track.ClearDot();
						}
					}
				}
			}
			else  
			{
				//进行选点操作
				if(m_track.IsOnDot(point,this))
				{
					//选中后对选中的点进行操作
					m_track.Edit(point,this);
					pDoc->DotEdit(&m_track,point,this);
				}
				else 
				{
					//用框选来选点
					m_track.Track(point,this);
					quad=(CQuad)m_track.m_rectOut;
					LogicToPhysic(quad);
					quad.Normal();
					if(m_track.HasDot(quad))
					{
						m_track.Edit(quad,this);
					}
					else 
					{
						m_track.DelRect(this);
					}			
				}
			}
			Invalidate();
			break;
		}
	////////////////////////////////////////////////////////////////////////////////
	case ID_DRAW_HAND:
		{
			ClearMark(); // 清除对象标示

			CNode *pNode=new CNodeLine;
			pNode->SetProperty(PTY_INIT,pty);

			if(pNode->Track(this,point))
			{
				pDoc->AddNode(pNode, NODE_LINE);
			}
			else
			{
				delete pNode;
			}

			break;
		}
	case ID_DRAW_RECTANGLE:
		{
			ClearMark(); // 清除对象标示

			CNode *pNode=new CNodeRect;
			pNode->SetProperty(PTY_INIT,pty);

			if(pNode->Track(this,point))
			{
				pDoc->AddNode(pNode, NODE_RECT);
			}
			else
			{
				delete pNode;
			}

			break;
		}
	case ID_DRAW_ELLIPSE:
		{
			ClearMark(); // 清除对象标示

			CNode *pNode=new CNodeEllipse;
			pNode->SetProperty(PTY_INIT,pty);

			if(pNode->Track(this, point))
			{
				pDoc->AddNode(pNode, NODE_ELLIPSE);
			}
			else
			{
				delete pNode;
			}

			break;
		}
	case ID_DRAW_POLYGON:
		{
			ClearMark(); // 清除对象标示

			CNode *pNode = new CNodePolygon;
			pNode->SetProperty(PTY_INIT,pty);
			
			if(pNode->Track(this,point))
			{
				pDoc->AddNode(pNode, NODE_POLYGON);
			}
			else
			{
				delete pNode;
			}

			break;
		}
	case ID_DRAW_GRID:
		{
			ClearMark(); // 清除对象标示

			CNode *pNode=new CNodeGrid();
			pNode->SetProperty(PTY_INIT,pty);

			if(pNode->Track(this,point))
			{
				pDoc->AddNode(pNode, NODE_GRID);
			}
			else
			{
				delete pNode;
			}
			
			break;
		}
	case ID_DRAW_TEXT:
		{
			ClearMark(); // 清除对象标示

			pDoc->SetCurMgrText();
			
			CTextBaseDlg dlg;
			dlg.m_bCreate   = TRUE;
			if(dlg.DoModal() == IDOK)
			{
				POSITION pos = NULL;
				CNodeText* pNodeText = NULL;
				
				pos = dlg.m_Nodelist.GetHeadPosition();
				while(pos!=NULL)
				{
					pNodeText = dlg.m_Nodelist.GetNext(pos);
					pNodeText->SetProperty(PTY_INIT,pty);
					pNodeText->CreateString(FALSE);		
					{
						//居中	
						CDot dot=pNodeText->GetLimit().GetBaseDot();
						double moveX = CENTER - dot.x;
						double moveY = CENTER - dot.y;
						pNodeText->Move(moveX,moveY);
					}
					pDoc->AddNode(pNodeText, NODE_TEXT_DSF);
				} 
			}
			break;
		}
		
	case ID_DRAW_CODEBAR:
		{
			ClearMark(); // 清除对象标示
			
			g_MgrText    = &(GetDocument()->m_chain.m_MgrText);
			CNode *pNode = new CNodeCodebar();
			if (NULL == pNode)
			{
				return;
			}

			pNode->SetProperty(PTY_INIT,pty);
			if(pNode->Track(this,point))
			{
				pDoc->AddNode(pNode, NODE_CODEBAR);
			}
			else
			{
				delete pNode;
			}
			break;
		}
	case ID_DRAW_DOT:
		{
			ClearMark(); // 清除对象标示
			CNode *pNode = new CNodeDot;
			pNode->SetProperty (PTY_INIT, pty);
			if(pNode->Track(this,point))
			{
				pDoc->AddNode(pNode, NODE_DOT);
			}
			else
			{
				delete pNode;
			}
			break;
		}
	default:
		{
			break;
		}
	}

	// 显示当前选中对象的属性
	DispSelObjProp();
	
	Invalidate();

	return;
}



void CHLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	SetCursor(m_hCursorCur);
	CZoomView::OnLButtonUp(nFlags,point);
}

BOOL CHLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CZoomView::OnMouseWheel(nFlags, zDelta, pt);
	{
//		CHLDoc* pDoc=(CHLDoc*)GetDocument();
//		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
//		CRect rectTrue;
//		m_track.GetTrueRect(rectTrue);
//		InvalidateRect(rectTrue);
	}
	
	return TRUE;
}

void CHLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_bCanMoveMeasureLine = FALSE;
	m_bCanSetLinePos = FALSE;
	CZoomView::OnMouseMove(nFlags,point);
	SetCursor(m_hCursorCur);
	if(m_nDrawTool==ID_DRAW_CLICK)
	{
		CHLDoc *pDoc=(CHLDoc*)GetDocument();
		if(pDoc->HaveSelectObject( ))
		{
			CQuad quad;
			quad=pDoc->GetLimit();
			m_nResult=m_track.GetResult(point,quad,this);//得到HIT点
			if(8 == m_nResult)
			{
				SetCursor(m_hDrag);
			}
			else if(m_nResult >= 0 && m_nResult < 8)
			{
				SetCursor(m_hCursorCrossRot);
			}
			else
			{
				SetCursor(m_hCursorClick);
			}
			goto END_MOUSE_MOVE;
		}// end if(pDoc->HaveSelectObject( ))
		
		for(int i=0; i<m_nMeasureLineCount; i++)
		{
			CMeasureLine line = m_arrayMeasureLine[i];
			if (line.m_nType == CMeasureLine::LEFT_TO_RIGHT)
			{
				double dbx = 0;
				double dby = line.m_dbPos;
				PhysicToLogic(dbx, dby);
				LONG ly = (LONG)dby;
				if (point.y>(ly-5) && point.y<(ly+5))
				{
					SetCursor(m_hCursorMLineV);
					m_bCanMoveMeasureLine = TRUE;
					m_bCanSetLinePos = TRUE;
					m_nMeasureLineIndex = i;
				}
			}
			else
			{
				double dbx = line.m_dbPos;
				double dby = 0;
				PhysicToLogic(dbx, dby);
				LONG lx = (LONG)dbx;
				if (point.x>(lx-5) && point.x<(lx+5))
				{
					SetCursor(m_hCursorMLineH);
					m_bCanMoveMeasureLine = TRUE;
					m_bCanSetLinePos = TRUE;
					m_nMeasureLineIndex = i;
				}
			}
		}
	}// end if(m_nDrawTool==ID_DRAW_CLICK)

END_MOUSE_MOVE:
	double x = point.x;
	double y = point.y;
	LogicToPhysic(x, y);
	((CMainFrame*)AfxGetApp()->GetMainWnd())->SetMousePosInfo(x, y);

	//	SetFocus();
	return;
}

void CHLView::OnDrawRectangle() 
{
	// TODO: Add your command handler code here
	m_nDrawTool  = ID_DRAW_RECTANGLE;
	m_hCursorCur = m_hCursorCross;
    
	if(m_bRotate)
	{
		m_bRotate=FALSE;
		Invalidate();
		CHLDoc *pDoc=GetDocument();
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	}

	return;
}

void CHLView::OnUpdateDrawRectangle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nDrawTool==ID_DRAW_RECTANGLE)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}

	return;
}

void CHLView::OnUpdatePos(CCmdUI *pCmdUI)
{
	CRect rect;
	GetClientRect(&rect);
	if(!rect.PtInRect(this->m_ptCur))
	{
		return;
	}

 	ViewDPtoLP(&m_ptCur);
	CDot dot(m_ptCur);
	
	CString str;
	str.Format("%.3f,%.3f",dot.x,dot.y);
	pCmdUI->Enable(TRUE);
	pCmdUI->SetText(str);

	return;
}

void CHLView::DrawTracker(XDC *pDC)
{
	CHLDoc *pDoc=GetDocument();

	CQuad quad;
	quad=pDoc->GetTrackerQuad();

	if(quad.IsEmpty())
	{
		pDoc->m_QuadList.RemoveAll();
		return;
	}
//	pDoc->GetQuadInTrackerQuad(quad);
	m_track.DrawSelTraker(pDC, quad, this, pDoc->m_QuadList);

	return;
}

void CHLView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch(lHint)
	{
	case 0:
		{
			//无任何参数时，更新全部
			InvalidateRect(NULL);
			break;
		}
	case UPDATE_TRACKER_VIEW:
		{
			//更新显示跟踪框部分
			CRect rectTrue;
			//m_tracker.GetTrueRect(&rectTrue);
			m_track.GetTrueRect(rectTrue);
			InvalidateRect((CRect)rectTrue);
			break;
		}
	case UPDATE_TRACKER_PROPERTY:
		{
			//改变跟踪器显示风格
			CWinApp *pApp=AfxGetApp();
		//	m_tracker.m_nHandleSize	=pApp->GetProfileInt("TRACKER","HANDLESIZE",4);
 		//	m_tracker.m_nStyle		=pApp->GetProfileInt("TRACKER","STYLE",0);
			break;
		}
	case UPDATE_QUAD:
		{
			CQuad* pQuad = (CQuad*)pHint;
			CQuad Quad=*pQuad;
			PhysicToLogic(Quad);
			InvalidateRect((CRect*)&Quad);//no use
			break;
		}
	case UPDATE_LOGIC:
		{			
			//更新显示指定逻辑区域,并设置跟踪框
			CQuad* pQuad = (CQuad*)pHint;
			CQuad Quad=*pQuad;
			//m_tracker.SetPhysicQuad(this,Quad);
			m_track.SetPhysicQuad(this,Quad);
			CRect rectTrue;
			//m_tracker.GetTrueRect(&rectTrue);
			m_track.GetTrueRect(rectTrue);
			InvalidateRect(rectTrue);
			break;
		}
	case UPDATE_DEVICE:
		{
			//更新显示指定设备区域
			InvalidateRect((CRect*)pHint);
			break;
		}
	}

	return;
}

void CHLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CHLDoc *pDoc=GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	switch(nChar)
	{
		case VK_DELETE:
			{
				if(m_nDrawTool==ID_DRAW_CLICK)
				{
					//删除对象
					pDoc->Delete();
					m_track.ClearAll();
					pDoc->ReLoadStrokeList();
					m_hCursorCur=m_hCursorClick;
				}
				else if(m_nDrawTool==ID_DRAW_DRAG)
				{
					short  nVKey=GetKeyState(VK_SHIFT);//short = 2 * BYTE
					if(nVKey & SHIFTED)
					{
						//将结点断开一分为二
						double dbPoint=g_SysCfg.m_dbOneToTwo;
						pDoc->SplitDot(this,dbPoint);
						Invalidate();
					}
					else
					{
						//删结点
						pDoc->DelDot(this);
						m_track.ClearDot();
						Invalidate();
					}
					break;
				}
				break;
			}
		case VK_DOWN:
			{
				double micro=g_SysCfg.m_dbMicro;
				CQuad quad(CDot(0,0),micro,micro);
				micro=quad.Height();
				pDoc->Drag(CDot(0,0),1.0,1.0,0,-micro);
				pDoc->SetTrackerQuad(pDoc->GetLimit());
				break;
			}
		case VK_UP:
			{
				double micro=g_SysCfg.m_dbMicro;
				CQuad quad(CDot(0,0),micro,micro);
				micro=quad.Height();
				pDoc->Drag(CDot(0,0),1.0,1.0,0,micro);
				pDoc->SetTrackerQuad(pDoc->GetLimit());
				break;
			}
		case VK_LEFT:
			{
				double micro=g_SysCfg.m_dbMicro;
				CQuad quad(CDot(0,0),micro,micro);
				micro=quad.Height();
				pDoc->Drag(CDot(0,0),1.0,1.0,-micro,0);
				pDoc->SetTrackerQuad(pDoc->GetLimit());
				break;
			}
		case VK_RIGHT:
			{
				double micro=g_SysCfg.m_dbMicro;
				CQuad quad(CDot(0,0),micro,micro);
				micro=quad.Height();
				pDoc->Drag(CDot(0,0),1.0,1.0,micro,0);
				pDoc->SetTrackerQuad(pDoc->GetLimit());
				break;
			}
		case VK_F5:
			{
				CHLApp* pApp=(CHLApp*)AfxGetApp();
				if (g_bIsRedPreview)
				{
					g_bIsRedPreview = FALSE;
					pApp->m_RedPreView.StopPreview();
					//		theApp.GetMainWnd()->UpdateWindow();
					//		UpdateAllViews();
				}

				if (g_bIsSelMark) 
				{
					pApp->m_SelMark.StopSelMark();
				}

				if (g_bIsFastMark) 
				{
					pApp->m_FastMark.StopFastMark();
				}

				//设置等待光标
				AfxGetApp()->BeginWaitCursor();
				//刷新外部参数
				//g_bFirstSetPara=TRUE;
				
				CMainFrame *pMainWnd=(CMainFrame *)AfxGetMainWnd();
			//	CViewProperty* viewProperty=(CViewProperty*)pMainWnd->m_wndSTCBar.GetView(1);
			//	viewProperty->FlashProperty();

				Sleep(100);
				//设置等待光标
				AfxGetApp()->EndWaitCursor();
				break;
			}
		case VK_J:
			{
				short  nVKey=GetKeyState(VK_CONTROL);//short = 2 * BYTE
				int n=nVKey;
				if(nVKey & SHIFTED)
				{
					pDoc->JoinPoint(this);
					Invalidate();
				}
				break;
			}
		//case VK_A:
		//	{
		//		short  nVKey=GetKeyState(VK_SHIFT);
		//		if(nVKey & SHIFTED)
		//		{
		//			m_track.InverDrawDot(this);
		//			double dbMoveX = MyPrecision(g_SysCfg.m_dbDotMoveStepLen);
		//			pDoc->MoveAccurate(-dbMoveX, 0);
		//			pDoc->SetTrackerQuad(pDoc->GetLimit());
		//			Invalidate();
		//		}
		//		else
		//		{
		//			OnSelAllStroke();
		//			Invalidate();
		//		}
		//		break;
		//	}
		//case VK_W: // 上移
		//	{
		//		short  nVKey=GetKeyState(VK_SHIFT);
		//		if(nVKey & SHIFTED)
		//		{
		//			m_track.InverDrawDot(this);
		//			double dbMoveY = MyPrecision(g_SysCfg.m_dbDotMoveStepLen);
		//			pDoc->MoveAccurate(0, dbMoveY);
		//			pDoc->SetTrackerQuad(pDoc->GetLimit());
		//			Invalidate();
		//		}
		//		break;
		//	}
		//case VK_S: // 下移
		//	{
		//		short  nVKey=GetKeyState(VK_SHIFT);
		//		if(nVKey & SHIFTED)
		//		{
		//			m_track.InverDrawDot(this);
		//			double dbMoveY = MyPrecision(g_SysCfg.m_dbDotMoveStepLen);
		//			pDoc->MoveAccurate(0, -dbMoveY);
		//			pDoc->SetTrackerQuad(pDoc->GetLimit());
		//			Invalidate();
		//		}
		//		else
		//		{
		//			// 设置为曲线的起点
		//			OnSetCurDotToStart();
		//		}
		//		break;
		//	}
		//case VK_D: // 右移
		//	{
		//		short  nVKey=GetKeyState(VK_SHIFT);
		//		if(nVKey & SHIFTED)
		//		{
		//			m_track.InverDrawDot(this);
		//			double dbMoveX = MyPrecision(g_SysCfg.m_dbDotMoveStepLen);
		//			pDoc->MoveAccurate(dbMoveX, 0);
		//			pDoc->SetTrackerQuad(pDoc->GetLimit());
		//			Invalidate();
		//		}
		//		else
		//		{
		//			// 删除选中节点
		//			OnDeldot();
		//		}
		//		break;
		//	}
		case VK_C: // 连接选中节点
			{
				OnJoindot();
				break;
			}
		case VK_X: // 断点
			{
				OnSplitdot();
				break;
			}
		case VK_V: // 导角
			{
				OnKillAngle();
				break;
			}
		case VK_F: // 滤点
			{
				OnFilterPoint();
				break;
			}
		case VK_R: // 反序
			{
				OnReverseStroke();
				break;
			}
		//case VK_M: // 精确移动
		//	{
		//		OnMoveaccu();
		//		break;
		//	}
		case VK_0 :
		case VK_1 :
		case VK_2 :
		case VK_3 :
		case VK_4 :
		case VK_5 :
		case VK_6 :
		case VK_7 :
		case VK_8 :
		case VK_9 :
			{
				short  nVKey=GetKeyState(VK_CONTROL);
				if(nVKey & SHIFTED)
				{
					pDoc->SetGroupNum(nChar);
				}
				else
				{
					pDoc->SelGroupByNum(nChar, this);
					Invalidate();
				}

				break;
			}
		default:
			{
				break;
			}
	}

	CZoomView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CHLView::OnZoomSelect() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc = GetDocument();
	CQuad quad   = pDoc->GetLimit();
	if(quad.IsNull())
	{
		CString str;
		str.LoadString(IDS_NOOBJECT);
		MessageBox(str,NULL,MB_ICONWARNING);
	}
	else
	{	
		double x,y,ratiox,ratioy;
		x=(quad.left+quad.right)/2;
		y=(quad.top+quad.bottom)/2;
		ratiox=(quad.right-quad.left);
		ratioy=(quad.top-quad.bottom);
		DoZoomIn(ratiox,ratioy,x,y);
	}

	return;
}

void CHLView::OnZoomAll() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc=GetDocument();
	CQuad quad=pDoc->GetAllLimit();
	if(quad.IsNull())
	{
		CString str;
		str.LoadString(IDS_NOOBJECT);
		MessageBox(str,NULL,MB_ICONWARNING);
	}
	else
	{
		double x,y,ratiox,ratioy;
		x=(quad.right+quad.left)/2;
		y=(quad.top+quad.bottom)/2;
		ratiox=quad.right-quad.left;
		ratioy=quad.top-quad.bottom;
 		DoZoomIn(ratiox,ratioy,x,y);
	}

	return;
}

void CHLView::OnDrawText() 
{
	m_nDrawTool  = ID_DRAW_TEXT;
	m_hCursorCur = m_hCursorCross;	
	if(m_bRotate)
	{
		m_bRotate=FALSE;
		Invalidate();
		CHLDoc *pDoc=GetDocument();
		m_track.m_rect=(CRect)pDoc->GetTrackerQuad();
	}

	return;
}

void CHLView::OnUpdateDrawText(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nDrawTool==ID_DRAW_TEXT)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}

	return;
}

void CHLView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	NotifyZoom();
	CZoomView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CHLView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	NotifyZoom();
	CZoomView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CHLView::OnFileImport() 
{
	CHLDoc *pDoc=GetDocument();
	if (NULL == pDoc)
	{
		return;
	}
	
	pDoc->m_pTracker=&m_track;
	CString FileExt("");
	CString ImportFileName("");
	CWinApp *pApp=AfxGetApp();
	CString str("");
		
	static char szFilter[]=
		"Plt File(*.plt)|*.plt|CAD File(*.dxf;*.dwg)|*.dxf;*.dwg|GCode File(*.NC)|*.nc|Gerber File(*.gbx;*.gbr)|*.gbx;*.gbr|jpg File(*.jpg;*.jpeg)|*.jpg;*.jpeg|bmp File(*.bmp)|*.bmp|gif File(*.gif)|*.gif|png File(*.png)|*.png|ico File(*.ico)|*.ico|tif File(*.tif)|*.tif|tga File(*.tga)|*.tga|pcx File(*.pcx)|*.pcx|All Files(*.*)|*.*||";

	CDlgImport dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,szFilter);
	
	ImportFileName=pApp->GetProfileString("FILEIMPORT","PATH","C:\\YW");
	str.LoadString(IDS_TITLE_FILEIMPORT);
	dlg.m_ofn.lpstrTitle=str;
	dlg.m_ofn.lpstrInitialDir=ImportFileName;
	dlg.m_bGerberEnable = CNodeGerber::CheckGerberDog();

	BOOL result=FALSE;
	if(dlg.DoModal()==IDOK)
	{
		result=TRUE;
	}

	if(result)
	{
		g_ParaIni.m_dbDxfPrecision = g_ParaIni.ReadDouble("DXF", "Precison", 0.1);
		g_ParaIni.m_dbGerberOffset = g_ParaIni.ReadDouble("Gerber", "Offset", 0.1);

		ImportFileName=dlg.GetPathName();
		pApp->WriteProfileString("FILEIMPORT","PATH",ImportFileName.Left(ImportFileName.ReverseFind('\\')));
		FileExt=dlg.GetFileExt();
		FileExt.MakeUpper();		
		//设置等待光标
		AfxGetApp()->BeginWaitCursor();		
		CHLDoc *pDoc=GetDocument();
		if (NULL == pDoc)
		{
			return;
		}

		CLay lay  = pDoc->GetLay();
		CLayer ly = lay.GetCurLayer();
		CProperty pty=ly.GetProperty();
		
		if (0 == FileExt.CompareNoCase("GBX") || 0 == FileExt.CompareNoCase("GBR"))
		{
			CNodeGerber *pNode =new CNodeGerber;
			if (NULL == pNode)
				return;

			pNode->SetProperty(PTY_INIT,pty);			
			if(pNode->DeCode(dlg.GetPathName(), dlg.m_bCenter, g_ParaIni.m_dbDxfPrecision))
			{
				pDoc->SetSelect(FALSE);
				m_track.ClearStrokeList();

				CTree *pTreeAdd=NULL;	
				while(TRUE)
				{
					CNodeGerber *pNodeTemp = NULL;
					pNodeTemp =	pNode->UnGroupplt();//按图层分解导入的线条，每组相同图层的线条作为一个PLT对象。
					if (pNodeTemp==NULL)
					{
						if (pTreeAdd)
						{
							pDoc->AddTree(pTreeAdd);
							pDoc->InserObjToList(NODE_DXF);
						}
						break;
					}
					else
					{
						if (pTreeAdd==NULL)
						{
							pTreeAdd = new CTree(pNodeTemp);
						}
						else
						{
							CTree treeTemp(pNodeTemp);
							pTreeAdd->Group(&treeTemp);//将分解出来的不同图层的PLT对象组合成一个组合对象
						}
					}	
				}
			}
			delete pNode;
			////////////////////////////////////////////////////////////////////////
		} 
		else if(0 == FileExt.CompareNoCase("PLT"))
		{	
			CNodePlt *pNode =new CNodePlt;
			if (NULL == pNode)
			{
				return;
			}

			pNode->SetProperty(PTY_INIT,pty);			
			if(pNode->DeCode(dlg.GetPathName(),dlg.m_bCenter, dlg.m_bPltBetter))
			{
				pDoc->SetSelect(FALSE);
				m_track.ClearStrokeList();
				CTree *pTreeAdd=new CTree;
				if (NULL == pTreeAdd)
				{
					if (pNode != NULL)
					{
						delete pNode;
						pNode = NULL;
					}
					return;
				}

				BOOL b     = TRUE;
				BOOL bFirst=TRUE;
				while(b)
				{
					CNodePlt *pNodeTemp = NULL;
					pNodeTemp=	pNode->UnGroupplt();//按图层分解导入的线条，每组相同图层的线条作为一个PLT对象。
					if(pNodeTemp==NULL)
					{
						if(bFirst)//如果导入的图形对象中没有一个线条，该导入操作无效，不进行任何对象的添加
						{
							delete pTreeAdd;
						}
						else
						{
						   pDoc->AddTree(pTreeAdd);
		                   pDoc->InserObjToList(NODE_PLT);
						}
						b=FALSE;
					}
					else
					{
						CTree treeTemp(pNodeTemp);
						pTreeAdd->Group(&treeTemp);//将分解出来的不同图层的PLT对象组合成一个组合对象
						bFirst=FALSE;
					}
				}
				delete pNode;
			}
			else
			{
				delete pNode;
			}
		}
		else if (0 == FileExt.CompareNoCase("dxf") ||  0 == FileExt.CompareNoCase("dwg"))
		{
			CDlgImportAutoCadFile dxfDlg;
			dxfDlg.m_nCadUnit = g_ParaIni.m_nDxfUnit;
			if (dxfDlg.DoModal()==IDOK)
			{
				g_ParaIni.m_nDxfUnit = dxfDlg.m_nCadUnit;
				g_ParaIni.SavePara();
			}
			else
			{
				return;
			}
			
			CString strShxDir;
			if (!dxfDlg.m_bFilterText)
				strShxDir = ::g_sys.GetInstallPath()+_T("EFonts\\");	


			CNodeDxf *pNode =new CNodeDxf;
			if (NULL == pNode)
				return;

			pNode->SetProperty(PTY_INIT,pty);			
			if(pNode->DeCode(dlg.GetPathName(), strShxDir, dxfDlg.m_dbUnitTrans, g_ParaIni.m_dbDxfPrecision, dlg.m_bCenter, dlg.m_bPltBetter))
			{
				pDoc->SetSelect(FALSE);
				m_track.ClearStrokeList();

				CTree *pTreeAdd=NULL;
				while(TRUE)
				{
					CNodeDxf *pNodeTemp = NULL;
					pNodeTemp =	pNode->UnGroupplt();//按图层分解导入的线条，每组相同图层的线条作为一个PLT对象。
					if (pNodeTemp==NULL)
					{
						if (pTreeAdd)
						{
							pDoc->AddTree(pTreeAdd);
							pDoc->InserObjToList(NODE_DXF);
						}
						break;
					}
					else
					{
						if (pTreeAdd==NULL)
						{
							pTreeAdd = new CTree(pNodeTemp);
						}
						else
						{
							CTree treeTemp(pNodeTemp);
							pTreeAdd->Group(&treeTemp);//将分解出来的不同图层的PLT对象组合成一个组合对象
						}
					}	
				}
			}
			delete pNode;
			////////////////////////////////////////////////////////////////////////
			
		}
		else if(0 == FileExt.CompareNoCase("NC"))
		{	
			CTree *pTreeAdd=new CTree;
			if (pTreeAdd==NULL)	
				return;

			CNodeNC *pNode =new CNodeNC;
			if (NULL == pNode)	
				return;

			pNode->SetProperty(PTY_INIT,pty);			
			if(pNode->DeCode(dlg.GetPathName(),dlg.m_bCenter, dlg.m_bPltBetter))
			{
				BOOL bFirst=TRUE;
				CNodeLine *pNodeTemp = NULL;
				while(1)
				{
					pNodeTemp =	pNode->UnGroupplt();
					if(pNodeTemp == NULL)
					{
						if(bFirst)//如果导入的图形对象中没有一个线条，该导入操作无效，不进行任何对象的添加
						{
							delete pTreeAdd;
						}
						else
						{
							pDoc->AddTree(pTreeAdd);
							pDoc->InserObjToList(NODE_NC);
						}
						break;
					}
					else
					{
						CTree treeTemp(pNodeTemp);
						pTreeAdd->Group(&treeTemp);//将分解出来的不同图层的PLT对象组合成一个组合对象
						bFirst=FALSE;

						pNodeTemp = NULL;
					}
				}

				delete pNode;
			}
			else
			{
				delete pNode;
			}
		}
		else
		{
			CHLDoc *pDoc=GetDocument();
			CString strFileName = dlg.GetPathName();
			CNode *pNode = new CNodeDib(strFileName);
			pNode->SetProperty(PTY_INIT,pty, &m_DC);		
			if(pNode->Track(NULL,CPoint(0,0)))
			{
				pDoc->AddNode(pNode, NODE_BMP);
			}
			else
			{
				delete pNode;
			}
		}
		//结束等待光标
		AfxGetApp()->EndWaitCursor();
		pDoc->SetTrackerQuad(pDoc->GetLimit());
		Invalidate();
	}	

	pDoc->SetObjProp();

	return;
}

void CHLView::OnDrawGrid() 
{
	// TODO: Add your command handler code here
	if(m_nDrawTool==ID_DRAW_GRID)
	{
		CDrawGridDlg dlg;
		dlg.m_nCol = g_SysCfg.m_nGridCol;
		dlg.m_nRow = g_SysCfg.m_nGridRow;
		if(dlg.DoModal()==IDOK)
		{
			 g_SysCfg.m_nGridCol = dlg.m_nCol;
		     g_SysCfg.m_nGridRow = dlg.m_nRow;

			 g_sys.WriteSysCfg();
		}
	}
	
	m_nDrawTool=ID_DRAW_GRID;
	m_hCursorCur=m_hCursorCross;

	if(m_bRotate)
	{
		m_bRotate=FALSE;
		Invalidate();
		CHLDoc *pDoc=GetDocument();
		m_track.m_rect=(CRect)pDoc->GetTrackerQuad();
	}

}

void CHLView::OnUpdateDrawGrid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nDrawTool==ID_DRAW_GRID)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
	
}

void CHLView::OnDrawPolygon() 
{
	// TODO: Add your command handler code here
	if(m_nDrawTool==ID_DRAW_POLYGON)
	{
		CDrawPolygonDlg dlg;

		dlg.m_nPolygon = g_SysCfg.m_nPolygon;
		dlg.m_nNum     = g_SysCfg.m_nPolyNum;
		dlg.m_nCross   = g_SysCfg.m_nPolyCross;
		dlg.m_nStar    = g_SysCfg.m_nPolyStar;
		if(dlg.DoModal()==IDOK)
		{
			g_SysCfg.m_nPolygon   = dlg.m_nPolygon;
		    g_SysCfg.m_nPolyNum   = dlg.m_nNum;
		    g_SysCfg.m_nPolyCross = dlg.m_nCross;
		    g_SysCfg.m_nPolyStar  = dlg.m_nStar;

			g_sys.WriteSysCfg();
		}
	}

	m_nDrawTool=ID_DRAW_POLYGON;
	m_hCursorCur=m_hCursorCross;

	if(m_bRotate)
	{
		m_bRotate=FALSE;
		Invalidate();
		CHLDoc *pDoc=GetDocument();
		m_track.m_rect=(CRect)pDoc->GetTrackerQuad();
	}
}

void CHLView::OnUpdateDrawPolygon(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nDrawTool==ID_DRAW_POLYGON)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CHLView::OnDrawEllipse() 
{
	if(m_nDrawTool==ID_DRAW_ELLIPSE)
	{
		EllipSetDlg dlg;
		dlg.m_dbRuler = g_SysCfg.m_dbDrawEllipSet;
		if(dlg.DoModal()==IDOK)
		{

		}
	}

	m_nDrawTool=ID_DRAW_ELLIPSE;
	m_hCursorCur=m_hCursorCross;
	if(m_bRotate)
	{
		m_bRotate=FALSE;
		Invalidate();
		CHLDoc *pDoc=GetDocument();
		m_track.m_rect=(CRect)pDoc->GetTrackerQuad();
	}

	return;
}

void CHLView::OnUpdateDrawEllipse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nDrawTool==ID_DRAW_ELLIPSE)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}

	return;
}

void CHLView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CZoomView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		if (m_pActiveView!=pActivateView)
		{
			m_pActiveView = pActivateView;
			if (g_pDlgMarkNormalPane)
			{
				g_pDlgMarkNormalPane->m_pDoc = GetDocument();
				g_pDlgMarkNormalPane->ResetProductDocPara(0xFF);
			}

			CMainFrame *pMainWnd = (CMainFrame*)AfxGetMainWnd();
			if (pMainWnd)
			{
				pMainWnd->m_XBarMarkPara.m_PropDlg.OnDocChange();
				pMainWnd->m_XBarMarkPara.m_PropDlg.UpdateProperty();
				pMainWnd->m_XBarObjectlist.m_ObjectListDlg.ObjListLoad();
			}
		}	

	}
	else
	{
		
		CMainFrame *pMainWnd = (CMainFrame*)AfxGetMainWnd();
		if (pMainWnd)
		{	
			pMainWnd->m_XBarProperty.m_FillEditDlg.m_TextEditDlg.OnNewDocment();
			pMainWnd->m_XBarObjectlist.m_ObjectListDlg.ObjListLoad();
		}
	}

}

void CHLView::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_pActiveView==this)
	{
		m_pActiveView = NULL;
	}

	CZoomView::PostNcDestroy();
}
void CHLView::OnDrawCodebar() 
{
	m_nDrawTool=ID_DRAW_CODEBAR;
	m_hCursorCur=m_hCursorCross;
	if(m_bRotate)
	{
		m_bRotate=FALSE;
		Invalidate();
		CHLDoc *pDoc=GetDocument();
		m_track.m_rect=(CRect)pDoc->GetTrackerQuad();
	}

	return;
}

void CHLView::OnUpdateDrawCodebar(CCmdUI* pCmdUI) 
{	
	// TODO: Add your command update UI handler code here
	if(m_nDrawTool==ID_DRAW_CODEBAR)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}

	return;
}

void CHLView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(GetZoomMode()==MODE_ZOOMOPER)
	{
		OnDrawClick();
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = ID_DRAW_CLICK;
		m_hCursorCur = m_hCursorClick;
		Invalidate();
		return;
	}

	if(GetZoomMode()==MODE_ZOOMSEL)
	{
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = ID_DRAW_CLICK;
		m_hCursorCur = m_hCursorClick;
		Invalidate();
		return;
	}
	
	if (!(g_CurUserInfo.dwRight1 & 0x00000001))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	if(m_nDrawTool==ID_DRAW_CLICK)
	{
		CHLDoc *pDoc=(CHLDoc*)GetDocument();
		int index=pDoc->GetMenuIndex();
		CMenu menu,*pMenu;
		menu.LoadMenu(IDR_POP);
		pMenu=menu.GetSubMenu(index);

		if(pMenu)
		{
			HCURSOR hcursor;
			hcursor=m_hCursorCur;
			m_hCursorCur=::LoadCursor(NULL,IDC_ARROW);
			
			ClientToScreen(&point);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
			m_hCursorCur=hcursor;
			SetFocus();
			return;
		}
	}

	if(m_nDrawTool==ID_DRAW_DRAG)
	{
		if(!m_track.ListEmpty())
		{
			CHLDoc *pDoc=(CHLDoc*)GetDocument();
			CMenu menu,*pMenu;
			menu.LoadMenu(IDR_POP);
			pMenu=menu.GetSubMenu(7);
			if(pMenu)
			{
				HCURSOR hcursor;
				hcursor=m_hCursorCur;
				m_hCursorCur=::LoadCursor(NULL,IDC_ARROW);
				
				ClientToScreen(&point);
				pMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
				m_hCursorCur=hcursor;
				SetFocus();
				return;
			}
		}
	}

	CZoomView::OnRButtonDown(nFlags, point);

	return;
}

void CHLView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(GetZoomMode()==MODE_ZOOMOPER)
	{
		OnDrawClick();
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = ID_DRAW_CLICK;
		m_hCursorCur = m_hCursorClick;
		Invalidate();
		return;
	}

	if(GetZoomMode()==MODE_ZOOMSEL)
	{
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = ID_DRAW_CLICK;
		m_hCursorCur = m_hCursorClick;
		Invalidate();
		return;
	}

	switch(m_nDrawTool)
	{
	case ID_DRAW_CLICK:
		{
			CZoomView::OnLButtonDblClk(nFlags, point);
			if (m_bCanSetLinePos)
			{
				m_bCanSetLinePos = FALSE;
				break;
			}
			OnDrawDrag();
			break;
		}
	case ID_DRAW_DRAG:
		{
			CHLDoc *pDoc=(CHLDoc*)GetDocument();
			if(m_track.HitValidPoint(point,this))
			{
				CHLDoc *pDoc=GetDocument();
				pDoc->AddPoint(point,this);
				Invalidate();
			}
			else
			{
				OnDrawClick();
			}
			break;
		}
	case ID_SORT_OBJECT_LIST:
		{
			m_nDrawTool = ID_DRAW_CLICK;
			break;
		}
	default:
		{
			break;
		}
	}

	return;
}


void CHLView::TrackRotate(CPoint OrgPoint, BOOL bShift, BOOL bControl)
{
	CWnd *pWin = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CDC *pDC   = GetDC();
    CDot pointLT,pointRB;

    double dbRotateAngle(0);
	double dbOrgAngle(0.0),dbNewAngle(0.0);
	int cx(0),cy(0);
	CPoint CurMousePoint;
	STROKELIST *pPreviewStrokeList = NULL;

	pPreviewStrokeList = new STROKELIST;
	if (NULL == pPreviewStrokeList) 
	{
		return;
	}

	CHLDoc *pDoc    = GetDocument();
	CQuad TrackRect = pDoc->GetQuadTracker();

	TrackRect.Normal();
	PhysicToLogic(TrackRect.left, TrackRect.top);
	PhysicToLogic(TrackRect.right, TrackRect.bottom);
	CDot CenterPoint=TrackRect.GetBaseDot(BASE_MM);
	PhysicToLogic(CenterPoint.x, CenterPoint.y);
	CPoint pointCC((int)CenterPoint.x,(int)CenterPoint.y);
	
    cx=OrgPoint.x-pointCC.x;
	cy=OrgPoint.y-pointCC.y;

	dbOrgAngle=CalAngle(cx,cy);
	dbNewAngle=dbOrgAngle;

	CStroke stroke,strokeTemp;
	stroke.Add(CDot(TrackRect.left,TrackRect.top));
	stroke.Add(CDot(TrackRect.right,TrackRect.top));
	stroke.Add(CDot(TrackRect.right,TrackRect.bottom));
	stroke.Add(CDot(TrackRect.left,TrackRect.bottom));
	stroke.Add(CDot(TrackRect.left,TrackRect.top));
	
	SetCapture();
	
	BOOL bExit=FALSE;
	while (!bExit)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		switch (msg.message)
		{
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			bExit=TRUE;
			m_RotateRectStroke.RemoveAllPoint();
			break;
		case WM_LBUTTONDBLCLK:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			break;
		case WM_RBUTTONDBLCLK:
			break;
		case WM_MOUSEWHEEL:
            break;
		case WM_MOUSEMOVE:
			{
				CQuad FlashRect;
				FlashRect=pDoc->GetQuadTracker();
				PhysicToLogic(FlashRect.left, FlashRect.top);
				PhysicToLogic(FlashRect.right, FlashRect.bottom);

		        PreviewRotateTracker(FlashRect);
				PreviewStroke(pPreviewStrokeList);
			
				EmptyStroke(pPreviewStrokeList);
				if(bShift)
					pDoc->GetSelectedStrokeList(pPreviewStrokeList);
			
				CurMousePoint.x = (int)(short)LOWORD(msg.lParam);
				CurMousePoint.y = (int)(short)HIWORD(msg.lParam);
				cx=CurMousePoint.x-pointCC.x;
				cy=CurMousePoint.y-pointCC.y;

				dbNewAngle=CalAngle(cx,cy);
				strokeTemp=stroke;
				strokeTemp.Rotate(CenterPoint,(dbNewAngle-dbOrgAngle));
				RotatePreviewStrokeList(pPreviewStrokeList,CenterPoint,dbNewAngle-dbOrgAngle);
				m_RotateRectStroke.CopyStroke(&strokeTemp);
				
				CQuad dbRect=strokeTemp.CalLimit();
			    pDoc->SetTrackerQuad(dbRect); 
				PreviewRotateTracker(dbRect);
				PreviewStroke(pPreviewStrokeList);

                
                CString strInfo,strFormat;
				double dbAngleTemp;
				dbAngleTemp=dbNewAngle-dbOrgAngle;
				if(dbAngleTemp<-180)
					dbAngleTemp+=360;
				if(dbAngleTemp>=180)
					dbAngleTemp-=360;
							
				break;
			}
		default:
			break;
		}
	}

	ReleaseDC(pDC);
	ReleaseCapture();

	TrackRect.Union(pDoc->GetQuadTracker());
	CRect rectflash((int)TrackRect.left,(int)TrackRect.top,(int)TrackRect.right,(int)TrackRect.bottom);
	rectflash.NormalizeRect();
	rectflash.InflateRect(14,14);

    InvalidateRect(rectflash);
	
	dbRotateAngle=dbNewAngle-dbOrgAngle;
	
	if(dbRotateAngle<-180)
	{
		dbRotateAngle+=360;
	}
	
	if(dbRotateAngle>=180)
	{
		dbRotateAngle-=360;
	}
	
	pDoc->Rotate(CenterPoint,dbRotateAngle);

	this->EmptyStroke(pPreviewStrokeList);

	if (pPreviewStrokeList)
	{
		delete pPreviewStrokeList;
		pPreviewStrokeList = NULL;
	}

	pWin->SendMessage(WM_SETMESSAGESTRING,0,(LPARAM)(LPCSTR)"");
	return;
}

double CHLView::CalAngle(double cx, double cy)
{
	double r;
	double angle;
    
	if(cx==0&&cy==0)
		return 0;

	if(cy==0)
	{
		if(cx>0)
			return 0;
		else
			return 180;
	}
	else if(cx==0)
	{
		if(cy>0)
			return 90;
		else
			return 270;
	}

	r=sqrt(cx*cx+cy*cy);
	angle=180.0*asin(cy/r)/PAI;
    
	
	if(cx<0&&cy>0)
	{//第二象限
      ASSERT(angle>0);
	  angle=180-angle;
	}
    else if(cx<0&&cy<0)
	{//第三象限
      ASSERT(angle<0);
      angle=fabs(angle)+180;
	}
	else if(cx>0&&cy<0)
	{//第四象限
      ASSERT(angle<0);
	  angle=360+angle;
	}
	
	return angle;
}

void CHLView::PreviewRotateTracker(CQuad rect)
{
    CDC *pDC=GetDC();
	int mode=pDC->SetROP2(R2_NOTXORPEN);
	CQuad rectTracker=rect;
	CRect r((int)(rectTracker.left),(int)(rectTracker.top),(int)(rectTracker.right),(int)(rectTracker.bottom));
	r.NormalizeRect();
	CBrush brush;
	CPen pen,*poldpen;

	m_track.m_rect.left=m_track.m_rect.right=m_track.m_rect.top=m_track.m_rect.bottom=0;
	r.InflateRect(8,8);
	
	CSize size(8,4);

	pen.CreatePen(PS_DOT,1,RGB(0,0,255));
	poldpen=pDC->SelectObject(&pen);

	pDC->MoveTo(r.left+size.cx,r.top+size.cx);
	pDC->LineTo(r.right-size.cx,r.top+size.cx);
	pDC->LineTo(r.right-size.cx,r.bottom-size.cx);
	pDC->LineTo(r.left+size.cx,r.bottom-size.cx);
	pDC->LineTo(r.left+size.cx,r.top+size.cx);
	pDC->SelectObject(poldpen);

	pen.DeleteObject();

	//画拖动旋转时的当前转动到的位置预缆框
    if(m_RotateRectStroke.GetSize()!=0)
	{
		CPoint point;
		CDot pointTemp;
		pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
		poldpen=pDC->SelectObject(&pen);
		for(int k=0; k<4; k++)
		{
			pointTemp=m_RotateRectStroke.GetAt(k);
			point.x=(int)pointTemp.x;point.y=(int)pointTemp.y;
			if(k==0)
			{
		        pDC->MoveTo(point);
			}
			else
			{
				pDC->LineTo(point);
			}
		}
		
		pointTemp=m_RotateRectStroke.GetAt(0);
		point.x=(int)pointTemp.x;point.y=(int)pointTemp.y;
		pDC->LineTo(point);
		pDC->SelectObject(poldpen);
		pen.DeleteObject();
	}

 /*	CSize size1(14,14);
	int x,y;
  	for(int i=0;i<8;i++)
	{
		switch(i)
		{
		case 0:
			x=r.left;y=r.top;
			break;
		case 1:
            x=r.left+(int)((r.Width()-size1.cx)/2);y=r.top-(int)(size1.cy/2);
			break;
		case 2:
			x=r.right-size1.cx;y=r.top;
			break;
		case 3:
			x=r.right-(int)(size1.cx/2);y=r.top+(int)((r.Height()-size1.cy)/2);
			break;
		case 4:
			x=r.right-size1.cx;y=r.bottom-size1.cx;
			break;
		case 5:
			x=r.left+(int)((r.Width()-size1.cy)/2);y=r.bottom-(int)(size1.cx/2);
			break;
		case 6:
			x=r.left;y=r.bottom-size1.cx;
			break;
		case 7:
			x=r.left-(int)(size1.cx/2);y=r.top+(int)((r.Height()-size1.cy)/2);
			break;
		}
		DrawIcon(pDC,x,y,m_DragRotateIcon[i]);

	}
*/
	pDC->SetROP2(mode);
	ReleaseDC(pDC);

	return;
}

void CHLView::InitIcon()
{
    //左上
	m_DragRotateIcon[0][0]=0x7f;  
	m_DragRotateIcon[0][1]=0x3f;
	m_DragRotateIcon[0][2]=0x7e;
	m_DragRotateIcon[0][3]=0x1fc;
	m_DragRotateIcon[0][4]=0x398;
	m_DragRotateIcon[0][5]=0x610;
	m_DragRotateIcon[0][6]=0x600;
	m_DragRotateIcon[0][7]=0x2c00;
	m_DragRotateIcon[0][8]=0x3c00;
	m_DragRotateIcon[0][9]=0x3f00;
	m_DragRotateIcon[0][10]=0x3e00;
	m_DragRotateIcon[0][11]=0x3c00;
	m_DragRotateIcon[0][12]=0x3800;
	m_DragRotateIcon[0][13]=0x3000;
	
	//上
	m_DragRotateIcon[1][0]=0x00;
	m_DragRotateIcon[1][1]=0x00;
	m_DragRotateIcon[1][2]=0x00;
	m_DragRotateIcon[1][3]=0x00;
	m_DragRotateIcon[1][4]=0x618;
	m_DragRotateIcon[1][5]=0x1e1e;
	m_DragRotateIcon[1][6]=0x3fff;
	m_DragRotateIcon[1][7]=0x1e1e;
	m_DragRotateIcon[1][8]=0x618;
	m_DragRotateIcon[1][9]=0x00;
	m_DragRotateIcon[1][10]=0x00;
	m_DragRotateIcon[1][11]=0x00;
	m_DragRotateIcon[1][12]=0x00;
	m_DragRotateIcon[1][13]=0x00;
	
	//右上
	m_DragRotateIcon[2][0]=0x3f80;
	m_DragRotateIcon[2][1]=0x3f00;
	m_DragRotateIcon[2][2]=0x1f80;
	m_DragRotateIcon[2][3]=0xfe0;
	m_DragRotateIcon[2][4]=0x670;
	m_DragRotateIcon[2][5]=0x218;
	m_DragRotateIcon[2][6]=0x18;
	m_DragRotateIcon[2][7]=0x0d;
	m_DragRotateIcon[2][8]=0x0f;
	m_DragRotateIcon[2][9]=0x03f;
	m_DragRotateIcon[2][10]=0x1f;
	m_DragRotateIcon[2][11]=0x0f;
	m_DragRotateIcon[2][12]=0x07;
	m_DragRotateIcon[2][13]=0x03;
	
	//右
	m_DragRotateIcon[3][0]=0x80;
	m_DragRotateIcon[3][1]=0x1c0;
	m_DragRotateIcon[3][2]=0x1c0;
	m_DragRotateIcon[3][3]=0x3e0;
	m_DragRotateIcon[3][4]=0x3e0;
	m_DragRotateIcon[3][5]=0x80;
	m_DragRotateIcon[3][6]=0x80;
	m_DragRotateIcon[3][7]=0x80;
	m_DragRotateIcon[3][8]=0x80;
	m_DragRotateIcon[3][9]=0x3e0;
	m_DragRotateIcon[3][10]=0x3e0;
	m_DragRotateIcon[3][11]=0x1c0;
	m_DragRotateIcon[3][12]=0x1c0;
	m_DragRotateIcon[3][13]=0x80;
	
	//右下
	m_DragRotateIcon[4][0]=0x03;
	m_DragRotateIcon[4][1]=0x07;
	m_DragRotateIcon[4][2]=0x0f;
	m_DragRotateIcon[4][3]=0x1f;
	m_DragRotateIcon[4][4]=0x3f;
	m_DragRotateIcon[4][5]=0x0f;
	m_DragRotateIcon[4][6]=0x0d;
	m_DragRotateIcon[4][7]=0x18;
	m_DragRotateIcon[4][8]=0x218;
	m_DragRotateIcon[4][9]=0x670;
	m_DragRotateIcon[4][10]=0xfe0;
	m_DragRotateIcon[4][11]=0x1f80;
	m_DragRotateIcon[4][12]=0x3f00;
	m_DragRotateIcon[4][13]=0x3f80;
	
	//下
	m_DragRotateIcon[5][0]=0x00;
	m_DragRotateIcon[5][1]=0x00;
	m_DragRotateIcon[5][2]=0x00;
	m_DragRotateIcon[5][3]=0x00;
	m_DragRotateIcon[5][4]=0x618;
	m_DragRotateIcon[5][5]=0x1e1e;
	m_DragRotateIcon[5][6]=0x3fff;
	m_DragRotateIcon[5][7]=0x1e1e;
	m_DragRotateIcon[5][8]=0x618;
	m_DragRotateIcon[5][9]=0x00;
	m_DragRotateIcon[5][10]=0x00;
	m_DragRotateIcon[5][11]=0x00;
	m_DragRotateIcon[5][12]=0x00;
	m_DragRotateIcon[5][13]=0x00;
	
	//左下
	m_DragRotateIcon[6][0]=0x3000;
	m_DragRotateIcon[6][1]=0x3800;
	m_DragRotateIcon[6][2]=0x3c00;
	m_DragRotateIcon[6][3]=0x3e00;
	m_DragRotateIcon[6][4]=0x3f00;
	m_DragRotateIcon[6][5]=0x3c00;
	m_DragRotateIcon[6][6]=0x2c00;
	m_DragRotateIcon[6][7]=0x600;
	m_DragRotateIcon[6][8]=0x610;
	m_DragRotateIcon[6][9]=0x398;
	m_DragRotateIcon[6][10]=0x1fc;
	m_DragRotateIcon[6][11]=0x7e;
	m_DragRotateIcon[6][12]=0x3f;
	m_DragRotateIcon[6][13]=0x7f;
	
	//左
	m_DragRotateIcon[7][0]=0x80;
	m_DragRotateIcon[7][1]=0x1c0;
	m_DragRotateIcon[7][2]=0x1c0;
	m_DragRotateIcon[7][3]=0x3e0;
	m_DragRotateIcon[7][4]=0x3e0;
	m_DragRotateIcon[7][5]=0x80;
	m_DragRotateIcon[7][6]=0x80;
	m_DragRotateIcon[7][7]=0x80;
	m_DragRotateIcon[7][8]=0x80;
	m_DragRotateIcon[7][9]=0x3e0;
	m_DragRotateIcon[7][10]=0x3e0;
	m_DragRotateIcon[7][11]=0x1c0;
	m_DragRotateIcon[7][12]=0x1c0;
	m_DragRotateIcon[7][13]=0x80;
}

void CHLView::PreviewStroke(STROKELIST *pStrokeList)
{
    POSITION pos     = NULL;
	CStroke *pStroke = NULL;
	CPoint   point;
	CDot     pointTemp;
	CDC     *pDC = GetDC();

	int mode = pDC->SetROP2(R2_NOTXORPEN);
	pos = pStrokeList->GetHeadPosition();
	while(pos!=NULL)
	{
		pStroke=pStrokeList->GetNext(pos);
		for(int k=0;k<pStroke->GetSize();k++)
		{
			pointTemp = pStroke->GetAt(k);
			point.x = (int)pointTemp.x;
			point.y = (int)pointTemp.y;
			if(0 == k)
			{
				pDC->MoveTo(point);
			}
			else
			{
				pDC->LineTo(point);
			}
		}
	}
	
	pDC->SetROP2(mode);
	ReleaseDC(pDC);

	return;
}

void CHLView::EmptyStroke(STROKELIST *pStrokeList)
{
	POSITION pos,prev;
	CStroke *pStroke;
	pos=pStrokeList->GetHeadPosition();

	while(pos!=NULL)
	{
		prev=pos;
		pStroke=pStrokeList->GetNext(pos);
		pStrokeList->RemoveAt(prev);
	    delete pStroke;
	}

	return;
}

void CHLView::RotatePreviewStrokeList(STROKELIST *pStrokeList, CDot CenterPoint, double dbAngle)
{
    POSITION pos ;
	CStroke *pStroke = NULL;
	pos=pStrokeList->GetHeadPosition();
	while(pos!=NULL)
	{
		pStroke=pStrokeList->GetNext(pos);
		pStroke->Rotate(CenterPoint,dbAngle);
	}

	return;
}

void CHLView::DrawIcon(CDC *pDC, int x, int y, unsigned short *pIconMap)
{
    unsigned short *pTemp = NULL;
	pTemp = pIconMap;

	for(int i=0;i<14;i++)
	{
		for(int k=0;k<14;k++)
		{
			if(IsBitSigned(*pTemp,13-k))
			{
			    pDC->SetPixel(x+k,y+i,RGB(0,0,0));
			}
		}
		pTemp++;
	}

	return;
}

BOOL CHLView::IsBitSigned(unsigned short n, unsigned int nBit)
{
    n = n>>nBit;
	n = n&0x01;

	return n!=0?TRUE:FALSE;
}

void CHLView::DrawRotateTrack(XDC *pDC)
{
}

void CHLView::OnHelppath() 
{
	CDlgHelpPath dlg;
	dlg.DoModal();
}

void CHLView::OnZoomoff() 
{
	if(GetZoomMode()==MODE_ZOOMSEL)
	{
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = ID_DRAW_CLICK;
		SetCursor(m_hCursorClick);
		m_hCursorCur = m_hCursorClick;
	}
	else
	{
		SetZoomMode(MODE_ZOOMSEL);
		SetCursor(m_hCursorZoomIn);
		m_hCursorCur = m_hCursorZoomIn;
		m_nDrawTool = 0;
	}

	return;
}

void CHLView::OnZoomoper()
{
	if(MODE_ZOOMOPER == GetZoomMode())
	{
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = ID_DRAW_CLICK;
		SetCursor(m_hCursorClick);
		m_hCursorCur = m_hCursorClick;
	}
	else
	{
		SetZoomMode(MODE_ZOOMOPER);	
		m_nDrawTool = 0;
		CZoomView::m_bLBDowm = FALSE;
		SetCursor(m_hCursorDoxMove);
		m_hCursorCur = m_hCursorDoxMove;
	}

	return;
}

void CHLView::OnUpdateZoomoff(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetZoomMode() == MODE_ZOOMSEL);

	if (m_nDrawTool)
	{
		SetZoomMode(MODE_ZOOMOFF);
	}

	return;
}

void CHLView::OnUpdateZoomoper(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetZoomMode()==MODE_ZOOMOPER);
}

void CHLView::ReplaceDot()
{
	m_track.ReplaceDot();
}

void CHLView::DelDot()
{
	m_track.DelDot();
}

BOOL CHLView::HitOtherObject(CPoint &point)//选中其它对象
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	return pDoc->HitOtherObject(point,this);
}

// 画线条上的标示点
void CHLView::DrawRect()
{
	m_track.AddRect(this);
	m_track.FillRect(this);
}

void CHLView::AddPoint(CPoint &point)
{
	m_track.AddPoint(point,this);
}

void CHLView::SplitPoint(double dbPoint)
{
	m_track.SplitPoint(dbPoint);
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	m_track.ClearAll();
	pDoc->ReLoadStrokeList();
}

void CHLView::JoinPoint()
{
	m_track.JoinPoint();
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	m_track.ClearAll();
	pDoc->ReLoadStrokeList();
}

void CHLView::OnMoveaccu() 
{
	MOVEACCUDLG dlg;
	if(dlg.DoModal()==IDOK)
	{
	}
//	Invalidate();
}

void CHLView::OnChangeconvert() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->Changeconvert();

	return;
}

void CHLView::OnConvert() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->Changeconvert();
	return;
}

void CHLView::OnDeldot() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->DelDot(this);
	m_track.ClearDot();
	Invalidate();
}

void CHLView::OnJoindot() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->JoinPoint(this);
	Invalidate();
}

void CHLView::OnSplitdot() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	double dbPoint = g_SysCfg.m_dbOneToTwo;
	pDoc->SplitDot(this, dbPoint);
	Invalidate();

	return;
}

void CHLView::OnSelAllStroke() 
{
	m_track.SelectStrokeBySelPt(this);
}

void CHLView::OnSetCurDotToStart() 
{
	m_track.SetDotToStart(this);
}

void CHLView::OnFilterPoint() 
{
	m_track.FilterPoint(this, g_SysCfg.m_dbDotFilterPrecis, g_SysCfg.m_dbDotFilterAngle);
	m_track.ClearDot();
	Invalidate();
}

void CHLView::OnReverseStroke() 
{
	m_track.ReverseStroke(this);
}

void CHLView::OnFilterCurDocPoint() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->FilterPointCmd();
	Invalidate();	
}

void CHLView::OnKillAngle() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->OnKillAngle();
	Invalidate();	
}

void CHLView::OnSortObj() 
{
	if (ID_SORT_OBJECT_LIST != m_nDrawTool)
	{
		g_bIsSort = TRUE;
		m_nDrawTool = ID_SORT_OBJECT_LIST;
		SetZoomMode(MODE_ZOOMOFF);
		m_hCursorCur=m_hCursorClick;
		Invalidate();
		CHLDoc *pDoc = (CHLDoc*)GetDocument();
		pDoc->IniSort(this);
		SortPro();
		m_nDrawTool = ID_DRAW_CLICK;
		g_bIsSort = FALSE;
		Invalidate();
	}
	else
	{
		g_bIsSort = FALSE;
		m_nDrawTool = ID_DRAW_CLICK;
		Invalidate();
	}

	return;
}

void CHLView::OnUpdateSortObj(CCmdUI* pCmdUI) 
{
	if(ID_SORT_OBJECT_LIST == m_nDrawTool)
	{
		g_bIsSort = TRUE;
		pCmdUI->SetCheck(1);
	}
	else
	{
		g_bIsSort = FALSE;
		pCmdUI->SetCheck(0);
	}
	
	return;
}

void CHLView::DrawMark(CDC* pDC) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->DrawMark(pDC, this);
	
	return;
}

void CHLView::SortPro()
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}
	BOOL bRet = pDoc->m_SortListObj.Track(this);
	if (bRet)
	{
		CString strMsg("");
		strMsg.LoadString(IDS_APPLYALLOBJECT);
		if (IDOK == AfxMessageBox(strMsg, MB_OKCANCEL))
		{
			pDoc->SortAllObj();
		}
	}
	return;
}

void CHLView::OnAutoSortObj() 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	m_nDrawTool = ID_ASORT_OBJECT_LIST;
	m_hCursorCur=m_hCursorClick;
	SetZoomMode(MODE_ZOOMOFF);
	if (!pDoc->InitASort(this))
	{
	}
	else
	{
		pDoc->ExitASort();
	}
	Invalidate();
	m_nDrawTool = ID_DRAW_CLICK;
	return;
}

void CHLView::OnUpdateAutoSortObj(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	return;
	
	if(ID_ASORT_OBJECT_LIST == m_nDrawTool)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
	
	return;
}

void CHLView::OnGSortObj() 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	m_nDrawTool = ID_G_OBJECT_LIST;
	SetZoomMode(MODE_ZOOMOFF);
	AfxGetMainWnd()->Invalidate();
	pDoc->InitGSort(this);
	m_nDrawTool = ID_DRAW_CLICK;
	
	return;
}

void CHLView::OnUpdateGSortObj(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	return;
	if(ID_ASORT_OBJECT_LIST == m_nDrawTool)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
	
	return;
}

void CHLView::OnSelSortObj() 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	m_nDrawTool = ID_SEL_OBJECT_LIST;
	SetZoomMode(MODE_ZOOMOFF);
	AfxGetMainWnd()->Invalidate();
	pDoc->InitSelSort(this);
	m_nDrawTool = ID_DRAW_CLICK;
	
	return;
}

void CHLView::OnUpdateSelSortObj(CCmdUI* pCmdUI) 
{
	if(ID_SEL_OBJECT_LIST == m_nDrawTool)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
	
	return;
}

void CHLView::AutoSortPro()
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	BOOL bRet = pDoc->m_ASortListObj.Track(this);
	if (bRet)
	{
		CString strMsg("");
		strMsg.LoadString(IDS_APPLYALLOBJECT);
		if (IDOK == AfxMessageBox(strMsg, MB_OKCANCEL))
		{
			pDoc->SortAllObj();
		}
	}

	pDoc->m_Sort.RemoveAll();

	return;
}

void CHLView::AutoSortDraw(CDC *pDc) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if (!pDoc)
	{
		return;
	}

	pDoc->m_ASortListObj.GetResultMsg();

	CPen   pen(PS_SOLID, 1, RGB(0,0,255));
	CPen  *OldPen = pDc->SelectObject(&pen);

	int nSize = pDoc->m_ASort.GetSize();
	ADRAWNODE *pNl = new ADRAWNODE[nSize];
	if (!pNl)
	{
		return;
	}
	for(int i = 0; i < nSize; i++)
	{
		ASNODE nd = pDoc->m_ASort[i];
		int nSort = nd.m_nSort;
		ASNODE nd1 = pDoc->m_ASort[nSort];
		pNl[i].m_SDt = nd1.m_SDt;
		pNl[i].m_EDt = nd1.m_EDt;
	}

	for(int j = 0; j < nSize; j++)
	{
		double sx = pNl[j].m_SDt.x;
		double sy = pNl[j].m_SDt.y;
		PhysicToLogic(sx, sy);
		double ex = pNl[j].m_EDt.x;
		double ey = pNl[j].m_EDt.y;
		PhysicToLogic(ex, ey);

		CRect rts((int)sx - 3, (int)sy - 3
			     ,(int)sx + 3, (int)sy + 3);
		pDc->FillSolidRect(&rts, RGB(255,0,0));

		CRect rte((int)ex - 3, (int)ey - 3
			     ,(int)ex + 3, (int)ey + 3);
		pDc->FillSolidRect(&rte, RGB(255,0,0));

		if (j == (nSize - 1)) 
		{
			CRect rts((int)sx - 3, (int)sy - 3
					 ,(int)sx + 3, (int)sy + 3);
			pDc->FillSolidRect(&rts, RGB(255, 0, 0));

			CRect rte((int)ex - 3, (int)ey - 3
			     ,(int)ex + 3, (int)ey + 3);
		    pDc->FillSolidRect(&rte, RGB(0, 255, 0));
		}
	}

	if(nSize > 0)
	{
		double endx,endy;

		for( int k=0; k < nSize; k++)
		{
			double sx = pNl[k].m_SDt.x;
			double sy = pNl[k].m_SDt.y;
			PhysicToLogic(sx, sy);
			double ex = pNl[k].m_EDt.x;
			double ey = pNl[k].m_EDt.y;
			PhysicToLogic(ex, ey);
			if(k==0)
			{
				endx = sx;
				endy = sy;
				pDc->MoveTo((int)sx, (int)sy);
				pDc->LineTo((int)ex, (int)ey);
			}
			else
			{
				pDc->LineTo((int)sx, (int)sy);
				pDc->LineTo((int)ex, (int)ey);
			}
		}// end for
		pDc->LineTo((int)endx,(int)endy);
	}// end if (size > 0)

	pDc->SelectObject(OldPen);

	pen.DeleteObject();
	if (pNl)
	{
		delete[] pNl;
		pNl = NULL;
	}

	return;
}

void CHLView::InvalidateSort()
{
	CRect rt;
	GetClientRect(&rt);
	rt.top = rt.bottom - 20;
	InvalidateRect(rt);
}

void CHLView::SetGroupNum(int nNum) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if (!pDoc)
	{
		return;
	}

	pDoc->SetGroupNum(nNum);
}

void CHLView::DispSelObjProp()
{
	CMainFrame *pFrame=(CMainFrame *)AfxGetMainWnd();
	if (NULL == pFrame)
	{
		return;
	} 

	CHLDoc *pDoc=GetDocument();
	if (NULL == pDoc)
	{
		return;
	}


//	pFrame->m_ObjectListDlg.ObjListLoad();

	if (pDoc->HaveSelectObject())
	{
		pFrame->m_XBarProperty.m_FillEditDlg.ShowSetTable(TRUE);
	}
	else
	{
		pFrame->m_XBarProperty.m_FillEditDlg.ShowSetTable(FALSE);
	}

	CQuad quad;
	CDot dotCur;
	DOUBLEOUTPARA DoubleOutParaIns;
	pDoc->m_chain.GetAllSelTreeProp(DoubleOutParaIns, quad);
#ifdef _DOUBLE_HEAD_MARK
	pFrame->m_XBarProperty.m_FillEditDlg.m_DBSpecialtyDlg.m_dbLeftOutX=DoubleOutParaIns.dbLeftOutAliX;
	pFrame->m_XBarProperty.m_FillEditDlg.m_DBSpecialtyDlg.m_dbLeftOutY=DoubleOutParaIns.dbLeftOutAliY;
	pFrame->m_XBarProperty.m_FillEditDlg.m_DBSpecialtyDlg.m_dbRightOutX=DoubleOutParaIns.dbRightOutAliX;
	pFrame->m_XBarProperty.m_FillEditDlg.m_DBSpecialtyDlg.m_dbRightOutY=DoubleOutParaIns.dbRightOutAliY;
	pFrame->m_XBarProperty.m_FillEditDlg.m_DBSpecialtyDlg.UpdateUIData(FALSE);
#endif

	FILLDATA fillOld;
	pDoc->GetFillProp(fillOld);
	pFrame->m_XBarProperty.m_FillEditDlg.m_ObjFillEdit.DisplyFillProp(fillOld);

	OBJMARKSET ObjMarkSet;
    pDoc->GetObjMarkSet(ObjMarkSet);
	pFrame->m_XBarProperty.m_FillEditDlg.m_FillObjMarkSet.DisplayMarkSet(ObjMarkSet);

	pDoc->m_chain.RefreshSelectQuadList(pDoc->m_QuadList);

	int nLayer = 0,nFill = 0;
	pDoc->m_chain.GetLayColor(nLayer,nFill);
	if( nLayer >= 0 )
		pFrame->m_XBarMarkPara.m_PropDlg.DispSelObjLayer(nLayer);

	pFrame->m_XBarProperty.m_PSDlg.UpdateProp(quad);
	pFrame->SetLengthInfo(pDoc->GetSelTotalLength());

	ShowTextInfo();
	ShowBarInfo();
	
	return;
}

void CHLView::OnLoadObjList() 
{
	CHLDoc *pDoc=GetDocument();
	if (NULL == pDoc)
	{
		return;
	}
	
    pDoc->ObjListUnLoad();
	pDoc->ObjListLoad();

	return;
}


void CHLView::OnAlignBottom() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdAlignBottom);

	return;
}

void CHLView::OnAlignLeft() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdAlignLeft);

	return;
}

void CHLView::OnAlignRight() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdAlignRight);

	return;
}

void CHLView::OnAlignTop() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdAlignTop);

	return;
}

void CHLView::OnDistanceX() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdDistanceX);

	return;
}

void CHLView::OnDistanceY() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdDistanceY);

	return;
}

void CHLView::OnEqualHeight() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdEqualHeight);

	return;
}

void CHLView::OnEqualWidth() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdEqualWidth);

	return;
}

void CHLView::OnEqualWh() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnMAlignCmd(CmdEqualWh);

	return;
}

void CHLView::OnRightMng() 
{
	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (bMng) 
	{
		CUserRightCfgMng RightMngDlg;
		RightMngDlg.DoModal();
	}
	else
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
	}
	
	return;
}

void CHLView::OnChangeUser() 
{
	CChangeUserDlg ChangeUserDlg;
	ChangeUserDlg.m_strUserName = g_CurUserInfo.szUserName;
	ChangeUserDlg.DoModal();

}

void CHLView::ClearMark() 
{
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}
	m_track.ClearAll();
    pDoc->m_QuadList.RemoveAll();
	pDoc->SetSelect(FALSE);
	pDoc->m_quadTracker.SetEmpty();
	Invalidate();
	
	return;
}

BOOL CHLView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
	{
		OnDrawClick();
		CQuad quad;
		GetDocument()->SetTrackerQuad(quad);
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN )
	{
		if ((GetKeyState(VK_SHIFT) & 0x8000) && (GetKeyState(VK_Z) & 0x8000) )
		{
			CHLDoc *pDoc=GetDocument();
			if (NULL != pDoc)
			{
				g_pDlgMarkNormalPane->OnBnClickedButtonMark();
				return TRUE;
			}
		}
	}*/


	return CZoomView::PreTranslateMessage(pMsg);
}

void CHLView::ShowSpecSet(BOOL bShow)
{
	CMainFrame *pFrame=(CMainFrame *)AfxGetMainWnd();
	if (NULL == pFrame)
	{
		return;
	}

	pFrame->m_XBarProperty.m_FillEditDlg.ShowSetTable(bShow);
	
	return;
}


void CHLView::ShowTextInfo()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (NULL == pFrame)
	{
		return;
	}

	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	FILLDATA fill;
	CNode *pNode = pDoc->GetSelNode(fill);
	if (pNode) 
	{
		int nType = pNode->GetType();
		if (NODE_TEXT_DSF != nType) 
		{
			pFrame->m_XBarProperty.m_FillEditDlg.ShowTextItem(pNode, fill, FALSE);
			return;
		}

		pFrame->m_XBarProperty.m_FillEditDlg.ShowBarItem(pNode, fill, FALSE);
		pFrame->m_XBarProperty.m_FillEditDlg.ShowTextItem(pNode, fill, TRUE);
	}
	else
	{
		pFrame->m_XBarProperty.m_FillEditDlg.ShowTextItem(pNode, fill, FALSE);
	}

	
	return;
}


void CHLView::ShowBarInfo()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (NULL == pFrame)
	{
		return;
	}

	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	FILLDATA fill;
	CNode *pNode = pDoc->GetSelNode(fill);
	if (pNode) 
	{
		int nType = pNode->GetType();
		if (NODE_CODEBAR != nType) 
		{
			pFrame->m_XBarProperty.m_FillEditDlg.ShowBarItem(pNode, fill, FALSE);
			return;
		}
		
		pFrame->m_XBarProperty.m_FillEditDlg.ShowTextItem(pNode, fill, FALSE);
		pFrame->m_XBarProperty.m_FillEditDlg.ShowBarItem(pNode, fill, TRUE);
		
	}
	else
	{
		pFrame->m_XBarProperty.m_FillEditDlg.ShowBarItem(pNode, fill, FALSE);
	}

	
	return;
}

BOOL CHLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CZoomView::OnEraseBkgnd(pDC);
}

void CHLView::MarkTicps(BOOL bShow,BOOL bMarkTicps)
{
	static CCMarkTicps *dlg  = NULL;

	if (bShow)
	{
		if ( dlg )
		{
			dlg->m_bMarkTicps = bMarkTicps;
			dlg->ShowWindow(SW_SHOW);
		}
		else
		{
			dlg = new CCMarkTicps();
			dlg->m_bMarkTicps = bMarkTicps;
			dlg->Create(CCMarkTicps::IDD);

			dlg->CenterWindow();
			dlg->ShowWindow(SW_SHOW);
		}
	/*	::SetWindowLong(dlg->m_hWnd, GWL_EXSTYLE,
			GetWindowLong(dlg->m_hWnd,
			GWL_EXSTYLE) ^ WS_EX_LAYERED);
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		lpfn pSetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32,
			"SetLayeredWindowAttributes");
		pSetLayeredWindowAttributes(dlg->m_hWnd, 0,200, 2);*/
	}
	else
	{
		if (dlg)
		{
			dlg->DestroyWindow();
			dlg = NULL;
		}
	}
}


void CHLView::OnDrawDot() 
{
	// TODO: Add your command handler code here
	if(GetZoomMode()!=MODE_ZOOMOFF)
		SetZoomMode(MODE_ZOOMOFF);
	/////////////////////////////////zg add 2005/1/12 end

	
	m_nDrawTool=ID_DRAW_DOT;
	m_hCursorCur= m_hCursorDot;	


	if(m_bRotate)
	{
		m_bRotate=FALSE;
		Invalidate();
		CHLDoc *pDoc=GetDocument();
		m_track.m_rect=(CRect)pDoc->GetTrackerQuad();
		ViewLPtoDP((LPPOINT)&m_track.m_rect,2);
	}	

}

void CHLView::OnUpdateDrawDot(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_nDrawTool==ID_DRAW_DOT)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CHLView::OnJointstrokeCurdoc()
{
	// TODO: 在此添加命令处理程序代码
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc ) 
		return;

	pDoc->JoinStrokeCmd();
}
