// DlgCheckPower.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgCheckPower.h"
#include "DlgMarkNormalPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCheckPower dialog


CDlgCheckPower::CDlgCheckPower(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCheckPower::IDD, pParent)
	, m_bTime(FALSE)
	, m_dbLfPos(0)
	, m_dbRtPos(0)
{
	//{{AFX_DATA_INIT(CDlgCheckPower)
	m_nThermaTrack = 0;
	m_nThermaTrackAdd = 5;
	//}}AFX_DATA_INIT
	m_dbCurPower = 0;
	m_pPowerMeter = NULL;
}


void CDlgCheckPower::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCheckPower)
	DDX_Text(pDX, IDC_EDIT_THERMA_TRACK, m_nThermaTrack);
	DDX_Text(pDX, IDC_EDIT_THERMA_TRACK_ADD, m_nThermaTrackAdd);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_LFPOS, m_dbLfPos);
	DDX_Text(pDX, IDC_EDIT_RTPOS, m_dbRtPos);
}


BEGIN_MESSAGE_MAP(CDlgCheckPower, CDialog)
	//{{AFX_MSG_MAP(CDlgCheckPower)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_CHECK, OnButtonStartCheck)
	ON_BN_CLICKED(IDC_BUTTON_TH_ADD, OnButtonThAdd)
	ON_BN_CLICKED(IDC_BUTTON_TH_MINUS, OnButtonThMinus)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_QUIT_CHECK, &CDlgCheckPower::OnBnClickedButtonQuitCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCheckPower message handlers

BOOL CDlgCheckPower::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strPower;
	strPower.Format("%.3f", m_dbTestPowerFrequency);
	SetDlgItemText(IDC_EDIT_PRODUCT_POWER, strPower);
	strPower.Format("%.3f", m_dbProductPowerMax);
	SetDlgItemText(IDC_EDIT_PRODUCT_POWER_MAX, strPower);
	strPower.Format("%.3f", m_dbProductPowerMin);
	SetDlgItemText(IDC_EDIT_PRODUCT_POWER_MIN, strPower);
	g_pMotion->GetPara("XY", "AutoOfferRightPtX", m_dbRtPos);
	g_pMotion->GetPara("XY", "AutoOfferLeftPtX", m_dbLfPos);
	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT_CHECK)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCheckPower::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pPowerMeter->QueryPower(m_dbCurPower))
	{
		double dPowerMeter;
		g_pMotion->GetPara("XY", "POWERMETER", dPowerMeter);
		if (dPowerMeter == 0)
		{
			dPowerMeter = 1;
			g_pMotion->WritePara("XY", "POWERMETER", dPowerMeter);
		}
		m_dbCurPower *= dPowerMeter;
		CString strCurPower;
		strCurPower.Format("%.3f", m_dbCurPower);
		SetDlgItemText(IDC_EDIT_CUR_POWER, strCurPower);

		if (::WaitForSingleObject(g_hEventMainThreadEmergency,0)==WAIT_OBJECT_0)
		{
			g_DAT.BeamOff();
			g_IO.OUTPUT_SOFT_OK(TRUE);
			g_IO.OUTPUT_CLOSE_DOOR(FALSE);
			KillTimer(1);
			m_bTime = FALSE;
			SetDlgItemText(IDC_EDIT_CUR_POWER, "");

			m_pPowerMeter->Close();
			GetDlgItem(IDC_BUTTON_START_CHECK)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_QUIT_CHECK)->EnableWindow(FALSE);
		}
	}
	else
	{
		g_DAT.BeamOff();
		g_IO.OUTPUT_SOFT_OK(TRUE);
		g_IO.OUTPUT_CLOSE_DOOR(FALSE);
		KillTimer(1);
		m_bTime = FALSE;
		SetDlgItemText(IDC_EDIT_CUR_POWER, "");

		m_pPowerMeter->Close();
		GetDlgItem(IDC_BUTTON_START_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT_CHECK)->EnableWindow(FALSE);
		AfxMessageBox("Power meter query failed!");
	}	
	CDialog::OnTimer(nIDEvent);
}

