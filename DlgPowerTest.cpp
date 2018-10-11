// DlgPowerTest.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgPowerTest.h"
#include "afxdialogex.h"
#include "DlgMarkNormalPane.h"

#define TEST_STEP_IDLE			0		
#define TEST_STEP_START			1
#define TEST_STEP_MARK_DOING	2
#define TEST_STEP_POWER_START	3
#define TEST_STEP_POWER_DOING	4

// CDlgPowerTest 对话框

IMPLEMENT_DYNAMIC(CDlgPowerTest, CDialogEx)

CDlgPowerTest::CDlgPowerTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPowerTest::IDD, pParent)
{

	m_dbTotalTime = 1.0;
	m_nMarkInterval = 0;
	m_dbPowerCheckInterval = 1.0;
	m_nPowerCheckWait = 1;
	m_strRunTime = _T("");
	m_nPowerCheckCount = 0;
	m_nCurStep = TEST_STEP_IDLE;
	m_dbLastPower = 0;
}

CDlgPowerTest::~CDlgPowerTest()
{
}

void CDlgPowerTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOTAL_TIME, m_dbTotalTime);
	DDX_Text(pDX, IDC_EDIT_MARK_INTERVAL, m_nMarkInterval);
	DDX_Text(pDX, IDC_EDIT_POWER_CHECK_INTERVAL, m_dbPowerCheckInterval);
	DDX_Text(pDX, IDC_EDIT_POWER_CHECK_WAIT, m_nPowerCheckWait);
	DDX_Text(pDX, IDC_EDIT_RUN_TIME, m_strRunTime);
	DDX_Text(pDX, IDC_EDIT_POWER_CHECK_COUNT, m_nPowerCheckCount);
	DDX_Text(pDX, IDC_EDIT_LAST_POWER, m_dbLastPower);
}


BEGIN_MESSAGE_MAP(CDlgPowerTest, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgPowerTest::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDlgPowerTest::OnBnClickedButtonStart)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgPowerTest 消息处理程序


BOOL CDlgPowerTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_pDlgMarkNormalPane->m_bDisableOtherWindowInputOnMarking = FALSE;

	m_dbTotalTime = g_ParaIni.m_dbPowerTestTotalTime;
	m_dbPowerCheckInterval = g_ParaIni.m_dbPowerCheckInterval;
	m_nPowerCheckWait = g_ParaIni.m_nPowerCheckWait;

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	m_strRunTime = "00:00:00";
	UpdateData(FALSE);

	if (g_sys.m_nPowerMeterType==0)
		m_pPowerMeter = new CCoherentPowerMeter();
	else
		m_pPowerMeter = new CHansPowerMeter();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPowerTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	try
	{
		//显示运行时间
		CTimeSpan tmSpanTmp;
		tmSpanTmp = CTime::GetCurrentTime() - m_tmStart;
		m_strRunTime = tmSpanTmp.Format("%H:%M:%S");
		UpdateData(FALSE);

		double dbCurTime = double(tmSpanTmp.GetTotalMinutes())/60.0;
		if (dbCurTime>=m_dbTotalTime)
		{
			SetDlgItemText(IDC_EDIT_TIP, "流程完成!");
			OnBnClickedButtonStop();
			return;
		}

		if (m_nCurStep==TEST_STEP_START)
		{
			tmSpanTmp = CTime::GetCurrentTime() - m_tmLastMarkEnd;
			if (tmSpanTmp.GetTotalSeconds()>m_nMarkInterval)
			{
				::ResetEvent(g_hEventMainThreadEmergency);
				g_pDlgMarkNormalPane->m_bMarking = TRUE;
				g_pDlgMarkNormalPane->PostMessage(WM_MARK_START, NULL, NULL);

				m_nCurStep = TEST_STEP_MARK_DOING;
			}
		}

		if (m_nCurStep==TEST_STEP_MARK_DOING)
		{
			if (WaitForSingleObject(g_hEventMainThreadEmergency, 0)==WAIT_OBJECT_0)
				throw 0;

			if (!g_pDlgMarkNormalPane->m_bMarking)
			{
				if (g_pDlgMarkNormalPane->m_nMarkError)
					throw 1;

				m_tmLastMarkEnd = CTime::GetCurrentTime();
				m_nCurStep = TEST_STEP_POWER_START;
			}
		}
		
		if (m_nCurStep==TEST_STEP_POWER_START)
		{
			tmSpanTmp = CTime::GetCurrentTime()-m_tmLastCheckPower;
			if (tmSpanTmp.GetTotalSeconds() < m_dbPowerCheckInterval*3600)
			{
				m_nCurStep = TEST_STEP_START;
			}
			else
			{
				m_tmLastCheckPower = CTime::GetCurrentTime();

				//运动到检测点
				g_pMotion->AMove(eMaskZ, g_ParaIni.m_dbPowerCheckZ);
				g_pMotion->LnXY(g_ParaIni.m_dbPowerCheckX, g_ParaIni.m_dbPowerCheckY);

				if (WaitForSingleObject(g_hEventMainThreadEmergency, 0)==WAIT_OBJECT_0)
					throw 0;

				g_DAT.BeamOn();
				m_nCurStep=TEST_STEP_POWER_DOING;
			}
		}
	
		if (m_nCurStep==TEST_STEP_POWER_DOING)
		{
			if (!m_pPowerMeter->QueryPower(m_dbLastPower))
				throw 2;

			UpdateData(FALSE);

			tmSpanTmp = CTime::GetCurrentTime() - m_tmLastCheckPower;
			if (tmSpanTmp.GetTotalSeconds() >= m_nPowerCheckWait)
			{
				m_pPowerMeter->Close();

				g_DAT.BeamOff();

				m_nPowerCheckCount++;
				LogResult(m_nPowerCheckCount, m_dbLastPower);

				m_nCurStep = TEST_STEP_START;
			}					
		}	
	}
	catch (int e)
	{
		CString strErr;
		switch(e)
		{
		case 0:
			strErr = "流程中止";
			break;
		case 1:
			strErr = "切割出错";
			break;
		case 2:
			strErr = "查询功能出错";
			break;
		}

		SetDlgItemText(IDC_EDIT_TIP, strErr);
		OnBnClickedButtonStop();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgPowerTest::LogResult(int nCount, double dPowerMeter)
{
	CStdioFile file;
	if (file.Open(m_strLogFilePath, CFile::modeWrite))
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);

		CString strNewLine;
		strNewLine.Format("%02d:%02d:%02d\t%d\t%.3f\n", time.wHour, time.wMinute, time.wSecond, nCount, dPowerMeter);
		
		file.SeekToEnd();
		file.WriteString(strNewLine);
		file.Close();
	}
}

