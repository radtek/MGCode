// ExternViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ExternViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExternViewDlg dialog


CExternViewDlg::CExternViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExternViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExternViewDlg)
	m_bIsKillOldObj = FALSE;
	m_dbLineSpace = 0.1;
	m_bOpt = TRUE;
	m_strLineSpace = _T("0.1");
	//}}AFX_DATA_INIT
	CRuntimeClass *pClass = RUNTIME_CLASS(CExternDoc);
	m_pDoc = (CExternDoc*)pClass->CreateObject();
	m_pView = NULL;
	m_pChildFrame = new CFrameWnd;
	m_pHLDoc = NULL;
}


void CExternViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExternViewDlg)
	DDX_Check(pDX, IDC_CHECK_ISKILL_OLDOBJ, m_bIsKillOldObj);
	DDX_Check(pDX, IDC_CHECK_OPT, m_bOpt);
	DDX_Text(pDX, IDC_EDIT_LINE_SPACE, m_strLineSpace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExternViewDlg, CDialog)
	//{{AFX_MSG_MAP(CExternViewDlg)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_ADJUSTPAGE, OnBtnAdjustpage)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExternViewDlg message handlers

void CExternViewDlg::OnBtnApply() 
{
	CWaitCursor cursor;
	UpdateData(TRUE);
	m_dbLineSpace = atof(m_strLineSpace);
	m_pDoc->ApplyExtern(m_dbLineSpace, m_bOpt);
}

BOOL CExternViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CreateView();

	OnBtnAdjustpage();

	CRect rect;
	GetClientRect(&rect);
	m_oldSize = rect.Size();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExternViewDlg::CreateView()
{
	CRect rect;
	GetDlgItem(IDC_STATIC_PANEL)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CCreateContext context;
	context.m_pCurrentDoc = m_pDoc;
	context.m_pCurrentFrame = m_pChildFrame;
	context.m_pLastView = NULL;
	context.m_pNewDocTemplate = NULL;
	context.m_pNewViewClass = RUNTIME_CLASS(CExternView);
	m_pChildFrame->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, 
		rect, this, NULL, 0, &context);
	
	POSITION pos = m_pDoc->GetFirstViewPosition();
	m_pView = (CExternView*)m_pDoc->GetNextView(pos);

	m_pView->SetFocus();
}

void CExternViewDlg::SetList(CHLDoc *pHLDoc)
{
	m_pHLDoc = pHLDoc;

	POSITION pos = pHLDoc->m_chain.m_list.GetHeadPosition();
	while (pos != NULL)
	{
		CGroupObj *pGroupObj = (CGroupObj *)(pHLDoc->m_chain.m_list.GetNext(pos));
		if(pGroupObj->IsSelect() == TRUE)
		{
			m_pDoc->m_list.AddTail(pGroupObj);
		}
	}
}

void CExternViewDlg::OnOK() 
{
	CWaitCursor cursor;
	UpdateData(TRUE);
	m_dbLineSpace = atof(m_strLineSpace);
	g_SysCfg.m_dbExtendSpace = m_dbLineSpace;
	m_pHLDoc->m_bExternOpt = m_bOpt;
	m_pHLDoc->OnExtendObj(m_bIsKillOldObj);
	CDialog::OnOK();
}

void CExternViewDlg::OnBtnAdjustpage() 
{
	CQuad quad = m_pDoc->CalLimit();
	if(!quad.IsEmpty())
	{
		double width = quad.Width()+2;
		double height = quad.Height()+2;
		double x = quad.Center().x;
		double y = quad.Center().y;
		m_pView->DoZoomIn(width, height, x, y);
	}
}

void CExternViewDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_pChildFrame->GetSafeHwnd())
	{
		int ox = cx - m_oldSize.cx;
		int oy = cy - m_oldSize.cy;
		
		CRect rect;
		m_pChildFrame->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.right += ox;
		rect.bottom += oy;
		m_pChildFrame->MoveWindow(&rect);
		
		SetWndOffset(ox, 0, IDC_STATIC_GROUP);
		SetWndOffset(ox, 0, IDC_STATIC_LINESPACE);
		SetWndOffset(ox, 0, IDC_EDIT_LINE_SPACE);
		SetWndOffset(ox, 0, IDC_BTN_APPLY);
		SetWndOffset(ox, 0, IDC_STATIC_NOTE);
		SetWndOffset(ox, 0, IDC_BTN_ADJUSTPAGE);
		SetWndOffset(ox, oy, IDC_CHECK_ISKILL_OLDOBJ);
		SetWndOffset(ox, oy, IDOK);
		SetWndOffset(ox, oy, IDCANCEL);
	}

	m_oldSize.cx = cx;
	m_oldSize.cy = cy;
}

void CExternViewDlg::SetWndOffset(int ox, int oy, UINT nID)
{
	CRect rect;
	CWnd *pWnd = GetDlgItem(nID);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.OffsetRect(ox, oy);
	pWnd->MoveWindow(&rect);
}
