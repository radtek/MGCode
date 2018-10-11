// DlgAutoOffset.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgAutoOffset.h"
#include "CCDCalibrate.h"
#include "DlgMarkNormalPane.h"

#define TIMER_DLG_INI 2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoOffset dialog


CDlgAutoOffset::CDlgAutoOffset(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoOffset::IDD, pParent)
	, m_dbLfPos(0)
	, m_dbRtPos(0)
{
	//{{AFX_DATA_INIT(CDlgAutoOffset)
	m_dbHandMoveDis = 1.0;
	m_dbLastX = 0.0;
	m_dbLastY = 0.0;
	//}}AFX_DATA_INIT

	m_bInit = FALSE;
	m_left.SetOper(MOVE_LEFT);
	m_right.SetOper(MOVE_RIGHT);
	m_top.SetOper(MOVE_TOP);
	m_bottom.SetOper(MOVE_BOTTOM);

}


void CDlgAutoOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoOffset)
	DDX_Text(pDX, IDC_EDIT_LAST_X, m_dbLastX);
	DDX_Text(pDX, IDC_EDIT_LAST_Y, m_dbLastY);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_left);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_right);
	DDX_Control(pDX, IDC_STATIC_TOP, m_top);
	DDX_Control(pDX, IDC_STATIC_BOTTOM, m_bottom);
	DDX_Text(pDX, IDC_EDIT_HAND_MOVE_DIS, m_dbHandMoveDis);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_LFPOS, m_dbLfPos);
	DDX_Text(pDX, IDC_EDIT_RTPOS, m_dbRtPos);
}


BEGIN_MESSAGE_MAP(CDlgAutoOffset, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoOffset)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_HAND_MOVE_DIS, OnChangeEditHandMoveDis)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL, OnRadioMoveModel)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL1, OnRadioMoveModel1)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL2, OnRadioMoveModel2)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL3, OnRadioMoveModel3)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDOK, &CDlgAutoOffset::OnBnClickedOk)
//ON_BN_CLICKED(IDC_BUTTON1, &CDlgAutoOffset::OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoOffset message handlers

BOOL CDlgAutoOffset::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dbOldHandMoveDis = CFStatic::m_dbMoveDis;

	CFStatic::m_nMoveModel = 2;
	CheckDlgButton(IDC_RADIO_MOVE_MODEL2, TRUE);
	CFStatic::m_dbMoveDis = m_dbHandMoveDis = 1.0;

	m_dbLastX = g_ParaIni.m_dbAutoOffsetLastX;
	m_dbLastY = g_ParaIni.m_dbAutoOffsetLastY;
	UpdateData(FALSE);

	SetTimer(TIMER_DLG_INI, 10, NULL);


	
	g_pMotion->GetPara("XY", "AutoOfferRightPtX", m_dbRtPos);
	g_pMotion->GetPara("XY", "AutoOfferLeftPtX", m_dbLfPos);
	
	UpdateData(FALSE);

	WriteRunLog("\n\n\n", eLogDataFlow);
	CString strLog;
	strLog.Format("进入偏移校正，校正文档(%s)", m_pDoc->GetPathName());
	WriteRunLog(strLog, eLogDataFlow);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAutoOffset::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default


	if (nIDEvent==TIMER_DLG_INI)
	{
		KillTimer(TIMER_DLG_INI);

		CString strTip;
		g_pDlgMarkNormalPane->SetGas(TRUE);

		g_pMotion->LnXY(m_dbLastX, m_dbLastY);
		g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);
		m_bInit = TRUE;
		
		strTip.Format("上次标记位置(%.3f,%.3f), CCD焦点位置(%.3f)", m_dbLastX, m_dbLastY, m_pDoc->m_dbZCCDFocus);
		WriteRunLog(strTip, eLogDataFlow);
	}
		
	CDialog::OnTimer(nIDEvent);
}

void CDlgAutoOffset::OnChangeEditHandMoveDis() 
{
	CString strMoveDis;
	GetDlgItemText(IDC_EDIT_HAND_MOVE_DIS, strMoveDis);
	CFStatic::m_dbMoveDis = atof(strMoveDis);
	
	// TODO: Add your control notification handler code here
}

