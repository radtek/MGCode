// FastMarkStopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "FastMarkStopDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFastMarkStopDlg dialog

extern CHLApp theApp;

CFastMarkStopDlg::CFastMarkStopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFastMarkStopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastMarkStopDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFastMarkStopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastMarkStopDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFastMarkStopDlg, CDialog)
	//{{AFX_MSG_MAP(CFastMarkStopDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FASTMARK_END, OnFastMarkEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastMarkStopDlg message handlers

BOOL CFastMarkStopDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnFastmark();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFastMarkStopDlg::OnOK() 
{
	theApp.m_FastMark.StopFastMark();
//	CDialog::OnOK();
}

void CFastMarkStopDlg::OnFastmark() 
{
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
//		return;
	}
	else
	{
		g_bIsFastMark = TRUE;

		theApp.m_FastMark.StartFastMark(this->m_hWnd);
	}

	return;
}


LRESULT CFastMarkStopDlg::OnFastMarkEnd(WPARAM wParam,LPARAM lParam)
{	
	if (g_bIsFastMark) 
	{
		g_DAT.LeaveMarkEnv();
	    g_bIsFastMark = FALSE;
	}
	
	CDialog::OnOK();

	return NULL;
}

void CFastMarkStopDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
//	theApp.m_FastMark.StopFastMark();
}
