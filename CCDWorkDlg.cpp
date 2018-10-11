// CCDWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "CCDWorkDlg.h"
#include "CCDAdjustDlg.h"
#include "CCDCalibrate.h"

#include "HLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCDWorkDlg dialog
IMPLEMENT_DYNCREATE(CCCDWorkDlg, CPropertyPage)
CCCDWorkDlg::CCCDWorkDlg() : CPropertyPage(CCCDWorkDlg::IDD)
{
	//{{AFX_DATA_INIT(CCCDWorkDlg)
	m_dbPixelX1 = 0.0;
	m_dbPixelX2 = 0.0;
	m_dbPixelY1 = 0.0;
	m_dbPixelY2 = 0.0;
	m_dbWorkX1 = 0.0;
	m_dbWorkX2 = 0.0;
	m_dbWorkY1 = 0.0;
	m_dbWorkY2 = 0.0;
	m_nProgID1 = 0;
	m_nProgID2 = 0;
	//}}AFX_DATA_INIT
	m_dbCCDtoWorkx1=0.0;
	m_dbCCDtoWorky1=0.0;
	m_dbCCDtoWorkx2=0.0;
	m_dbCCDtoWorky2=0.0;
	m_dbRotateAngle=0;
}


void CCCDWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCCDWorkDlg)
	DDX_Text(pDX, IDC_EDIT_PIXEL_X1, m_dbPixelX1);
	DDX_Text(pDX, IDC_EDIT_PIXEL_X2, m_dbPixelX2);
	DDX_Text(pDX, IDC_EDIT_PIXEL_Y1, m_dbPixelY1);
	DDX_Text(pDX, IDC_EDIT_PIXEL_Y2, m_dbPixelY2);
	DDX_Text(pDX, IDC_EDIT_WORK_X1, m_dbWorkX1);
	DDX_Text(pDX, IDC_EDIT_WORK_X2, m_dbWorkX2);
	DDX_Text(pDX, IDC_EDIT_WORK_Y1, m_dbWorkY1);
	DDX_Text(pDX, IDC_EDIT_WORK_Y2, m_dbWorkY2);
	DDX_Text(pDX, IDC_EDIT_PROGID1, m_nProgID1);
	DDX_Text(pDX, IDC_EDIT_PROGID2, m_nProgID2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCCDWorkDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CCCDWorkDlg)
	ON_BN_CLICKED(IDC_BUTTON_CCDLOCATE1, OnButtonCcdlocate1)
	ON_BN_CLICKED(IDC_BUTTON_CCDLOCATE2, OnButtonCcdlocate2)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PARA, OnButtonSavePara)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_TO_WORK1, OnButtonMoveToWork1)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_TO_WORK2, OnButtonMoveToWork2)
	ON_BN_CLICKED(IDC_BUTTON_CCD_CENTER1, OnButtonCcdCenter1)
	ON_BN_CLICKED(IDC_BUTTON_CCD_CENTER2, OnButtonCcdCenter2)
	ON_BN_CLICKED(IDC_BUTTON_CCD1_TO_CCD2, OnButtonCcd1ToCcd2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCDWorkDlg message handlers

BOOL CCCDWorkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadData();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCCDWorkDlg::OnButtonCcdlocate1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SaveData();
	g_ccd.SetWorkProgNo(m_nProgID1);
	g_ccd.CCDLocate();
	::g_ccd.GetTrigOK(m_dbPixelX1,m_dbPixelY1,m_dbRotateAngle);		
	::g_ccd.GetWorkPos(m_dbWorkX1,m_dbWorkY1);	
	double deg=0.0;
	::g_ccd.GetWorkPosPointToCCDCenter(TRUE, m_dbCCDtoWorkx1,m_dbCCDtoWorky1,deg);
	CString str;
	str.Format("x1a %.3f,y1a %.3f",m_dbCCDtoWorkx1,m_dbCCDtoWorky1);
	::OutputDebugString(str);

	UpdateData(FALSE);
}

void CCCDWorkDlg::OnButtonCcdlocate2() 
{
	UpdateData(TRUE);
	SaveData();
	g_ccd.SetWorkProgNo(m_nProgID2);
	g_ccd.CCDLocate();
	::g_ccd.GetTrigOK(m_dbPixelX2,m_dbPixelY2,m_dbRotateAngle);
	::g_ccd.GetWorkPos(m_dbWorkX2,m_dbWorkY2);		
	double deg=0.0;
	::g_ccd.GetWorkPosPointToCCDCenter(FALSE, m_dbCCDtoWorkx2,m_dbCCDtoWorky2,deg);
	CString str;
	str.Format("x2a %.3f,y2a %.3f",m_dbCCDtoWorkx2,m_dbCCDtoWorky2);
	::OutputDebugString(str);
	UpdateData(FALSE);			
}

