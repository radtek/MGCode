// DlgMarkNormalPane.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgMarkNormalPane.h"
#include "afxdialogex.h"

#include "PtpDlg.h"
#include "PTPZDlg.h"
#include "CCDCalibrate.h"
#include "DlgSetWorkbench.h"
#include "KBChar.h"
#include "EmergencyWatcher.h"
#include "DlgEmergencyWarning.h"
#include "DlgCodeRecord.h"
#include "CHandPos.h"

#define TIMER_MARK_END		11
#define TIMER_HOME_END		120
#define TIMER_MOVE_END		121
#define TIMER_GETIO			110
#define TIMER_CHECK_MOTION_ERROR	111
#define TIMER_CHECK_MACHINE_READY   112
#define TIMER_AUTOOFFSET	130

HANDLE hAutoOffset = ::CreateEvent(NULL,TRUE,FALSE,"AutoOffset");

BOOL CDlgMarkNormalPane::m_bReplaceTxt=FALSE;
CDlgMarkNormalPane *g_pDlgMarkNormalPane=NULL;
CLaserController *CDlgMarkNormalPane::m_pLaserController=NULL;


// CDlgMarkNormalPane 对话框

IMPLEMENT_DYNAMIC(CDlgMarkNormalPane, CDialogEx)

CDlgMarkNormalPane::CDlgMarkNormalPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMarkNormalPane::IDD, pParent)
	, m_bgoLoadPt(FALSE)
	,m_bgoUnLPt(FALSE)
	,m_bInputMark(FALSE)
	,m_bInputError(FALSE)
	,m_bDoor(FALSE)
	, m_bFirstPts(FALSE)
	, m_strIPAddress(_T("127.0.0.1"))
	, m_nPort(9001)
	,m_bConnected(FALSE)
	, m_bIsNet(TRUE)
	, m_NetInfo(_T(""))
	, m_nCutNum(0)
	, m_strOperatorcode(_T(""))
	, m_strMarking(_T(""))
	, m_bSetTimeAutoOffset(FALSE)
	, m_strReverMatriCode(_T(""))
	, m_bStandby(TRUE)
	, m_strSaveMatrix(_T(""))
	, m_bOpenDust(FALSE)
	, m_bSingleRefPointMark(FALSE)
{
	m_tmRunStart = CTime::GetCurrentTime();
	m_dwSingleTime = 0;
	m_dwMarkCount = 0;

	m_bMarking = FALSE;

	m_nMarkType = MARK_MODEL_DIVIDE_AUTO_CCD;
	m_nGasStatus = 0;

	m_dbHandMoveDis = 1.0;
	m_nProductSide = -1;

	m_nMarkError = 0;

	m_up.SetOper(MOVE_UP);
	m_down.SetOper(MOVE_DOWN);
	m_left.SetOper(MOVE_LEFT);
	m_right.SetOper(MOVE_RIGHT);
	m_top.SetOper(MOVE_TOP);
	m_bottom.SetOper(MOVE_BOTTOM);
	m_fast.SetOper(MOVE_MODEL);

	m_pPowerMeter = NULL;
	m_bDisableOtherWindowInputOnMarking = TRUE;
	m_pOldDoc = NULL;

	m_strSendCode = "";
	m_strSendLast = "";
}

CDlgMarkNormalPane::~CDlgMarkNormalPane()
{
	
}

void CDlgMarkNormalPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LP4, m_LP3);
	DDX_Control(pDX, IDC_STATIC_LP3, m_LP2);
	DDX_Control(pDX, IDC_STATIC_LP2, m_LP1);
	DDX_Control(pDX, IDC_STATIC_LP1, m_LP0);
	DDX_Control(pDX, IDC_STATIC_UP, m_up);
	DDX_Control(pDX, IDC_STATIC_DOWN, m_down);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_left);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_right);
	DDX_Control(pDX, IDC_STATIC_TOP, m_top);
	DDX_Control(pDX, IDC_STATIC_BOTTOM, m_bottom);
	DDX_Control(pDX, IDC_STATIC_FAST, m_fast);
	DDX_Text(pDX, IDC_EDIT_HAND_MOVE_DIS, m_dbHandMoveDis);
	DDX_CBIndex(pDX, IDC_COMBO_MARK_TYPE, m_nMarkType);
	DDX_Radio(pDX, IDC_RADIO_GAS_STATUS, m_nGasStatus);
	DDX_Control(pDX, IDC_PROGRESS_MARK, m_progressMark);
	DDX_Control(pDX, IDC_LIST_LOCATE_POINT, m_listLocatePoint);
	DDX_Control(pDX, IDC_LIST_REF_POINTS, m_lbxRefPoints);
	DDX_Check(pDX, IDC_CHECK_MUL_DOC, m_bMulDocMark);
	DDX_Text(pDX, IDC_EDIT_OPERATORCODE, m_strOperatorcode);
	DDX_Text(pDX, IDC_STATIC_MARKING, m_strMarking);
	DDX_Text(pDX, IDC_EDIT_REVERMATRI, m_strReverMatriCode);
	DDX_Check(pDX, IDC_CHECK_SINGLE_REFPOINT_MARK, m_bSingleRefPointMark);
}


BEGIN_MESSAGE_MAP(CDlgMarkNormalPane, CDialogEx)
	ON_WM_NCPAINT()
	ON_BN_CLICKED(IDC_BUTTON_HADN_POINT, &CDlgMarkNormalPane::OnBnClickedButtonHadnPoint)
	ON_BN_CLICKED(IDC_BUTTON_HAND_OK_POINT, &CDlgMarkNormalPane::OnBnClickedButtonHandOkPoint)
	ON_BN_CLICKED(IDC_BUTTON_CHECK_POINT, &CDlgMarkNormalPane::OnBnClickedButtonCheckPoint)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgMarkNormalPane::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_MARK, &CDlgMarkNormalPane::OnBnClickedButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_TOMARK, &CDlgMarkNormalPane::OnBnClickedButtonToMark)
	ON_BN_CLICKED(IDC_BUTTON_TOCCD, &CDlgMarkNormalPane::OnBnClickedButtonToCCD)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_TOCCD, &CDlgMarkNormalPane::OnBnClickedButtonSelectToCCD)
	ON_BN_CLICKED(IDC_BUTTON_XY, &CDlgMarkNormalPane::OnBnClickedButtonXy)
	ON_BN_CLICKED(IDC_BUTTON_Z, &CDlgMarkNormalPane::OnBnClickedButtonZ)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_POSITION, &CDlgMarkNormalPane::OnBnClickedButtonLoadPosition)
	ON_BN_CLICKED(IDC_BUTTON_UNLOAD_POSITION, &CDlgMarkNormalPane::OnBnClickedButtonUnloadPosition)
	ON_BN_CLICKED(IDC_BUTTON_HOME, &CDlgMarkNormalPane::OnBnClickedButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_STOP_MOVE, &CDlgMarkNormalPane::OnBnClickedButtonStopMove)
	ON_BN_CLICKED(IDC_BUTTON_Z_TO_FOCUS, &CDlgMarkNormalPane::OnBnClickedButtonZToFocus)
	ON_BN_CLICKED(IDC_BUTTON_SWITCH_GAS_STATUS, &CDlgMarkNormalPane::OnBnClickedButtonSwitchGasStatus)
	ON_EN_CHANGE(IDC_EDIT_HAND_MOVE_DIS, &CDlgMarkNormalPane::OnChangeEditHandMoveDis)
	ON_WM_TIMER()
	ON_MESSAGE(WM_RESET_PRODUCT_DOC_PARA, &CDlgMarkNormalPane::OnResetProductDocPara)
	ON_MESSAGE(WM_SHOW_MARKING_INFO_BOX, &CDlgMarkNormalPane::OnShowMarkingInfoBox)
	ON_MESSAGE(WM_MARK_END,&CDlgMarkNormalPane::OnMarkEnd)
	ON_MESSAGE(WM_MARK_START,&CDlgMarkNormalPane::OnMarkStart)
	ON_MESSAGE(WM_MARK_PAUSE,&CDlgMarkNormalPane::OnMarkPause)
	ON_MESSAGE(WM_MARK_STOP,&CDlgMarkNormalPane::OnMarkStop)
	ON_MESSAGE(WM_EMERGENCY_EVENT, &CDlgMarkNormalPane::OnEmergencyEventHandler)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_REF_POINT_ADD, &CDlgMarkNormalPane::OnBnClickedButtonRefPointAdd)
	ON_BN_CLICKED(IDC_BUTTON_REF_POINT_ARRAY, &CDlgMarkNormalPane::OnBnClickedButtonRefPointArray)
	ON_BN_CLICKED(IDC_BUTTON_REF_POINT_DEL, &CDlgMarkNormalPane::OnBnClickedButtonRefPointDel)
	ON_BN_CLICKED(IDC_BUTTON_Z_TO_FOCUS2, &CDlgMarkNormalPane::OnBnClickedButtonZToFocus2)
	ON_LBN_SELCHANGE(IDC_LIST_REF_POINTS, &CDlgMarkNormalPane::OnSelchangeListRefPoints)
	ON_BN_CLICKED(IDC_BUTTON_MARK_COUNT_CLEAR, &CDlgMarkNormalPane::OnBnClickedButtonMarkCountClear)
	ON_BN_CLICKED(IDC_BTN_LOADSCAN, &CDlgMarkNormalPane::OnBnClickedBtnLoadscan)
	ON_BN_CLICKED(IDC_BTN_CCDVIEW, &CDlgMarkNormalPane::OnBnClickedBtnCcdview)
	ON_BN_CLICKED(IDC_BTN_GETCODE, &CDlgMarkNormalPane::OnBnClickedBtnGetcode)
//	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_LFDATA, &CDlgMarkNormalPane::OnBnClickedBtnLfdata)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_AUTOOFFSET, &CDlgMarkNormalPane::OnBnClickedBtnAutooffset)
	ON_BN_CLICKED(IDC_BTN_OPCLASER_RESET, &CDlgMarkNormalPane::OnBnClickedBtnOpclaserReset)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_STANDBY, &CDlgMarkNormalPane::OnBnClickedButtonLightStandby)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_SCANPT, &CDlgMarkNormalPane::OnBnClickedButtonMoveScanpt)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CDlgMarkNormalPane::OnBnClickedButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_DUST, &CDlgMarkNormalPane::OnBnClickedButtonDust)
END_MESSAGE_MAP()

