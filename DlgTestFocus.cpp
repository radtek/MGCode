// DlgTestFocus.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgTestFocus.h"
#include <math.h>
#include "DlgMarkNormalPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTestFocus dialog

#define TIMEMARK 110

CDlgTestFocus::CDlgTestFocus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestFocus::IDD, pParent)
	, m_nLay(0)
	, m_dbHandMoveDis(0)
{
	//{{AFX_DATA_INIT(CDlgTestFocus)
	m_dbZStart = 0.0;
	m_dbZStep = 0.0;
	m_dbZStepNum = 0.0;
	m_dbXDis = 0.0;
	m_dbYDis = 0.0;

	m_up.SetOper(MOVE_UP);
	m_Down.SetOper(MOVE_DOWN);
	m_bTime = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTestFocus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTestFocus)
	DDX_Text(pDX, IDC_EDIT_Z_START, m_dbZStart);
	DDX_Text(pDX, IDC_EDIT_Z_STEP, m_dbZStep);
	DDX_Text(pDX, IDC_EDIT_Z_STEP_NUM, m_dbZStepNum);
	DDX_Text(pDX, IDC_EDIT_X_DIS, m_dbXDis);
	DDX_Text(pDX, IDC_EDIT_Y_DIS, m_dbYDis);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_UP, m_up);
	DDX_Control(pDX, IDC_STATIC_DOWN, m_Down);
}


BEGIN_MESSAGE_MAP(CDlgTestFocus, CDialog)
	//{{AFX_MSG_MAP(CDlgTestFocus)
	ON_BN_CLICKED(IDC_BUTTON_MARK, OnButtonMark)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_TEST, &CDlgTestFocus::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL, &CDlgTestFocus::OnBnClickedRadioMoveModel)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL1, &CDlgTestFocus::OnBnClickedRadioMoveModel1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_STOP, &CDlgTestFocus::OnBnClickedBtnStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTestFocus message handlers

BOOL CDlgTestFocus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CHLApp *pApp = (CHLApp *)AfxGetApp();
	m_dbZStart   = pApp->GetProfileDouble("DlgTestFocus", "ZStart", 0);
	m_dbZStep    = pApp->GetProfileDouble("DlgTestFocus", "ZStep", 0.1);
	m_dbZStepNum = pApp->GetProfileDouble("DlgTestFocus", "ZStepNum", 9);
	m_dbXDis    = pApp->GetProfileDouble("DlgTestFocus", "XDis", 5);
	m_dbYDis    = pApp->GetProfileDouble("DlgTestFocus", "YDis", 5);

	CheckDlgButton(IDC_RADIO_MOVE_MODEL, TRUE);
	//m_nOldMoveModel = CFStatic::m_nMoveModel;
	CFStatic::m_nMoveModel = 4;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTestFocus::OnButtonMark() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData())
		return;

	if (m_dbZStepNum<=1 )
	{
		AfxMessageBox("参数错误!");
		return;
	}

	CHLApp *pApp = (CHLApp *)AfxGetApp();
	pApp->WriteProfileDouble("DlgTestFocus", "ZStart", m_dbZStart);
	pApp->WriteProfileDouble("DlgTestFocus", "ZStep", m_dbZStep);
	pApp->WriteProfileDouble("DlgTestFocus", "ZStepNum", m_dbZStepNum);
	pApp->WriteProfileDouble("DlgTestFocus", "XDis", m_dbXDis);
	pApp->WriteProfileDouble("DlgTestFocus", "YDis", m_dbYDis);

	double dbX, dbXStart = g_pMotion->GetPos(eMaskX);
	double dbY, dbYStart = g_pMotion->GetPos(eMaskY);
	double dbZ;
	ResetEvent(hStop);
	//if (g_sys.m_nLaserCurrentMode)	
	{
		PARA curMarkPara = m_pDoc->GetLay().GetPara(0);
		if(m_oldMarkPara.dbCurrent != curMarkPara.dbCurrent)
		{		
			if (!g_pDlgMarkNormalPane->SetCurrent(curMarkPara.dbCurrent)) 
				return ;
			m_oldMarkPara.dbCurrent = curMarkPara.dbCurrent;
		}
	}

	g_DAT.AutoCal();
	g_IO.OUTPUT_SOFT_OK(FALSE);
	g_IO.OUTPUT_CLOSE_DOOR(TRUE);
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);

	for (int i=0; i<m_dbZStepNum; i++)
	{
		   if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
		   {
			     WriteRunLog("人工中止打标!");
			      break;
		   }
			dbX = dbXStart + i*m_dbXDis;
			dbY = dbYStart + i*m_dbYDis;
			dbZ = m_dbZStart + i*m_dbZStep;
			g_pMotion->AMove(eMaskZ, dbZ);
			g_pMotion->AMove(eMaskX, dbX);
			g_pMotion->AMove(eMaskY, dbY);
			g_pMotion->WaitMoveFinish(eMaskXYZ);
			Sleep(g_ParaIni.m_nMotionDelayForMark);
	
			CStrokeList strokeList;
			CQuad quad(CDot(0,0), 10, 10);
			m_pDoc->ResetDivideFlag();
			g_DAT.SetMarkPara(m_pDoc->GetLay());
			m_pDoc->m_chain.GetStrokeInQuad(quad, strokeList.m_list, FALSE);
			strokeList.MarkNormal();
	}
	GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(TRUE);
	g_IO.OUTPUT_CLOSE_DOOR(FALSE);
	g_IO.OUTPUT_SOFT_OK(TRUE);
}



