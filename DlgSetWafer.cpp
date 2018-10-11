// DlgSetWafer.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgSetWafer.h"
#include "CCDCalibrate.h"
#include "Marknormaldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetWafer dialog


CDlgSetWafer::CDlgSetWafer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetWafer::IDD, pParent)
	, m_nGasModel(0)
	, m_dMarkLay(1)
	, m_dDownHigh(0)
	, m_dbScanPtX(0)
	, m_dbScanPtY(0)
	, m_dbScanZpos(0)
	, m_ScanPriNum(-1)
	, m_bScan(FALSE)
	//, m_bFocusMark(FALSE)
	, m_bQPosite(FALSE)
	, m_dbScaleLimit(0.02)
{
	//{{AFX_DATA_INIT(CDlgSetWafer)
	m_nLight1 = 0;
	m_nLight2 = 0;
	m_nLight3 = 0;
	m_nLight4 = 0;
	m_dbDisMaxErr = 0.0;
	m_dbAngleMaxErr = 0.0;
	m_dbHandOffsetX = 0.0;
	m_dbHandOffsetY = 0.0;
	m_dbHandOffsetCCWAngle = 0.0;
	m_dbXAutoScaleAdd = 0.0;
	m_dbYAutoScaleAdd = 0.0;
	m_bEnableXFixSacle = FALSE;
	m_bEnableYFixScale = FALSE;
	m_dbFixXScale = 0.0;
	m_dbFixYScale = 0.0;
	m_dbPoint0X = 0.0;
	m_dbPoint0Y = 0.0;
	m_dbPoint1X = 0.0;
	m_dbPoint1Y = 0.0;
	m_dbPoint2X = 0.0;
	m_dbPoint2Y = 0.0;
	m_dbPoint3X = 0.0;
	m_dbPoint3Y = 0.0;
	m_nLocateModel = 0;
	m_bEnableMarkFocus = FALSE;
	m_dbZCCDFocus = 0.0;
	m_dbZMarkFocus = 0.0;
	m_strProgID1 = _T("");
	m_strProgNewID2 = _T("");
	m_strProgNewID3 = _T("");
	m_strProgNewID4 = _T("");

	m_dbTestPowerFrequency = -1;
	m_dbProductPowerMax = 0.0;
	m_dbProductPowerMin = 0.0;

//	m_bFocusMark = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgSetWafer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetWafer)
	DDX_Text(pDX, IDC_EDIT_LIGHT1, m_nLight1);
	DDX_Text(pDX, IDC_EDIT_LIGHT2, m_nLight2);
	DDX_Text(pDX, IDC_EDIT_LIGHT3, m_nLight3);
	DDX_Text(pDX, IDC_EDIT_LIGHT4, m_nLight4);
	DDX_Text(pDX, IDC_EDIT_DIS_MAXERR, m_dbDisMaxErr);
	DDX_Text(pDX, IDC_EDIT_ANGLE_MAXERR, m_dbAngleMaxErr);
	DDX_Text(pDX, IDC_EDIT_HAND_OFFSET_X, m_dbHandOffsetX);
	DDX_Text(pDX, IDC_EDIT_HAND_OFFSET_Y, m_dbHandOffsetY);
	DDX_Text(pDX, IDC_EDIT_HAND_OFFSET_ANGLE, m_dbHandOffsetCCWAngle);
	DDX_Text(pDX, IDC_EDIT_X_AUTO_SCALE_ADD, m_dbXAutoScaleAdd);
	DDX_Text(pDX, IDC_EDIT_Y_AUTO_SCALE_ADD, m_dbYAutoScaleAdd);
	DDX_Check(pDX, IDC_CHECK_ENABLE_X_FIX_SCALE, m_bEnableXFixSacle);
	DDX_Check(pDX, IDC_CHECK_ENABLE_Y_FIX_SCALE, m_bEnableYFixScale);
	DDX_Text(pDX, IDC_EDIT_FIX_X_SCALE, m_dbFixXScale);
	DDX_Text(pDX, IDC_EDIT_FIX_Y_SCALE, m_dbFixYScale);
	DDX_Text(pDX, IDC_EDIT_POINT0_X, m_dbPoint0X);
	DDX_Text(pDX, IDC_EDIT_POINT0_Y, m_dbPoint0Y);
	DDX_Text(pDX, IDC_EDIT_POINT1_X, m_dbPoint1X);
	DDX_Text(pDX, IDC_EDIT_POINT1_Y, m_dbPoint1Y);
	DDX_Text(pDX, IDC_EDIT_POINT2_X, m_dbPoint2X);
	DDX_Text(pDX, IDC_EDIT_POINT2_Y, m_dbPoint2Y);
	DDX_Text(pDX, IDC_EDIT_POINT3_X, m_dbPoint3X);
	DDX_Text(pDX, IDC_EDIT_POINT3_Y, m_dbPoint3Y);
	DDX_Check(pDX, IDC_CHECK_ENABLE_MARKING_FOCUS, m_bEnableMarkFocus);
	DDX_Text(pDX, IDC_EDIT_Z_CCD_FOCUS, m_dbZCCDFocus);
	DDX_Text(pDX, IDC_EDIT_Z_MARK_FOCUS, m_dbZMarkFocus);
	DDX_Text(pDX, IDC_EDIT_PROGID1, m_strProgID1);
	DDX_Text(pDX, IDC_EDIT_PROGID2, m_strProgNewID2);
	DDX_Text(pDX, IDC_EDIT_PROGID3, m_strProgNewID3);
	DDX_Text(pDX, IDC_EDIT_PROGID4, m_strProgNewID4);
	DDX_Radio(pDX, IDC_RADIO_GAS, m_nGasModel);
	DDX_Radio(pDX, IDC_RADIO_DIVIDE_BY_STROKE, m_nDivideType);
	DDX_Text(pDX, IDC_EDIT_DIVIDE_X, m_dbDivideX);
	DDX_Text(pDX, IDC_EDIT_DIVIDE_Y, m_dbDivideY);
	DDX_Text(pDX, IDC_EDIT_DIVIDE_ADD_X, m_dbDivideAddX);
	DDX_Text(pDX, IDC_EDIT_DIVIDE_ADD_Y, m_dbDivideAddY);
	DDX_Text(pDX, IDC_EDIT_DIVIDE_OFFSET_X, m_dbDivideOffsetX);
	DDX_Text(pDX, IDC_EDIT_DIVIDE_OFFSET_Y, m_dbDivideOffsetY);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_POEWR, m_dbTestPowerFrequency);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_POEWR_MAX, m_dbProductPowerMax);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_POEWR_MIN, m_dbProductPowerMin);
	DDX_Text(pDX, IDC_EDIT_MARKLAY, m_dMarkLay);
	DDX_Text(pDX, IDC_EDIT_DOWNHIGH, m_dDownHigh);
	//	DDX_Check(pDX, IDC_CHECK_BFOUSMARK, m_bFocusMark);
	DDX_Control(pDX, IDC_LIST_FOCUS_TYPE, m_listCtrlFocusType);
	DDX_Text(pDX, IDC_EDIT_FOCUS2, m_strFocus);
	DDX_Text(pDX, IDC_EDIT_MOVE_DIS2, m_strMoveDis);
	DDX_Text(pDX, IDC_EDIT_MOVE_NUM2, m_strMoveNum);
	DDX_CBIndex(pDX, IDC_COMBO_LOCATE_MODEL, m_nLocateModel);
	DDX_Text(pDX, IDC_EDIT_SCANPTX, m_dbScanPtX);
	DDX_Text(pDX, IDC_EDIT_SCANPTY2, m_dbScanPtY);
	DDX_Text(pDX, IDC_EDIT_SCANNUM2, m_ScanPriNum);
	DDX_Text(pDX, IDC_EDIT_SCANZPOS, m_dbScanZpos);
	DDX_Check(pDX, IDC_CHECK_SACN, m_bScan);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_USE_Q_MOVE, m_bQPosite);
	DDX_Text(pDX, IDC_EDIT_SCALE_LIMIT, m_dbScaleLimit);
	DDX_Control(pDX, IDC_STATIC_MARK1, m_picMark1);
	DDX_Control(pDX, IDC_STATIC_MARK2, m_picMark2);
	DDX_Control(pDX, IDC_STATIC_MARK3, m_picMark3);
	DDX_Control(pDX, IDC_STATIC_MARK4, m_picMark4);
	DDX_Control(pDX, IDC_STATIC_MARK_BORDER, m_PicMarkBorder);
}