void CDlgCheckPower::OnButtonStartCheck() 
{
	// TODO: Add your control notification handler code here
	if (!g_IO.INPUT_MACHINE_READY())
	{
		AfxMessageBox(_T("the table is not ready"));
		return;
	}

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (pFWnd==NULL) 
		return ;
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return ;
	}
	CHLDoc *pDoc = pView->GetDocument();
	if (NULL == pDoc)
	{
		return ;
	}
	g_pDlgMarkNormalPane->SetFocus();//使参数变更生效
	g_DAT.SetMarkPara(pDoc->GetLay());

	
	PARA curMarkPara;
	//if (g_sys.m_nLaserCurrentMode)
	{
		curMarkPara = pDoc->GetLay().GetPara(0);
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
			g_nFreq = (int)curMarkPara.dbQFreq;
			WriteRunLog("g_OPCUaClient->writeFreqcurMarkPara.dbQFreq");
		}
		else
		{
			WriteRunLog("g_OPCUaClient->writeFreqcur Fail");
		}
	}


	//运动到检测点

	UpdateData(TRUE);
	if (g_ParaIni.m_dbPowerCheckX < m_dbLfPos || g_ParaIni.m_dbPowerCheckX > m_dbRtPos)
	{
		AfxMessageBox("Beyond Moving Limit");
		return;
	}
	
	g_pDlgMarkNormalPane->UpdateMotionView(FALSE);

	g_pMotion->AMove(eMaskZ, g_ParaIni.m_dbPowerCheckZ);
	g_pMotion->LnXY(g_ParaIni.m_dbPowerCheckX, g_ParaIni.m_dbPowerCheckY);

	if (AfxMessageBox("Will Test power  in the current location ", MB_OKCANCEL)==IDCANCEL)
		return;
	
	//打开功率计
	if (g_sys.m_nPowerMeterType==0)
		m_pPowerMeter = new CCoherentPowerMeter();
	else
		m_pPowerMeter = new CHansPowerMeter();

	if (!m_pPowerMeter->Open(g_sys.m_nPowerMeterPort))
	{
		g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
		AfxMessageBox(m_pPowerMeter->m_strErrInfo);
		return;
	}
		
	m_pPowerMeter->Close();

	g_IO.OUTPUT_SOFT_OK(FALSE);
	g_IO.OUTPUT_CLOSE_DOOR(TRUE);
	Sleep(g_ParaIni.m_nCloseDoorTime);

	g_DAT.BeamOn();
	GetDlgItem(IDC_BUTTON_START_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT_CHECK)->EnableWindow(TRUE);
	SetTimer(1, 200, NULL);
	m_bTime = TRUE;

}

void CDlgCheckPower::OnButtonThAdd() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData())
		return;

	CString strTip;
	int nTH = m_nThermaTrack + m_nThermaTrackAdd;	
	if (m_pLaserController->SetThermaTrack(nTH))
	{
		m_nThermaTrack = nTH;
		UpdateData(FALSE);
	}
}

void CDlgCheckPower::OnButtonThMinus() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData())
		return;

	int nTH = m_nThermaTrack - m_nThermaTrackAdd;
	if (m_pLaserController->SetThermaTrack(nTH))
	{
		m_nThermaTrack = nTH;
		UpdateData(FALSE);
	}
}

void CDlgCheckPower::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString strTip;
	WriteRunLog(m_strDocPath);
	strTip.Format("功率检测: 检测后功率(%.3f)", m_dbCurPower);
	WriteRunLog(strTip);

	if (m_bTime)
	{
		KillTimer(1);
		SetDlgItemText(IDC_EDIT_CUR_POWER, "");
		GetDlgItem(IDC_BUTTON_START_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT_CHECK)->EnableWindow(FALSE);
		m_pPowerMeter->Close();
		m_bTime = FALSE;
	}
	g_DAT.BeamOff();

	g_IO.OUTPUT_SOFT_OK(TRUE);
	g_IO.OUTPUT_CLOSE_DOOR(FALSE);
	
	if (m_pPowerMeter)
	{
		delete m_pPowerMeter;
		m_pPowerMeter = NULL;
	}


	if (g_sys.m_nLaserCurrentKeep)//维持电流
	{
		g_pDlgMarkNormalPane->SetCurrent(g_sys.m_nLaserCurrentKeep);
		m_oldMarkPara.dbCurrent = g_sys.m_nLaserCurrentKeep;
	}
	g_pDlgMarkNormalPane->UpdateMotionView(TRUE);


	UpdateData(TRUE);
	g_pMotion->WritePara("XY", "AutoOfferRightPtX", m_dbRtPos);
	g_pMotion->WritePara("XY", "AutoOfferLeftPtX", m_dbLfPos);

	CDialog::OnClose();
}


void CDlgCheckPower::OnBnClickedButtonQuitCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	g_DAT.BeamOff();
	g_IO.OUTPUT_SOFT_OK(FALSE);
	g_IO.OUTPUT_CLOSE_DOOR(TRUE);

	KillTimer(1);
	m_bTime = FALSE;
	SetDlgItemText(IDC_EDIT_CUR_POWER, "");

	m_pPowerMeter->Close();
	GetDlgItem(IDC_BUTTON_START_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUIT_CHECK)->EnableWindow(FALSE);
	g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
}