void CCCDWorkDlg::LoadData()
{	
	m_dbPixelX1		= g_ParaIni.ReadDouble("CCDWORK","PixelX1",0.0);
	m_dbPixelX2		= g_ParaIni.ReadDouble("CCDWORK","PixelX2",0.0);
	m_dbPixelY1		= g_ParaIni.ReadDouble("CCDWORK","PixelY1",0.0);
	m_dbPixelY2		= g_ParaIni.ReadDouble("CCDWORK","PixelY2",0.0);

	m_dbWorkX1	= g_ParaIni.ReadDouble("CCDWORK","WorkX1",0.0);
	m_dbWorkX2	= g_ParaIni.ReadDouble("CCDWORK","WorkX2",0.0);
	m_dbWorkY1	= g_ParaIni.ReadDouble("CCDWORK","WorkY1",0.0);
	m_dbWorkY2	= g_ParaIni.ReadDouble("CCDWORK","WorkY2",0.0);

	m_nProgID1	= g_ParaIni.ReadInt("CCDWORK","ProgID1",0);
	m_nProgID2	= g_ParaIni.ReadInt("CCDWORK","ProgID2",1);

	m_dbCCDtoWorkx1=g_ParaIni.ReadDouble("CCDWORK","CCDtoWorkx1",0.0);
	m_dbCCDtoWorky1=g_ParaIni.ReadDouble("CCDWORK","CCDtoWorky1",0.0);
	m_dbCCDtoWorkx2=g_ParaIni.ReadDouble("CCDWORK","CCDtoWorkx2",0.0);
	m_dbCCDtoWorky2=g_ParaIni.ReadDouble("CCDWORK","CCDtoWorky2",0.0);

}

void CCCDWorkDlg::SaveData()
{ 
	g_ParaIni.WriteDouble("CCDWORK","PixelX1",m_dbPixelX1);
	g_ParaIni.WriteDouble("CCDWORK","PixelX2",m_dbPixelX2);
	g_ParaIni.WriteDouble("CCDWORK","PixelY1",m_dbPixelY1);
	g_ParaIni.WriteDouble("CCDWORK","PixelY2",m_dbPixelY2);

	g_ParaIni.WriteDouble("CCDWORK","WorkX1",m_dbWorkX1);
	g_ParaIni.WriteDouble("CCDWORK","WorkX2",m_dbWorkX2);
	g_ParaIni.WriteDouble("CCDWORK","WorkY1",m_dbWorkY1);
	g_ParaIni.WriteDouble("CCDWORK","WorkY2",m_dbWorkY2);

	g_ParaIni.WriteInt("CCDWORK","ProgID1",m_nProgID1);
	g_ParaIni.WriteInt("CCDWORK","ProgID2",m_nProgID2);

	g_ParaIni.WriteDouble("CCDWORK","CCDtoWorkx1",m_dbCCDtoWorkx1);
	g_ParaIni.WriteDouble("CCDWORK","CCDtoWorky1",m_dbCCDtoWorky1);
	g_ParaIni.WriteDouble("CCDWORK","CCDtoWorkx2",m_dbCCDtoWorkx2);
	g_ParaIni.WriteDouble("CCDWORK","CCDtoWorky2",m_dbCCDtoWorky2);

	UpdateData(FALSE);
}


void CCCDWorkDlg::OnButtonSavePara() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SaveData();
}

void CCCDWorkDlg::OnButtonMoveToWork1() 
{
	m_dbWorkX1	= g_ParaIni.ReadDouble("CCDWORK","WorkX1",0.0);	
	m_dbWorkY1	= g_ParaIni.ReadDouble("CCDWORK","WorkY1",0.0);	
	UpdateData(FALSE);
	g_pMotion->LnXY(m_dbWorkX1,m_dbWorkY1);
}

void CCCDWorkDlg::OnButtonMoveToWork2() 
{
	m_dbWorkX2	= g_ParaIni.ReadDouble("CCDWORK","WorkX2",0.0);
	m_dbWorkY2	= g_ParaIni.ReadDouble("CCDWORK","WorkY2",0.0);
	UpdateData(FALSE);
	g_pMotion->LnXY(m_dbWorkX2,m_dbWorkY2);
}

void CCCDWorkDlg::OnButtonCcdCenter1() 
{
	UpdateData(TRUE);
	SaveData();
	double x=m_dbWorkX1;
	double y=m_dbWorkY1;
	g_ccd.MoveToCCDCenter(TRUE, m_dbPixelX1, m_dbPixelY1, x, y);

	g_pMotion->LnXY(x, y);
}

void CCCDWorkDlg::OnButtonCcdCenter2() 
{
	UpdateData(TRUE);
	SaveData();
	double x=m_dbWorkX2;
	double y=m_dbWorkY2;
	g_ccd.MoveToCCDCenter(FALSE, m_dbPixelX2, m_dbPixelY2, x, y);	

	g_pMotion->LnXY(x,y);
}


void CCCDWorkDlg::OnButtonCcd1ToCcd2() 
{
	// TODO: Add your control notification handler code here
	double dbX = g_pMotion->GetPos(eMaskX);
	double dbY = g_pMotion->GetPos(eMaskY);
	dbX += (g_ParaIni.m_dbMarkToCCD2X - g_ParaIni.m_dbMarkToCCD1X);
	dbY += (g_ParaIni.m_dbMarkToCCD2Y - g_ParaIni.m_dbMarkToCCD1Y);
	g_pMotion->LnXY(dbX, dbY);
}