// CDlgMarkNormalPane 消息处理程序
BOOL CDlgMarkNormalPane::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	if (g_sys.m_bDEV_3030B)
	{
		HWND hWndCtrl;
		GetDlgItem(IDC_STATIC_UP, &hWndCtrl);
		m_down.SubclassWindow(hWndCtrl);

		GetDlgItem(IDC_STATIC_DOWN, &hWndCtrl);
		m_up.SubclassWindow(hWndCtrl);
	}
	UpdateMotionView(FALSE);

	g_pDlgMarkNormalPane = this;
	g_ParaIni.LoadPara();
	g_EmergencyWatcher.RunWatcher(m_hWnd);
	
	m_pMarkMgr = new CMarkMgr();
	m_pMarkMgr->RunThread((LPVOID)GetSafeHwnd());

	m_progressMark.SetRange32(0,10);
	m_progressMark.SetStep(1);
	
	m_listLocatePoint.AddString("Mark1");
	m_listLocatePoint.AddString("Mark2");
	m_listLocatePoint.AddString("Mark3");
	m_listLocatePoint.AddString("Mark4");
	m_listLocatePoint.SetCurSel(0);

	//RefreshGas();
	SetLightStandby();
	
	//初始化激光器
	if (g_sys.m_nLaserControllerPort)
	{
		m_pLaserController = new CHansLaserController();
		m_pLaserController->Open(g_sys.m_nLaserControllerPort);
	}

	//维持电流，可能是模拟量调电流
	if (g_sys.m_nLaserCurrentKeep)
	{
		SetCurrent(g_sys.m_nLaserCurrentKeep);
	}


	//Manual CCD
	m_pLP[0] = &m_LP0;
	m_pLP[1] = &m_LP1;
	m_pLP[2] = &m_LP2;
	m_pLP[3] = &m_LP3;

	if(!SetTimer(0,500,NULL))
	{
		CString strError;
		strError.LoadString(IDS_ERROR_TIMER);
		MessageBox(strError);
	}


	if(!SetTimer(TIMER_GETIO,50,NULL))
	{
		CString strError;
		strError.LoadString(IDS_ERROR_TIMER);
		MessageBox(strError);
	}

	if(!SetTimer(TIMER_CHECK_MOTION_ERROR, 500, NULL))
	{
		CString strError;
		strError.LoadString(IDS_ERROR_TIMER);
		MessageBox(strError);
	}

	if (g_sys.m_bPLCOnline)
	{
		if(!SetTimer(TIMER_CHECK_MACHINE_READY, 500, NULL))
		{
			CString strError;
			strError.LoadString(IDS_ERROR_TIMER);
			MessageBox(strError);
		}
	}	

	m_font_.CreatePointFont(260, "MS Shell Dlg");

	GetDlgItem(IDC_STATIC_MARKING)->SetFont(&m_font_);

	m_bIsNet = g_ParaIni.ReadInt(_T("ISNET"),_T("IsNet"),1);
	m_strIPAddress = g_ParaIni.ReadString(_T("IPADDR"),_T("IpAddr"),_T("127.0.0.1"));
	m_nPort = g_ParaIni.ReadInt(_T("NETPORT"),_T("NetPort"),9001);

	m_strSendLast = g_ParaIni.ReadString(_T("STRSENDLAST"),_T("strSendLast"),"");


	initMatrixDoc();
	
	//ConnectDatebase();
	
	CString str1;
	str1.LoadString(IDS_DUST_COLLECTOR_ON);
	GetDlgItem(IDC_BUTTON_DUST)->SetWindowText(str1);
	g_IO.OUTPUT_SMOKE_FILTER(FALSE);
	m_bOpenDust = FALSE;
	
	SetDlgItemText(IDC_STATIC_WARNING_INFO, "");

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgMarkNormalPane::initMatrixDoc()
{
	m_arrMatrixMarkDoc.RemoveAll();

	CString strPath = "d:\\MatrixDoc.dat";
	CStdioFile fReadtxt;
	BOOL bReadOpen = fReadtxt.Open(strPath,CFile::modeRead|CFile::modeNoTruncate);
	if (bReadOpen)
	{
		int nLength = static_cast<int>(fReadtxt.GetLength());
		CString strData;
		CMatrixMarkDoc MatrixMarkDoc;
		for (int i = 0; i < nLength; i++)
		{
			fReadtxt.ReadString(strData);
			strData.Trim();
			if (strData == "")
			{
				break;
			}
			int nRet = strData.Find("*");
			strData.Mid(nRet + 1);
			MatrixMarkDoc.strMatrix = strData.Left(nRet);
			MatrixMarkDoc.strPath = strData.Mid(nRet + 1);
			m_arrMatrixMarkDoc.Add(MatrixMarkDoc);

		}
	}
	fReadtxt.Close();
}

HRESULT CDlgMarkNormalPane::OnMarkEnd(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd();
		break;
//	case MARK_SEND:
//		MarkSEnd();
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}
	return 0;
}

HRESULT CDlgMarkNormalPane::OnMarkStart(WPARAM wParam,LPARAM lParam)
{
	if (!UpdateData())	
		return 0;

	WriteRunLog("\n\n\n由“脚踏按钮”触发打标!");
	Mark() ;
	return 0;
}

HRESULT CDlgMarkNormalPane::OnMarkStop(WPARAM wParam,LPARAM lParam)
{
	OnBnClickedButtonStop();
	return 0;
}

void CDlgMarkNormalPane::OnBnClickedButtonMark()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())	
		return;

	WriteRunLog("\n\n\n", eLogDataFlow);
	WriteRunLog("Clicked Button Mark", eLogDataFlow);

	m_bFirstPts = FALSE; //从第0个参考点开始打
	Mark();
}

void CDlgMarkNormalPane::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pMarkMgr->StopMark();
	m_bMarking = FALSE;

	OnBnClickedButtonStopMove();

	g_IO.OUTPUT_SOFT_OK(TRUE);
}
void CDlgMarkNormalPane::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnNcPaint()
}


void CDlgMarkNormalPane::OnBnClickedButtonHadnPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPointIndex = m_listLocatePoint.GetCurSel();
	if (nPointIndex<0) return;

	double dbCaptureX, dbCaptureY;
	m_pDoc->CalculateLPCCDInitPos(nPointIndex, m_pDoc->m_nCCDUseModel, dbCaptureX, dbCaptureY);

	if (m_pDoc->m_nCCDUseModel==1)	//CCD2 + CCD1
	{
		m_pDoc->PointCCD2AssistCapture(dbCaptureX, dbCaptureY);
	}
	else
	{
		UpdateMotionView(FALSE);
		g_pMotion->StartXY(dbCaptureX, dbCaptureY, eSpeedFast);
		SetTimer(TIMER_MOVE_END, 100, NULL);
	}
	
	POSITION pos = m_pDoc->GetFirstViewPosition();
	CView *pView = m_pDoc->GetNextView(pos);
	if (pView)	pView->Invalidate();
}

void CDlgMarkNormalPane::OnBnClickedButtonHandOkPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPointIndex = m_listLocatePoint.GetCurSel();
	m_pDoc->m_dbLPCCDTrueX[nPointIndex] = g_pMotion->GetPos(eMaskX);
	m_pDoc->m_dbLPCCDTrueY[nPointIndex] = g_pMotion->GetPos(eMaskY);
	m_pLP[nPointIndex]->SetLed(CLed::LED_OFF);
}


void CDlgMarkNormalPane::OnBnClickedButtonCheckPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateMotionView(FALSE);
	int nPointIndex = m_listLocatePoint.GetCurSel();
	g_pMotion->StartXY(m_pDoc->m_dbLPCCDTrueX[nPointIndex], m_pDoc->m_dbLPCCDTrueY[nPointIndex],eSpeedFast);
	SetTimer(TIMER_MOVE_END, 100, NULL);

	POSITION pos = m_pDoc->GetFirstViewPosition();
	CView *pView = m_pDoc->GetNextView(pos);
	if (pView)	pView->Invalidate();
}

void CDlgMarkNormalPane::UpdateTime()
{
	CTimeSpan tmSpan = CTime::GetCurrentTime()-m_tmRunStart;
	SetDlgItemText(IDC_EDIT_RUN_TIME, tmSpan.Format("%D:%H:%M:%S"));
	
	//单个打标时间显示
	CString str;
	int hour,min,ms;
	double sec;

	DWORD dwSingle = m_dwSingleTime;
	ms=dwSingle%1000;
	dwSingle/=1000;

	sec=dwSingle%60;
	dwSingle/=60;
	sec+=(ms/1000.0);

	min=dwSingle%60;
	dwSingle/=60;

	hour=dwSingle%24;

	str.Format("%2d:%2d:%6.3f",hour,min,sec);
	SetDlgItemText(IDC_EDIT_SINGLE_TIME, str);

	SetDlgItemInt(IDC_EDIT_MARK_COUNT, m_dwMarkCount);
}

void CDlgMarkNormalPane::UpdatePos()
{
	DWORD dwInfoX, dwInfoY;
	double dbX = g_pMotion->GetPos(eMaskX, dwInfoX);
	double dbY = g_pMotion->GetPos(eMaskY, dwInfoY);
	double dbZ = g_pMotion->GetPos(eMaskZ);

	CString strPos;
	if (dwInfoX==0)
		strPos.Format("%.3f  ", dbX);
	else
		strPos.Format("%.3f *", dbX);
	SetDlgItemText(IDC_EDIT_POSX, strPos);

	strPos.Format("%.3f", dbY);
	if (dwInfoY==0)
		strPos.Format("%.3f  ", dbY);
	else
		strPos.Format("%.3f *", dbY);
	SetDlgItemText(IDC_EDIT_POSY, strPos);

	strPos.Format("%.3f", dbZ);
	SetDlgItemText(IDC_EDIT_POSZ, strPos);


	double dbloadPosX,dbUnloadPosX,dbLoadDis,dbUnLoadDis;
	g_pMotion->GetPara("XY", "UnloadPositionX", dbUnloadPosX);
	g_pMotion->GetPara("XY", "loadPositionX", dbloadPosX);
	g_pMotion->GetPara("XY", "LoadDis", dbLoadDis);
	g_pMotion->GetPara("XY", "UnLoadDis", dbUnLoadDis);

	double dbloadPosY,dbUnloadPosY;
	g_pMotion->GetPara("XY", "UnloadPositionY", dbUnloadPosY);
	g_pMotion->GetPara("XY", "loadPositionY", dbloadPosY);

	double dbLoadCurDis = dbloadPosX - dbX;
	double dbUnLoadCurDis = dbUnloadPosX - dbX;

	double dbLoadCurY = dbloadPosY - dbY;
	double dbUnLoadCurY = dbUnloadPosY - dbY;


	if ( fabs(dbLoadCurDis) < 0.1   &&  fabs(dbLoadCurY) < 0.1)
	{
		g_IO.OUTPUT_LOADPT(TRUE);
	}

	if (fabs(dbUnLoadCurDis) < 0.1 &&  fabs(dbUnLoadCurY) < 0.1)
	{
		g_IO.OUTPUT_UNLOADPT(TRUE);
	}

	if (fabs(dbUnLoadCurDis) - dbUnLoadDis > 0)
	{
		g_IO.OUTPUT_UNLOADPT(FALSE);
	}

	if (fabs(dbLoadCurDis) - dbLoadDis > 0)
	{
		g_IO.OUTPUT_LOADPT(FALSE);
	}
}

