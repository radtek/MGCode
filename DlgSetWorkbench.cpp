// DlgSetWorkbench.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgSetWorkbench.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetWorkbench dialog
double CDlgSetWorkbench::m_dbCrossInMarkPosX=0;
double CDlgSetWorkbench::m_dbCrossInMarkPosY=0;

CDlgSetWorkbench::CDlgSetWorkbench(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetWorkbench::IDD, pParent)
	, m_dbLeftPtX(0)
	, m_dbLeftPtY(0)
	, m_dbRightPtX(0)
	, m_dbRightPtY(0)
	, m_dbLoadDis(10)
	, m_dbUnLoadDis(10)
	, m_bMove(FALSE)
	, m_bPosTicps(FALSE)
	, m_dbWorkbenchFixtureAngle(0)
	, m_dbCloseDoorTime(0)
	, m_bDocProGress(FALSE)
	,m_bUseCutBlow(TRUE)
	, m_bUseSafeDoor(FALSE)
	, m_dbWorkPosX(0)
	, m_dbWorkPosY(0)
{
	//{{AFX_DATA_INIT(CDlgSetWorkbench)
	m_dbMarkToCCD1X = 0.0;
	m_dbMarkToCCD1Y = 0.0;
	m_dbMarkToCCD2X = 0.0;
	m_dbMarkToCCD2Y = 0.0;
	m_dbRefPointX = 0.0;
	m_dbRefPointY = 0.0;
	m_dbLoadPosX = 0.0;
	m_dbLoadPosY = 0.0;
	m_dbUnloadPosX = 0.0;
	m_dbUnloadPosY = 0.0;
	m_dbFastVel = 0.0;
	m_dbSlowVel = 0.0;
	m_nMotionDelayForCCD = 0;
	m_nMotionDelayForMark = 0;
	m_dbPowerCheckX = 0.0;
	m_dbPowerCheckY = 0.0;
	m_dbPowerCheckZ = 0.0;
	m_dbAutoOffsetDeviationMax = 0.0;
	m_dbAutoOffsetDeviationMin = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgSetWorkbench::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetWorkbench)
	DDX_Text(pDX, IDC_EDIT_MARKTOCCD1_X, m_dbMarkToCCD1X);
	DDX_Text(pDX, IDC_EDIT_MARKTOCCD1_Y, m_dbMarkToCCD1Y);
	DDX_Text(pDX, IDC_EDIT_MARKTOCCD2_X, m_dbMarkToCCD2X);
	DDX_Text(pDX, IDC_EDIT_MARKTOCCD2_Y, m_dbMarkToCCD2Y);
	DDX_Text(pDX, IDC_EDIT_RP_X, m_dbRefPointX);
	DDX_Text(pDX, IDC_EDIT_RP_Y, m_dbRefPointY);
	DDX_Text(pDX, IDC_EDIT_LOADPOSX, m_dbLoadPosX);
	DDX_Text(pDX, IDC_EDIT_LOADPOSY, m_dbLoadPosY);
	DDX_Text(pDX, IDC_EDIT_UNLOADPOSX, m_dbUnloadPosX);
	DDX_Text(pDX, IDC_EDIT_UNLOADPOSY, m_dbUnloadPosY);
	DDX_Text(pDX, IDC_EDIT_FAST_VEL, m_dbFastVel);
	DDX_Text(pDX, IDC_EDIT_SLOW_VEL, m_dbSlowVel);
	DDX_Text(pDX, IDC_EDIT_MOTION_DELAY_FOR_CCD, m_nMotionDelayForCCD);
	DDX_Text(pDX, IDC_EDIT_MOTION_DELAY_FOR_MARK, m_nMotionDelayForMark);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_LEFTPTX, m_dbLeftPtX);
	DDX_Text(pDX, IDC_EDIT_LEFTPTY, m_dbLeftPtY);
	DDX_Text(pDX, IDC_EDIT_RIGHTPTX, m_dbRightPtX);
	DDX_Text(pDX, IDC_EDIT_RIGHTPTY, m_dbRightPtY);
	DDX_Text(pDX, IDC_EDIT_LOADDIS, m_dbLoadDis);
	DDX_Text(pDX, IDC_EDIT_UNLOADDIS, m_dbUnLoadDis);
	DDX_Check(pDX, IDC_CHECK_MOVE, m_bMove);
	DDX_Check(pDX, IDC_CHECK_POSTICPS, m_bPosTicps);
	DDX_Text(pDX, IDC_EDIT_POWER_CHECK_X, m_dbPowerCheckX);
	DDX_Text(pDX, IDC_EDIT_POWER_CHECK_Y, m_dbPowerCheckY);
	DDX_Text(pDX, IDC_EDIT_POWER_CHECK_Z, m_dbPowerCheckZ);
	DDX_Text(pDX, IDC_EDIT_WORKBENCH_FIXTURE_ANGLE, m_dbWorkbenchFixtureAngle);
	DDX_Text(pDX, IDC_EDIT_DOOR_CLOSE_TIME, m_dbCloseDoorTime);
	DDX_Check(pDX, IDC_CHECK_DOCPROGRESS, m_bDocProGress);
	DDX_Check(pDX, IDC_CHECK_USE_CUTBLOW, m_bUseCutBlow);
	DDX_Check(pDX, IDC_CHECK_USE_SAFEDOOR, m_bUseSafeDoor);
	DDX_Text(pDX, IDC_EDIT_WORK_POS_X, m_dbWorkPosX);
	DDX_Text(pDX, IDC_EDIT_WORK_POS_Y, m_dbWorkPosY);
}


BEGIN_MESSAGE_MAP(CDlgSetWorkbench, CDialog)
	//{{AFX_MSG_MAP(CDlgSetWorkbench)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CALCUlATE_MARKTOCCD1, &CDlgSetWorkbench::OnBnClickedButtonCalculateMarktoccd1)
	ON_BN_CLICKED(IDC_BUTTON_GET_POS_AS_WORK_POS, &CDlgSetWorkbench::OnBnClickedButtonGetPosAsWorkPos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetWorkbench message handlers

BOOL CDlgSetWorkbench::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSetWorkbench::OnBnClickedButtonCalculateMarktoccd1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;

	m_dbMarkToCCD1X = m_dbCrossInCCDPosX - m_dbCrossInMarkPosX;
	m_dbMarkToCCD1Y = m_dbCrossInCCDPosY - m_dbCrossInMarkPosY;

	UpdateData(FALSE);
}


void CDlgSetWorkbench::OnBnClickedButtonGetPosAsWorkPos()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;

	m_dbWorkPosX = g_pMotion->GetPos(eMaskX);
	m_dbWorkPosY = g_pMotion->GetPos(eMaskY);

	UpdateData(FALSE);
}
