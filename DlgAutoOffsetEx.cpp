// DlgAutoOffsetEx.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgAutoOffsetEx.h"
#include "afxdialogex.h"

#include "CCDCalibrate.h"

#include "DlgMarkNormalPane.h"


#define TIMER_DLG_INI 2
#define List_initCol  10

// CDlgAutoOffsetEx 对话框

IMPLEMENT_DYNAMIC(CDlgAutoOffsetEx, CDialogEx)

CDlgAutoOffsetEx::CDlgAutoOffsetEx(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAutoOffsetEx::IDD, pParent)
	, m_dbLfPos(0)
	, m_dbRtPos(0)
	, m_strPath(_T(""))
	, m_dbLTposX(0)
	, m_dbLtPosY(0)
	, m_dbRBPosX(0)
	, m_dbRBPosY(0)
	, m_dbDis(1)
	, m_nAllCol(0)
	, m_nAllRow(0)
	, m_nSelCol(0)
	, m_nSelRow(0)
	, m_nCCDNum(-1)
{
	m_dbHandMoveDis = 1.0;
	m_dbLastX = 0.0;
	m_dbLastY = 0.0;
	m_bInit = FALSE;
	m_left.SetOper(MOVE_LEFT);
	m_right.SetOper(MOVE_RIGHT);
	m_top.SetOper(MOVE_TOP);
	m_bottom.SetOper(MOVE_BOTTOM);

	m_pApp = NULL;
}

CDlgAutoOffsetEx::~CDlgAutoOffsetEx()
{
}

void CDlgAutoOffsetEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoOffsetEx)
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
	DDX_Text(pDX, IDC_EDIT_OFFSET_PATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_LTPOSX, m_dbLTposX);
	DDX_Text(pDX, IDC_EDIT_LTPOSY, m_dbLtPosY);
	DDX_Text(pDX, IDC_EDIT_DIS, m_dbDis);
	DDX_Control(pDX, IDC_LIST_POSINFO, m_ListPos);
	DDX_Text(pDX, IDC_EDIT_ALLCOL, m_nAllCol);
	DDX_Text(pDX, IDC_EDIT_ALLROW, m_nAllRow);
	DDX_Text(pDX, IDC_EDIT_SELCOL, m_nSelCol);
	DDX_Text(pDX, IDC_EDIT_SELROW, m_nSelRow);
	DDX_Text(pDX, IDC_EDIT_CCDNUM, m_nCCDNum);
}


BEGIN_MESSAGE_MAP(CDlgAutoOffsetEx, CDialogEx)
	//{{AFX_MSG_MAP(CDlgAutoOffsetEx)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_HAND_MOVE_DIS, OnChangeEditHandMoveDis)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL, OnRadioMoveModel)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL1, OnRadioMoveModel1)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL2, OnRadioMoveModel2)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL3, OnRadioMoveModel3)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_LOAD, &CDlgAutoOffsetEx::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_AUTO_START, &CDlgAutoOffsetEx::OnBnClickedBtnAutoStart)
	ON_BN_CLICKED(IDC_BTN_SET_LTPOS, &CDlgAutoOffsetEx::OnBnClickedBtnSetLtpos)
	//ON_BN_CLICKED(IDOK, &CDlgAutoOffset::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_RESET_ALLPOS, &CDlgAutoOffsetEx::OnBnClickedBtnResetAllpos)
	ON_NOTIFY(NM_CLICK, IDC_LIST_POSINFO, &CDlgAutoOffsetEx::OnNMClickListPosinfo)
	ON_BN_CLICKED(IDC_BTN_MARK, &CDlgAutoOffsetEx::OnBnClickedBtnMark)
	ON_BN_CLICKED(IDC_BTN_SELTOCCD, &CDlgAutoOffsetEx::OnBnClickedBtnSeltoccd)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgAutoOffsetEx 消息处理程序