void CDlgMarkNormalPane::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==0)
	{
		UpdateTime();
		UpdatePos();
	}

	if (nIDEvent==TIMER_HOME_END)
	{
		if (g_pMotion->WaitHomeFinish(0))
		{
			KillTimer(nIDEvent);

			if (g_pMotion->IsHomed())
				UpdateMotionView(TRUE);
			else
				GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(TRUE);
		}
	}

	if (nIDEvent==TIMER_MOVE_END)
	{
		BOOL bMoving = 
			(g_pMotion->GetStatus(eMaskX) & eStatusBitMoving) ||
			(g_pMotion->GetStatus(eMaskY) & eStatusBitMoving) ||
			(g_pMotion->GetStatus(eMaskZ) & eStatusBitMoving);

		if (!bMoving)
		{
			KillTimer(nIDEvent);
			UpdateMotionView(TRUE);
		}		
	}

	if (nIDEvent == TIMER_CHECK_MOTION_ERROR)
	{
		char buffer[1024];
		if (g_pMotion->CheckError(buffer))
		{
			CString strTip;
			strTip.Format("Workbench Error: %s!", buffer);
			KillTimer(TIMER_CHECK_MOTION_ERROR);
			OnShowMarkingInfoBox(0, (LPARAM)(LPCSTR)strTip);
		}
	}

	if (nIDEvent==TIMER_CHECK_MACHINE_READY)
	{
		CheckMachineReady();
	}

	if(nIDEvent==TIMER_GETIO)
	{
		GetIOinfo();
	}

	if (nIDEvent==TIMER_MARK_END)
	{
		//	SetLightStandby();
		KillTimer(TIMER_MARK_END);
	}
	
	if (nIDEvent == TIMER_AUTOOFFSET)
	{
		if(WaitForSingleObject(hAutoOffset,0)==WAIT_OBJECT_0)
		{
			ResetEvent(hAutoOffset);
			KillTimer(nIDEvent);

			CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
			CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
			CHLApp *pApp = (CHLApp *)AfxGetApp();
			BOOL bAutoOffSet = Autooffset();
			//pView->MarkTicps(FALSE);
			UpdateMotionView(TRUE);

			CString strErr;
			GetDlgItemText(IDC_STATIC_WARNING_INFO, strErr);
			if (!bAutoOffSet && strErr == "")
			{
				SetDlgItemText(IDC_STATIC_WARNING_INFO, "No AutoOffset Pos");
			}
			
			m_pDoc->Setlight(m_pDoc->m_nLight1,m_pDoc->m_nLight2,m_pDoc->m_nLight3,m_pDoc->m_nLight4);
			g_ccd.SetWorkProgNo(atoi(m_pDoc->m_strProgID1));
		}

	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgMarkNormalPane::OnBnClickedButtonMarkCountClear()
{
	m_dwMarkCount = 0;
}

BOOL CDlgMarkNormalPane::SetCurrent(double dbCurrent)
{
	// TODO: Add your control notification handler code here
	if (g_sys.m_nLaserCurrentMode)
	{
		if (m_pLaserController->IsOpen())
			return m_pLaserController->SetCurrent(dbCurrent);
	}
	else
	{
		g_DAT.SetCurrent(dbCurrent);
	}
	
	return TRUE;
}

void CDlgMarkNormalPane::ResetProductDocPara(int nItem)
{
		PostMessage(WM_RESET_PRODUCT_DOC_PARA,(WPARAM)nItem);
}

HRESULT CDlgMarkNormalPane::OnResetProductDocPara(WPARAM wParam,LPARAM lParam)
{
	if (m_pDoc==NULL)
		return 0;

	if (wParam&(0x04+0x08))
	{
		//工作台参数及文档参数
		m_pLP[0]->SetLed(CLed::LED_ON);
		m_pLP[1]->SetLed(CLed::LED_ON);
		m_pLP[2]->SetLed(CLed::LED_ON);
		m_pLP[3]->SetLed(CLed::LED_ON);
	}

	if (wParam&0x04)
	{
		//文档参数
		//g_ccd.SetLighter(m_pDoc->m_nLight1, m_pDoc->m_nLight1);
		 g_ccd.SetCurLighter(1,m_pDoc->m_nLight1);
		 g_ccd.SetCurLighter(2,m_pDoc->m_nLight2);
		 g_ccd.SetCurLighter(3,m_pDoc->m_nLight3);
		 g_ccd.SetCurLighter(4,m_pDoc->m_nLight4);
		CString strProgID = m_pDoc->m_strProgID1;
		if (m_pDoc->m_nCCDUseModel==2)
			strProgID = m_pDoc->m_strProgID2;
		if (!g_ccd.SetWorkProgNo(atoi(strProgID)))
			AfxMessageBox("ExChange CCD Fail!");
	}

	RefreshRefPointsListBox(0);
	return 0;
}



BOOL CDlgMarkNormalPane::ScanMatriDoc(CString& strBarcode, CString &strErrInfo)
{
	g_pMotion->LnXY(m_pDoc->m_dbScanPtX,m_pDoc->m_dbScanPtY,eSpeedFast);
	g_pMotion->AMove(eMaskZ, m_pDoc->m_dbScanZpos);
	g_ccd.SetWorkProgNo(m_pDoc->m_ScanPriNum);

	CString strRec;
	g_ccd.ClearCCDBuffer();
	g_ccd.SendCCDCmd("T1");
	strRec = g_ccd.RecCCDReply();
	WriteRunLog(strRec);
	//strRec = "T1,0,123456789";
	g_ccd.SetWorkProgNo(atoi(m_pDoc->m_strProgID1));

	CString strCode = strRec;
	int nRet = strCode.Find(_T("T1"));
	CString strResult = "";
	if (nRet != -1)
	{
		strCode = strCode.Right(strCode.GetLength() - nRet - 3);
		strResult = strCode.Left(1);
		if (strResult == "0")
		{
			strBarcode  = strCode.Mid(2);
			strBarcode.Trim();
			if (strBarcode == "")
			{
				strErrInfo = "ScanError";
				return FALSE;
			}
			//读取数据库
			else
			{
				WriteRunLog(strBarcode);
				SetDlgItemText(IDC_STATIC_WARNING_INFO, strBarcode);
				return TRUE;
			}
		}
		else
		{
			strErrInfo = strCode + "ScanError";
			return FALSE;
		}
	}
	return TRUE;
}



BOOL CDlgMarkNormalPane::ScanCodeMark()
{

	//double dbScanPtX,dbScanPtY;
	//g_pMotion->GetPara("XY", "ScanPtX", dbScanPtX);
	//g_pMotion->GetPara("XY", "ScanPtY", dbScanPtY);
	//g_pMotion->LnXY(dbScanPtX,dbScanPtY,eSpeedFast);
	g_pMotion->LnXY(m_pDoc->m_dbScanPtX,m_pDoc->m_dbScanPtY,eSpeedFast);

	//double dbScanZpos;
	//g_pMotion->GetPara("XY", "dbScanZpos", dbScanZpos);
	//g_pMotion->AMove(eMaskZ, dbScanZpos);
	g_pMotion->AMove(eMaskZ, m_pDoc->m_dbScanZpos);

	if (m_pDoc->m_bUseFile)
	{
		if (!m_pDoc->ScanMatri(FALSE))
		{
			return FALSE;
		}

		if (!LoadFileData())
		{
			return FALSE;
		}
	}
	else
	{
		BOOL bDebug = g_ParaIni.ReadInt(_T("ISDEBUG"),_T("IsDebug"),0);
		CString strMatriCode = g_ParaIni.ReadString(_T("MATRICODE"),_T("MatriCode"),_T(""));
		if (bDebug)
		{
			if (strMatriCode == "")
			{
				return FALSE;
			}
			m_strSendCode = strMatriCode;
		}
		else
		{
			if (!m_pDoc->ScanMatri(m_bIsNet))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CDlgMarkNormalPane::ApplyMarkData()
{
	BOOL arrNeedMark[600] = {TRUE,TRUE};
	for (int i = 0 ; i < 300; i++)
		arrNeedMark[i] = TRUE;
	for (int i = 301; i < 600; i++)
		arrNeedMark[i] = FALSE;

	int nCount = m_pDoc->m_strNetRecv.GetCount();
	for (int i = 1; i < nCount; i++)
	{
		CString strTemp = m_pDoc->m_strNetRecv.GetAt(i);
		int nRet = strTemp.Find(_T("|"));
		if (-1 != nRet)
		{
			CString strMatri = strTemp.Left(nRet);
			CString strText = strTemp.Right(strTemp.GetLength() - nRet - 1);
			if (strText == "")
			{
				arrNeedMark[i] = FALSE;
				arrNeedMark[i + 300] = TRUE;
			}
			CString strTextID,strMatriID;
			strTextID.Format(_T("%d"),i);
			strMatriID.Format(_T("%d"),i + 1000);

			if (strText != "")
				m_pDoc->m_chain.ChangeText(strText,strTextID);
			if (strMatri != "")
				m_pDoc->m_chain.ChangeText(strMatri,strMatriID);
		}
		else
		{
			strTemp = strTemp.Trim();

			if (strTemp == "0")
			{
				arrNeedMark[i] = TRUE;
				arrNeedMark[i + 300] = FALSE;
			}
			else
			{
				arrNeedMark[i] = FALSE;
				arrNeedMark[i + 300] = TRUE;
			}
		}
	}
	m_pDoc->m_chain.SetNeedMarkFlag(arrNeedMark);
	m_pDoc->m_chain.PreprocessMark();
	m_pDoc->UpdateAllViews(NULL);
}

BOOL CDlgMarkNormalPane::CheckPower()
{
	g_pMotion->AMove(eMaskZ, g_ParaIni.m_dbPowerCheckZ);
	g_pMotion->LnXY(g_ParaIni.m_dbPowerCheckX, g_ParaIni.m_dbPowerCheckY);

	//打开功率计
	if (g_sys.m_nPowerMeterType==0)
		m_pPowerMeter = new CCoherentPowerMeter();
	else
		m_pPowerMeter = new CHansPowerMeter();
	g_DAT.BeamOn();
	Sleep(30000);
	double dbCurPower;
	BOOL bRet = FALSE;
	if (m_pPowerMeter->QueryPower(dbCurPower))
	{
		double dPowerMeter;
		g_pMotion->GetPara("XY", "POWERMETER", dPowerMeter);
		if (dPowerMeter == 0)
		{
			dPowerMeter = 1;
			g_pMotion->WritePara("XY", "POWERMETER", dPowerMeter);
		}
		dbCurPower *= dPowerMeter;
		if (dbCurPower < m_pDoc->m_dbProductPowerMin || dbCurPower > m_pDoc->m_dbProductPowerMax)
		{
			g_IO.OUTPUT_CCD_LOCATE_ERROR(TRUE);
			Sleep(100);
			g_IO.OUTPUT_CCD_LOCATE_ERROR(FALSE);
			bRet =  FALSE;
		}
		else
		{
			bRet =  TRUE;
		}
		
	}

	m_pPowerMeter->Close();
	g_DAT.BeamOff();
	return bRet;
}


BOOL CDlgMarkNormalPane::SetNodeCodeLength()
{
	int nCodeId = 0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_pDoc->m_chain.m_list.GetHeadPosition();
	while( pos  != NULL )
	{	
		pGroupObj = (CGroupObj *)m_pDoc->m_chain.m_list.GetNext(pos);
		POSITION pos = pGroupObj->m_list.GetHeadPosition();
		CTree *pTree = NULL;
		while( pos != NULL )
		{	
			pTree = (CTree*)pGroupObj->m_list.GetNext(pos);
			if ( pTree )
			{
				if (pTree->IsHaveCodeBar(pTree->m_pRoot))
				{
					pTree->SetBarcodeLength(pTree->m_pRoot,m_QuadDot,nCodeId);
				}
			}
		}
	}
	return TRUE;
}



BOOL CDlgMarkNormalPane::GetNodeCodeLength()
{
	int nCodeId = 0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_pDoc->m_chain.m_list.GetHeadPosition();
	while( pos  != NULL )
	{	
		pGroupObj = (CGroupObj *)m_pDoc->m_chain.m_list.GetNext(pos);
		POSITION pos = pGroupObj->m_list.GetHeadPosition();
		CTree *pTree = NULL;
		while( pos != NULL )
		{	
			pTree = (CTree*)pGroupObj->m_list.GetNext(pos);
			if ( pTree )
			{
				if (pTree->IsHaveCodeBar(pTree->m_pRoot))
				{
					pTree->GetBarcodeLength(pTree->m_pRoot,m_QuadDot,nCodeId);
				}
			}
		}
	}

	double dbBarcodeLen=10, dbBarcodeWid=10, dbSameLen=0;
	if (dbSameLen)
	{
		for (int i = 0; i < 100; i++)
		{
			CQuad quad(m_QuadDot[i].quad.GetBaseDot(),dbBarcodeLen,dbBarcodeWid);
			m_QuadDot[i].quad = quad;
		}
	}

	return TRUE;
}


//Exception: CHLMCIException
void CDlgMarkNormalPane::CheckWorkbench()
{
	if(m_pMarkMgr->MarkIsOver())
		throw new CHLMCIException(_T("Last mark is not over!"));

	char buffer[1024];
	if (g_pMotion->CheckError(buffer))
	{
		CString strTip;
		strTip.Format("Workbench Error: %s!", buffer);
		throw new CHLMCIException(strTip);
	}

	if (!g_IO.INPUT_MACHINE_READY())
		throw new CHLMCIException(_T("the table is not ready"));

	if (!m_bStandby)
		throw new CHLMCIException("the table is not ready");

	if (!g_pMotion->IsHomed())
		throw new CHLMCIException(_T("The workbench is not home"));

	if ((g_pMotion->GetStatus(eMaskX)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskY)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskZ)&eStatusBitMoving))
	{
		throw new CHLMCIException(_T("The workbench is moving!"));
	}
}

//Exception: CHLMCIException
void CDlgMarkNormalPane::CheckBox()
{
	if (m_nMarkType==MARK_MODEL_NORMAL_MARK || m_nMarkType == MARK_MODEL_FRAM)
	{
		CQuad quadDoc = m_pDoc->GetAllLimit();
		if (m_pDoc->m_chain.m_bSelMark)
			quadDoc = m_pDoc->GetLimit();

		double BoxX, BoxY;
		double x1,y1,x2,y2;
		g_DAT.GetMarkRange(BoxX,BoxY);
		x1 = (CENTER-BoxX/2);
		x2 = (CENTER+BoxX/2);
		y1 = (CENTER-BoxY/2);
		y2 = (CENTER+BoxY/2);
		if(quadDoc.left<x1 || quadDoc.bottom<y1 || quadDoc.right>x2 || quadDoc.top>y2)
		{
			double BoxX, BoxY;
			g_DAT.GetMarkRange(BoxX,BoxY);
			CString strError;
			strError.Format("Beyond the scope of marking(%.0fX%.0f)!", BoxX, BoxY);
			throw new CHLMCIException(strError);
		}
	}
}

//Exception: CHLMCIException
void CDlgMarkNormalPane::CheckRefPoints()
{
	if (m_nMarkType==MARK_MODEL_DIVIDE_NO_CCD || m_nMarkType==MARK_MODEL_DIVIDE_MANUAL_CCD  || m_nMarkType==MARK_MODEL_DIVIDE_AUTO_CCD)
	{
		if ( m_lbxRefPoints.GetCount()==0)
			throw new CHLMCIException("Not set reference point!");

		if (!m_bFirstPts)
		{
			m_dwMarkStart = ::GetTickCount();
			if (m_bSingleRefPointMark)
			{
				if (m_lbxRefPoints.GetCurSel()<0)
					throw new CHLMCIException("no reference point is selected!");
			}
			else
			{
				if (m_nMarkType == MARK_MODEL_DIVIDE_AUTO_CCD)
				{
					m_lbxRefPoints.SetCurSel(0);
					OnSelchangeListRefPoints();	
				}
			}
		}
	}
}

//Exception: CHLMCIException
void CDlgMarkNormalPane::CheckDocument()
{
	if (m_pDoc->m_bEnableMarkFocus && m_pDoc->m_arrMarkFocus.GetCount()==0)
		throw new CHLMCIException("No mark focus!");

	CString strInfo;
	if (m_nMarkType==MARK_MODEL_DIVIDE_NO_CCD || m_nMarkType==MARK_MODEL_DIVIDE_MANUAL_CCD  || m_nMarkType==MARK_MODEL_DIVIDE_AUTO_CCD)
	{
		if (!m_pDoc->CheckGraphicAndWorkbenchLimit(m_pDoc->m_arrDocRefPointX[0], m_pDoc->m_arrDocRefPointY[0], strInfo))
			throw new CHLMCIException(strInfo);
	}

	if (m_nMarkType==MARK_MODEL_DIVIDE_NO_CCD || m_nMarkType==MARK_MODEL_DIVIDE_MANUAL_CCD  || m_nMarkType==MARK_MODEL_DIVIDE_AUTO_CCD)
	{
		if (m_nMarkType==MARK_MODEL_DIVIDE_MANUAL_CCD)
		{
			if ((m_pLP[0]->GetLed()==CLed::LED_ON || m_pLP[1]->GetLed()==CLed::LED_ON) ||				
				(m_pDoc->m_nLocateModel==LOCATE_MODEL_3_POINTS && (m_pLP[2]->GetLed()==CLed::LED_ON)) ||
				(m_pDoc->m_nLocateModel==LOCATE_MODEL_4_POINTS && (m_pLP[2]->GetLed()==CLed::LED_ON || m_pLP[3]->GetLed()==CLed::LED_ON)))
			{
				throw new CHLMCIException("Did not execute manually positioning!");				
			}
		}
	}
}

//Exception: CHLMCIException
void CDlgMarkNormalPane::MarkCheck()
{
	CheckWorkbench();

	CheckBox();

	CheckRefPoints();

	CheckDocument();
}

//Exception: CHLMCIException
void CDlgMarkNormalPane::MarkInit()
{
	ResetBarcodeSize();

	initOpcLaser();

	InitLaserPara();

	WriteRunLog("RTC5 AutoCal", eLogTime);
	g_DAT.AutoCal();
	WriteRunLog("RTC5 AutoCal End", eLogTime);

	SetGas(TRUE);
			
	if (g_ParaIni.m_bUseCutBlow)
		g_IO.OUTPUT_AIR_BLOW(TRUE);

	SetLightRun();

	m_strMarking.LoadString(IDS_STRINGMARKING);
	::ShowWindow(GetDlgItem(IDC_STATIC_MARKING)->GetSafeHwnd(), SW_SHOW);
	UpdateData(FALSE);
}

void CDlgMarkNormalPane::LogStartInfoAndInitVariable()
{
	//Log
	CString strTip;
	strTip.Format("current production: %s", m_pDoc->GetPathName());
	WriteRunLog(strTip);

	char szModel[][32] = {"Normal Marking","Border preview" , "Mesh lattice marking", "Ordinary segmentation", "CCD Segmentation(Manual para)", "CCD Segmentation(Auto para)"};
	strTip.Format("工作模式: %s, 定位方式:%d, 是否单CCD定位:%d", szModel[m_nMarkType], !m_pDoc->m_nLocateModel, !m_pDoc->m_nCCDUseModel);
	WriteRunLog(strTip);

	//初始化变量
	m_bMarking = TRUE;
	m_nMarkError = 0;
	m_strSendCode = "";
	m_strSaveMatrix = "";
	CDlgSetWorkbench::m_dbCrossInMarkPosX = g_pMotion->GetPos(eMaskX);
	CDlgSetWorkbench::m_dbCrossInMarkPosY = g_pMotion->GetPos(eMaskY);
	SetDlgItemText(IDC_STATIC_WARNING_INFO, "");
}

void CDlgMarkNormalPane::Mark() 
{
	LogStartInfoAndInitVariable();
	
	try
	{
		MarkCheck();

		ScanMatrix();

		ApplyMarkData();

		MarkInit();
	}
	catch (CHLMCIException* e)
	{
		SetDlgItemText(IDC_STATIC_WARNING_INFO, e->m_strMessage);

		g_IO.OUTPUT_CCD_LOCATE_ERROR(TRUE);
		Sleep(100);
		g_IO.OUTPUT_CCD_LOCATE_ERROR(FALSE);

		g_IO.OUTPUT_SOFT_OK(FALSE);

		SetLightError();

		m_bMarking = FALSE;
		m_nMarkError = 1;

		e->Delete();
		return;
	}

	//使缺省按钮失效
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(FALSE);
	UpdateMotionView(FALSE);

	m_pMarkMgr->SetMarkType(0);
	m_pMarkMgr->SetChain(m_pDoc);
	m_pMarkMgr->SetLay(m_pDoc->GetLay());
	m_pMarkMgr->StartMark();
}

//Exception: CHLMCIException
void CDlgMarkNormalPane::initOpcLaser()
{
	if (g_sys.m_bUseOpcLaser)
	{
		bool bExtActive= false;
		bool bLaserOn = false;

		if (!g_OPCLaser->CheckLaserOn(bExtActive, bLaserOn))
			throw new CHLMCIException("通讯出错,请重启软件!");

		if (!bExtActive)
			throw new CHLMCIException("外部未激活,请重启软件!");

		if(!bLaserOn)
			throw new CHLMCIException("激光未打开,请稍等!");

		if (!g_OPCLaser->Reset())
			throw new CHLMCIException("通讯出错,请重启软件!");


		if (!g_bReady)
		{
			bool bReady=false;
			if (!g_OPCLaser->CheckLaserReady(bReady))
				throw new CHLMCIException("通讯出错,请重启软件!");

			if (!bReady)
				throw new CHLMCIException("激光器未就绪,请稍等!");

			if (!g_OPCLaser->OpenLaser())
				throw new CHLMCIException("通讯出错,请重启软件!");

			g_bReady = TRUE;
		}
		else
		{
			bool bActive=false;
			if (!g_OPCLaser->CheckProgActive(bActive))
				throw new CHLMCIException("通讯出错,请重启软件!");

			if (!bActive)
				throw new CHLMCIException("程序未激活,请稍等!");
		}
	}
}

void CDlgMarkNormalPane::InitLaserPara()
{
	PARA curMarkPara;
	//串口调电流
	//if (g_sys.m_nLaserCurrentMode)	
	{
		curMarkPara = m_pDoc->GetLay().GetPara(0);
		if(m_oldMarkPara.dbCurrent != curMarkPara.dbCurrent)
		{		
			if (!SetCurrent(curMarkPara.dbCurrent)) 
				throw new CHLMCIException("SetCurrent Fail!");
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

}



void CDlgMarkNormalPane::ResetBarcodeSize()
{
	//保持一维码大小不变
	if (!GetNodeCodeLength())
		throw new CHLMCIException("Fail to get barcode size!");
	
	for (int i = 0 ; i < 5; i++)
	{
		if (!SetNodeCodeLength())
		{
			m_pDoc->m_chain.PreprocessMark();
			m_pDoc->UpdateAllViews(NULL);
			throw new CHLMCIException("Fail to get barcode size!");
		}
	}
}

void CDlgMarkNormalPane::ScanMatrix()
{
	if (m_pDoc->m_bScan)
	{
		CString strErrInfo;
		if (!ScanMatriDoc(m_strSaveMatrix, strErrInfo))
			throw CHLMCIException(strErrInfo);
	}
}

void CDlgMarkNormalPane::CheckMachineReady()
{
	if (g_sys.m_bPLCOnline)
	{
		static BOOL bLast_INPUT_MACHINE_READY = FALSE;
		if (g_IO.INPUT_MACHINE_READY())
		{
			if (!bLast_INPUT_MACHINE_READY)
			{
				if (g_pMotion->IsHomed())
				{
					BOOL bMoving = 
						(g_pMotion->GetStatus(eMaskX) & eStatusBitMoving) ||
						(g_pMotion->GetStatus(eMaskY) & eStatusBitMoving) ||
						(g_pMotion->GetStatus(eMaskZ) & eStatusBitMoving);

					if (!bMoving)
						UpdateMotionView(TRUE);
				}
				else
				{
					GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(TRUE);
				}
			}
			bLast_INPUT_MACHINE_READY = TRUE;
		}
		else
		{
			UpdateMotionView(FALSE);
			bLast_INPUT_MACHINE_READY = FALSE;
		}
	}
}

void CDlgMarkNormalPane::UpdateMotionView(BOOL bEnable)
{
	GetDlgItem(IDC_BUTTON_Z)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_Z_TO_FOCUS)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LOAD_POSITION)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_UNLOAD_POSITION)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MOVE_SCANPT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_LFDATA)->EnableWindow(bEnable);
	
	GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_XY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_GETCODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_TOMARK)->EnableWindow(bEnable);

	GetDlgItem(IDC_BUTTON_TOCCD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_SELECT_TOCCD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_Z_TO_FOCUS2)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_MARK_TYPE)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_TOP)->EnableWindow(bEnable);
	
	GetDlgItem(IDC_STATIC_LEFT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_RIGHT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_BOTTOM)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_AUTOOFFSET)->EnableWindow(bEnable);

	if (m_nMarkType != MARK_MODEL_NORMAL_MARK)
	{
		GetDlgItem(IDC_STATIC_UP)->EnableWindow(bEnable);
		GetDlgItem(IDC_STATIC_DOWN)->EnableWindow(bEnable);
		m_up.EnableWin(bEnable);
		m_down.EnableWin(bEnable);
	}
	m_top.EnableWin(bEnable);
	m_left.EnableWin(bEnable);
	m_right.EnableWin(bEnable);
	m_bottom.EnableWin(bEnable);
}

