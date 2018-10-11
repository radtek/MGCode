// CCDAdjustDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "CCDAdjustDlg.h"
#include "CCDCalibrate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCDAdjustDlg dialog
IMPLEMENT_DYNCREATE(CCCDAdjustDlg, CPropertyPage)
CCCDAdjustDlg::CCCDAdjustDlg() : CPropertyPage(CCCDAdjustDlg::IDD)
{
	//{{AFX_DATA_INIT(CCCDAdjustDlg)
	m_dbXMMPerPixel = 0.0;
	m_dbYMMPerPixel = 0.0;
	m_dbAngleCCDAndWorktable = 0.0;
	m_dbYMMPerPixel2 = 0.0;
	m_dbXMMPerPixel2 = 0.0;
	//}}AFX_DATA_INIT
}


void CCCDAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCCDAdjustDlg)
	DDX_Text(pDX, IDC_EDIT_XMM_PER_PULSE, m_dbXMMPerPixel);
	DDX_Text(pDX, IDC_EDIT_YMM_PER_PULS, m_dbYMMPerPixel);
	DDX_Text(pDX, IDC_EDIT_INCLINATION, m_dbAngleCCDAndWorktable);
	DDX_Text(pDX, IDC_EDIT_YMM_PER_PIXEL2, m_dbYMMPerPixel2);
	DDX_Text(pDX, IDC_EDIT_XMM_PER_PIXEL2, m_dbXMMPerPixel2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCCDAdjustDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CCCDAdjustDlg)
	ON_BN_CLICKED(IDC_BUTTON_COUNT_RATIO, OnButtonCountRatio)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_COUNT_RATIO2, OnButtonCountRatio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCDAdjustDlg message handlers


BOOL CCCDAdjustDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here	
	LoadData();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCCDAdjustDlg::CountRatio(double &dbXRatio, double &dbYRatio, double &dbXAngle, double dbMove)
{
	UpdateData(TRUE);
	AfxGetApp()->BeginWaitCursor();
	
	double dbCCDOx,dbCCDOy,dbCCDXx,dbCCDXy,dbCCDYx,dbCCDYy;	//在CCD中的坐标   3,4,2
	double dbTableOx,dbTableOy,dbTableXx,dbTableXy,dbTableYx,dbTableYy; //在工作台上的坐标

	CString str;
	double dbcurx,dbcury,dbcurz,dbDegree;
	dbcurx = g_pMotion->GetPos(eMaskX);
	dbcury = g_pMotion->GetPos(eMaskY);  
	dbcurz = g_pMotion->GetPos(eMaskZ);
	
	//Y点
	g_pMotion->LnXY((dbcurx-dbMove),(dbcury+dbMove));
	Sleep(1000);

	if (g_ccd.CCDLocate())
	{
		::g_ccd.GetTrigOK(dbCCDYx,dbCCDYy,dbDegree);
		::g_ccd.GetWorkPos(dbTableYx,dbTableYy);
		OutputDebugString("Y点");
	}
	else
	{
		AfxGetApp()->EndWaitCursor();
		AfxMessageBox("CCD Catch Fail");
		return;
	}
	
	//原点
	g_pMotion->LnXY((dbcurx-dbMove),(dbcury-dbMove));
	Sleep(1000);
	if (g_ccd.CCDLocate())
	{
		::g_ccd.GetTrigOK(dbCCDOx,dbCCDOy,dbDegree);
		::g_ccd.GetWorkPos(dbTableOx,dbTableOy);
		OutputDebugString("原点");
	}
	else
	{
		AfxGetApp()->EndWaitCursor();
		AfxMessageBox("CCD Catch Fail");
		return;
	}
	
	//X点
	g_pMotion->LnXY((dbcurx+dbMove),(dbcury-dbMove));
	Sleep(1000);
	if (g_ccd.CCDLocate())
	{
		::g_ccd.GetTrigOK(dbCCDXx,dbCCDXy,dbDegree);
		::g_ccd.GetWorkPos(dbTableXx,dbTableXy);
		OutputDebugString("X点");
	}
	else
	{
		AfxGetApp()->EndWaitCursor();
		AfxMessageBox("CCD Catch Fail");
		return;
	}
	
	//缩放比例
	double length1,length2;
	
	length1=sqrt((dbTableXx-dbTableOx)*(dbTableXx-dbTableOx) + (dbTableXy-dbTableOy)*(dbTableXy-dbTableOy) );
	length2=sqrt((dbCCDXx-dbCCDOx)*(dbCCDXx-dbCCDOx) + (dbCCDXy-dbCCDOy)*(dbCCDXy-dbCCDOy) );
	dbXRatio=length1/length2;//单位:mm/pixel
	dbXRatio=MyPrecision(dbXRatio,5);
	
	length1=sqrt((dbTableYx-dbTableOx)*(dbTableYx-dbTableOx) + (dbTableYy-dbTableOy)*(dbTableYy-dbTableOy) );
	length2=sqrt((dbCCDYx-dbCCDOx)*(dbCCDYx-dbCCDOx) + (dbCCDYy-dbCCDOy)*(dbCCDYy-dbCCDOy) );
	dbYRatio=length1/length2;//单位:mm/pixel
	dbYRatio=MyPrecision(dbYRatio,5);
	
	
	//“CCD的x轴”相对于“工作台的x轴”旋转的角度
	double xMM,yMM;	
	xMM=(dbTableXx-dbTableOx);
	yMM=(dbCCDXy-dbCCDOy)*dbYRatio;
	dbXAngle=atan(yMM/xMM);
	dbXAngle=MyPrecision(dbXAngle,5);
	/////////////////////////////////////
	
	UpdateData(FALSE);
	AfxGetApp()->EndWaitCursor();
	return ; 
}

void CCCDAdjustDlg::OnButtonCountRatio() 
{
	// TODO: Add your control notification handler code here
	double dbAngle;
	CountRatio(m_dbXMMPerPixel, m_dbYMMPerPixel, dbAngle, 0.2);

}


void CCCDAdjustDlg::OnButtonCountRatio2() 
{
	// TODO: Add your control notification handler code here
	CountRatio(m_dbXMMPerPixel2, m_dbYMMPerPixel2, m_dbAngleCCDAndWorktable);
}


void CCCDAdjustDlg::LoadData()
{
	// 比例系数及夹角
	m_dbXMMPerPixel	= g_ParaIni.ReadDouble("CCDPARA","XMMPPULSE",0.0225);
	m_dbYMMPerPixel	= g_ParaIni.ReadDouble("CCDPARA","YMMPPULSE",0.0226); 
	m_dbXMMPerPixel2 = g_ParaIni.ReadDouble("CCDPARA","XMMPPULSE2",0.0225);
	m_dbYMMPerPixel2 = g_ParaIni.ReadDouble("CCDPARA","YMMPPULSE2",0.0226);
	m_dbAngleCCDAndWorktable = g_ParaIni.ReadDouble("CCDPARA","INCLINATION",0.0);
}

void CCCDAdjustDlg::SaveData()
{	
	g_ParaIni.m_dbXMMPerPixel = m_dbXMMPerPixel;
	g_ParaIni.m_dbYMMPerPixel  = m_dbYMMPerPixel;
	g_ParaIni.m_dbXMMPerPixel2 = m_dbXMMPerPixel2;
	g_ParaIni.m_dbYMMPerPixel2 = m_dbYMMPerPixel2;
	g_ParaIni.SavePara();
}

void CCCDAdjustDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	SaveData();
}
