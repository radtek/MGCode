// BarPage.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "BarPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarPage property page

IMPLEMENT_DYNCREATE(CBarPage, CPropertyPage)

CBarPage::CBarPage() : CPropertyPage(CBarPage::IDD)
{
	//{{AFX_DATA_INIT(CBarPage)
	m_bRev = FALSE;
	m_bGoBack  = FALSE;
	m_bConnect = FALSE;
	m_dbHeight = 0.0;
	m_dbNarrow = 0.0;
	m_dbShrink = 0.0;
	m_dbWidth = 0.0;
	m_dbSpace = 0.0;
	m_dbAngle = 0.0;
	m_nSingleLineMarkCount = 1;
	m_nNarrowLineNum = 2;
	//}}AFX_DATA_INIT
	m_bPointDataMatrix = FALSE;
	m_nPointRadiusPercent = 0;
}

CBarPage::~CBarPage()
{
}

void CBarPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarPage)
	DDX_Check(pDX, IDC_CHECK_REV, m_bRev);
	DDX_Check(pDX, IDC_CHECK_GO_BACK, m_bGoBack);
	DDX_Check(pDX, IDC_CHECK_LINE_CONNECT, m_bConnect);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_dbHeight);
	DDX_Text(pDX, IDC_EDIT_NARROW, m_dbNarrow);
	DDX_Text(pDX, IDC_EDIT_SHRINK, m_dbShrink);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_dbWidth);
	DDX_Text(pDX, IDC_EDIT_SPACE, m_dbSpace);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_dbAngle);
	DDX_Text(pDX, IDC_EDIT_SINGLELINE_MARKCOUNT, m_nSingleLineMarkCount);
	DDV_MinMaxDWord(pDX, m_nSingleLineMarkCount, 1, 9999999);
	DDX_Text(pDX, IDC_EDIT_NARROW_LINENUM, m_nNarrowLineNum);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_POINT_DATAMATRIX, m_bPointDataMatrix);
	DDX_Text(pDX, IDC_EDIT_POINT_RADIUS_PERCEN, m_nPointRadiusPercent);
}


BEGIN_MESSAGE_MAP(CBarPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBarPage)
	ON_BN_CLICKED(IDC_CHECK_REV, OnCheckRev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarPage message handlers

BOOL CBarPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(m_bRev);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBarPage::OnCheckRev() 
{
	UpdateData();
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(m_bRev);
}