void CDlgMarkNormalPane::MarkEnd()
{
	WriteRunLog("MarkEnd", eLogDataFlow);

	m_bReplaceTxt=FALSE;

	m_progressMark.SetPos(0);

	if (g_sys.m_nLaserCurrentKeep)//维持电流
	{
		SetCurrent(g_sys.m_nLaserCurrentKeep);
		m_oldMarkPara.dbCurrent = g_sys.m_nLaserCurrentKeep;
	}

	m_pDoc->UpdateAllViews(NULL);
	m_pDoc->SetTrackerQuad(m_pDoc->GetChain().GetLimit());

//	if(!m_pMarkMgr->GetRedPreview())
//		m_nCount++;

	m_LP0.SetLed(CLed::LED_ON);
	m_LP1.SetLed(CLed::LED_ON);
	m_LP2.SetLed(CLed::LED_ON);
	m_LP3.SetLed(CLed::LED_ON);

	
	if (m_nMarkType==MARK_MODEL_DIVIDE_AUTO_CCD  || m_nMarkType==MARK_MODEL_DIVIDE_NO_CCD)
	{
		int nCurSel = m_lbxRefPoints.GetCurSel()+1;
		int nMaxIndex = m_lbxRefPoints.GetCount();
		
		if (nCurSel<nMaxIndex && !m_bSingleRefPointMark)
		{
			m_bFirstPts = TRUE;
			m_lbxRefPoints.SetCurSel(nCurSel);
			OnSelchangeListRefPoints();	
			PostMessage(WM_MARK_START);
		}
		else 
		{
			//SetLightError();
			SetTimer(TIMER_MARK_END, 3000, NULL);

			m_bFirstPts = FALSE;
			if (m_pDoc->m_bEnableMarkFocus)
				g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);

			g_IO.OUTPUT_MARKEND(TRUE);
			Sleep(500);
			g_IO.OUTPUT_MARKEND(FALSE);

			
			g_IO.OUTPUT_SOFT_OK(TRUE);

			if (!g_sys.m_bPLCOnline)	//离线时才能去下料点，在线时要等待工作台准备好并给出去下料点信号
			{
				if (WaitForSingleObject(g_hEventMainThreadEmergency, 0)!=WAIT_OBJECT_0)
				{

					OnBnClickedButtonUnloadPosition();

					g_IO.OUTPUT_LIGHT_HUMMER(TRUE);
					Sleep(2000);
					g_IO.OUTPUT_LIGHT_HUMMER(FALSE);

					if (m_bStandby)
					{
						SetLightStandby();

						SaveMarkCode(m_strSaveMatrix);					
					}
				}
			}
		}		
	}
	else
	{
		g_IO.OUTPUT_MARKEND(TRUE);
		Sleep(500);
		g_IO.OUTPUT_MARKEND(FALSE);


		g_IO.OUTPUT_SOFT_OK(TRUE);

		if (m_bStandby)
		{
			SetLightStandby();
		}
	}


	m_dwMarkCount++;
	if (m_nMarkType==MARK_MODEL_NORMAL_MARK)
		m_dwSingleTime = g_DAT.GetMarkTime();
	else
		m_dwSingleTime = ::GetTickCount()-m_dwMarkStart;


	BOOL arrNeedMark[600] = {TRUE};
	for (int i = 0 ; i < 300; i++)
	{
		arrNeedMark[i] = TRUE;
	}
	for (int i = 301; i < 600; i++)
	{
		arrNeedMark[i] = FALSE;
	}
	m_pDoc->m_chain.SetNeedMarkFlag(arrNeedMark);


	if (m_pDoc->m_bUseFile)
	{
		DeleteFile(m_pDoc->m_strFileDataPath);
		CString strTemp = "ABCDEF";
		for (int i = 1; i < 192; i++)
		{
			CString strTextID,strMatriID;
			strTextID.Format(_T("%d"),i);
			strMatriID.Format(_T("%d"),i + 1000);
			m_pDoc->m_chain.ChangeText(strTemp,strTextID);
			m_pDoc->m_chain.ChangeText(strTemp,strMatriID);
		}
		m_pDoc->m_chain.PreprocessMark();
		m_pDoc->UpdateAllViews(NULL);

	}



	if (m_pDoc->m_dbTestPowerFrequency > 0)
	{
		m_nCutNum++;
	}

	m_strMarking = "";
	::ShowWindow(GetDlgItem(IDC_STATIC_MARKING)->GetSafeHwnd(), SW_HIDE); 
	UpdateData(FALSE);

	g_IO.OUTPUT_AIR_BLOW(FALSE);
	g_IO.OUTPUT_CLOSE_DOOR(FALSE);

	if (m_nMarkError==0)
	{
		m_pDoc->m_chain.PreprocessMark(0x01);
		m_pDoc->UpdateAllViews(NULL);
	}

	m_bMarking = FALSE;
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(TRUE);
	UpdateMotionView(TRUE);
}