BOOL CDlgAutoOffsetEx::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_dbOldHandMoveDis = CFStatic::m_dbMoveDis;

	CFStatic::m_nMoveModel = 2;
	CheckDlgButton(IDC_RADIO_MOVE_MODEL2, TRUE);
	CFStatic::m_dbMoveDis = m_dbHandMoveDis = 1.0;

	m_dbLastX = g_ParaIni.m_dbAutoOffsetLastX;
	m_dbLastY = g_ParaIni.m_dbAutoOffsetLastY;
	UpdateData(FALSE);

	SetTimer(TIMER_DLG_INI, 10, NULL);

	m_pApp = (CHLApp *)AfxGetApp();

	g_pMotion->GetPara("XY", "AutoOfferRightPtX", m_dbRtPos);
	g_pMotion->GetPara("XY", "AutoOfferLeftPtX", m_dbLfPos);


	WriteRunLog("\n\n\n");
	CString strLog;
	strLog.Format("进入偏移校正，校正文档(%s)", m_pDoc->GetPathName());
	WriteRunLog(strLog);

	LONG lStyle;
	lStyle = GetWindowLong(m_ListPos, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_ListPos, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListPos.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_ListPos.SetExtendedStyle(dwStyle); //设置扩展风格


	for (int i = 0; i < List_initCol; i++)
	{
		CString strCol;
		strCol.Format("Col%d",i+1);
		m_ListPos.InsertColumn(i, strCol, LVCFMT_CENTER, 100);
	}

	m_on = AfxGetApp()->LoadIcon(IDI_ICON26);
	m_off = AfxGetApp()->LoadIcon(IDI_ICON27);

	m_image.Create(16,16,ILC_COLOR24|ILC_MASK,1,1);

	m_image.SetBkColor(RGB(255,255,255)); 
	m_image.Add(m_on);//icon变量
	m_image.Add(m_off);//icon变量

	m_ListPos.SetImageList(&m_image,LVSIL_SMALL);

	m_dbLTposX = m_pApp->GetProfileDouble("AUTOOFFSETLTPOSX","AutoOffsetLtPosX",0);
	m_dbLtPosY = m_pApp->GetProfileDouble("AUTOOFFSETLTPOSY","AutoOffsetLtPosY",0);
	m_dbDis = m_pApp->GetProfileDouble("AUTOOFFSETDIS","AutoOffsetDis",1);
	m_strPath = m_pApp->GetProfileString("AUTOOFFSETPATH","AutoOffsetPath","");

	m_nCCDNum = m_pApp->GetProfileInt("AUTOOFFSETCCDNUM","AutoOffsetCCDNum",-1);

	m_pDoc->Setlight(m_pDoc->m_nLight1,m_pDoc->m_nLight2,m_pDoc->m_nLight3,m_pDoc->m_nLight4);
	g_ccd.SetWorkProgNo(m_nCCDNum);


	ReadFileData();

	if (m_strPath != "")
	{

		CFileFind finder;
		BOOL bWorking = finder.FindFile(m_strPath);
		if (bWorking)
		{
			m_pApp->OpenDocumentFile(m_strPath);
			g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);
		}
		else
		{
			AfxMessageBox("OpenDocumentFile Fail! CurDocumentFile isnot exist");
		}
	}
	UpdateData(FALSE);
	return TRUE; 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgAutoOffsetEx::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent==TIMER_DLG_INI)
	{
		KillTimer(TIMER_DLG_INI);

		CString strTip;
		g_pDlgMarkNormalPane->SetGas(TRUE);

		g_pMotion->LnXY(m_dbLastX, m_dbLastY);
		g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);
		m_bInit = TRUE;

		strTip.Format("上次标记位置(%.3f,%.3f), CCD焦点位置(%.3f)", m_dbLastX, m_dbLastY, m_pDoc->m_dbZCCDFocus);
		WriteRunLog(strTip);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgAutoOffsetEx::OnChangeEditHandMoveDis() 
{
	CString strMoveDis;
	GetDlgItemText(IDC_EDIT_HAND_MOVE_DIS, strMoveDis);
	CFStatic::m_dbMoveDis = atof(strMoveDis);

	// TODO: Add your control notification handler code here
}


void CDlgAutoOffsetEx::OnDestroy() 
{
	CDialog::OnDestroy();

	if (g_sys.m_nLaserCurrentKeep)//维持电流
	{
		g_pDlgMarkNormalPane->SetCurrent(g_sys.m_nLaserCurrentKeep);
		m_oldMarkPara.dbCurrent = g_sys.m_nLaserCurrentKeep;
	}

	//CFStatic::m_nMoveModel = m_nOldMoveModel;
	CFStatic::m_dbMoveDis = m_dbOldHandMoveDis;

	if (m_bInit)
	{
		g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);	

		CString strProgID = (m_pDoc->m_nCCDUseModel==2) ? m_pDoc->m_strProgID2 : m_pDoc->m_strProgID1;
		if (!g_ccd.SetWorkProgNo(atoi(strProgID)))
			AfxMessageBox(" Exchange CCD Fail!");	
	}
	UpdateData(TRUE);
	m_pDoc->Setlight(m_pDoc->m_nLight1,m_pDoc->m_nLight2,m_pDoc->m_nLight3,m_pDoc->m_nLight4);
	g_ccd.SetWorkProgNo(atoi(m_pDoc->m_strProgID1));
}