void CDlgTestFocus::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;

	ResetEvent(hStop);

	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
	//if (g_sys.m_nLaserCurrentMode)	
	{
		PARA curMarkPara = m_pDoc->GetLay().GetPara(0);
		if(m_oldMarkPara.dbCurrent != curMarkPara.dbCurrent)
		{		
			if (!g_pDlgMarkNormalPane->SetCurrent(curMarkPara.dbCurrent)) 
				return ;
			m_oldMarkPara.dbCurrent = curMarkPara.dbCurrent;
		}
	}
	g_DAT.AutoCal();
	g_IO.OUTPUT_SOFT_OK(FALSE);
	g_IO.OUTPUT_CLOSE_DOOR(TRUE);


	SetTimer(TIMEMARK,1000,NULL);
	m_bTime = TRUE;
}


void CDlgTestFocus::OnBnClickedRadioMoveModel()
{
	// TODO: 在此添加控件通知处理程序代码
	CFStatic::m_nMoveModel = 4;
}



void CDlgTestFocus::OnBnClickedRadioMoveModel1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFStatic::m_nMoveModel = 1;
}


void CDlgTestFocus::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFStatic::m_nMoveModel = m_nOldMoveModel;

	if (g_sys.m_nLaserCurrentKeep)//维持电流
	{
		g_pDlgMarkNormalPane->SetCurrent(g_sys.m_nLaserCurrentKeep);
		m_oldMarkPara.dbCurrent = g_sys.m_nLaserCurrentKeep;
	}
	g_pDlgMarkNormalPane->OnBnClickedButtonStop();
	g_pDlgMarkNormalPane->UpdateData(FALSE);
	if (m_bTime)
	{
		KillTimer(TIMEMARK);
		m_bTime = FALSE;
	}

	GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(TRUE);
	g_IO.OUTPUT_CLOSE_DOOR(FALSE);
	g_IO.OUTPUT_SOFT_OK(TRUE);
	CDialog::OnClose();
}


void CDlgTestFocus::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pDlgMarkNormalPane->OnBnClickedButtonStop();

	GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(TRUE);
	g_IO.OUTPUT_CLOSE_DOOR(FALSE);
	g_IO.OUTPUT_SOFT_OK(TRUE);
}


void CDlgTestFocus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIMEMARK)
	{
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
		{
			WriteRunLog("人工中止打标!");
			KillTimer(TIMEMARK);
			m_bTime = FALSE;
			return;
		}
		CStrokeList strokeList;
		CQuad quad(CDot(0,0), 10, 10);
		m_pDoc->ResetDivideFlag();
		g_DAT.SetMarkPara(m_pDoc->GetLay());
		m_pDoc->m_chain.GetStrokeInQuad(quad, strokeList.m_list, FALSE);
		strokeList.MarkNormal();
	}
	CDialog::OnTimer(nIDEvent);
}