void CDlgAutoOffset::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(TRUE);

	if (!g_IO.INPUT_MACHINE_READY())
	{
		AfxMessageBox(_T("the table is not ready"));
		return;
	}

	if (AfxMessageBox("Will display the location of the marker in the CCD! \n please confirm 'tag' is correct？", MB_OKCANCEL)==IDCANCEL)
		return;

	g_pDlgMarkNormalPane->UpdateMotionView(FALSE);
	
	g_ParaIni.m_dbAutoOffsetLastX = g_pMotion->GetPos(eMaskX);
	g_ParaIni.m_dbAutoOffsetLastY = g_pMotion->GetPos(eMaskY);
	g_ParaIni.SavePara();

	CString strTip;
	strTip.Format("Start offset correction, marking the location(%.3f,%.3f), the old 'marking center distance into the center of the CCD'：(%.3f,%.3f)", 
		g_ParaIni.m_dbAutoOffsetLastX, g_ParaIni.m_dbAutoOffsetLastY,  g_ParaIni.m_dbMarkToCCD1X, g_ParaIni.m_dbMarkToCCD1Y);
	WriteRunLog(strTip, eLogDebug);
	
	double dbZ = m_pDoc->GetFirstMarkFocus();

	g_pMotion->AMove(eMaskZ, dbZ);


	double CurX = g_pMotion->GetPos(eMaskX);
	double CurY = g_pMotion->GetPos(eMaskY);

	if (CurX-g_ParaIni.m_dbMarkToCCD1X > m_dbRtPos)
	{
		AfxMessageBox("Beyond Moving Limit");
		g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
		return;
	}

	g_pMotion->LnXY(CurX-g_ParaIni.m_dbMarkToCCD1X,CurY -g_ParaIni.m_dbMarkToCCD1Y);
	Sleep(g_ParaIni.m_nMotionDelayForMark+3000);
	
	double dbMarkCrossPosX = g_pMotion->GetPos(eMaskX);
	double dbMarkCrossPosY = g_pMotion->GetPos(eMaskY);


	strTip.Format("打标(%.3f,%.3f)", dbMarkCrossPosX, dbMarkCrossPosY);
	WriteRunLog(strTip, eLogDebug);

	PARA curMarkPara;
	//if (g_sys.m_nLaserCurrentMode)	
	{
		curMarkPara = m_pDoc->GetLay().GetPara(0);
		if(m_oldMarkPara.dbCurrent != curMarkPara.dbCurrent)
		{		
			if (!g_pDlgMarkNormalPane->SetCurrent(curMarkPara.dbCurrent)) 
				return ;
			m_oldMarkPara.dbCurrent = curMarkPara.dbCurrent;
		}
	}
	if (g_sys.m_bUseOpcLaser)
	{
		BOOL bRet = g_OPCLaser->WriteFreq((int)curMarkPara.dbQFreq);
		if (bRet)
		{
			g_nFreq = curMarkPara.dbQFreq;
			WriteRunLog("g_OPCUaClient->writeFreqcurMarkPara.dbQFreq", eLogDebug);
		}
		else
		{
			WriteRunLog("g_OPCUaClient->writeFreqcur Fail", eLogDebug);
		}
	}

	


	g_DAT.AutoCal();

	g_IO.OUTPUT_SOFT_OK(FALSE);

	g_IO.OUTPUT_CLOSE_DOOR(TRUE);
	Sleep(g_ParaIni.m_nCloseDoorTime);
	g_IO.OUTPUT_CLOSE_DOOR(FALSE);

	
	/*标记十字*/
	CStrokeList strokeList;
	CQuad quad(CDot(0,0), 10, 10);
	m_pDoc->ResetDivideFlag();
	g_DAT.SetMarkPara(m_pDoc->GetLay());
	m_pDoc->m_chain.GetStrokeInQuad(quad, strokeList.m_list, FALSE);
	strokeList.MarkNormal();

	if (g_sys.m_nLaserCurrentKeep)//维持电流
	{
		g_pDlgMarkNormalPane->SetCurrent(g_sys.m_nLaserCurrentKeep);
		m_oldMarkPara.dbCurrent = g_sys.m_nLaserCurrentKeep;
	}

	g_IO.OUTPUT_SOFT_OK(TRUE);
	Sleep(1000);
	
	if (m_pDoc->m_bEnableMarkFocus)
		g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);

	CurX = g_pMotion->GetPos(eMaskX);
	CurY = g_pMotion->GetPos(eMaskY);

	
	if (CurX+g_ParaIni.m_dbMarkToCCD1X < m_dbLfPos)
	{
		AfxMessageBox("Beyond Moving Limit");
		g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
		return;
	}
	g_pMotion->LnXY(CurX+g_ParaIni.m_dbMarkToCCD1X,CurY +g_ParaIni.m_dbMarkToCCD1Y);
	Sleep(g_ParaIni.m_nMotionDelayForCCD);


	double dbMarkCrossPosX1 = g_pMotion->GetPos(eMaskX);
	double dbMarkCrossPosY1 = g_pMotion->GetPos(eMaskY);


	strTip.Format("CCD(%.3f,%.3f)", dbMarkCrossPosX1, dbMarkCrossPosY1);
	WriteRunLog(strTip, eLogDebug);
	
	
	g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
	////移动到CCD中心	
	double dbCaptureX = dbMarkCrossPosX+g_ParaIni.m_dbMarkToCCD1X;
	double dbCaptureY = dbMarkCrossPosY+g_ParaIni.m_dbMarkToCCD1Y;
	if(!m_pDoc->PointCCD1Capture(dbCaptureX, dbCaptureY,TRUE))
	{
		WriteRunLog("移动十字至CCD中心失败!",eLogDataFlow);
		g_IO.OUTPUT_SOFT_OK(TRUE);
		return;
	}
		
	double dbNewMarkToCCD1X = dbCaptureX - dbMarkCrossPosX;
	double dbNewMarkToCCD1Y = dbCaptureY - dbMarkCrossPosY;


	double dbMarkCrossPosX2 = dbCaptureX;
	double dbMarkCrossPosY2 = dbCaptureY;


	strTip.Format("CCD(%.3f,%.3f)", dbMarkCrossPosX2, dbMarkCrossPosY2);
	WriteRunLog(strTip, eLogDebug);
	
	strTip.Format("当前测量的“打标中心到CCD中心距离”：(%.3f,%.3f)", dbNewMarkToCCD1X, dbNewMarkToCCD1Y);
	WriteRunLog(strTip, eLogDebug);
	
	BOOL bDeviationOK = TRUE;
	double dbDeviationX = dbNewMarkToCCD1X-g_ParaIni.m_dbMarkToCCD1X;
	double dbDeviationY = dbNewMarkToCCD1Y-g_ParaIni.m_dbMarkToCCD1Y;
	if (dbDeviationX<g_ParaIni.m_dbAutoOffsetDeviationMin || dbDeviationX>g_ParaIni.m_dbAutoOffsetDeviationMax)
		bDeviationOK = FALSE;
	if (dbDeviationY<g_ParaIni.m_dbAutoOffsetDeviationMin || dbDeviationY>g_ParaIni.m_dbAutoOffsetDeviationMax)
		bDeviationOK = FALSE;

	if (bDeviationOK)
	{
		strTip.Format("偏移变化(%.1fμm,%.1fμm)，确认使用？", (dbNewMarkToCCD1X-g_ParaIni.m_dbMarkToCCD1X)*1000, (dbNewMarkToCCD1Y-g_ParaIni.m_dbMarkToCCD1Y)*1000);
		WriteRunLog(strTip,eLogDataFlow);
		if (AfxMessageBox(strTip, MB_OKCANCEL)==IDCANCEL)
			return;
	}
	else
	{
		strTip.Format("偏移变化(%.1fμm,%.1fμm)，超出允许校正范围(%.0fμm,%.0fμm)!\n请重新校正,或更改允许校正范围!", 
			1000*(dbNewMarkToCCD1X-g_ParaIni.m_dbMarkToCCD1X), 1000*(dbNewMarkToCCD1Y-g_ParaIni.m_dbMarkToCCD1Y), 
			1000*(g_ParaIni.m_dbAutoOffsetDeviationMin), 1000*(g_ParaIni.m_dbAutoOffsetDeviationMax));
		WriteRunLog(strTip, eLogDataFlow);
		AfxMessageBox(strTip);
		return;
	}


	
	g_ParaIni.m_dbMarkToCCD1X = dbNewMarkToCCD1X;
	g_ParaIni.m_dbMarkToCCD1Y = dbNewMarkToCCD1Y;
	g_ParaIni.SavePara();
	
	strTip.Format("“打标中心到CCD中心距离”已更新：(%.3f,%.3f)", g_ParaIni.m_dbMarkToCCD1X, g_ParaIni.m_dbMarkToCCD1Y);
	WriteRunLog(strTip, eLogDataFlow);
}