BEGIN_MESSAGE_MAP(CDlgSetWafer, CDialog)
	//{{AFX_MSG_MAP(CDlgSetWafer)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_ENABLE_MARKING_FOCUS, &CDlgSetWafer::OnBnClickedCheckEnableMarkingFocus)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSetWafer::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgSetWafer::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgSetWafer::OnBnClickedButtonEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FOCUS_TYPE, &CDlgSetWafer::OnNMClickListFocusType)
	ON_BN_CLICKED(IDOK, &CDlgSetWafer::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SETLIGHT, &CDlgSetWafer::OnBnClickedBtnSetlight)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATE_MODEL, &CDlgSetWafer::OnCbnSelchangeComboLocateModel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetWafer message handlers
BOOL CDlgSetWafer::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnCbnSelchangeComboLocateModel();

	// TODO:  在此添加额外的初始化
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_Z_MARK_FOCUS)->EnableWindow(m_bEnableMarkFocus);



	m_listCtrlFocusType.ModifyStyle(0, LVS_REPORT&LVS_SINGLESEL);
	m_listCtrlFocusType.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_listCtrlFocusType.InsertColumn(0, "", 0, 0);
	m_listCtrlFocusType.InsertColumn(1, "Type", 0, 50);
	m_listCtrlFocusType.InsertColumn(2, "Pos", 0, 50);
	m_listCtrlFocusType.InsertColumn(3, "MoveTimes", 0, 100);
	m_listCtrlFocusType.InsertColumn(4, "Move", 0, 50);

	//初始化List
	CString strItem;
	for (int i=0; i<m_pDoc->m_arrMarkFocus.GetSize(); i++)
	{
		m_listCtrlFocusType.InsertItem(i, "");
		m_listCtrlFocusType.SetItemText(i, 1, "0");

		strItem.Format("%.3f", m_pDoc->m_arrMarkFocus.GetAt(i).dbZPos);
		m_listCtrlFocusType.SetItemText(i, 2, strItem);
		strItem.Format("%d", m_pDoc->m_arrMarkFocus.GetAt(i).nMoveNum);
		m_listCtrlFocusType.SetItemText(i, 3, strItem);
		strItem.Format("%.3f", m_pDoc->m_arrMarkFocus.GetAt(i).dbMoveDis);
		m_listCtrlFocusType.SetItemText(i, 4, strItem);
	}	
	RefreshFocusID();
	m_listCtrlFocusType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);;
	OnNMClickListFocusType(NULL, NULL);
	OnBnClickedCheckEnableMarkingFocus();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSetWafer::OnBnClickedCheckEnableMarkingFocus()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_Z_MARK_FOCUS)->EnableWindow(m_bEnableMarkFocus);

	m_listCtrlFocusType.EnableWindow(m_bEnableMarkFocus);
	GetDlgItem(IDC_EDIT_FOCUS2)->EnableWindow(m_bEnableMarkFocus);
	GetDlgItem(IDC_EDIT_MOVE_NUM2)->EnableWindow(m_bEnableMarkFocus);
	GetDlgItem(IDC_EDIT_MOVE_DIS2)->EnableWindow(m_bEnableMarkFocus);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(m_bEnableMarkFocus);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(m_bEnableMarkFocus);
	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(m_bEnableMarkFocus);
}