void CDlgAutoOffsetEx::OnRadioMoveModel() 
{
	// TODO: Add your control notification handler code here
	CFStatic::m_nMoveModel = 0;
}

void CDlgAutoOffsetEx::OnRadioMoveModel1() 
{
	// TODO: Add your control notification handler code here
	CFStatic::m_nMoveModel = 1;
}

void CDlgAutoOffsetEx::OnRadioMoveModel2() 
{
	// TODO: Add your control notification handler code here
	CFStatic::m_nMoveModel = 2;
}

void CDlgAutoOffsetEx::OnRadioMoveModel3() 
{
	// TODO: Add your control notification handler code here
	CFStatic::m_nMoveModel = 3;
}

void CDlgAutoOffsetEx::OnBnClickedBtnLoad()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strFilter = "HS File(*.hs)|*.hs|All Files(*.*)|*.*||";

	CFileDialog dlg(TRUE,NULL,m_strPath,OFN_HIDEREADONLY,strFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_strPath = dlg.GetPathName();
		CFileFind finder;
		BOOL bWorking = finder.FindFile(m_strPath);
		if (bWorking)
		{
			m_pApp->OpenDocumentFile(m_strPath);

			g_pMotion->AMove(eMaskZ, m_pDoc->m_dbZCCDFocus);

			UpdateData(FALSE);
			m_pApp->WriteProfileString("AUTOOFFSETPATH","AutoOffsetPath",m_strPath);
		}
		else
		{
			AfxMessageBox("OpenDocumentFile Fail! CurDocumentFile isnot exist");
		}
	}

}

