// ExtendObjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ExtendObjDlg.h"
#include "HLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtendObjDlg dialog


CExtendObjDlg::CExtendObjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtendObjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtendObjDlg)
	m_dbLineSpace = 1;
	m_bIsKillOldObj = FALSE;
	//}}AFX_DATA_INIT
}


void CExtendObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtendObjDlg)
	DDX_Text(pDX, IDC_EDIT_LINE_SPACE, m_dbLineSpace);
	DDX_Check(pDX, IDC_CHECK_ISKILL_OLDOBJ, m_bIsKillOldObj);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtendObjDlg, CDialog)
	//{{AFX_MSG_MAP(CExtendObjDlg)
	ON_BN_CLICKED(IDC_BUTTON_EXTENDOBJ, OnButtonExtendobj)
	ON_BN_CLICKED(IDC_CHECK_ISKILL_OLDOBJ, OnCheckIskillOldobj)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtendObjDlg message handlers
BOOL CExtendObjDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dbLineSpace = g_SysCfg.m_dbExtendSpace;
	UpdateData(FALSE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExtendObjDlg::OnButtonExtendobj() 
{
	UpdateData(TRUE);
	g_SysCfg.m_dbExtendSpace = m_dbLineSpace;

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->OnExtendObj(m_bIsKillOldObj);

	CDialog::OnOK();
	
}

void CExtendObjDlg::OnCheckIskillOldobj() 
{
	// TODO: Add your control notification handler code here
	
}
