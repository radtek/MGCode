// DlgSplash.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSplash dialog


CDlgSplash::CDlgSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSplash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInitFinished = FALSE;
}


void CDlgSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSplash)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSplash, CDialog)
	//{{AFX_MSG_MAP(CDlgSplash)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSplash message handlers

BOOL CDlgSplash::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	SetTimer(1, 2000, NULL);
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;
}

BOOL CDlgSplash::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSplash::SetPaneText(const CString& str)
{
	RedrawWindow();

	CDC *pDC = GetWindowDC();
	CRect rt;
	GetWindowRect(rt);
	ScreenToClient(rt);
	CFont *pFont = GetFont();
	CFont *pOldFont = pDC->SelectObject(pFont);
	pDC->TextOut(rt.left+10, rt.bottom-20, str);
	if(pOldFont)
		pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}


void CDlgSplash::OnDestroy()
{
	CDialog::OnDestroy();
	delete this;
}


void CDlgSplash::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1 && m_bInitFinished)
	{
		KillTimer(1);
		OnOK();
	}
	//CDialog::OnTimer(nIDEvent);
}


void CDlgSplash::OnOK()
{
	CDialog::OnOK();
	DestroyWindow();
}


void CDlgSplash::OnCancel()
{
	CDialog::OnCancel();
	DestroyWindow();
}