void CDlgPowerTest::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (pFWnd==NULL) 	return ;
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)	return ;
	CHLDoc *pDoc = pView->GetDocument();
	if (NULL == pDoc) return;
	
	CFileStatus status;
	CString strLogDir = "D:\\PowerLog";
	if (!CFile::GetStatus(strLogDir, status))
		::CreateDirectory(strLogDir, NULL);

	SYSTEMTIME time;
	::GetLocalTime(&time);
	m_strLogFilePath.Format("%s\\PowerLog %4d%02d%02d %02d%02d%02d.txt", strLogDir, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	
	CStdioFile file;
	if (file.Open(m_strLogFilePath, CFile::modeCreate|CFile::modeWrite))
	{
		CString strNewRow;
		strNewRow.Format("TotalTime=%.3f PowerCheckInterval=%.3f PowerCheckWait=%d\n", 
			m_dbTotalTime, m_dbPowerCheckInterval, m_nPowerCheckWait);

		file.WriteString(strNewRow);
		file.Close();
	}
	else
	{
		CString strTip;
		strTip.Format("创建日志文件失败!\n%s", m_strLogFilePath);
		AfxMessageBox(strTip);
		return;
	}
	
	m_tmStart =  CTime::GetCurrentTime();
	m_tmLastMarkEnd = m_tmLastCheckPower = m_tmStart;
	SetDlgItemText(IDC_EDIT_TIP, "");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);

	m_nCurStep = TEST_STEP_START;
	SetTimer(1, 1000, NULL);
}

void CDlgPowerTest::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);

	if (m_nCurStep==TEST_STEP_MARK_DOING)
	{
		g_pDlgMarkNormalPane->OnBnClickedButtonStop();
	}

	if (m_nCurStep==TEST_STEP_POWER_DOING)
	{	
		g_DAT.BeamOff();
		m_dbLastPower = 0;
	}
	
	m_nCurStep = TEST_STEP_IDLE;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	m_strRunTime = "00:00:00";
	UpdateData(FALSE);
}

HBRUSH CDlgPowerTest::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID()==IDC_EDIT_TIP)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,0,0));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgPowerTest::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GetDlgItem(IDC_BUTTON_STOP)->IsWindowEnabled())
		return;

	if (!UpdateData())
		return;

	delete m_pPowerMeter;

	 g_ParaIni.m_dbPowerTestTotalTime = m_dbTotalTime;
	 g_ParaIni.m_dbPowerCheckInterval = m_dbPowerCheckInterval;
	 g_ParaIni.m_nPowerCheckWait = m_nPowerCheckWait;
	 g_ParaIni.SavePara();

	 g_pDlgMarkNormalPane->m_bDisableOtherWindowInputOnMarking = TRUE;

	CDialogEx::OnClose();
}