HRESULT CDlgMarkNormalPane::OnMarkPause(WPARAM wParam,LPARAM lParam)
{
//	m_bMark=TRUE;
//	UpdateView();
	int i=(int)lParam;
	if (i==1) {  //安全门关闭后自动继续打标
		//		SetTimer(10,300,NULL);
	}
	return 0;
}

void CDlgMarkNormalPane::OnChangeEditHandMoveDis() 
{
	if (UpdateData(TRUE))
	{
		CFStatic::m_dbMoveDis = m_dbHandMoveDis;
	}

	// TODO: Add your control notification handler code here
}

void CDlgMarkNormalPane::OnBnClickedButtonToMark()
{
	// TODO: 在此添加控件通知处理程序代码


	double CurX = g_pMotion->GetPos(eMaskX);
	double CurY = g_pMotion->GetPos(eMaskY);

	UpdateMotionView(FALSE);
	g_pMotion->StartXY(CurX-g_ParaIni.m_dbMarkToCCD1X,CurY -g_ParaIni.m_dbMarkToCCD1Y,eSpeedFast);
	SetTimer(TIMER_MOVE_END, 100, NULL);
}




void CDlgMarkNormalPane::OnBnClickedButtonToCCD()
{
	// TODO: 在此添加控件通知处理程序代码
	double CurX = g_pMotion->GetPos(eMaskX);
	double CurY = g_pMotion->GetPos(eMaskY);

	UpdateMotionView(FALSE);
	g_pMotion->StartXY(CurX + g_ParaIni.m_dbMarkToCCD1X,CurY + g_ParaIni.m_dbMarkToCCD1Y,eSpeedFast);
	SetTimer(TIMER_MOVE_END, 100, NULL);
}

void CDlgMarkNormalPane::OnBnClickedBtnGetcode()
{
	// TODO: 在此添加控件通知处理程序代码
	double dbUseScan;
	g_pMotion->GetPara("XY", "UseScan", dbUseScan);
	BOOL dUseScan = (BOOL)dbUseScan;
	if (!dUseScan)
	{
		AfxMessageBox(_T("No Use Scan"));
		return;
	}
	double dbScanX,dbScanY;
	g_pMotion->GetPara("XY", "ScanX", dbScanX);
	g_pMotion->GetPara("XY", "ScanY", dbScanY);

	double CurX = g_pMotion->GetPos(eMaskX);
	double CurY = g_pMotion->GetPos(eMaskY);

	g_pMotion->LnXY(CurX + dbScanX, CurY + dbScanY,eSpeedFast);
	m_pDoc->ScanMatri(FALSE);

}



void CDlgMarkNormalPane::OnBnClickedButtonSelectToCCD()
{
	// TODO: 在此添加控件通知处理程序代码
	CQuad quadCheck = m_pDoc->GetChain().GetLimit();
	if (quadCheck.IsEmpty())
	{
		AfxMessageBox("No object selected!");
		return;
	}

	CDot dotCheck = quadCheck.GetBaseDot();	

	double dbX, dbY;
	if (m_pDoc->m_nCCDUseModel==2)
	{
		dbX = (g_ParaIni.m_dbRefPointX - dotCheck.x) + (g_ParaIni.m_dbMarkToCCD2X-g_ParaIni.m_dbMarkToCCD1X);
		dbY = (g_ParaIni.m_dbRefPointY - dotCheck.y) + (g_ParaIni.m_dbMarkToCCD2Y-g_ParaIni.m_dbMarkToCCD1Y);
	}
	else
	{
		dbX = g_ParaIni.m_dbRefPointX-dotCheck.x;
		dbY = g_ParaIni.m_dbRefPointY-dotCheck.y;
	}

	UpdateMotionView(FALSE);
	g_pMotion->StartXY(dbX, dbY,eSpeedFast);
	SetTimer(TIMER_MOVE_END, 100, NULL);
}


void CDlgMarkNormalPane::OnBnClickedButtonXy()
{
	// TODO: 在此添加控件通知处理程序代码
	CPtpDlg dlg;

	dlg.m_dbX = g_pMotion->GetPos(eMaskX);
	dlg.m_dbY = g_pMotion->GetPos(eMaskY);
	dlg.m_dbX = double(int(dlg.m_dbX*1000))/1000;
	dlg.m_dbY = double(int(dlg.m_dbY*1000))/1000;

	if(dlg.DoModal()==IDOK)
	{
		UpdateMotionView(FALSE);
		g_pMotion->StartXY(dlg.m_dbX,dlg.m_dbY,eSpeedFast);
		SetTimer(TIMER_MOVE_END, 100, NULL);
	}

	POSITION pos = m_pDoc->GetFirstViewPosition();
	CHLView *pView = dynamic_cast<CHLView*>(m_pDoc->GetNextView(pos));
	if (pView)	pView->Invalidate();
}


void CDlgMarkNormalPane::OnBnClickedButtonZ()
{
	// TODO: 在此添加控件通知处理程序代码
	CPTPZDlg dlg;

	dlg.m_dbPosZ=g_pMotion->GetPos(eMaskZ);	 
	dlg.m_dbPosZ = double(int(dlg.m_dbPosZ*1000))/1000;

	if(dlg.DoModal()==IDOK)
	{
		UpdateMotionView(FALSE);
		g_pMotion->StartAMove(eMaskZ, dlg.m_dbPosZ);
		SetTimer(TIMER_MOVE_END, 100, NULL);
	}	
}


void CDlgMarkNormalPane::OnBnClickedButtonLoadPosition()
{
	// TODO: 在此添加控件通知处理程序代码
	double dbLoadPosX,dbLoadPosY;
	g_pMotion->GetPara("XY", "LoadPositionX", dbLoadPosX);
	g_pMotion->GetPara("XY", "LoadPositionY", dbLoadPosY);

	UpdateMotionView(FALSE);
	g_pMotion->StartXY(dbLoadPosX,dbLoadPosY,eSpeedFast);
	SetTimer(TIMER_MOVE_END, 100, NULL);

}


void CDlgMarkNormalPane::OnBnClickedButtonUnloadPosition()
{
	// TODO: 在此添加控件通知处理程序代码
	//g_pMotion->StartMoveToUnloadPosition();
	double dbUnloadPosX,dbUnloadPosY;
	g_pMotion->GetPara("XY", "UnloadPositionX", dbUnloadPosX);
	g_pMotion->GetPara("XY", "UnloadPositionY", dbUnloadPosY);

	UpdateMotionView(FALSE);
	g_pMotion->StartXY(dbUnloadPosX,dbUnloadPosY,eSpeedFast);
	SetTimer(TIMER_MOVE_END, 100, NULL);

}


void CDlgMarkNormalPane::OnBnClickedButtonHome()
{
	// TODO: ?ú′?ìí?ó???tí¨?a′|àí3ìDò′ú??
	if (g_sys.m_bPLCOnline && !g_IO.INPUT_MACHINE_READY())
	{
		AfxMessageBox(_T("the table is not ready!"));
		return;
	}

	char buffer[1024];
	if (!g_pMotion->IsInit() || g_pMotion->CheckError(buffer))
	{
		if (!g_pMotion->Init(buffer))
		{
			AfxMessageBox(buffer);
			return;
		}
	}
	SetTimer(TIMER_CHECK_MOTION_ERROR, 500, NULL);

	UpdateMotionView(FALSE);
	g_pMotion->StartHome(eMaskXYZ);
	SetTimer(TIMER_HOME_END,100,NULL);

	//CCD重连
	g_ccd.InitCCD();
}