void CDlgSetWafer::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_listCtrlFocusType.InsertItem(100, "");
	m_listCtrlFocusType.SetItemText(nIndex, 1, "0");
	m_listCtrlFocusType.SetItemText(nIndex, 2, "0");
	m_listCtrlFocusType.SetItemText(nIndex, 3, "0");
	m_listCtrlFocusType.SetItemText(nIndex, 4, "0");

	RefreshFocusID();
}


void CDlgSetWafer::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION posSel = m_listCtrlFocusType.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_listCtrlFocusType.GetNextSelectedItem(posSel);
		m_listCtrlFocusType.DeleteItem(nCurSel);
	}


	RefreshFocusID();
}


void CDlgSetWafer::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;

	POSITION posSel = m_listCtrlFocusType.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_listCtrlFocusType.GetNextSelectedItem(posSel);

		m_listCtrlFocusType.SetItemText(nCurSel, 2, m_strFocus);
		m_listCtrlFocusType.SetItemText(nCurSel, 3, m_strMoveNum);
		m_listCtrlFocusType.SetItemText(nCurSel, 4, m_strMoveDis);
	}
}


void CDlgSetWafer::OnNMClickListFocusType(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	POSITION posSel = m_listCtrlFocusType.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_listCtrlFocusType.GetNextSelectedItem(posSel);

		m_strFocus   = m_listCtrlFocusType.GetItemText(nCurSel, 2);
		m_strMoveNum = m_listCtrlFocusType.GetItemText(nCurSel, 3);
		m_strMoveDis = m_listCtrlFocusType.GetItemText(nCurSel, 4);
		UpdateData(FALSE);
	}

}