void CDlgAutoOffsetEx::OnBnClickedBtnAutoStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_IO.INPUT_MACHINE_READY())
	{
		AfxMessageBox(_T("the table is not ready"));
		return;
	}

	m_pDoc->Setlight(m_pDoc->m_nLight1,m_pDoc->m_nLight2,m_pDoc->m_nLight3,m_pDoc->m_nLight4);
	g_ccd.SetWorkProgNo(m_nCCDNum);

	GetDlgItem(IDC_BTN_AUTO_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RESET_ALLPOS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SET_LTPOS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_MARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SELTOCCD)->EnableWindow(FALSE);


	UpdateData(TRUE);

	ResetEvent(hStop);
	CFileFind finder;
	BOOL bWorking = finder.FindFile(m_strPath);
	if (bWorking)
	{
		m_pDoc = (CHLDoc *)m_pApp->OpenDocumentFile(m_strPath);
		BOOL bAutoOffset = FALSE;
		int nLength = m_ListPos.GetItemCount();
		if (nLength < 1)
		{
			AfxMessageBox("No SetPos");
			bAutoOffset = TRUE;
		}
		else
		{
			g_pDlgMarkNormalPane->UpdateMotionView(FALSE);


			for (int i = 0; i < m_nAllRow; i++)
			{
				for (int j = 0; j < m_nAllCol; j++)
				{

					if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
					{
						WriteRunLog("人工中止自动校正!");
						goto Labelstop;
					}

					CString strItem = m_ListPos.GetItemText(i, j);
					int nRet = strItem.Find("/");
					CString strFlag = strItem.Mid(nRet + 1);
					strItem = strItem.Left(nRet);
					double CurX = m_dbLTposX - m_dbDis * j;
					double CurY = m_dbLtPosY + m_dbDis * i;

					if (_ttoi(strFlag) == 1)
					{
						continue;
					}
					else
					{

						g_pMotion->LnXY(CurX,CurY);
						Sleep(g_ParaIni.m_nMotionDelayForMark+3000);

						if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
						{
							WriteRunLog("人工中止自动校正!");
							goto Labelstop;
						}


						double dbCaptureX = CurX+g_ParaIni.m_dbMarkToCCD1X;
						double dbCaptureY = CurY+g_ParaIni.m_dbMarkToCCD1Y;
						BOOL bDevOk = m_pDoc->NoTicpsPointCCD1Capture(dbCaptureX, dbCaptureY,TRUE);

						double dbNewMarkToCCD1X = dbCaptureX - CurX;
						double dbNewMarkToCCD1Y = dbCaptureY - CurY;
						if (bDevOk)
						{
							bDevOk = m_pDoc->isDeviationOK(dbNewMarkToCCD1X,dbNewMarkToCCD1Y,FALSE);
						//	if (bDevOk)
							{
								strItem += "/1";
								m_ListPos.SetItem(i,j,LVIF_TEXT|LVIF_IMAGE,strItem,1,0,0,0);
								WriteFileData();
								continue;
							}
						}

						if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
						{
							WriteRunLog("人工中止自动校正!");
							goto Labelstop;
						}


						if (!m_pDoc->MoveMarkPos())
						{
							continue;
						}

						CString strTip;
						double dbMarkCrossPosX = g_pMotion->GetPos(eMaskX);
						double dbMarkCrossPosY = g_pMotion->GetPos(eMaskY);
						strTip.Format("打标(%.3f,%.3f)", dbMarkCrossPosX, dbMarkCrossPosY);
						WriteRunLog(strTip);

						if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
						{
							WriteRunLog("人工中止自动校正!");
							goto Labelstop;
						}

						OffsetMark();

						if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
						{
							WriteRunLog("人工中止自动校正!");
							goto Labelstop;
						}

						if (!m_pDoc->AfterMarkMove())
						{
							continue;
						}

						if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
						{
							WriteRunLog("人工中止自动校正!");
							goto Labelstop;
						}

						g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
						////移动到CCD中心	
						dbCaptureX = dbMarkCrossPosX+g_ParaIni.m_dbMarkToCCD1X;
						dbCaptureY = dbMarkCrossPosY+g_ParaIni.m_dbMarkToCCD1Y;
						if(!m_pDoc->NoTicpsPointCCD1Capture(dbCaptureX, dbCaptureY,TRUE))
						{
							WriteRunLog("移动十字至CCD中心失败!");
							strItem += "/1";
							m_ListPos.SetItem(i,j,LVIF_TEXT|LVIF_IMAGE,strItem,1,0,0,0);
							WriteFileData();
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
							strItem += "/1";
							m_ListPos.SetItem(i,j,LVIF_TEXT|LVIF_IMAGE,strItem,1,0,0,0);
							WriteFileData();
							bAutoOffset = TRUE;
							goto Label;
						}

					}
				}
			}
		}
Label:
		if (!bAutoOffset)
		{
			AfxMessageBox("No AutoOffset Pos");
		}
Labelstop:
		g_pDlgMarkNormalPane->UpdateMotionView(TRUE);

	}
	else
	{
		AfxMessageBox("OpenDocumentFile Fail! CurDocumentFile isnot exist");
	}
	GetDlgItem(IDC_BTN_AUTO_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_RESET_ALLPOS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SET_LTPOS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_LOAD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_MARK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SELTOCCD)->EnableWindow(TRUE);

}


void CDlgAutoOffsetEx::OnBnClickedBtnSetLtpos()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double CurX = g_pMotion->GetPos(eMaskX);
	double CurY = g_pMotion->GetPos(eMaskY);
	if (CurX-g_ParaIni.m_dbMarkToCCD1X < m_dbRtPos && CurX > m_dbLfPos)
	{
		double oldPosX = m_dbLTposX;
		double oldPosY = m_dbLtPosY;
		int oldRow = m_nAllRow;
		int oldCol = m_nAllCol;

		m_dbLTposX = CurX;
		m_dbLtPosY = CurY;
		UpdateData(FALSE);
		if (!ResetAllpos())
		{
			m_dbLTposX = oldPosX;
			m_dbLtPosY = oldPosY;
			m_nAllRow = oldRow;
			m_nAllCol = oldCol;

			UpdateData(FALSE);
			return;
		}

		m_pApp->WriteProfileDouble("AUTOOFFSETLTPOSX","AutoOffsetLtPosX",m_dbLTposX);
		m_pApp->WriteProfileDouble("AUTOOFFSETLTPOSY","AutoOffsetLtPosY",m_dbLtPosY);
		WriteFileData();
	}
	else
	{
		AfxMessageBox("LeftTopPos Beyond Moving Limit");
	}


}