void CDlgMarkNormalPane::OnBnClickedButtonStopMove()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pMotion->Stop(eMaskXYZ);
	g_pMotion->WaitMoveFinish(eMaskXYZ);
	
}


void CDlgMarkNormalPane::OnBnClickedButtonZToFocus()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateMotionView(FALSE);
	g_pMotion->StartAMove(eMaskZ, m_pDoc->m_dbZCCDFocus);
	SetTimer(TIMER_MOVE_END, 100, NULL);
}

void CDlgMarkNormalPane::OnBnClickedButtonSwitchGasStatus() 
{
	// TODO: Add your control notification handler code here
	m_nGasStatus = !m_nGasStatus;
	UpdateData(FALSE);
	//RefreshGas();
}


void CDlgMarkNormalPane::SetLightStandby()
{
	m_bStandby = TRUE;
	g_IO.OUTPUT_LIGHT_RED(FALSE);
	g_IO.OUTPUT_LIGHT_YELLOW(TRUE);
	g_IO.OUTPUT_LIGHT_GREEN(FALSE);
	g_IO.OUTPUT_LIGHT_HUMMER(FALSE);

}

void CDlgMarkNormalPane::SetLightRun()
{
	g_IO.OUTPUT_LIGHT_RED(FALSE);
	g_IO.OUTPUT_LIGHT_YELLOW(FALSE);
	g_IO.OUTPUT_LIGHT_GREEN(TRUE);
	g_IO.OUTPUT_LIGHT_HUMMER(FALSE);
}

void CDlgMarkNormalPane::SetLightError()
{
	m_bStandby = FALSE;
	g_IO.OUTPUT_LIGHT_RED(TRUE);
	g_IO.OUTPUT_LIGHT_YELLOW(FALSE);
	g_IO.OUTPUT_LIGHT_GREEN(FALSE);
	g_IO.OUTPUT_LIGHT_HUMMER(TRUE);
}

void CDlgMarkNormalPane::SetGas(BOOL bOpen)
{
	if (!UpdateData())
	{
		m_nGasStatus = bOpen;
		UpdateData(FALSE);
	}
}


HRESULT CDlgMarkNormalPane::OnShowMarkingInfoBox(WPARAM wParam,LPARAM lParam)
{
	int nRtn = 0;
	m_bDisableOtherWindowInputOnMarking = FALSE;

	if (wParam==0)
	{
		nRtn = AfxMessageBox(LPCTSTR(lParam));
	}
	else if (wParam==1)
	{
		CCHandPos dlg;
		nRtn = (dlg.DoModal() == IDOK);
	}

	m_bDisableOtherWindowInputOnMarking = TRUE;
	return nRtn;
}

BOOL CDlgMarkNormalPane::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	/*if (pMsg->message == WM_KEYDOWN && pMsg->wParam != VK_TAB)
	{
		CMDIFrameWnd *pMainWnd=(CMDIFrameWnd *)AfxGetMainWnd();
		CMDIChildWnd *pChildWnd=pMainWnd->MDIGetActive();
		if(pChildWnd)
		{
			CHLDoc *pDoc=(CHLDoc *)pChildWnd->GetActiveDocument();
			if (pDoc != NULL)
			{
				if ( (GetKeyState(VK_SHIFT) & 0x8000) && (GetKeyState(VK_Z) & 0x8000) )
				{
					OnBnClickedButtonMark();
					return TRUE;
				}
			}
		}
	}*/

	/*UINT nNewMsg=0;
	static UINT nFirstDownKey=0;	
	if (pMsg->message==WM_KEYDOWN)
	{
	if (nFirstDownKey==0)
	{
	nNewMsg = WM_LBUTTONDOWN;
	nFirstDownKey = pMsg->wParam;
	}
	}
	if (pMsg->message==WM_KEYUP)
	{
	if (nFirstDownKey==pMsg->wParam)
	{
	nNewMsg = WM_LBUTTONUP;
	nFirstDownKey = 0;
	}
	}	

	if (nNewMsg)
	{
	BOOL bKeyDone=TRUE;
	switch (pMsg->wParam)
	{
	case VK_LEFT:
	m_left.SendMessage(nNewMsg);
	break;
	case VK_RIGHT:
	m_right.SendMessage(nNewMsg);
	break;
	case VK_UP:
	m_top.SendMessage(nNewMsg);
	break;
	case VK_DOWN:
	m_bottom.SendMessage(nNewMsg);
	break;
	default:
	bKeyDone=FALSE;
	}

	if (bKeyDone)
	{
	POSITION pos = m_pDoc->GetFirstViewPosition();
	CHLView *pView = dynamic_cast<CHLView*>(m_pDoc->GetNextView(pos));
	if (pView)	pView->Invalidate();
	return TRUE;
	}
	}
	*/


	if (pMsg->message==WM_KEYDOWN && pMsg->wParam!=VK_TAB)
	{
		if (pMsg->wParam==VK_RETURN)  ////回车键按键消息
		{
			CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OPERATORCODE);
			if (pMsg->hwnd==pEdit->GetSafeHwnd())
			{
				CString strText = "";
				SetDlgItemText(IDC_STATIC_WARNING_INFO, "");
				GetDlgItemText(IDC_EDIT_OPERATORCODE,strText); 	
				strText.TrimRight();
				strText.TrimLeft();

				pEdit->SetFocus();
				pEdit->SetSel(0,-1);
				pEdit->SetWindowText(_T(""));

				UpdateData(TRUE);
				//提前处理
				PreProcessBarcode(strText);

			}
			return FALSE;
		}
		return 0;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgMarkNormalPane::PreProcessBarcode(CString strScanText)
{
	strScanText.Trim();
	if (strScanText == "")
	{
		return FALSE;
	}
	if (m_arrMatrixMarkDoc.GetCount() > 0)
	{
		BOOL bOpenMatrixDoc = FALSE;
		CHLApp* pApp = (CHLApp *)AfxGetApp();
		for (int i = 0; i < m_arrMatrixMarkDoc.GetCount(); i++)
		{
			CMatrixMarkDoc MatrixDoc = m_arrMatrixMarkDoc.GetAt(i);
			if (-1 != MatrixDoc.strMatrix.Find(strScanText))
			{
				CFileFind finder;
				BOOL bWorking = finder.FindFile(MatrixDoc.strPath);
				if (bWorking)
				{
					m_pDoc = (CHLDoc *)pApp->OpenDocumentFile(MatrixDoc.strPath);
					CString strLog;
					strLog.Format("手动读取文档二维码：%s",strScanText);
					WriteRunLog(strLog);
					bOpenMatrixDoc = TRUE;
					break;
				}
			}
		}
		if (!bOpenMatrixDoc)
		{
			SetDlgItemText(IDC_STATIC_WARNING_INFO, "MatrixDoc isnot exist");
			SetLightError();
			return FALSE;
		}
	}
	return TRUE;
}


void CDlgMarkNormalPane::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();
	CDialogEx::OnLButtonDown(nFlags, point);
}

BOOL CDlgMarkNormalPane::CheckLeftLP()
{
	if (m_pDoc->m_dbLPDrawX[0]==0 && m_pDoc->m_dbLPDrawY[0]==0)
	{
		AfxMessageBox("Not set Mark point coordinates (product)!");
		return FALSE;
	}
	return TRUE;
}

void CDlgMarkNormalPane::RefreshRefPointsListBox(int nSel)
{
	m_lbxRefPoints.ResetContent();
	for (int i=0; i<m_pDoc->m_nRefPointNum; i++)
	{
		CString strItem;
		strItem.Format("%02d (%.3f,%.3f)", i+1, m_pDoc->m_arrDocRefPointX[i], m_pDoc->m_arrDocRefPointY[i]);
		m_lbxRefPoints.AddString(strItem);
	}
	m_lbxRefPoints.SetCurSel(nSel);

	OnSelchangeListRefPoints();		//代码设置选中项时，不会自动调用改函数

	BOOL bEnalbeHandOP = (m_lbxRefPoints.GetCount()!=0);
	GetDlgItem(IDC_BUTTON_HADN_POINT)->EnableWindow(bEnalbeHandOP);
	GetDlgItem(IDC_BUTTON_HAND_OK_POINT)->EnableWindow(bEnalbeHandOP);	
	GetDlgItem(IDC_BUTTON_CHECK_POINT)->EnableWindow(bEnalbeHandOP);
	GetDlgItem(IDC_BUTTON_SELECT_TOCCD)->EnableWindow(bEnalbeHandOP);
}

void CDlgMarkNormalPane::OnBnClickedButtonRefPointAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!CheckLeftLP())
		return;
	
	if (m_lbxRefPoints.GetCount()>0 && m_lbxRefPoints.GetCurSel()<0)
		return;

	double dbPosX = g_pMotion->GetPos(eMaskX) + m_pDoc->m_dbLPDrawX[0];
	double dbPosY = g_pMotion->GetPos(eMaskY) + m_pDoc->m_dbLPDrawY[0];

	int nInsertIndex = m_lbxRefPoints.GetCurSel()+1;
	for (int i=m_pDoc->m_nRefPointNum; i>nInsertIndex; i--)
	{
		m_pDoc->m_arrDocRefPointX[i] = m_pDoc->m_arrDocRefPointX[i-1];
		m_pDoc->m_arrDocRefPointY[i] = m_pDoc->m_arrDocRefPointY[i-1];
	}
	m_pDoc->m_arrDocRefPointX[nInsertIndex] = dbPosX;
	m_pDoc->m_arrDocRefPointY[nInsertIndex] = dbPosY;
	m_pDoc->m_nRefPointNum++;

	RefreshRefPointsListBox(nInsertIndex);	
}


void CDlgMarkNormalPane::OnBnClickedButtonRefPointArray()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!CheckLeftLP())
		return;

	if (m_lbxRefPoints.GetCount()<0 || m_lbxRefPoints.GetCurSel()<0)
		return;

	int nInsertIndex = m_lbxRefPoints.GetCurSel();

	CPtpDlg dlg;
	dlg.m_dbX = m_pDoc->m_arrDocRefPointX[nInsertIndex];
	dlg.m_dbY = m_pDoc->m_arrDocRefPointY[nInsertIndex];
	if(dlg.DoModal()==IDOK)
	{
		m_pDoc->m_arrDocRefPointX[nInsertIndex] = dlg.m_dbX;
		m_pDoc->m_arrDocRefPointY[nInsertIndex] = dlg.m_dbY;
		RefreshRefPointsListBox(0);
	}
	

}


void CDlgMarkNormalPane::OnBnClickedButtonRefPointDel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_lbxRefPoints.GetCount()<=0 || m_lbxRefPoints.GetCurSel()<0)
		return;

	int nDelIndex = m_lbxRefPoints.GetCurSel();

	m_pDoc->m_nRefPointNum--;
	for (int i=nDelIndex; i<m_pDoc->m_nRefPointNum; i++)
	{
		m_pDoc->m_arrDocRefPointX[i] = m_pDoc->m_arrDocRefPointX[i+1];
		m_pDoc->m_arrDocRefPointY[i] = m_pDoc->m_arrDocRefPointY[i+1];
	}
	if (nDelIndex==m_pDoc->m_nRefPointNum)
		nDelIndex--;

	RefreshRefPointsListBox(nDelIndex);
}


void CDlgMarkNormalPane::OnBnClickedButtonZToFocus2()
{
	// TODO: 在此添加控件通知处理程序代码
	double dbZ = m_pDoc->GetFirstMarkFocus();
	UpdateMotionView(FALSE);
	g_pMotion->StartAMove(eMaskZ, dbZ);
	SetTimer(TIMER_MOVE_END, 100, NULL);
}