void CDlgSetWafer::RefreshFocusID()
{
	CString strID;
	int nCount = m_listCtrlFocusType.GetItemCount();
	for (int i=0; i<nCount; i++)
	{
		strID.Format("%d", i);
		m_listCtrlFocusType.SetItemText(i, 1, strID);
	}
}

void CDlgSetWafer::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;

	CString strItem;
	CMarkFocus focus;
	CArray<CMarkFocus> arrFocus;

	int nCount = m_listCtrlFocusType.GetItemCount();
	if (m_bEnableMarkFocus && nCount==0)
	{
		AfxMessageBox("No mark focus");
		return;
	}
	
	for (int i=0; i<nCount; i++)
	{
		strItem = m_listCtrlFocusType.GetItemText(i, 2);
		strItem += " ";
		strItem += m_listCtrlFocusType.GetItemText(i, 3);
		strItem += " ";
		strItem += m_listCtrlFocusType.GetItemText(i, 4);

		if (sscanf(strItem, "%lf%d%lf", &focus.dbZPos, &focus.nMoveNum, &focus.dbMoveDis)!=3)
		{
			AfxMessageBox("Marking the focus error data format!");
			return;
		}
		arrFocus.Add(focus);
	}

	m_pDoc->m_arrMarkFocus.Copy(arrFocus);	

	CDialog::OnOK();
}

void CDlgSetWafer::OnBnClickedBtnSetlight()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pDoc->Setlight(m_nLight1,m_nLight2,m_nLight3,m_nLight4);
	if (!g_ccd.SetWorkProgNo(atoi(m_strProgID1)))
		AfxMessageBox("Exchange CCD Fail!");
}

void CDlgSetWafer::OnCbnSelchangeComboLocateModel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;

	CWinApp *pApp = AfxGetApp();
	m_picMark1.SetIcon(pApp->LoadIcon(IDI_ICON_MARK1));
	m_picMark2.SetIcon(pApp->LoadIcon(IDI_ICON_MARK2));
	m_picMark3.SetIcon(pApp->LoadIcon(IDI_ICON_MARK3));
	m_picMark4.SetIcon(pApp->LoadIcon(IDI_ICON_MARK4));


	if (m_nLocateModel==LOCATE_MODEL_2_POINTS)
	{
		m_picMark3.SetIcon(pApp->LoadIcon(IDI_ICON_MARK2));

		m_picMark2.ShowWindow(FALSE);
		m_picMark3.ShowWindow(TRUE);
		m_picMark4.ShowWindow(FALSE);

		GetDlgItem(IDC_EDIT_LIGHT3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LIGHT4)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_PROGID3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROGID4)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_POINT2_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POINT2_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POINT3_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POINT3_Y)->EnableWindow(FALSE);
	}
	else if (m_nLocateModel==LOCATE_MODEL_3_POINTS)
	{
		m_picMark2.ShowWindow(TRUE);
		m_picMark3.ShowWindow(TRUE);
		m_picMark4.ShowWindow(FALSE);

		GetDlgItem(IDC_EDIT_LIGHT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LIGHT4)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_PROGID3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROGID4)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_POINT2_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT2_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT3_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POINT3_Y)->EnableWindow(FALSE);
	}
	else
	{
		m_picMark2.ShowWindow(TRUE);
		m_picMark3.ShowWindow(TRUE);
		m_picMark4.ShowWindow(TRUE);

		GetDlgItem(IDC_EDIT_LIGHT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LIGHT4)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_PROGID3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROGID4)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_POINT2_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT2_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT3_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT3_Y)->EnableWindow(TRUE);
	}
	m_PicMarkBorder.Invalidate();
}
