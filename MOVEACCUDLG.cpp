// MOVEACCUDLG.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "MOVEACCUDLG.h"
#include "HLView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MOVEACCUDLG dialog
extern CHLApp theApp;

MOVEACCUDLG::MOVEACCUDLG(CWnd* pParent /*=NULL*/)
	: CDialog(MOVEACCUDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(MOVEACCUDLG)
	m_dbMoveX = 0.0;
	m_dbMoveY = 0.0;
	m_dbMoveStep = 0.0;
	//}}AFX_DATA_INIT
}


void MOVEACCUDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MOVEACCUDLG)
	DDX_Text(pDX, IDC_MOVEX, m_dbMoveX);
	DDX_Text(pDX, IDC_MOVEY, m_dbMoveY);
	DDX_Text(pDX, IDC_MOVE_STEP_VALUE, m_dbMoveStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MOVEACCUDLG, CDialog)
	//{{AFX_MSG_MAP(MOVEACCUDLG)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, OnButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DWON, OnButtonMoveDwon)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_LEFT, OnButtonMoveLeft)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_RIGTH, OnButtonMoveRigth)
    ON_BN_CLICKED(IDC_BUTTON_SELMARK, OnButtonSelMark)
	ON_WM_CANCELMODE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MOVEACCUDLG message handlers

BOOL MOVEACCUDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dbMoveStep = g_SysCfg.m_dbDotMoveStepLen;
	UpdateData(FALSE);
	
	return TRUE;
}


void MOVEACCUDLG::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
		g_bIsRedPreview = FALSE;
	}

	if (g_bIsRedPreview) 
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
		g_bIsFastMark = FALSE;
	}

	
	g_SysCfg.m_dbDotMoveStepLen = m_dbMoveStep;

	CDialog::OnOK();
}


CHLDoc * MOVEACCUDLG::GetDocument()
{
	CMDIFrameWnd *pMainWnd=(CMDIFrameWnd *)AfxGetMainWnd();
	CMDIChildWnd *pChildWnd=pMainWnd->MDIGetActive();
	if(pChildWnd)
	{
		CHLDoc *pDoc=(CHLDoc *)pChildWnd->GetActiveDocument();
		return pDoc;
	}
	return NULL;
}

void MOVEACCUDLG::OnButtonMoveUp() 
{
	UpdateData(TRUE);
	
	CFrameWnd *pFWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pFWnd->GetActiveFrame()->GetActiveView();

	pView->m_track.InverDrawDot(this);
	m_dbMoveY += m_dbMoveStep;
	double dbMoveY = MyPrecision(m_dbMoveStep);
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->MoveAccurate(0, dbMoveY);
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	UpdateData(FALSE);

	pView->Invalidate();

	return;
}

void MOVEACCUDLG::OnButtonMoveDwon() 
{
	UpdateData(TRUE);
	
	CFrameWnd *pFWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pFWnd->GetActiveFrame()->GetActiveView();

	pView->m_track.InverDrawDot(this);
	m_dbMoveY -= m_dbMoveStep;
	double dbMoveY = MyPrecision(-m_dbMoveStep);
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->MoveAccurate(0, dbMoveY);
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	UpdateData(FALSE);

	pView->Invalidate();

	return;
}

void MOVEACCUDLG::OnButtonMoveLeft() 
{
	UpdateData(TRUE);
	
	CFrameWnd *pFWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pFWnd->GetActiveFrame()->GetActiveView();

	pView->m_track.InverDrawDot(this);
	m_dbMoveX -= m_dbMoveStep;
	double dbMoveX = MyPrecision(-m_dbMoveStep);
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->MoveAccurate(dbMoveX, 0);
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	UpdateData(FALSE);

	pView->Invalidate();

	return;
	
}

void MOVEACCUDLG::OnButtonMoveRigth() 
{
	UpdateData(TRUE);

	CFrameWnd *pFWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pFWnd->GetActiveFrame()->GetActiveView();

	pView->m_track.InverDrawDot(this);
	m_dbMoveX += m_dbMoveStep;
	double dbMoveX = MyPrecision(m_dbMoveStep);
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	pDoc->MoveAccurate(dbMoveX, 0);
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	UpdateData(FALSE);

	pView->Invalidate();

	return;
}

void MOVEACCUDLG::OnButtonSelMark() 
{
	CMainFrame *pMainWnd = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainWnd)
	{
		return;
	}

	int nCount = pMainWnd->m_XBarMarkPara.m_PropDlg.m_listLayer.GetItemCount();
	CHLDoc *pDoc = pMainWnd->m_XBarMarkPara.m_PropDlg.GetDocument();
	if (NULL == pDoc)
	{
		return;
	}
	
	int nCurSel = pMainWnd->m_XBarMarkPara.m_PropDlg.m_listLayer.GetCurSel();
	if ((nCurSel < 0) || (nCurSel >= nCount))
	{
		return;
	}

	CLayer ly = pDoc->GetLayer(nCurSel);
	
	CLay lay;
	lay.Add(ly);
	g_DAT.SetMarkPara(lay);

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
		
//		return;
	}

	if (g_bIsRedPreview) 
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
		return;
	}
	else
	{
		g_bIsSelMark = TRUE;

		SUBOBJMARKSET stSub;
		stSub.bIsFillFirst = TRUE;
		stSub.bIsMarkFill  = TRUE;
		stSub.bIsMarkFram  = TRUE;
		stSub.nLayer       = 0;
		stSub.nSubCnt      = 1;

		theApp.m_SelMark.StartSelMark(this->m_hWnd, stSub, ENU_MARKFASTFPKNORMALL);
		
	}

	return;
}




void MOVEACCUDLG::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
		g_bIsSelMark = FALSE;
	}

	if (g_bIsRedPreview) 
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
		g_bIsFastMark = FALSE;
	}

	return;
	
}