void CDlgAutoOffset::OnDestroy() 
{
	CDialog::OnDestroy();

	CFStatic::m_nMoveModel = m_nOldMoveModel;
	CFStatic::m_dbMoveDis = m_dbOldHandMoveDis;
	
	if (m_bInit)
	{
		g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);	
		
		CString strProgID = (m_pDoc->m_nCCDUseModel==2) ? m_pDoc->m_strProgID2 : m_pDoc->m_strProgID1;
		if (!g_ccd.SetWorkProgNo(atoi(strProgID)))
			AfxMessageBox(" Exchange CCD Fail!");	
	}
	UpdateData(TRUE);

	g_pMotion->WritePara("XY", "AutoOfferRightPtX", m_dbRtPos);
	g_pMotion->WritePara("XY", "AutoOfferLeftPtX", m_dbLfPos);
}

void CDlgAutoOffset::OnRadioMoveModel() 
{
	// TODO: Add your control notification handler code here
	CFStatic::m_nMoveModel = 0;
}

void CDlgAutoOffset::OnRadioMoveModel1() 
{
	// TODO: Add your control notification handler code here
	CFStatic::m_nMoveModel = 1;
}

void CDlgAutoOffset::OnRadioMoveModel2() 
{
	// TODO: Add your control notification handler code here
	CFStatic::m_nMoveModel = 2;
}

void CDlgAutoOffset::OnRadioMoveModel3() 
{
	// TODO: Add your control notification handler code here
	CFStatic::m_nMoveModel = 3;
}