void CDlgMarkNormalPane::OnSelchangeListRefPoints()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSel = m_lbxRefPoints.GetCurSel();
	if (nCurSel<0)
		return;

	g_ParaIni.m_dbRefPointX = m_pDoc->m_arrDocRefPointX[nCurSel];
	g_ParaIni.m_dbRefPointY = m_pDoc->m_arrDocRefPointY[nCurSel];
}


BOOL CDlgMarkNormalPane::CheckMulDoc()
{
	CStringArray arrDoc;
	m_arrDocPoint.RemoveAll();

	CHLDoc *pDoc;
	CMultiDocTemplate *pDocTem;
	CWinApp *pApp = AfxGetApp();
	POSITION posTem = pApp->GetFirstDocTemplatePosition();
	while(posTem != NULL)
	{
		pDocTem = (CMultiDocTemplate *)pApp->GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while(posDoc != NULL)
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			m_arrDocPoint.Add(pDoc);

			if(!pDoc->GetPathName().IsEmpty())
				arrDoc.Add(pDoc->GetPathName());
		}
	}

	if (g_arrMulDoc.GetSize()!=arrDoc.GetSize())
		return FALSE;

	for (int i=0; i<g_arrMulDoc.GetSize(); i++)
	{
		if (g_arrMulDoc.GetAt(i)!=arrDoc.GetAt(i))
			return FALSE;
	}
	return TRUE;
}

void CDlgMarkNormalPane::OnBnClickedBtnLoadscan()
{
	// TODO: 在此添加控件通知处理程序代码
	if (AfxMessageBox("Confirm?", MB_YESNO)==IDNO)
		return;

	DWORD dwInfoX, dwInfoY;
	double dbX = g_pMotion->GetPos(eMaskX, dwInfoX);
	double dbY = g_pMotion->GetPos(eMaskY, dwInfoY);

	g_pMotion->WritePara("XY", "ScanPtX", dbX);
	g_pMotion->WritePara("XY", "ScanPtY", dbY);

	m_pDoc->m_dbScanPtX = dbX;
	m_pDoc->m_dbScanPtY = dbY;

}


void CDlgMarkNormalPane::GetIOinfo(void)
{
	CMDIFrameWnd *pMainWnd=(CMDIFrameWnd *)AfxGetMainWnd();
	CMDIChildWnd *pChildWnd=pMainWnd->MDIGetActive();
	if(pChildWnd)
	{
		CHLDoc *pDoc=(CHLDoc *)pChildWnd->GetActiveDocument();
		if (pDoc != NULL)
		{
			//接收上料信号
			if (g_IO.INPUT_GOLOADPT())
			{
				if (!m_bgoLoadPt)
				{
					if (WaitForSingleObject(g_hEventMainThreadEmergency, 0)!=WAIT_OBJECT_0)
					{
						m_bgoLoadPt = TRUE;

						CString strError;
						strError.LoadStringA(IDS_INPUTPRODUCT);
						SetDlgItemText(IDC_STATIC_WARNING_INFO, strError);

						g_IO.OUTPUT_LOADPT(FALSE);
						Sleep(50);
						OnBnClickedButtonLoadPosition();
					}
				}
			}
			else
			{
				m_bgoLoadPt  = FALSE;
			}

			//接收下料信号
			if (g_IO.INPUT_GOUNLOADPT())
			{
				if (!m_bgoUnLPt)
				{
					if (WaitForSingleObject(g_hEventMainThreadEmergency, 0)!=WAIT_OBJECT_0)
					{
						m_bgoUnLPt = TRUE;

						CString strError;
						strError.LoadStringA(IDS_GETPRODUCT);
						SetDlgItemText(IDC_STATIC_WARNING_INFO, strError);					
		
						g_IO.OUTPUT_UNLOADPT(FALSE);
						Sleep(50);
						OnBnClickedButtonUnloadPosition();
					}
				}
			}
			else
			{
				m_bgoUnLPt  = FALSE;
			}


			//打标开始信号
			if (g_IO.INPUT_MARK())
			{
				if (!m_bInputMark)
				{
					if (WaitForSingleObject(g_hEventMainThreadEmergency, 0)!=WAIT_OBJECT_0)
					{
						m_bInputMark = TRUE;
						if (!UpdateData())	
							return;
						
						WriteRunLog("\n\n\n", eLogDataFlow);
						WriteRunLog("Receiving IO_INPUT_MARK", eLogDataFlow);
						m_bFirstPts = FALSE; //从第0个参考点开始打
						Mark();
					}
				}
			}
			else
			{
				m_bInputMark  = FALSE;
			}
		}
		else
		{
			g_IO.OUTPUT_SOFT_OK(FALSE);
		}
	}



	//清除错误提示框
	if (g_IO.INPUT_RESET_ERROR())
	{
		if (!m_bInputError)
		{
			m_bInputError = TRUE;
			for (int i = 0; i < 10; i ++)
			{
				CWnd *cwndError = NULL;
				cwndError = FindWindow(NULL,"Error");
				if (NULL != cwndError)
				{
					cwndError->SendMessage(WM_CLOSE);
					cwndError= NULL;
				}

				CWnd *cwndWarn = NULL;
				cwndWarn = FindWindow(NULL,"Warning");
				if (NULL != cwndWarn)
				{
					cwndWarn->SendMessage(WM_CLOSE);
					cwndWarn= NULL;
				}
			}

			SetLightStandby();
			
			g_IO.OUTPUT_SOFT_OK(TRUE);
		}
	}
	else
	{
		m_bInputError  = FALSE;
	}
}

HRESULT CDlgMarkNormalPane::OnEmergencyEventHandler(WPARAM wParam,LPARAM lParam)
{	
	m_bMarking = FALSE;
	g_IO.OUTPUT_SOFT_OK(TRUE);

	SetLightError();

	CWnd *cwnd = NULL;
	cwnd = FindWindow(NULL,"Warning");
	if (cwnd == NULL)
	{
		m_strMarking = "";
		::ShowWindow(GetDlgItem(IDC_STATIC_MARKING)->GetSafeHwnd(), SW_HIDE); 
		UpdateData(FALSE);

		CDlgEmergencyWarning dlg;
		dlg.DoModal();
	}
	return 0;
}



void CDlgMarkNormalPane::OnBnClickedBtnCcdview()
{
	// TODO: 在此添加控件通知处理程序代码
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	pApp->OnCcdView();
}


void CDlgMarkNormalPane::ConnectNet()
{
	if (!m_bConnected)
	{
		m_sockClient.SetSocket(1,m_strIPAddress,m_nPort);
		m_bConnected = m_sockClient.Connet();
	}
	ShowConnect();
}

void CDlgMarkNormalPane::ShowConnect()
{
	CString strNet = m_NetInfo;
	if (m_bConnected)
	{
		m_NetInfo = "NetIsConnect";
	}
	else
	{
		m_NetInfo = "NetDisConnect";
	}
	if (strNet != m_NetInfo)
	{
		UpdateData(FALSE);
	}
}


void CDlgMarkNormalPane::DisConnectNet()
{
	if (m_bConnected)
	{
		m_bConnected=FALSE;
		m_sock.CloseSock();
	}
}

BOOL CDlgMarkNormalPane::LoadFileData()
{
	if (m_pDoc->m_bUseFile)
	{
		if (m_pDoc->m_strFileDataPath != "")
		{
			if (!m_pDoc->LoadFileDataChange(m_pDoc->m_strFileDataPath))
			{
				return FALSE;
			}
		}
		else
		{
			AfxMessageBox("No Use File Import Data！");
			return FALSE;
		}
	}
	else
	{
		AfxMessageBox("No Use File Import Data！");
		return FALSE;
	}
	return TRUE;
}

void CDlgMarkNormalPane::OnBnClickedBtnLfdata()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateMotionView(FALSE);
	g_pMotion->StartXY(g_ParaIni.m_dbWorkPosX, g_ParaIni.m_dbWorkPosY, eSpeedFast);
	SetTimer(TIMER_MOVE_END, 100, NULL);
}


void CDlgMarkNormalPane::OnDestroy()
{
	CDialogEx::OnDestroy();

	g_ParaIni.WriteString(_T("STRSENDLAST"),_T("strSendLast"),m_strSendLast);

	DisConnectNet();
	g_EmergencyWatcher.StopWatcher();
	g_pDlgMarkNormalPane = NULL;

}