void CDlgAutoOffsetEx::OffsetMark(void)
{

	PARA curMarkPara;
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

BOOL CDlgAutoOffsetEx::ResetAllpos()
{
	UpdateData(TRUE);

	if (m_nAllRow < 1 || m_nAllCol < 1)
	{
		AfxMessageBox("AllCol or AllRow  less than 1");
		return FALSE;
	}

	if (m_dbDis < 0)
	{
		AfxMessageBox("OffsetDis Less than 0");
		return FALSE;
	}

	if (m_dbLTposX - m_nAllCol * m_dbDis - g_ParaIni.m_dbMarkToCCD1X > m_dbRtPos)
	{
		AfxMessageBox("AllCol is too large");
		return FALSE;
	}

	m_ListPos.DeleteAllItems();
	while ( m_ListPos.DeleteColumn (0));

	for (int iClo = 0; iClo < m_nAllCol; iClo ++)
	{
		CString strCol;
		strCol.Format("Col%d",iClo+1);
		m_ListPos.InsertColumn(iClo, strCol, LVCFMT_CENTER, 100);
	}

	BOOL bAdd = TRUE;
	for (int i = 0; i < m_nAllRow; i++)
	{
		CString strId;
		strId.Format(_T("%d"),i);
		m_ListPos.InsertItem(i,strId);

		for (int j = 0; j < m_nAllCol; j++)
		{
			CString strcurXY;
			strcurXY.Format(_T("%d,%d/0"),-j,i);
			m_ListPos.SetItem(i,j,LVIF_TEXT|LVIF_IMAGE,strcurXY,0,0,0,0);

		}
	}

	WriteFileData();
	m_pApp->WriteProfileInt("AUTOOFFSETALLROW","AutoOffsetAllRow",m_nAllRow);
	m_pApp->WriteProfileInt("AUTOOFFSETALLCOL","AutoOffsetAllCol",m_nAllCol);
	return TRUE;
}

void CDlgAutoOffsetEx::OnBnClickedBtnResetAllpos()
{
	// TODO: 在此添加控件通知处理程序代码
	ResetAllpos();
}

BOOL CDlgAutoOffsetEx::ReadFileData(void)
{

	CString strPath = "d:\\AutoOffset.dat";
	CStdioFile fReadtxt;
	BOOL bReadOpen = fReadtxt.Open(strPath,CFile::modeRead|CFile::modeNoTruncate);
	if (bReadOpen)
	{
		int nLength = static_cast<int>(fReadtxt.GetLength());
		CString strData;
		fReadtxt.ReadString(strData);
		if (strData == "")
		{
			return FALSE;
		}
		int nRetRow = strData.Find("*");
		int nRetCol = strData.Find("#");
		CString strRow = strData.Mid(nRetRow + 1,nRetCol-nRetRow);
		m_nAllRow = _ttoi(strRow);
		strData = strData.Mid(nRetCol + 1);
		m_nAllCol = _ttoi(strData);

		m_ListPos.DeleteAllItems();
		while ( m_ListPos.DeleteColumn (0));
		for (int iClo = 0; iClo < m_nAllCol; iClo ++)
		{
			CString strCol;
			strCol.Format("Col%d",iClo+1);
			m_ListPos.InsertColumn(iClo, strCol, LVCFMT_CENTER, 100);
		}

		for (int i = 0; i < nLength; i++)
		{
			for (int j = 0; j < m_nAllCol; j++)
			{
				fReadtxt.ReadString(strData);
				strData.Trim();
				if (strData == "")
				{
					break;
				}
				if (j == 0)
				{
					CString strId;
					strId.Format(_T("%d"),i);
					m_ListPos.InsertItem(i,strId);
				}
				int nRet = strData.Find("/");
				CString strFlag = strData.Mid(nRet + 1);
				if (strFlag == "0")
				{
					m_ListPos.SetItem(i,j,LVIF_TEXT|LVIF_IMAGE,strData,0,0,0,0);
				}
				else
				{
					m_ListPos.SetItem(i,j,LVIF_TEXT|LVIF_IMAGE,strData,1,0,0,0);
				}
			}
		}
		fReadtxt.Close();
	}

	return TRUE;
}


BOOL CDlgAutoOffsetEx::WriteFileData(void)
{
	UpdateData(TRUE);

	if (m_nAllRow < 1 || m_nAllCol < 1)
	{
		AfxMessageBox("AllCol or AllRow  less than 1");
		return FALSE;
	}

	if (m_dbDis < 0)
	{
		AfxMessageBox("OffsetDis Less than 0");
		return FALSE;
	}

	if (m_dbLTposX - m_nAllCol * m_dbDis - g_ParaIni.m_dbMarkToCCD1X > m_dbRtPos)
	{
		AfxMessageBox("AllCol is too large");
		return FALSE;
	}


	CString strPath = "d:\\AutoOffset.dat";
	CStdioFile fWritetxt;
	BOOL bWriteOpen = fWritetxt.Open(strPath,CFile::modeCreate | CFile::modeWrite);
	if ( bWriteOpen)
	{
		CString strItem;
		strItem.Format("%.3f,%.3f*%d#%d",m_dbLTposX,m_dbLtPosY,m_nAllRow,m_nAllCol);
		strItem += "\n";
		fWritetxt.WriteString(strItem);

		for (int i = 0; i < m_nAllRow; i++)
		{
			for (int j = 0; j < m_nAllCol; j++)
			{
				strItem = m_ListPos.GetItemText(i, j);
				strItem += "\n";
				fWritetxt.WriteString(strItem);
			}
		}
		fWritetxt.Close();

	}

	m_pApp->WriteProfileDouble("AUTOOFFSETDIS","AutoOffsetDis",m_dbDis);
	m_pApp->WriteProfileInt("AUTOOFFSETCCDNUM","AutoOffsetCCDNum",m_nCCDNum);
	return TRUE;
}


void CDlgAutoOffsetEx::OnNMClickListPosinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	int nCurSel = pNMListView->iItem;
	//m_Col = pNMListView->iSubItem;
	CString strcurX   = m_ListPos.GetItemText(nCurSel, 1);
	CString strcurY = m_ListPos.GetItemText(nCurSel, 2);
	CString strFlag = m_ListPos.GetItemText(nCurSel, 0);
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgAutoOffsetEx::OnBnClickedBtnMark()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_IO.INPUT_MACHINE_READY())
	{
		AfxMessageBox(_T("the table is not ready"));
		return;
	}

	UpdateData(TRUE);
	if (m_nSelCol < 0 || m_nSelRow < 0)
	{
		AfxMessageBox("SelCol or SelRow  less than 0");
		return ;
	}
	CString strItem = m_ListPos.GetItemText(m_nSelRow, m_nSelCol);
	if (strItem == "")
	{
		AfxMessageBox("SelCol or SelRow too large");
		return;
	}
	int nRet = strItem.Find("/");
	CString strFlag = strItem.Mid(nRet + 1);
	strItem = strItem.Left(nRet);
	double CurX = m_dbLTposX - m_dbDis * m_nSelCol;
	double CurY = m_dbLtPosY + m_dbDis * m_nSelRow;
	g_pMotion->LnXY(CurX,CurY);

	int nFlag = _ttoi(strFlag);
	if (nFlag == 1)
	{	
		AfxMessageBox("CurPos is Marked");
	}
	else
	{
		g_pMotion->LnXY(CurX,CurY);
		Sleep(g_ParaIni.m_nMotionDelayForMark+3000);
		g_pDlgMarkNormalPane->UpdateMotionView(FALSE);
		if (!m_pDoc->MoveMarkPos())
		{
			return;
		}
		CString strTip;
		double dbMarkCrossPosX = g_pMotion->GetPos(eMaskX);
		double dbMarkCrossPosY = g_pMotion->GetPos(eMaskY);
		strTip.Format("打标(%.3f,%.3f)", dbMarkCrossPosX, dbMarkCrossPosY);
		WriteRunLog(strTip);
		OffsetMark();
		g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
	}
}


void CDlgAutoOffsetEx::OnBnClickedBtnSeltoccd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nSelCol < 0 || m_nSelRow < 0)
	{
		AfxMessageBox("SelCol or SelRow  less than 0");
		return ;
	}
	CString strItem = m_ListPos.GetItemText(m_nSelRow, m_nSelCol);
	if (strItem == "")
	{
		AfxMessageBox("SelCol or SelRow too large");
		return;
	}
	double CurX = m_dbLTposX - m_dbDis * m_nSelCol;
	double CurY = m_dbLtPosY + m_dbDis * m_nSelRow;
	g_pMotion->LnXY(CurX,CurY);

}


//void CDlgAutoOffsetEx::OnBnClickedBtnQuit()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	SetEvent(hStop);
//	g_DAT.MarkCmdStop();
//	DSF_TRACE("CMD STOP MARK !!!");
//
//	g_pMotion->Stop(eMaskXYZ);
//
//
//}
