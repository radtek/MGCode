// MarkPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "MarkPreviewDlg.h"
#include "HLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkPreviewDlg dialog
CMarkPreviewDlg::CMarkPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkPreviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkPreviewDlg)
	//m_nPreviewSpeed = 0;
	m_nPreviewSpeed=0;
	//}}AFX_DATA_INIT
	m_ViewList.RemoveAll();
	m_bPreview = FALSE;
}


void CMarkPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkPreviewDlg)
	DDX_Slider(pDX, IDC_SLIDER_SPEED, m_nPreviewSpeed);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarkPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkPreviewDlg)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SPEED, OnReleasedcaptureSliderSpeed)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PREVIEW_END, OnPreviewEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkPreviewDlg message handlers

void CMarkPreviewDlg::OnButtonStart() 
{
	g_sys.m_bStopPreview = FALSE;
	m_bPreview = TRUE;
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	//GetDlgItem(IDC_SLIDER_SPEED)->EnableWindow(FALSE);
	m_xzoom.SetPreviewSpeed(m_nPreviewSpeed*1.0);
	m_xzoom.PreviewStart();
}

void CMarkPreviewDlg::OnButtonStop() 
{
	m_bPreview = FALSE;
	g_sys.m_bStopPreview = TRUE;
	m_xzoom.PreviewStop();
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_SPEED)->EnableWindow(TRUE);
	g_sys.m_bStopPreview = FALSE;
}

void CMarkPreviewDlg::OnClose() 
{
	if(m_bPreview)
		OnButtonStop();	

	m_xzoom.DestroyWindow();
	CDialog::OnClose();
}

BOOL CMarkPreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SPEED);
	pSlider->SetRange(1, MARKPREVIEW_SPEEDMAX-1);
	pSlider->SetPos(MARKPREVIEW_SPEEDMAX/2);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	
	m_ViewList.RemoveAll();
//	StrokeList slist;
	m_pChain->ReLoadStrokeList(&m_ViewList,TRUE);
	m_xzoom.SetStrokeList(&m_ViewList);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CMarkPreviewDlg::OnPreviewEnd(WPARAM wParam,LPARAM lParam)
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_SPEED)->EnableWindow(TRUE);
	m_bPreview = FALSE;
	return NULL;
}

void CMarkPreviewDlg::OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	*pResult = 0;
	m_xzoom.SetPreviewSpeed(m_nPreviewSpeed*1.0);
}

int CMarkPreviewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rt;
	GetClientRect(&rt);
	rt.top    += 5;
	rt.left   += 5;
	rt.right  -= 5;
	rt.bottom -= 60;
	double x(0.0), y(0.0);
	g_DAT.GetMarkRange(x,y);

	m_xzoom.CreateZoomEx(rt,this,ID_ZOOM_WND,x,y);
 	m_xzoom.SetZoomScale(g_SysCfg.m_dbPreViewZoomScale);
	
	return 0;
}