HBRUSH CDlgMarkNormalPane::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	int nId = pWnd->GetDlgCtrlID();
	if (nId == IDC_STATIC_MARKING || nId == IDC_STATIC_WARNING_INFO)
	{
		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CDlgMarkNormalPane::PreNetSendRecv(CString & strCode)
{
	
	if (m_bIsNet)
	{
		if (m_strOperatorcode == "")
		{
			SetDlgItemText(IDC_STATIC_WARNING_INFO, "Operator code is not input");
			return FALSE;
		}
		DisConnectNet();
		ConnectNet();
		if (m_bConnected)
		{
			if (m_strSendLast != strCode)
			{
				if (!m_pDoc->NetSendRecvOpenDoc())
				{
					SetDlgItemText(IDC_STATIC_WARNING_INFO, "OpenFileNetRecvError");
					return FALSE;
				}
				m_strSendLast = strCode;
				int nCount = m_pDoc->m_strNetRecv.GetCount();
				m_pDoc->m_strNetLastRecv.RemoveAll();
				for (int i = 0 ; i < nCount; i++)
				{
					m_pDoc->m_strNetLastRecv.Add( m_pDoc->m_strNetRecv.GetAt(i));
				}
			}
			else
			{
				int nCount = m_pDoc->m_strNetLastRecv.GetCount();
				m_pDoc->m_strNetRecv.RemoveAll();
				for (int i = 0 ; i < nCount; i++)
				{
					m_pDoc->m_strNetRecv.Add( m_pDoc->m_strNetLastRecv.GetAt(i));
				}
			}
		}
		else
		{
			SetDlgItemText(IDC_STATIC_WARNING_INFO, "NetIsDisConnect");
			return FALSE;
		}
		DisConnectNet();

		CString strDocName = m_pDoc->m_strNetRecv.GetAt(0);
		int nRet = strDocName.Find(":");
		if (nRet == -1)
		{
			strDocName = "D:\\Doc\\" + strDocName;
		}
		if (strDocName != m_pDoc->GetPathName())
		{
			CHLApp* pApp = (CHLApp *)AfxGetApp();

			//strDocName = "D:\\Doc\\123.hs";
			CHLDoc *pOldDoc = m_pDoc;

			CFileFind finder;
			BOOL bWorking = finder.FindFile(strDocName);
			if (bWorking)
			{
				m_pDoc = (CHLDoc *)pApp->OpenDocumentFile(strDocName);
				int nCount = pOldDoc->m_strNetRecv.GetCount();
				m_pDoc->m_strNetRecv.RemoveAll();
				for (int i = 0 ; i < nCount; i++)
				{
					m_pDoc->m_strNetRecv.Add( pOldDoc->m_strNetRecv.GetAt(i));
				}
			}
			else
			{
				SetDlgItemText(IDC_STATIC_WARNING_INFO, "CurDocument isnot exist or Path is not correct");
				return FALSE;
			}
		}
	}

	return TRUE;
}
BOOL CDlgMarkNormalPane::Autooffset()
{
	CHLApp *pApp = (CHLApp *)AfxGetApp();
	double dbDis = pApp->GetProfileDouble("AUTOOFFSETDIS","AutoOffsetDis",1);

	CString strPath = "d:\\AutoOffset.dat";

	CString strNewfile = "d:\\tempAutoOffset.dat";
	DeleteFile(strNewfile);
	if (!CopyFile(strPath,strNewfile,TRUE))
	{
		SetDlgItemText(IDC_STATIC_WARNING_INFO, "CopyFile Fail");
		return FALSE;
	}

	CStdioFile fReadtxt,fWritetxt;
	BOOL bReadOpen = fReadtxt.Open(strNewfile,CFile::modeRead);
	BOOL bWriteOpen = fWritetxt.Open(strPath,CFile::modeCreate | CFile::modeWrite);

	
	BOOL bAutoOffSet = FALSE;

	if (bReadOpen  && bWriteOpen)
	{
		int nLength = static_cast<int>(fReadtxt.GetLength());
		CString strData;
		fReadtxt.ReadString(strData);
		strData.Trim();
		if (strData != "")
		{
			fWritetxt.WriteString(strData);
			fWritetxt.WriteString((LPTSTR)("\r\n"));
		}
		int nRetRow = strData.Find("*");
		int nRetCol = strData.Find("#");
		CString strLtPosXY = strData.Mid(0,nRetRow);
		int nRet = strLtPosXY.Find(",");
		double dbLTposX = _ttof(strLtPosXY.Mid(0,nRet));
		double dbLTposY = _ttof(strLtPosXY.Mid(nRet+1));

		CString strRow = strData.Mid(nRetRow + 1,nRetCol-nRetRow);
		int nAllRow = _ttoi(strRow);
		strData = strData.Mid(nRetCol + 1);
		int nAllCol = _ttoi(strData);


		BOOL bBreak = FALSE;
		BOOL bReadWrite = FALSE;
		for (int i = 0; i < nAllRow; i++)
		{
			if (bBreak)
			{
				break;
			}

			for (int j = 0; j < nAllCol; j++)
			{
				fReadtxt.ReadString(strData);
				strData.Trim();
				if (strData == "")
				{
					bBreak = TRUE;
					break;
				}
				if (bReadWrite)
				{
					fWritetxt.WriteString(strData);
					fWritetxt.WriteString((LPTSTR)("\r\n"));
					continue;
				}

				if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
				{
					WriteRunLog("人工中止自动校正!");
					SetDlgItemText(IDC_STATIC_WARNING_INFO, "Handle Stop");
					bReadWrite = TRUE;
					fWritetxt.WriteString(strData);
					fWritetxt.WriteString((LPTSTR)("\r\n"));
					continue;
				}
				int nRet = strData.Find("/");
				CString strFlag = strData.Mid(nRet + 1);
				strData = strData.Left(nRet);

				double CurX = dbLTposX - dbDis * j;
				double CurY = dbLTposY + dbDis * i;

				if (strFlag == "0")
				{
					g_pMotion->LnXY(CurX,CurY);
					Sleep(g_ParaIni.m_nMotionDelayForMark+3000);

					double dbCaptureX = CurX+g_ParaIni.m_dbMarkToCCD1X;
					double dbCaptureY = CurY+g_ParaIni.m_dbMarkToCCD1Y;

					BOOL bDevOk  = m_pDoc->NoTicpsPointCCD1Capture(dbCaptureX, dbCaptureY);

					double dbNewMarkToCCD1X = dbCaptureX - CurX;
					double dbNewMarkToCCD1Y = dbCaptureY - CurY;
					if (bDevOk)
					{
						bDevOk = m_pDoc->isDeviationOK(dbNewMarkToCCD1X,dbNewMarkToCCD1Y,FALSE);
						//if (bDevOk)
						{
							strData += "/1";
							fWritetxt.WriteString(strData);
							fWritetxt.WriteString((LPTSTR)("\r\n"));
							continue;
						}
					}
					if (!m_pDoc->MoveMarkPos())
					{
						strData += "/0";
						fWritetxt.WriteString(strData);
						fWritetxt.WriteString((LPTSTR)("\r\n"));
						continue;
					}

					CString strTip;
					double dbMarkCrossPosX = g_pMotion->GetPos(eMaskX);
					double dbMarkCrossPosY = g_pMotion->GetPos(eMaskY);
					strTip.Format("打标(%.3f,%.3f)", dbMarkCrossPosX, dbMarkCrossPosY);
					WriteRunLog(strTip);

					OffsetMark();

					if (!m_pDoc->AfterMarkMove())
					{
						strData += "/0";
						fWritetxt.WriteString(strData);
						fWritetxt.WriteString((LPTSTR)("\r\n"));
						continue;
					}

					////移动到CCD中心	
					dbCaptureX = dbMarkCrossPosX+g_ParaIni.m_dbMarkToCCD1X;
					dbCaptureY = dbMarkCrossPosY+g_ParaIni.m_dbMarkToCCD1Y;
					if(!m_pDoc->NoTicpsPointCCD1Capture(dbCaptureX, dbCaptureY))
					{
						WriteRunLog("移动十字至CCD中心失败!");
						strData += "/0";
						fWritetxt.WriteString(strData);
						fWritetxt.WriteString((LPTSTR)("\r\n"));
						continue;
					}
					dbNewMarkToCCD1X = dbCaptureX - dbMarkCrossPosX;
					dbNewMarkToCCD1Y = dbCaptureY - dbMarkCrossPosY;
					strTip.Format("CCD(%.3f,%.3f)", dbCaptureX, dbCaptureY);
					WriteRunLog(strTip);
					strTip.Format("当前测量的“打标中心到CCD中心距离”：(%.3f,%.3f)", dbNewMarkToCCD1X, dbNewMarkToCCD1Y);
					WriteRunLog(strTip);
					bDevOk = m_pDoc->isDeviationOK(dbNewMarkToCCD1X,dbNewMarkToCCD1Y,TRUE);

					if (bDevOk)
					{
						strData += "/1";
						bReadWrite = TRUE;
						fWritetxt.WriteString(strData);
						fWritetxt.WriteString((LPTSTR)("\r\n"));
						bAutoOffSet = TRUE;
					}
				}
				else
				{
					strData += "/1";
					fWritetxt.WriteString(strData);
					fWritetxt.WriteString((LPTSTR)("\r\n"));
				}
			}
		}

		fReadtxt.Close();
		fWritetxt.Close();
		DeleteFile(strNewfile);

	//	if (g_sys.m_nLaserCurrentMode)	
		{
			if (g_sys.m_nLaserCurrentKeep)//维持电流
			{
				SetCurrent(g_sys.m_nLaserCurrentKeep);
				m_oldMarkPara.dbCurrent = g_sys.m_nLaserCurrentKeep;
			}
		}	
	}
	return bAutoOffSet;
}


void CDlgMarkNormalPane::OnBnClickedBtnAutooffset()
{
	// TODO: 在此添加控件通知处理程序代码

	if (AfxMessageBox("Will AutoStartOffsetcal  in the current location ", MB_OKCANCEL)==IDCANCEL)
		return;
	ResetEvent(hStop);

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());

	CHLApp *pApp = (CHLApp *)AfxGetApp();
	CString strPath;
	strPath = pApp->GetProfileString("AUTOOFFSETPATH","AutoOffsetPath","");
	double dbDis = pApp->GetProfileDouble("AUTOOFFSETDIS","AutoOffsetDis",1);

	m_pOldDoc = m_pDoc;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strPath);
	if (bWorking)
	{
		m_pDoc = (CHLDoc *)pApp->OpenDocumentFile(strPath);
		int nCCDNum = pApp->GetProfileInt("AUTOOFFSETCCDNUM","AutoOffsetCCDNum",-1);
		m_pDoc->Setlight(m_pDoc->m_nLight1,m_pDoc->m_nLight2,m_pDoc->m_nLight3,m_pDoc->m_nLight4);
		g_ccd.SetWorkProgNo(nCCDNum);

		CString strPath = "d:\\AutoOffset.dat";
		CFileFind finder;
		BOOL bWorking = finder.FindFile(strPath);
		if (!bWorking)
		{
			AfxMessageBox("No SetPos!");
			m_pDoc = m_pOldDoc;
			m_pOldDoc = NULL;
			return;
		}

		//pView->MarkTicps(TRUE,FALSE);
		UpdateMotionView(FALSE);
		SetDlgItemText(IDC_STATIC_WARNING_INFO, "");

		SetEvent(hAutoOffset);
		SetTimer(TIMER_AUTOOFFSET,1000,NULL);
	}
	else
	{
		AfxMessageBox("OpenDocument Fail!");
		m_pDoc = m_pOldDoc;
		m_pOldDoc = NULL;
	}
}


void CDlgMarkNormalPane::OffsetMark(void)
{
	//if (g_sys.m_nLaserCurrentMode)	
	{
		PARA curMarkPara = m_pDoc->GetLay().GetPara(0);
		if(m_oldMarkPara.dbCurrent != curMarkPara.dbCurrent)
		{		
			if (!SetCurrent(curMarkPara.dbCurrent)) 
				return ;
			m_oldMarkPara.dbCurrent = curMarkPara.dbCurrent;
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

	g_IO.OUTPUT_SOFT_OK(TRUE);
	Sleep(1000);

	if (m_pDoc->m_bEnableMarkFocus)
		g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);
}

void CDlgMarkNormalPane::OnBnClickedBtnOpclaserReset()
{
	// TODO: 在此添加控件通知处理程序代码

	//m_pDoc->m_chain.SetStrokeLayFreq();



	BOOL bRet = g_OPCLaser->Reset();
	if (!bRet)
	{
		AfxMessageBox(_T("Reset Error!"));
	}
}


void CDlgMarkNormalPane::OnBnClickedButtonLightStandby()
{
	// TODO: 在此添加控件通知处理程序代码
	SetLightStandby();
	SetDlgItemText(IDC_STATIC_WARNING_INFO, "");
	g_IO.OUTPUT_SOFT_OK(TRUE);
}


void CDlgMarkNormalPane::OnBnClickedButtonMoveScanpt()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateMotionView(FALSE);
	g_pMotion->LnXY(m_pDoc->m_dbScanPtX,m_pDoc->m_dbScanPtY,eSpeedFast);
	m_pDoc->ScanMatri(FALSE);
	UpdateMotionView(TRUE);
}

BOOL CDlgMarkNormalPane::ConnectDatebase()
{
	DBPARA  para;
	CHlParaIni ini;
	para._Database = _bstr_t(ini.GetDbDatabase());
	para._Server = _bstr_t(ini.GetDbServer());
	para._User  = _bstr_t(ini.GetDbLoginUser());
	para._Pwd   = _bstr_t(ini.GetDbLoginPwd());
	para.nDriverType = ini.GetDbDataType();
	if (!m_adoDB.IsDBConnected()) //未连接，则连接数据库
	{
		if (!m_adoDB.ConnectToDB(para,TRUE))
		{
			AfxMessageBox(_T("打开数据库失败，请重设参数!"));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgMarkNormalPane::SaveMarkCode(CString strVinCode)
{

	strVinCode.Trim();
	if (strVinCode == "")
	{
		return FALSE;
	}

	CString strCode = strVinCode;
	CString strSQL;
	BOOL  bMarkOk=FALSE;
	if (!m_adoDB.IsDBConnected())
	{
		AfxMessageBox(_T("打开数据库失败，请重设参数!"));
		return FALSE;
	}
	strSQL.Format(_T("INSERT INTO tb_MarkData(VinCode,UpdateDate) VALUES(\'%s\',now())"),strVinCode);
	if (!m_adoDB.OpenSQLQuery((_bstr_t)strSQL,m_recordset))
	{
		AfxMessageBox(_T("数据库表文件不存在，请重新选择数据库"));
		return FALSE;
	}
	return TRUE;
}


void CDlgMarkNormalPane::OnBnClickedButtonRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgCodeRecord codeRecord;
	codeRecord.DoModal();
}


void CDlgMarkNormalPane::OnBnClickedButtonDust()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bOpenDust)
	{
		g_IO.OUTPUT_SMOKE_FILTER(TRUE);
		m_bOpenDust = TRUE;
		CString str1;
		str1.LoadString(IDS_DUST_COLLECTOR_OFF);
		GetDlgItem(IDC_BUTTON_DUST)->SetWindowText(str1);
	}
	else
	{
		g_IO.OUTPUT_SMOKE_FILTER(FALSE);
		m_bOpenDust = FALSE;
		CString str1;
		str1.LoadString(IDS_DUST_COLLECTOR_ON);
		GetDlgItem(IDC_BUTTON_DUST)->SetWindowText(str1);
	}
}
